/*
 * Copyright (c) 2006-2007 The tyndur Project. All rights reserved.
 *
 * This code is derived from software contributed to the tyndur Project
 * by Burkhard Weseloh.
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

#include "jprintf.h"

struct sprintf_args
{
    char * buffer;
    unsigned int bytes_written;
};

int sprintf_putc(struct sprintf_args * arg, char c)
{
    arg->buffer[arg->bytes_written++] = c;

    return 1;
}

int vsprintf(char * buffer, const char * format, va_list ap)
{
    struct sprintf_args args = { buffer, 0 };
    struct jprintf_args sprintf_handler = { (pfn_putc)&sprintf_putc, 0, (void*)&args };
    int retval;

	retval = jvprintf(&sprintf_handler, format, ap);
	if(retval >= 0)
	{
	    buffer[retval] = 0;
	}
	
	return retval;
}

int sprintf(char * buffer, const char * format, ...)
{
	va_list ap;
	int retval;

	va_start(ap, format);
	retval = vsprintf(buffer, format, ap);
	va_end(ap);

	return retval;
}
