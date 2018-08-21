
typedef struct SYSCALL_t_{
    // 32 bit
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    // 16 bit
    unsigned short ax;
    unsigned short bx;
    unsigned short cx;
    unsigned short dx;
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
extern "C" SYSCALL_t reg_syscall(
    // 32 bit
    unsigned int eax = 0,
    unsigned int ebx = 0,
    unsigned int ecx = 0,
    unsigned int edx = 0,
    // 16 bit
    unsigned short ax = 0,
    unsigned short bx = 0,
    unsigned short cx = 0,
    unsigned short dx = 0,
    // 8 bit
    unsigned char al = 0,
    unsigned char ah = 0,
    unsigned char bh = 0,
    unsigned char bl = 0,
    unsigned char ch = 0,
    unsigned char cl = 0,
    unsigned char dh = 0,
    unsigned char dl = 0
)
{
    SYSCALL_t ret;
    __asm__ __volatile__ ("movl %0, %%eax"::"dN"(eax));
    __asm__ __volatile__ ("movl %0, %%ebx"::"dN"(ebx));
    __asm__ __volatile__ ("movl %0, %%ecx"::"dN"(ecx));
    __asm__ __volatile__ ("movl %0, %%edx"::"dN"(edx));
    __asm__ __volatile__ ("mov %0, %%ax"::"dN"(ax));
    __asm__ __volatile__ ("mov %0, %%bx"::"dN"(bx));
    __asm__ __volatile__ ("mov %0, %%cx"::"dN"(cx));
    __asm__ __volatile__ ("mov %0, %%dx"::"dN"(dx));
    __asm__ __volatile__ ("mov %0, %%al"::"dN"(al));
    __asm__ __volatile__ ("mov %0, %%ah"::"dN"(ah));
    __asm__ __volatile__ ("mov %0, %%bl"::"dN"(bl));
    __asm__ __volatile__ ("mov %0, %%bh"::"dN"(bh));
    __asm__ __volatile__ ("mov %0, %%ch"::"dN"(ch));
    __asm__ __volatile__ ("mov %0, %%cl"::"dN"(cl));
    __asm__ __volatile__ ("mov %0, %%dh"::"dN"(dh));
    __asm__ __volatile__ ("mov %0, %%dl"::"dN"(dl));
    __asm__ __volatile__ ("int $0x80");
    __asm__ __volatile__ ("movl %%eax, %0":"=m"(ret.eax));
    __asm__ __volatile__ ("movl %%ebx, %0":"=m"(ret.ebx));
    __asm__ __volatile__ ("movl %%ecx, %0":"=m"(ret.ecx));
    __asm__ __volatile__ ("movl %%edx, %0":"=m"(ret.edx));
    __asm__ __volatile__ ("mov %%ax, %0":"=m"(ret.ax));
    __asm__ __volatile__ ("mov %%bx, %0":"=m"(ret.bx));
    __asm__ __volatile__ ("mov %%cx, %0":"=m"(ret.cx));
    __asm__ __volatile__ ("mov %%dx, %0":"=m"(ret.dx));            
    __asm__ __volatile__ ("mov %%ah, %0":"=m"(ret.ah));
    __asm__ __volatile__ ("mov %%al, %0":"=m"(ret.al));
    __asm__ __volatile__ ("mov %%bh, %0":"=m"(ret.bh));
    __asm__ __volatile__ ("mov %%bl, %0":"=m"(ret.bl));
    __asm__ __volatile__ ("mov %%ch, %0":"=m"(ret.ch));
    __asm__ __volatile__ ("mov %%cl, %0":"=m"(ret.cl));
    __asm__ __volatile__ ("mov %%dh, %0":"=m"(ret.dh));
    __asm__ __volatile__ ("mov %%dl, %0":"=m"(ret.dl));
    return ret;
}

extern "C" SYSCALL_t syscall(
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
    __asm__ __volatile__ ("movl %0, %%ebx"::"dN"(ebx));
    __asm__ __volatile__ ("movl %0, %%ecx"::"dN"(ecx));
    __asm__ __volatile__ ("movl %0, %%edx"::"dN"(edx));
    __asm__ __volatile__ ("mov %0, %%al"::"dN"(al));
    __asm__ __volatile__ ("mov %0, %%ax"::"dN"(ax));
    __asm__ __volatile__ ("int $0x80");
    __asm__ __volatile__ ("movl %%eax, %0":"=m"(ret.eax));
    __asm__ __volatile__ ("movl %%ebx, %0":"=m"(ret.ebx));
    __asm__ __volatile__ ("movl %%ecx, %0":"=m"(ret.ecx));
    __asm__ __volatile__ ("movl %%edx, %0":"=m"(ret.edx));
    __asm__ __volatile__ ("mov %%ah, %0":"=m"(ret.ah));
    __asm__ __volatile__ ("mov %%al, %0":"=m"(ret.al));
    __asm__ __volatile__ ("mov %%ax, %0":"=m"(ret.ax));
    return ret;
}
class NOS{
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
            syscall(0,0);
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
        unsigned char strlen(char* ch)
        {
            int_strlen(ch);
        }
        unsigned short strcmp(char* ch1, char* ch2)
        {
            int_strcmp(ch1, ch2);
        }
        unsigned char screen_width = syscall(2,0).ah;
        unsigned char screen_height = syscall(2,1).ah;
    private:
        unsigned int * (*get_syscalls)(void) = (unsigned int * (*)())syscall(0xFF).ecx;
        unsigned char (*int_strlen)(char* ch) = (unsigned char (*)(char* ch))get_syscalls()[8];
        unsigned short (*int_strcmp)(char* ch1, char* ch2) = (unsigned short (*)(char* ch1, char* ch2))get_syscalls()[11];
};

extern "C" void* NOS_create()
{
    return new NOS;
}
extern "C" void NOS_release(void* nos) 
{
   delete static_cast<NOS*>(nos);
}
extern "C" void NOS_put_ch(void* nos, char c,int x, int y, int text_c, int bg_c)
{
    static_cast<NOS*>(nos)->put_ch(c,x,y,text_c,bg_c);
}
extern "C" unsigned char NOS_Get_sw(void* nos)
{
    return static_cast<NOS*>(nos)->screen_width;
}
extern "C" unsigned char NOS_Get_sh(void* nos)
{
    return static_cast<NOS*>(nos)->screen_height;
}
extern "C" unsigned char NOS_strlen(void* nos, char* string)
{
    return static_cast<NOS*>(nos)->strlen(string);
}