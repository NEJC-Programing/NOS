#ifndef KERNEL_H
#define KERNEL_H
    
//#define COOPERATIVE_MULTITASKING

#include <stdint.h>
#include <types.h>

__attribute__((noreturn)) void panic(char * message, ...);

paddr_t phys_alloc_page(void);
paddr_t phys_alloc_page_range(unsigned int num);

paddr_t phys_alloc_dma_page(void);
paddr_t phys_alloc_dma_page_range(unsigned int num);

paddr_t phys_alloc_page_limit(uint32_t lower_limit);

void phys_free_page(paddr_t page);
void phys_free_page_range(paddr_t page, unsigned int num);

extern void kernel_start(void);
extern void kernel_phys_start(void);
extern void kernel_end(void);
extern void kernel_phys_end(void);

#endif /* ndef KERNEL_H */
