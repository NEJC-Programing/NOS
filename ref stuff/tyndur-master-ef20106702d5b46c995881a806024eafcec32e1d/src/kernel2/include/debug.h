/*
 * Copyright (c) 2006-2008 The tyndur Project. All rights reserved.
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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdint.h>
#include <stdbool.h>

/* Debug-Funktionen und Helferlein */

#define DEBUG_FLAG_INIT 1
#define DEBUG_FLAG_STACK_BACKTRACE 2
#define DEBUG_FLAG_PEDANTIC 4
#define DEBUG_FLAG_SYSCALL 8

///Setzt die richtigen Debug-Flags anhand der Commandline vom bootloader
void debug_parse_cmdline(char* cmdline);

///Ueberprueft ob ein bestimmtes Debug-Flag gesetzt ist
bool debug_test_flag(uint32_t flag);

///Gibt die Debug-Meldung aus, wenn das Flag gesetzt ist
void debug_print(uint32_t flag, const char* message);

/*
 * Gibt einen Stack Backtrace aus, beginnend an den �bergebenen Werten
 * f�r bp und ip
 */
void stack_backtrace(uintptr_t start_bp, uintptr_t start_ip);

#endif
