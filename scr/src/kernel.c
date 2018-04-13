#include "../include/kb.h"
#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/util.h"
#include "../include/shell.h"
#include "../include/fs.h"
kmain()
{
	isr_install();
	clearScreen();
	//fat32OpenDisk("noam", 1);
	print("Hi and Welcome to NIDOS operating system\nPlease enter a command\n");
    launch_shell(0);    
	print("\nout\n...");
}
