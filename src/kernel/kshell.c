#include <screen.h>
#include <ver.h>
#include <types.h>
#include <keyboard.h>
#include <libk.h>
#include <NOS.h>

void help(char* cmd);
void exit(char* cmd);



void kshell(void)
{
    printf("\nKShell Version %d.%d Build %d\n\n", MAJOR, MINOR, BUILD);
    bool RUNNING = true;
    while (RUNNING)
    {
        printf("NOS > ");
        char* buff = readStr();
        print("\n");
        if (strncmp(buff, "exit",4) == 0){
        RUNNING = false;
        exit(buff);}
        else if (strncmp(buff, "help",4) == 0)
        help(buff);
        else if (strncmp(buff, "cls",3) == 0)
        clearScreen();
        else if (strncmp(buff, "nos",3) == 0)
        NOS();
        else if (strncmp(buff, "color", 5)==0)
        color(buff);
        else{
        print_colored("ERROR", 0x04);
        printf(" \"%s\" is not a command\n", buff);
        }
    }
    printch('\n');
}

void color(char* cmd)
{
    char* bb = cmd+6;
    
}

void help(char* cmd)
{
    int cmdleg = strlen(cmd);
    char* args = malloc(cmdleg-4);
    memcpy(args, cmd+4 , cmdleg-4);
    print(args);
}

void exit(char* cmd)
{
    clearScreen();
    print(cmd);
}