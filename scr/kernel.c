#include "kernel.h"
void kstart(char *bootarg)
{
    clearScreen();
    print("noam\nNoam is The Best");
    int i = 1;
    while(i){
        if(inb(0x60)==2){
            i=0;
        }
    }
    print("\rNHTHEBEST         \n");
    while(i){
        if(inb(0x60)==3){
            i=0;
        }
    }
    clearScreen();
    char text[] = "StringX";
    while(1){
        text[6] = inb(0x60)+ '0';
        print(text);
        print("\n");
    }
}