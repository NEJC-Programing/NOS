#include <all.h>
#ifndef SYS_H
#define SYS_H

#define PANIC(msg) die(msg)

typedef struct register16 {
    uint16_t di;
    uint16_t si;
    uint16_t bp;
    uint16_t sp;
    uint16_t bx;
    uint16_t dx;
    uint16_t cx;
    uint16_t ax;

    uint16_t ds;
    uint16_t es;
    uint16_t fs;
    uint16_t gs;
    uint16_t ss;
    uint16_t eflags;
}register16_t;

#endif
#ifndef BIOS32_H
#define BIOS32_H


extern void bios32_helper();
extern void bios32_helper_end();
extern void * asm_gdt_entries;
extern void * asm_gdt_ptr;
extern void * asm_idt_ptr;
extern void * asm_in_reg_ptr;
extern void * asm_out_reg_ptr;
extern void * asm_intnum_ptr;

#define REBASE(x) (void*)(0x7c00 + (void*)x - (uint32_t)bios32_helper)

#define BIOS_GRAPHICS_SERVICE 0x10

void bios32_init();
void bios32_service(uint8_t int_num, register16_t * in_reg, register16_t * out_reg);

#endif
#ifndef SYSIDT_H
#define SYSIDT_H

#define NUM_IDT_ENTRIES 256

typedef struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;


// Extern asm functions
extern void idt_flush(uint32_t ptr);

// Idt functions
void idt_init();
void idt_set_entry(int index, uint32_t base, uint16_t sel, uint8_t ring);

#endif
#ifndef GDT_H
#define GDT_H

// Number of global descriptors
#define NUM_DESCRIPTORS 8

// Gdt related structures
typedef struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

// Extern asm functions
extern void gdt_flush(uint32_t gdt_ptr);

// gdt functions
void gdt_init();
void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

extern gdt_entry_t gdt_entries[NUM_DESCRIPTORS];

#endif
#ifndef TSS_H
#define TSS_H
typedef struct tss_entry {
    uint32_t prevTss;
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
    uint16_t trap;
    uint16_t iomap;
}tss_entry_t;

extern void tss_flush();

void tss_init(uint32_t idx, uint32_t kss, uint32_t kesp);

void tss_set_stack(uint32_t kss, uint32_t kesp);

#endif