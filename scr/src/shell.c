#include "../include/shell.h"

void launch_shell()
{
	serial_init();
	string prompt = malloc(10);
	prompt = "NOS> ";
	string ch = (string) malloc(200); // util.h
	void * temp_mem = malloc(1024);
	int counter = 0;
	do
	{
		print(prompt);
		write_serial("H");
			ch = clean(200);
		    ch = readStr(); //memory_copy(readStr(), ch,100);

		    if(strEqle(ch, "help", 4))
		        help();
			else if(strEqle(ch, "prompt ", 7))
			{
				prompt = clean(10);
				prompt = first(remove_form_start(ch, 7),10);
				print("\n");
			}
			else if(strEqle(ch, "reboot", 6))
				reboot();
			else if(strEqle(ch, "shutdown", 8))
				shutdown();
			else if (strEqle(ch, "echo ", 5))
			{
				print("\n");
				print(remove_form_start(ch, 5));
				print("\n");
			}
			else if (strEqle(ch, "cls", 3))
				clearScreen();
			else if (strEqle(ch, "clear", 5))
				clearScreen();
			else if (strlength(ch) == 0)
				print("\n");
		    else
		    {
		        print("\n \"");
				print(ch);
				print("\" is not a command\n");
		    } 
	} while (!strEql(ch,"exit"));
}

void help()
{
	print("\n help      : Shows this messge");
	print("\n prompt    : Changes the prompt");
	print("\n reboot    : Reboots the pc");
	print("\n shutdown  : Shutdowns the pc");
	print("\n echo      : Prints output");
	print("\n cls\\clear : Clears Screen");
	print("\n\n");
}

int serial_received() {
   return inb(0x3f8 + 5) & 1;
}

char read_serial() {
   while (serial_received() == 0);
   return inb(0x3f8);
}

// Send

int is_transmit_empty() {
   return inb(0x3f8 + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);
   outb(0x3f8,a);
}

void serial_init() {
   outb(0x3f8 + 1, 0x00);
   outb(0x3f8 + 3, 0x80);
   outb(0x3f8 + 0, 0x03);
   outb(0x3f8 + 1, 0x00);
   outb(0x3f8 + 3, 0x03);
   outb(0x3f8 + 2, 0xC7);
   outb(0x3f8 + 4, 0x0B);
}