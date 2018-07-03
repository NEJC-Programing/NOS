#include "../include/shell.h"
void launch_shell()
{
	string prompt = malloc(10);
	prompt = "NOS> ";
	string ch = (string) malloc(200); // util.h
	void * temp_mem = malloc(1024);
	int counter = 0;
	do
	{
		print(prompt);
		    ch = readStr(); //memory_copy(readStr(), ch,100);
		    if(strEqle(ch, "help", 4))
		    {
		        help();
		    }
			else if(strEqle(ch, "prompt ", 7))
			{
				prompt = clean(10);
				prompt = remove_form_start(ch, 7);
			}
			else if(strEqle(ch, "reboot", 6))
			{
				reboot();
			}
			else if(strEqle(ch, "shutdown", 8))
			{
				shutdown();
			}
			else if (strEqle(ch, "echo ", 5))
			{
				print("\n");
				print(remove_form_start(ch, 5));
				print("\n");
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
	print("\n echo      : Prints output");
	print("\n\n");
}

