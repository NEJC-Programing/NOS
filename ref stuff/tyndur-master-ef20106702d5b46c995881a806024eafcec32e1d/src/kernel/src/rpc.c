/*  
 * Copyright (c) 2007 The tyndur Project. All rights reserved.
 *
 * This code is derived from software contributed to the tyndur Project
 * by Kevin Wolf.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *     This product includes software developed by the tyndur Project
 *     and its contributors.
 * 4. Neither the name of the tyndur Project nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */  

#include <stdint.h>

#include "rpc.h"
#include "tasks.h"
#include "paging.h"
#include "vmm.h"
#include "kmm.h"
#include "intr.h"
#include "string.h"
#include "stdlib.h"
#include "debug.h"

typedef struct {
    uint32_t   old_eip;
    uint32_t   old_esp;
    uint32_t   eflags;
    uint32_t   eax;

    uint32_t   reenable_irq;

    struct task* caller;
} rpc_t;

/**
 * F�hrt einen RPC durch.
 *
 * Der Aufruf einer RPC-Funktion besteht im allgemeinen aus Metadaten (z.B.
 * die Funktionsnummer der aufzurufenden Funktion, Nachrichten-ID usw.)
 * und Nutzdaten.
 *
 * Die Daten werden dem aufgerufenen Task so auf den Stack gelegt, da� von
 * esp bis esp + metadata_size die Metadaten liegen, von esp + metadata_size
 * bis esp + metadata_size + data_size die Nutzdaten. Daraus ergibt sich
 * insbesondere, da� sich die Grenze zwischen Metadaten und Daten beliebig 
 * ziehen l��t, wie sie f�r den Aufrufer am g�nstigsten ist. Unter anderem ist
 * es m�glich, alle Daten an einem Ort zu �bergeben und metadata_size = 0 zu
 * setzen.
 *
 * Im Allgemeinen liegen allerdings die Metadaten auf dem Stack des Aufrufers,
 * w�hrend die Nutzdaten oft ein Pointer in den Heap des Aufrufers sind.
 *
 * Zur Durchf�hrung eines RPC werden zun�chst einige Register des aufgerufenen
 * Tasks gesichtert (eax, eip, esp, eflags; die �brigen Register hat der
 * aufgerufene Task selbst zu sichern). Anschlie�end werden die Daten auf den
 * Stack des aufgerufenen Tasks gelegt, eip auf den RPc-Handler gesetzt und der
 * Scheduler angewiesen, zum aufgerufenen Task umzuschalten.
 *
 * Der RPC-Handler mu� als letzte Aktion den Syscall FASTRPC_RET aufrufen, der
 * die Register wiederherstellt und dadurch den R�cksprung vornimmt.
 *
 * @param callee        Aufzurufender Task
 * @param metadata_size L�nge der Metadaten in Bytes
 * @param metadata      Pointer auf die Metadaten
 * @param data_size     L�nger der Daten in Bytes
 * @param data          Pointer auf die Daten
 *
 * @return true, wenn der Aufruf erfolgreich durchgef�hrt wurde, false
 * sonst. Bei false als R�ckgabe sollte der aufrufende Task den RPC-Syscall
 * wiederholen.
 */
