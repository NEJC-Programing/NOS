/*  
 * Copyright (c) 2007 The tyndur Project. All rights reserved.
 *
 * This code is derived from software contributed to the tyndur Project
 * by Antoine Kaufmann.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
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

#include <types.h>
#include <string.h>
#include <page.h>
#include <loader.h>

/**
 * Eine Flache Binaerdatei laden.
 *
 * @param process PID
 * @param image_start Pointer auf den Anfang des Dateiinhalts
 * @param image_size Groesse des Dateiinhalts
 *
 * @return TRUE, wenn erfolgreich geladen, FALSE sonst.
 */
bool loader_load_flat_bin_image(pid_t process, vaddr_t image_start,
    size_t image_size)
{
    // Speicherimage anlegen und mit Date fuellen
    vaddr_t mem_image = loader_allocate_mem(image_size);
    memcpy(mem_image, image_start, image_size);

    // Thread erstellen
    loader_create_thread(process, (vaddr_t) USER_MEM_START);
    
    // Speicher an den Prozess zuweisen
    return loader_assign_mem(process, (vaddr_t) USER_MEM_START, mem_image,
        image_size);
}

