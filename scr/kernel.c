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
    while(1){
        //string noam = readStr();
        //print(noam);
        print("\n");
    }
}