bool fastrpc(struct task * callee, uint32_t metadata_size, void* metadata,
    uint32_t data_size, void* data)
{
    int i;

    //kprintf("[%d => %d, 0x%x + 0x%x]\n", current_task->pid, callee->pid, 
    //    metadata_size, data_size);
    //kprintf("<%08x = %08x %08x>\n", metadata, 0, 0);
    
    if (!callee) {
        abort_task("RPC zu nicht vorhandenem Prozess");
        return false;
    }

    if (metadata_size + data_size > 9 * PAGE_SIZE) {
        abort_task("RPC-Datenblock zu gross: %d Bytes", data_size);
        return false;
    }
    
    if (callee->rpc_handler == NULL) {
        return false;
    }

    // Gesamtgr��e, um die der Stack des aufgerufenen Prozesses vergr��ert
    // wird (zus�tzlich 4 Bytes f�r die R�cksprungadresse)
    uint32_t total_data_size = metadata_size + data_size
        + sizeof(data_size) + sizeof(pid_t);
    
    // Datengr��e auf ganze vier Bytes aufrunden
    uint32_t rounded_data_size = (total_data_size + 3) & ~0x3;

    // Wenn es sich um eine Antwort im Rahmen von asynchronem RPC handelt,
    // durchlassen, auch wenn der Proze� nicht blockiert werden kann
    bool ignore_blocked = false;
    if ((callee->status == TS_WAIT_FOR_RPC) && (
                ((metadata_size >= 4) 
            &&  (((uint32_t*)metadata)[0] == 513))

        ||      ((metadata_size == 0) 
            &&  (data_size >= 4) 
            &&  (((uint32_t*)data)[0] == 513))
        ))
    {
        ignore_blocked = true;
    }

    // W�hrend wir am Stack rumbasteln, sollte niemand dazwischenfunken und
    // erst recht nicht der Task weiterlaufen (momentan unn�tig, da im Kernel
    // keine Interrupts erlaubt sind). Au�erdem wird damit sichergestellt, da�
    // ein Task, der p() aufgerufen hat, keinen RPC bekommt.
    //
    // Wenn der Task nicht blockiert werden kann, soll der Aufrufer es sp�ter
    // nochmal versuchen.
    if (!ignore_blocked && 
        ((callee->blocked_by_pid) || !block_task(callee, current_task->pid)))
    {
        //kprintf("Task %d ist blockiert.\n", callee->pid);
        return false;
    }
    
    // Falls der Stack nicht ausreicht, vergr��ern
    paddr_t callee_isf_phys = 
        resolve_vaddr((page_directory_t) callee->cr3, (vaddr_t) callee->esp);

    struct int_stack_frame* callee_isf = (struct int_stack_frame*) 
        map_phys_addr(callee_isf_phys, sizeof(callee_isf));
    
    uint32_t pages_count = ((rounded_data_size + PAGE_SIZE - 1) / PAGE_SIZE);
    if ((rounded_data_size % PAGE_SIZE) > (callee_isf->esp % PAGE_SIZE)) {
        pages_count++;
    }

	if(resolve_vaddr((page_directory_t) callee->cr3,
        (vaddr_t) callee_isf->esp - rounded_data_size) == (paddr_t) NULL)
	{
        // TODO Das k�nnte bis zu (pages_count - 1) Pages zu viel reservieren
		increase_user_stack_size(callee, pages_count);
	}
    
    // Den Stack des aufgerufenen Tasks in den Kernelspeicher mappen
    uint32_t callee_esp_phys = (uint32_t) resolve_vaddr(
        (page_directory_t) callee->cr3, (vaddr_t) callee_isf->esp);
   
    // TODO Funktion, um komplette Bereiche eines anderen Tasks zu mappen
    vaddr_t callee_esp_kernel_pages = 
        find_contiguous_kernel_pages(pages_count);

    for (i = 0; i < pages_count; i++) {
        map_page(
            kernel_page_directory, 
            (vaddr_t) ((uint32_t) callee_esp_kernel_pages + (PAGE_SIZE * i)),
            (paddr_t) PAGE_ALIGN_ROUND_DOWN((uint32_t) resolve_vaddr(
                (page_directory_t) callee->cr3, 
                (vaddr_t) callee_isf->esp - (PAGE_SIZE * (pages_count - i - 1))
            )),
            PTE_P | PTE_W
        );
    }

    // RPC-Backlinkinformation anlegen
    rpc_t* rpc = malloc(sizeof(rpc_t));
    
    rpc->old_eip = callee_isf->eip;
    rpc->old_esp = callee_isf->esp;
    rpc->eax     = callee_isf->eax;
    rpc->eflags  = callee_isf->eflags;
    rpc->caller  = current_task;

    rpc->reenable_irq = 0;

    list_push(callee->rpcs, rpc);
    
    // Adresse des neuen Stackendes im Kernelspeicher berechnen. Diese Adresse
    // liegt irgendwo in der Mitte der ersten gemappten Stackpage. Das Offset
    // von der Pagegrenze ist also in der Rechnung zu ber�cksichtigen.
    //
    // Auf den Stack kommen neue Daten der Gr��e rounded_data_size.
    vaddr_t callee_esp = callee_esp_kernel_pages
        + (callee_esp_phys % PAGE_SIZE) + ((pages_count - 1) * PAGE_SIZE);

    callee_esp -= rounded_data_size;

    // Kopieren der Datengr��e, der Aufrufer-PID und der Daten auf den Stack
    *((uint32_t *) callee_esp) = data_size + metadata_size;
    *((uint32_t *) (callee_esp + 4)) = current_task->pid;
    memcpy(callee_esp + 8, metadata, metadata_size);
    memcpy(callee_esp + 8 + metadata_size, data, data_size);
    
    // Den Stackpointer des aufgerufenen Tasks anpassen, so da� er auf den 
    // Anfang der kopierten Daten zeigt, und in den RPC-Handler springen
    callee_isf->esp -= rounded_data_size;
    callee_isf->eip = (uint32_t) callee->rpc_handler;

    // Gemappte Seiten wieder freigeben
    for (i = 0; i < pages_count; i++) {
        unmap_page(kernel_page_directory, 
            (vaddr_t) ((uint32_t) callee_esp_kernel_pages + (PAGE_SIZE * i)));
    }

    unmap_page(kernel_page_directory, 
        (vaddr_t) PAGE_ALIGN_ROUND_DOWN((uint32_t) callee_isf));
   
    // Der Task darf wieder laufen
    if (callee->status == TS_WAIT_FOR_RPC) {
        callee->status = TS_RUNNING;
    }

    if (!ignore_blocked) {
        //kprintf("PID %d darf wieder laufen.\n", callee->pid);
        unblock_task(callee, current_task->pid);
    }

    return true;
}

