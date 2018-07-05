#include "../include/shell.h"

void launch_shell()
{
	string prompt = (string) malloc(10);
	prompt = "NOS> ";
	string ch = (string) malloc(200); // util.h
	void * temp_mem = malloc(1024);
	int counter = 0;
	do
	{
		print(prompt);
			ch = clean(200);
		    //ch = readStr(); //
			memory_copy(readStr(), ch,200);

		    if(strEqle(ch, "help", 4))
		        help();
			else if(strEqle(ch, "prompt ", 7))
			{
				prompt = clean(10);
				prompt = first(remove_form_start(ch, 7),10);
				print("\n");
			}
			else if(strEqle(ch, "prompt", 6))
				print("\nuse: prompt [text]\n");
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
			else if (strEqle(ch, "div", 3))
				divide();
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
	print("\n div       : divide ");
	print("\n\n");
}

void divide(){
	string t = malloc(100);
	string u = malloc(100);
	print("\nwhat do you want do be the first #/... [");
	memory_copy(readStr(), t,100);
	print("\nwhat do you want do be the other .../# [");
	memory_copy(readStr(), u,100);
	double a = str_to_int(t);
	double b = str_to_int(u);
	a = a/b;
	print("\n Answer=");
	print(int_to_string(a));
	print("\n");
}