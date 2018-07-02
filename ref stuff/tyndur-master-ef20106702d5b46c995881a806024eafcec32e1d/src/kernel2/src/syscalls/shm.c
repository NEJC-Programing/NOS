/*
 * Copyright (c) 2008 The tyndur Project. All rights reserved.
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

#include "syscall.h"
#include "mm.h"

/**
 * Neuen Shared Memory reservieren
 *
 * @return ID des neu angelegten Shared Memory. Der Aufrufer muss anschliessend
 * syscall_shm_attach aufrufen, um auf den Speicher zugreifen zu koennen.
 */
uint32_t syscall_shm_create(size_t size)
{
    return shm_create(size);
}

/**
 * Bestehenden Shared Memory oeffnen.
 *
 * @param id ID des zu oeffnenden Shared Memory
 * @return Virtuelle Adresse des geoeffneten Shared Memory; NULL, wenn der
 * Shared Memory mit der angegebenen ID nicht existiert.
 */
void* syscall_shm_attach(uint32_t id)
{
    return shm_attach(current_process, id);
}

/**
 * Shared Memory schliessen. Wenn keine Prozesse den Shared Memory mehr
 * geoeffnet haben, wird er geloescht.
 *
 * @param id ID des zu schliessenden Shared Memory
 */
void syscall_shm_detach(uint32_t id)
{
    shm_detach(current_process, id);
}

/// Größe eines Shared Memory zurückgeben
int32_t syscall_shm_size(uint32_t id)
{
    return shm_size(id);
}
