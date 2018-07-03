#include "../include/kb.h"
#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/util.h"
#include "../include/shell.h"
#include "../include/fs.h"
int kmain()
{
	clearScreen();
	print("Welcome to NOS\nType \"help\" for help\n\n");
    launch_shell();
	//die("out");
}
