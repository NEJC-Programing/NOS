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

#ifndef SYSCALLNO_H
#define SYSCALLNO_H

#define SYSCALL_PUTSN 0

#define SYSCALL_MEM_ALLOCATE 1
#define SYSCALL_MEM_FREE 2
#define SYSCALL_MEM_INFO 60
#define SYSCALL_MEM_ALLOCATE_PHYSICAL 61
#define SYSCALL_MEM_FREE_PHYSICAL 62
#define SYSCALL_MEM_RESOLVE_VADDR 66

#define SYSCALL_SHM_CREATE 63
#define SYSCALL_SHM_ATTACH 64
#define SYSCALL_SHM_DETACH 65
#define SYSCALL_SHM_SIZE 67

#define SYSCALL_PM_CREATE_PROCESS 3
#define SYSCALL_PM_INIT_PAGE 13
#define SYSCALL_PM_INIT_PAGE_COPY 82
#define SYSCALL_PM_INIT_PROC_PARAM_BLOCK 84
#define SYSCALL_PM_EXIT_PROCESS 5
#define SYSCALL_PM_SLEEP 6
#define SYSCALL_PM_GET_UID 7
#define SYSCALL_PM_SET_UID 8
#define SYSCALL_PM_REQUEST_PORT 9
#define SYSCALL_PM_RELEASE_PORT 10
#define KERN_SYSCALL_PM_SLEEP 124

#define SYSCALL_VM86 81
#define SYSCALL_VM86_BIOS_INT 83

#define SYSCALL_PM_P 11
#define SYSCALL_PM_V 12
#define SYSCALL_PM_V_AND_WAIT_FOR_RPC 19

#define SYSCALL_PM_GET_PID 14
#define SYSCALL_PM_GET_CMDLINE 15
#define SYSCALL_PM_GET_PARENT_PID 16
#define SYSCALL_PM_ENUMERATE_TASKS 18

#define SYSCALL_PM_WAIT_FOR_RPC 17

#define SYSCALL_PM_GET_TID 119
#define SYSCALL_PM_CREATE_THREAD 120
#define SYSCALL_PM_EXIT_THREAD 121

#define SYSCALL_MUTEX_LOCK 122
#define SYSCALL_MUTEX_UNLOCK 123

#define SYSCALL_GET_TICK_COUNT 40

#define SYSCALL_FORTY_TWO 42

#define SYSCALL_SET_RPC_HANDLER 50
#define SYSCALL_RPC 51
#define SYSCALL_ADD_INTERRUPT_HANDLER 52

#define SYSCALL_FASTRPC 55
#define SYSCALL_FASTRPC_RET 56

#define SYSCALL_WAIT_FOR_RPC 57

#define SYSCALL_ADD_TIMER 70

#define SYSCALL_DEBUG_STACKTRACE 80

#define SYSCALL_LIO_RESOURCE    85
#define SYSCALL_LIO_OPEN        86
#define SYSCALL_LIO_CLOSE       87
#define SYSCALL_LIO_READ        88
#define SYSCALL_LIO_WRITE       89
#define SYSCALL_LIO_SEEK        90
#define SYSCALL_LIO_MKFILE      91
#define SYSCALL_LIO_MKDIR       92
#define SYSCALL_LIO_READ_DIR    93
#define SYSCALL_LIO_SYNC        94
#define SYSCALL_LIO_MKSYMLINK   95
#define SYSCALL_LIO_STAT        96
#define SYSCALL_LIO_TRUNCATE    97
#define SYSCALL_LIO_UNLINK      98
#define SYSCALL_LIO_SYNC_ALL    99
#define SYSCALL_LIO_PASS_FD     100
#define SYSCALL_LIO_PROBE_SERVICE 101
#define SYSCALL_LIO_DUP           102
#define SYSCALL_LIO_STREAM_ORIGIN 103
#define SYSCALL_LIO_PIPE          104
#define SYSCALL_LIO_IOCTL         105
#define SYSCALL_LIO_COMPOSITE_STREAM    106

#define SYSCALL_LIO_SRV_SERVICE_REGISTER            110
#define SYSCALL_LIO_SRV_TREE_SET_RING               111
#define SYSCALL_LIO_SRV_RES_UPLOAD                  112
#define SYSCALL_LIO_SRV_NODE_ADD                    113
#define SYSCALL_LIO_SRV_OP_DONE                     114
#define SYSCALL_LIO_SRV_WAIT                        115
#define SYSCALL_LIO_SRV_REQUEST_TRANSLATED_BLOCKS   116
#define SYSCALL_LIO_SRV_NODE_REMOVE                 117

//ACHTUNG: Muss eine Zahl groesser als die Groesste Syscall-Nummer sein
#define SYSCALL_MAX 125

#endif
