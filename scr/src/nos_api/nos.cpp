typedef struct SYSCALL_t_{
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
} SYSCALL_t;

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
            syscall(0,1,0,0);
        }
        void shutdown()
        {
            syscall(0,0,0,0);
        }
        char toupper(char c)
        {
            SYSCALL_t data = syscall(1,0,c,0);
            return (char)data.ecx;
        }
        char tolower(char c)
        {
            SYSCALL_t data = syscall(1,1,c,0);
            return (char)data.ecx;
        }
    private:
        SYSCALL_t syscall(unsigned int eax,unsigned int ebx,unsigned int ecx,unsigned int edx)
        {
            __asm__ __volatile__ ("movl %0, %%eax"::"dN"(eax));
            __asm__ __volatile__ ("movl %0, %%ebx"::"dN"(ecx));
            __asm__ __volatile__ ("movl %0, %%ecx"::"dN"(ecx));
            __asm__ __volatile__ ("movl %0, %%edx"::"dN"(ecx));
            __asm__ __volatile__ ("int 0x80");
            SYSCALL_t ret;
            __asm__ __volatile__ ("movl %%eax, %0":"=m"(ret.eax));
	        __asm__ __volatile__ ("movl %%ebx, %0":"=m"(ret.ebx));
	        __asm__ __volatile__ ("movl %%ecx, %0":"=m"(ret.ecx));
	        __asm__ __volatile__ ("movl %%edx, %0":"=m"(ret.edx));
            return ret;
        }
}