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
    launch_shell();    
	print("\nout\n...");
}
