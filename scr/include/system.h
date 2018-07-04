#ifndef SYSTEM_H
#define SYSTEM_H
#include "types.h"
uint8 inportb (uint16 _port);

void outportb (uint16 _port, uint8 _data);

uint8 inb (uint16 port);

void outb (uint16 port, uint8 data);

void NMI_enable(void);

void NMI_disable(void);

extern void entering_v86(uint32 ss, uint32 esp, uint32 cs, uint32 eip);
#endif