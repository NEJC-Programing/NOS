#include "../include/shell.h"
string prompt = "NOS> ";
void launch_shell()
{
	string ch = (string) malloc(200); // util.h
	int counter = 0;
	do
	{
		print(prompt);
		    ch = readStr(); //memory_copy(readStr(), ch,100);
		    if(strEqle(ch, "help", 4))
		    {
		        help();
		    }
			else if(strEqle(ch, "prompt", 6))
			{
				string temp = remove_form_start(ch, 7);
				prompt = temp;
			}
			else if(strEqle(ch, "reboot", 6))
			{
				reboot();
			}
			else if(strEqle(ch, "shutdown", 8))
			{
				shutdown();
			}
		    else
		    {
		            print("\nBad command!\n");
		    } 
	} while (!strEql(ch,"exit"));
}

void help()
{
	print("\n help      : Shows this messge");
	print("\n prompt    : Changes the prompt");
	print("\n reboot    : Reboots the pc");
	print("\n shutdown  : Shutdowns the pc");
	print("\n\n");
}
void reboot()
{
    uint8 good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
}
void shutdown()
{
	//__asm__ __volatile__ ("outw %1, %0" : : "dN" ((uint16)0xB004), "a" ((uint16)0x2000));
	//for (const char *s = "Shutdown"; *s; ++s) {
  	//	outb(0x8900, *s);
	//}
	outb(0xf4, 0x00);
}

