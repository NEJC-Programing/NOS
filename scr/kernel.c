#include "kernel.h"
void kstart(void * bootarg)
{
    int ss =0;
    clearScreen();
    print("NHTHEBEST\nThis is NOS");
    int i =0;
    while(i!=300000000){
        i++;
    }
    print("\rThis is j");
    i =0;
    while(i!=300000000){
        i++;
    }
<<<<<<< HEAD
    clearScreen();
    char text[] = "StringX";
    while(1){
        text[6] = inb(0x60)+ '0';
        print(text);
        print("\n");
=======
    int f = 100000000;
    while(1){
    clearScreen();
    i =0;
    while(i!=f){
        i++;
    }
    nos();
    i =0;
    while(i!=f){
        i++;
    }
    f = f/2;
    if (ss <= 10){
    jos();
    i =0;
    while(i!=f){
        i++;
>>>>>>> 35b0669d4e1487bbf3bc85f3035090843218a8ff
    }
    f = f/2;}
    ss++;
    if (ss == 20){
        clearScreen();
        nos();
        asm("hlt");
    }
    }
}
void nos(){
    print("$$\\   $$\\  $$$$$$\\   $$$$$$\\  \n");
    print("$$$\\  $$ |$$  __$$\\ $$  __$$\\ \n");
    print("$$$$\\ $$ |$$ /  $$ |$$ /  \\__|\n");
    print("$$ $$\\$$ |$$ |  $$ |\\$$$$$$\\  \n");
    print("$$ \\$$$$ |$$ |  $$ | \\____$$\\ \n");
    print("$$ |\\$$$ |$$ |  $$ |$$\\   $$ |\n");
    print("$$ | \\$$ | $$$$$$  |\\$$$$$$  |\n");
    print("\\__|  \\__| \\______/  \\______/ \n");
}
void jos(){
    clearScreen();
    print("   $$$$$\\  $$$$$$\\   $$$$$$\\  \n");
    print("   \\__$$ |$$  __$$\\ $$  __$$\\ \n");
    print("      $$ |$$ /  $$ |$$ /  \\__|\n");
    print("      $$ |$$ |  $$ |\\$$$$$$\\  \n");
    print("$$\\   $$ |$$ |  $$ | \\____$$\\ \n");
    print("$$ |  $$ |$$ |  $$ |$$\\   $$ |\n");
    print("\\$$$$$$  | $$$$$$  |\\$$$$$$  |\n");
    print(" \\______/  \\______/  \\______/ \n");

}