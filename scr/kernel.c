#include "kernel.h"
void kstart(char *bootarg)
{
    clearScreen();
    print("noam\nnoam");
    int i = 0;
    while(1){
        print(i);
        print("\n----\n");
        i++;
    }
}