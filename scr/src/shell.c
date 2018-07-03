#include "../include/shell.h"
string old;
string new;
string ch;
void launch_shell()
{
	string prompt = malloc(10);
	prompt = "NOS> ";
	ch = (string) malloc(200); // util.h
	void * temp_mem = malloc(1024);
	int counter = 0;
	do
	{
		print(prompt);
		    ch = readStrcmd(); //memory_copy(readStr(), ch,100);

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
			old = clean(200);
			old = ch;
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

void switch_old(){
	new = clean(200);
	new = ch;
	ch=clean(200);
	ch = old;
}
void switch_new(){
	new = clean(200);
	new = ch;
	ch=clean(200);
	ch = old;
}
void set_ch(int i, char ch){
	ch[i] = ch;
}