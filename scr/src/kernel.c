#include "../include/kb.h"
#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/util.h"
#include "../include/shell.h"
#include "../include/fs.h"
int kmain()
{
	ata_init();
	Date_and_Time s = Get_Date_and_Time();
	printf("Date: %d/%d/%d\nTime: %d:%d %d\n",s.day,s.month,s.year,s.hour,s.minute,s.second);
	print("Press enter to continue");
	readStr();
	clearScreen();
	print("Welcome to NOS\nType \"help\" for help\n\n");
    launch_shell();
	//die("out");
}
