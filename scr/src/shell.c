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
				print("\n");
				print(temp);
				print("\n");
				print(int_to_string(strlength(temp)));
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
	print("\n\n");
}

