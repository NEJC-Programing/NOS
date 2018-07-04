#include "../include/system.h"
#include "../include/screen.h"
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

void reboot()
{
    uint8 good= 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
}
void shutdown()
{
	//__asm__ __volatile__ ("outw %1, %0" : : "dN" ((uint16)0xB004), "a" ((uint16)0x2000)); // system
	//for (const char *s = "Shutdown"; *s; ++s) { 
  	//	outb(0x8900, *s);                       // other
	//}
	outb(0xf4, 0x00);// qemu
}

void panic(string reason, int bsod)
{
	if (bsod){
		clearScreen();
		for(int i = 0; i<=3100; i++){ // blue screen
			print_colored(" ", 0,1);
		}
		int start = 40-(int)((double)strlength(reason)/(double)2);
		SetCursor(start, 12);
		print_colored(reason,0xf,1);
	}else{
		SetCursor(0,0);// mac style
		print(reason);
	}
	//delay(12345);
	//reboot();
	asm("hlt");
}

void die(string reason)
{
	panic(reason, 1);
}

void delay(int time_ms)
{
	time_ms = time_ms*100000;
	int i = 0;
	while(i != time_ms)
	{
		i++;
	}
}

