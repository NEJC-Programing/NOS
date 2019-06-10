#include <screen.h>
#include <ver.h>
#include <types.h>

void kshell(void)
{
    printf("\nKShell Version %d.%d Build %d\n\n", MAJOR, MINOR, BUILD);
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