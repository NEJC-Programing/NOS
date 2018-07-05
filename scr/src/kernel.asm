bits    32 
section         .text
        align   4
        dd      0x1BADB002
        dd      0x00
        dd      - (0x1BADB002+0x00)
        
global start
global entering_v86 
extern isr_install
extern kmain            ; this function is gonna be located in our c code(kernel.c)
start:
    cli             ;clears the interrupts 
    call isr_install
    call kmain      ;send processor to continue execution from the kamin funtion in c code
    extern panic
    call panic
    mov ax, 0013h
    int 10h

; extern void entering_v86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip);
entering_v86:
   mov ebp, esp               ; save stack pointer
   push dword  [ebp+4]        ; ss
   push dword  [ebp+8]        ; esp
   pushfd                     ; eflags
   or dword [esp], (1 << 17)  ; set VM flags
   push dword [ebp+12]        ; cs
   push dword  [ebp+16]       ; eip
   iret
