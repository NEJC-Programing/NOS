typedef struct SYSCALL_t_{
    // 32 bit
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
    unsigned int ebp;
    unsigned int esp;
    // 16 bit
    unsigned short ax;
    unsigned short bx;
    unsigned short cx;
    unsigned short dx;
    unsigned short si;
    unsigned short di;
    unsigned short bp;
    unsigned short sp;
    // 8 bit
    unsigned char al;
    unsigned char ah;
    unsigned char bh;
    unsigned char bl;
    unsigned char ch;
    unsigned char cl;
    unsigned char dh;
    unsigned char dl;
} SYSCALL_t;

/*
There are 8 32-bit registers: eax, ebx, ecx, edx, esi, edi, ebp, esp.
There are 8 16-bit registers: ax, bx, cx, dx, si, di, bp, sp.
There are 8 8-bit registers: ah, al, bh, bl, ch, cl, dh, dl.
*/
class NOS
{
    public:
        void put_ch(char c,int x, int y, int text_c, int bg_c)
        {
            syscall(0,4,x,y);
            syscall(0,3,text_c,bg_c);
            syscall(0,2,c,0);
        }
        void reboot()
        {
            syscall(0,1);
        }
        void shutdown()
        {
            syscall();
        }
        char toupper(char c)
        {
            return (char)syscall(1,0,c).ah;
        }
        char tolower(char c)
        {
            return (char)syscall(1,1,c).ah;
        }
        void memset(unsigned char dest, unsigned char val, unsigned int len)
        {
            syscall(1,2,dest,len,val);
        }
        unsigned char inb(unsigned short port)
        {
            return syscall(0,5,0,0,0,port).ah;
        }
        void outb(unsigned short port, unsigned char data)
        {
            syscall(0,6,0,0,data,port);
        }
    private:
        SYSCALL_t syscall(
            unsigned int eax = 0,
            unsigned int ebx = 0,
            unsigned int ecx = 0,
            unsigned int edx = 0,
            unsigned char al = 0,
            unsigned short ax = 0
        )
        {
            SYSCALL_t ret;
            __asm__ __volatile__ ("movl %0, %%eax"::"dN"(eax));
            __asm__ __volatile__ ("movl %0, %%ebx"::"dN"(ecx));
            __asm__ __volatile__ ("movl %0, %%ecx"::"dN"(ecx));
            __asm__ __volatile__ ("movl %0, %%edx"::"dN"(ecx));
            __asm__ __volatile__ ("mov %0, %%al"::"dN"(al));
            __asm__ __volatile__ ("mov %0, %%ax"::"dN"(ax));
            __asm__ __volatile__ ("int 0x80");
            __asm__ __volatile__ ("movl %%eax, %0":"=m"(ret.eax));
	        __asm__ __volatile__ ("movl %%ebx, %0":"=m"(ret.ebx));
	        __asm__ __volatile__ ("movl %%ecx, %0":"=m"(ret.ecx));
	        __asm__ __volatile__ ("movl %%edx, %0":"=m"(ret.edx));
            __asm__ __volatile__ ("mov %%ah, %0":"=m"(ret.ah));
            __asm__ __volatile__ ("mov %%al, %0":"=m"(ret.al));
            __asm__ __volatile__ ("mov %%ax, %0":"=m"(ret.ax));
            return ret;
        }
}