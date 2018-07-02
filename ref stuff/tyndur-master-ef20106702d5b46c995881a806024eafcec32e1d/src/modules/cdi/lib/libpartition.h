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

#ifndef _LIBPARTITION_H_
#define _LIBPARTITION_H_

#include <stdint.h>

#define PACKED __attribute__((packed))
#define PARTITION_TABLE_OFFSET 446
#define PARTITION_TABLE_SIG_OFFSET 510
#define PARTITION_TABLE_SIGNATURE 0xAA55

// Typ fuer eine Erweiterte Partition, deren Startsektor wieder eine
// Partitionstabelle beinhaltet.
#define PARTITION_TYPE_EXTENDED 0x05

struct partition {
    struct cdi_storage_device* dev;

    uint8_t     used;
    uint8_t     type;
    uint16_t    number;
    uint64_t    start;
    uint64_t    size;
};

/**
 * Partitionstabelle auf einem Massenspeichergeraet verarbeiten
 *
 * @param dev Geraet, auf dem nach Partitionen gesucht werden soll
 * @param partitions Liste, in die gefundene Partitionen eingefuegt werden
 * sollen (als struct partition)
 */
void cdi_tyndur_parse_partitions(struct cdi_storage_device* dev,
    cdi_list_t partitions);

#endif // ifndef _LIBPARTITION_H_

