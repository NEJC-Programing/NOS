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
		clearScreen_vga();
		vga_setgmode(1);
		PutRect(0,0,1000000,10000000,0x09);//0x07 gray//0x09 light blue//0x3F white
		sad();
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
void sleep(int s){delay(s);}

void sad()
{
	PutPixel(20,30,0x3f);
	PutPixel(21,30,0x3f);
	PutPixel(22,30,0x3f);
	PutPixel(20,29,0x3f);
	PutPixel(21,29,0x3f);
	PutPixel(22,29,0x3f);
	PutPixel(20,60,0x3f);
	PutPixel(21,60,0x3f);
	PutPixel(22,60,0x3f);
	PutPixel(20,59,0x3f);
	PutPixel(21,59,0x3f);
	PutPixel(22,59,0x3f);

	PutPixel(50,30,0x3f);
	PutPixel(50,31,0x3f);
	PutPixel(49,32,0x3f);
	PutPixel(48,33,0x3f);
	PutPixel(47,34,0x3f);
	PutPixel(47,35,0x3f);
	PutPixel(46,36,0x3f);
	PutPixel(45,37,0x3f);
	PutPixel(44,38,0x3f);
	PutPixel(44,39,0x3f);


	PutPixel(40,45,0x3f);

	PutPixel(50,59,0x3f);
}