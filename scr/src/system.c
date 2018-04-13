#include "../include/system.h"
uint8 inportb (uint16 _port)
{
    	uint8 rv;
    	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    	return rv;
}

void outportb (uint16 _port, uint8 _data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void outb (uint16 port, uint8 data){
	outportb(port, data);
}

uint8 inb(uint16 port){
	inportb(port);
}

void NMI_enable(void)
 {
    outportb(0x70, inportb(0x70)&0x7F);
 }
 
 void NMI_disable(void)
 {
    outportb(0x70, inportb(0x70)|0x80);
 }