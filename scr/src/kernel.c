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
	//int s = 1/0;
    launch_shell();    
	print("\nout\n...");
}
