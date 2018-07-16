#include "../include/kb.h"
#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/util.h"
#include "../include/shell.h"
#include "../include/fs.h"
void kmain()
{
	clearScreen();
	printf("kmain = 0x%x\n\n",kmain);
	syscall_init();
	ata_init();
	Date_and_Time s = Get_Date_and_Time();
	printf("Date: %d/%d/%d\nTime: %d:%d Sec: %d\n",s.day,s.month,s.year,s.hour,s.minute,s.second);
	print("Press enter to continue");
	readStr();
	clearScreen();
	print("Welcome to NOS\nType \"help\" for help\n\n");
    launch_shell();
	//die("out");
}

void syscall()
{
	__asm__ __volatile__ ("add $0x2c, %esp");
	__asm__ __volatile__ ("pusha");
	int eax = 0;
	int ebx = 0;
	int ecx = 0;
	int edx = 0;
	__asm__ __volatile__ ("movl %%eax, %0":"=m"(eax));
	__asm__ __volatile__ ("movl %%ebx, %0":"=m"(ebx));
	__asm__ __volatile__ ("movl %%ecx, %0":"=m"(ecx));
	__asm__ __volatile__ ("movl %%edx, %0":"=m"(edx));
	switch(eax){
		case 0:
			switch(ebx){
				case 1:
					reboot();
					break;
				case 0:
					shutdown();
					break;
			}
			break;
		/*case 1:
			//char *str = malloc(edx + 1);
			string str;
			memory_copy(ebx,str,edx);
			str[edx+1]=0;
			print(str);
			break;*/
	}
}

void syscall_init()
{
	print("Registering syscall interface\n");
	set_int(0x80, (uint32_t)syscall);
}