
;   call graphics_mode  ; Uncomment if you want to switch to graphics mode 0x13
    lgdt [gdtr]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp sss

graphics_mode:
    mov ax, 0013h
    int 10h
    ret


gdt_start:
    dd 0                ; null descriptor--just fill 8 bytes
    dd 0

gdt_code:
    dw 0FFFFh           ; limit low
    dw 0                ; base low
    db 0                ; base middle
    db 10011010b        ; access
    db 11001111b        ; granularity
    db 0                ; base high

gdt_data:
    dw 0FFFFh           ; limit low (Same as code)
    dw 0                ; base low
    db 0                ; base middle
    db 10010010b        ; access
    db 11001111b        ; granularity
    db 0                ; base high
end_of_gdt:

gdtr:
    dw end_of_gdt - gdt_start - 1   ; limit (Size of GDT)
    dd gdt_start        ; base of GDT

    CODE_SEG equ gdt_code - gdt_start
    DATA_SEG equ gdt_data - gdt_start


sss:
bits    32
section         .text
        align   4
        dd      0x1BADB002
        dd      0x00
        dd      - (0x1BADB002+0x00)
        
global start
global entering_v86
extern kmain            ; this function is gonna be located in our c code(kernel.c)
start:
        cli             ;clears the interrupts 
        call kmain      ;send processor to continue execution from the kamin funtion in c code
        hlt             ;halt the cpu(pause it from executing from this address

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

