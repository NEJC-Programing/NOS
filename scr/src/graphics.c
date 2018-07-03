#include "../include/graphics.h"
void test()
{
    char * screen = (char)0xA0000;
    for (int i = 0; i<=10000; i++)
        screen[i] = 0x02;
}
