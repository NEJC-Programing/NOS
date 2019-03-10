#include "sys.h"
idt_entry_t idt_entries[NUM_IDT_ENTRIES];
idt_ptr_t idt_ptr;

void idt_init() {
    isr_install();
    asm volatile("sti");
}