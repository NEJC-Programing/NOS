#ifndef TSS_H
#define TSS_H

#include <stdint.h>
#include <stddef.h>

#define IO_BITMAP_LENGTH 0xffff

// Dieser Wert muss ausserhalb des in der GDT definierten Limits
// fuer das TSS liegen
#define TSS_IO_BITMAP_NOT_LOADED (sizeof(tss) + 0x100)
#define TSS_IO_BITMAP_OFFSET offsetof(tss_t, io_bit_map)

typedef struct tss_s
{
	uint32_t backlink;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trace_trap;
	uint16_t io_bit_map_offset;
    uint8_t io_bit_map[IO_BITMAP_LENGTH / 8];
    uint8_t io_bit_map_end;
} __attribute__((packed)) tss_t;

extern tss_t tss;

#endif /* ndef TSS_H */