/**
 * F�hrt einen RPC zur Behandlung eines IRQ durch. Der IRQ wird dabei
 * deaktiviert, solange der Handler arbeitet und erst anschlie�end 
 * wieder freigegeben. Im sonstigen Verhalten gleicht diese Funktion
 * fastrpc.
 *
 * @see fastrpc
 */
bool fastrpc_irq(struct task * callee, uint32_t metadata_size, void* metadata,
    uint32_t data_size, void* data, uint8_t irq)
{
    if (callee->blocked_by_pid && (callee->status != TS_WAIT_FOR_RPC)) {
        return false;
    }

    if (fastrpc(callee, metadata_size, metadata, data_size, data)) {
        rpc_t* rpc = list_get_element_at(current_task->rpcs, 0);
        rpc->reenable_irq = irq;
        return true;
    } else {
        return false;
    }
}

/**
 * Wird nach der Ausf�hrung eines RPC-Handlers aufgerufen.
 *
 * Nach der R�ckkehr vom RPC-Handler wird der neueste Zustand vom RPC-Stack
 * gepopt und zur Wiederherstellung des urspr�nglichen Prozessorzustands
 * benutzt.
 *
 * Dies betrifft eip, esp, eax und eflags. Die �brigen Register sind vom
 * RPC-Handler zu sicher und vor dem Aufruf von SYSCALL_FASTRPC_RET
 * wiederherzustellen. eax ist davon ausgenommen, da es die Funktionsnummer
 * des Syscalls enthalten mu�.
 *
 * @param esp Interrupt-Stackframe des vom RPC-Handler zur�ckkehrenden Tasks
 */
void return_from_rpc(struct int_stack_frame ** esp)
{
    rpc_t* rpc = list_pop(current_task->rpcs);
    struct int_stack_frame* callee_isf = *esp;

    // Wenn der Task vom RPC-Handler zur�ckkehrt, obwohl der Handler
    // gar nicht aufgerufen wurde, l�uft was schief
    if (rpc == NULL) {
        if(debug_test_flag(DEBUG_FLAG_STACK_BACKTRACE)) {
            stack_backtrace_ebp(callee_isf->ebp, callee_isf->eip);
        }
        abort_task("Unerwartete Rueckkehr vom RPC-Handler");
    }
    
    // Wiederherstellen der Register
    callee_isf->eip = rpc->old_eip; 
    callee_isf->esp = rpc->old_esp;
    callee_isf->eax = rpc->eax; 
    callee_isf->eflags = rpc->eflags;

    // Wechsel zum aufrufenden Task
    if (rpc->caller) {
        schedule_to_task(rpc->caller, (uint32_t*) esp);
    }

    // Wenn es ein IRQ-verarbeitender RPC war, den Interrupt jetzt
    // wieder aktivieren
    if (rpc->reenable_irq) {
        //printf("reenable IRQ %d\n", rpc->reenable_irq);
        enable_irq(rpc->reenable_irq);
    }

    free(rpc);
}

/**
 * Entfernt den gegebenen Task aus allen RPC-Backlinks.
 *
 * Diese Funktion wird benoetigt, wenn ein Task einen RPC aufruft und beendet
 * wird, bevor der RPC fertig ist. Ansonsten zeigt rpc->caller ins Leere,
 * was in return_from_rpc() zur Katastrophe f�hrt.
 */
void rpc_destroy_task_backlinks(struct task* destroyed_task)
{
    struct task* task;
    for (task = first_task; task != NULL; task = task->next_task) {
        int i;
        rpc_t* rpc;
        for (i = 0; (rpc = list_get_element_at(task->rpcs, i)); i++) {
            if (rpc->caller == destroyed_task) {
                rpc->caller = NULL;
            }
        }
    }
}
