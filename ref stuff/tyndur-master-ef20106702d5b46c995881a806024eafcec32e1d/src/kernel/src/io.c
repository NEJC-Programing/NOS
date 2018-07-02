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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "types.h"
#include "tasks.h"
#include "tss.h"
#include "kernel.h"

/* Gesetztes Bit: Port wird benutzt */
uint8_t global_port_bitmap[IO_BITMAP_LENGTH / 8] = { 0 };

void init_io_ports(void)
{
    // Setze IO-Privilege-Level auf 0
    asm volatile(
        "pushf;"
        "pop %eax;"
        "andl $0xFFFFCFFF, %eax;"
        "push %eax;"
        "popf;"
    );
}

/**
 * Reserviert einen einzelnen IO-Port f�r einen Task
 *
 * @param task Task, dem der Port zugeordnet werden soll
 * @param port Nummer des zuzuordnenden IO-Ports
 *
 * @return Gibt true zur�ck, wenn der Task auf den Port zugreifen kann
 * (erfolgreiche oder bereits vorhandene Reservierung f�r denselben Task),
 * ansonsten false.
 */
static bool io_port_request(struct task* task, uint32_t port)
{
    uint32_t index = port / 8;
    uint8_t bit = 1 << (port % 8);
    uint8_t* io_bitmap = task->io_bitmap;

    // Pr�fen, ob der Port �berhaupt von der Bitmap abgedeckt wird
    if (port >= IO_BITMAP_LENGTH) {
        return false;
    }

    // Ist der Port noch frei?
    // Wenn der Port f�r denselben Task schon einmal reserviert wurde,
    // abbrechen, aber true zur�ckgeben
    if (global_port_bitmap[index] & bit) {
        return (io_bitmap && ((io_bitmap[index] & bit) == 0));
    }

    // Falls der Task noch keine zugeordnete Bitmap hat, wird es jetzt
    // Zeit, eine anzulegen
    if (io_bitmap == NULL) {
        io_bitmap = task->io_bitmap = malloc(IO_BITMAP_LENGTH / 8);
        memset(io_bitmap, 0xFF, IO_BITMAP_LENGTH / 8);
    }

    // Die eigentliche Reservierung
    global_port_bitmap[index] |= bit;
    io_bitmap[index] &= ~bit;

    return true;
}

/**
 * Gibt einen einzelnen IO-Port frei.
 *
 * @param task Task, der die Freigabe anfordert
 * @param port Freizugebender Port
 *
 * @return true, wenn der Port freigegeben werden konnte, false wenn der
 * Port nicht f�r den Task reserviert war.
 */
static bool io_port_release(struct task* task, uint32_t port)
{
    uint32_t index = port / 8;
    uint8_t bit = 1 << (port % 8);
    uint8_t* io_bitmap = task->io_bitmap;

    // Wenn der Port gar nicht f�r den Task reserviert ist, false zur�ckgeben
    if ((!io_bitmap) || (io_bitmap[index] & bit)) {
        return false;
    }

    // Wenn der Port zwar f�r den Task reserviert ist, aber in der globalen
    // Tabelle als frei markiert, haben wir einen Kernel-Bug.
    if ((global_port_bitmap[index] & bit) == 0) {
        panic("IO-Port-Bitmaps sind beschaedigt (Port %d, PID %d)", port, task->pid);
    }
    
    // Die eigentliche Freigabe
    global_port_bitmap[index] &= ~bit;
    io_bitmap[index] |= bit;

    return true;
}

/**
 * Reserviert einen Bereich von IO-Ports f�r einen Task.
 * Es werden entweder alle angeforderten oder im Fehlerfall gar keine Ports 
 * reserviert. Eine teilweise Reservierung tritt nicht auf.
 *
 * @param task Task, f�r den die Ports reserviert werden sollen
 * @param port Nummer des niedrigsten zu reservierenden Ports
 * @param length Anzahl der Ports, die zu reservieren sind
 */
bool io_ports_request(struct task* task, uint32_t port, uint32_t length)
{
    uint32_t n = length;
    while (n--) 
    {
        if (!io_port_request(task, port++)) 
        {
            // Bereits reservierte Ports wieder freigeben
            while (++n != length) {
                io_port_release(task, --port);
            }
            return false;
        }
    }

    return true;
}

/**
 * Gibt einen Bereich von Ports frei.
 *
 * @param task Task, der die Freigabe anfordert
 * @param port Niedrigster freizugebender Port
 * @param length Anzahl der freizugebenden Ports
 *
 * @return true, wenn alle Ports freigegeben werden konnten. Wenn ein Port
 * nicht freigegeben werden konnte (war nicht f�r den Task reserviert),
 * wird false zur�ckgegeben, die Bearbeitung allerdings nicht abgebrochen,
 * sondern die weiteren Ports versucht freizugeben.
 */
bool io_ports_release(struct task* task, uint32_t port, uint32_t length)
{
    bool success = true;

    while (length--) 
    {
        if (!io_port_release(task, port++)) {
            success = false;
        }
    }

    return success;
}

/**
 * Gibt alle von einem Task reservierten Ports frei
 *
 * @param task Task, dessen Ports freigegeben werden sollen
 */
void io_ports_release_all(struct task* task)
{
    uint32_t i;
    uint8_t* io_bitmap = task->io_bitmap;

    if (io_bitmap == NULL) {
        return;
    }

    for (i = 0; i < IO_BITMAP_LENGTH / 8; i++)
    {
        if (io_bitmap[i] != (uint8_t) -1) {
            io_ports_release(task, 8*i, 8);
        }
    }
}

void io_ports_check(struct task* task)
{
    uint32_t i, j;
    uint8_t* io_bitmap = task->io_bitmap;
    uint8_t bit;

    if (io_bitmap == NULL) {
        return;
    }

    for (i = 0; i < IO_BITMAP_LENGTH / 8; i++) {
        for (j = 0; j < 8; j++) {
            bit = 1 << j;
            if (((io_bitmap[i] & bit) == 0) && ((global_port_bitmap[i] & bit)
                == 0))
            {
                panic("IO-Port-Bitmaps sind beschaedigt (Port %d, PID %d)",
                    i * 8 + j, task->pid);
            }
        }
    }
}