#ifndef SYSTEM_H
#define SYSTEM_H
#include "types.h"
#include "graphics.h"
#include "fs.h"
#include "drivers.h"

uint8 inportb (uint16 _port);

void outportb (uint16 _port, uint8 _data);

uint8 inb (uint16 port);
extern uint16_t inw(uint16 port);
extern uint32_t inl(uint16 port);

void outb (uint16 port, uint8 data);
extern void outw(uint16 port, uint16_t data);
extern void outl(uint16 port, uint32_t data);

static inline void io_wait()
{
    outb(0x80, 0);
}

void NMI_enable(void);
void NMI_disable(void);

BYTE xor_hash(BYTE * data);
void reboot();
void shutdown();
void panic(string reason, int bsod);
void delay(int sec);
extern void entering_v86(uint32 ss, uint32 esp, uint32 cs, uint32 eip);
#endif