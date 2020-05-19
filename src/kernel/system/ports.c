#include <ports.h>

uint8_t inb (uint16_t port)
{
    	uint8 rv;
    	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
    	return rv;
}

void outb (uint16_t port, uint8_t data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}


