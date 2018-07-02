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

#ifndef _IM_H_
#define _IM_H_

#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "tasks.h"

#define IM_IRQ_BASE 0x20
#define IM_NUM_INTERRUPTS 256

void im_init(void);
void im_init_local(void);

void im_enable(void);
void im_disable(void);

void im_enable_irq(uint8_t irq);
void im_disable_irq(uint8_t irq);

/**
 * Zeigt an, dass ein RPC zu einem Task ausgelöst wurde, der den entsprechenden
 * IRQ verarbeiten soll.
 */
void im_irq_handlers_increment(uint8_t irq);
/**
 * Zeigt an, dass solch ein RPC beendet wurde. Ist die Zahl der Tasks, die sich
 * um diesen IRQ kümmern, auf 0 gesunken, kann der IRQ wieder per im_enable_irq
 * zugelassen werden.
 */
void im_irq_handlers_decrement(uint8_t irq);

void im_end_of_interrupt(uint8_t interrupt);
interrupt_stack_frame_t* im_handler(interrupt_stack_frame_t* isf);
interrupt_stack_frame_t* im_prepare_current_thread(void);

bool im_add_handler(uint32_t intr, pm_process_t* handler);

#endif //ifndef _IM_H_

