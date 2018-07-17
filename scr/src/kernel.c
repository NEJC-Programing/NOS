#include "../include/kb.h"
#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/util.h"
#include "../include/shell.h"
#include "../include/fs.h"
void start(void);
uint32_t syscalls[100] = {
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
};
int last_syscall = 0;
void kmain()
{
	clearScreen();
	printf("start = 0x%x  kmain = 0x%x\n",start,kmain);
	syscall_init();
	// init vfs and devs
	Date_and_Time s = Get_Date_and_Time();
	printf("Date: %d/%d/%d\nTime: %d:%d \nSec: %d\n",s.day,s.month,s.year,s.hour,s.minute,s.second);
	delay(3);
	print("Press enter to continue\n");
	readStr();
	clearScreen();
	print("Welcome to NOS\nType \"help\" for help\n\n");
    launch_shell();
	//die("out");
}

uint32_t * Get_Syscall_Table()
{
	return syscalls;
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
		case 3:
			ecx = (int)Get_Syscall_Table;
			__asm__ __volatile__ ("movl %0, %%ecx"::"dN"(ecx));
			break;
	}
}



void add_syscall(uint32_t syscall_function)
{
	if(last_syscall == 100)return;
	syscalls[last_syscall] = syscall_function;
	last_syscall++;
}

void syscall_init()
{
	print("Registering syscall interface\n");
	add_syscall((uint32_t)print);
	add_syscall((uint32_t)print_colored);
	add_syscall((uint32_t)printf); 
	add_syscall((uint32_t)readStr);
	add_syscall((uint32_t)malloc);
	add_syscall((uint32_t)outb);
	add_syscall((uint32_t)inb);
	add_syscall((uint32_t)strlen);
	add_syscall((uint32_t)toupper);
	add_syscall((uint32_t)tolower);
	add_syscall((uint32_t)strcmp);
	add_syscall((uint32_t)memcpy);
	add_syscall((uint32_t)memset);
	add_syscall((uint32_t)strcpy);
	add_syscall((uint32_t)PutPixel);
	add_syscall((uint32_t)PutRect);
	add_syscall((uint32_t)PutCercle);
	add_syscall((uint32_t)reboot);
	add_syscall((uint32_t)shutdown);
	add_syscall((uint32_t)panic);
	add_syscall((uint32_t)delay);
	add_syscall((uint32_t)clearScreen);
	add_syscall((uint32_t)SetCursor);
	add_syscall((uint32_t)set_screen_color_from_color_code);
	set_int(0x80, (uint32_t)syscall);
}