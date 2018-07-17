#include "nos.h"
extern int main(int argc, char const *argv[]);

int init(int argc, char *argv[])
{
    __asm__ __volatile__ ("int $0x80": :"a"(3));
    int ecx = 0;
    __asm__ __volatile__ ("movl %%ecx, %0":"=m"(ecx));
    uint32_t (*get_syscalls)(void);
    get_syscalls = (uint32_t)ecx;
    set_libc(get_syscalls());
    main(argc,argv);
}

void set_libc(uint32_t sc[100])
{
    print = sc[0];
    print_colored = sc[1];
    printf = sc[3];
    readStr = sc[4];
    malloc = sc[5];
    outb = sc[6];
    inb = sc[7];
    strlen = sc[8];
    toupper = sc[9];
    tolower = sc[10];
    strcmp = sc[11];
    memcpy = sc[12];
    memset = sc[13];
    PutPixel = sc[14];
    PutRect = sc[15];
    PutCercle = sc[16];
    reboot = sc[17];
    shutdown = sc[18];
    panic = sc[19];
    delay = sc[20];
    clearScreen = sc[21];
    SetCursor = sc[22];
    set_screen_color = sc[23];
}
