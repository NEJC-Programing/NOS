#include <screen.h>
#include <ver.h>
#include <types.h>

void kshell(void)
{
    printf("\nKShell v %d.%d %d\n", MAJOR, MINOR, BUILD);
    bool RUNNING = true;
    while (RUNNING)
    {
        printf("NOS > ");
        char* buff = "exit";//readstring();

        if (buff == "exit")
        RUNNING = false;
    }
    printch('\n');
}