bits    32
section         .text
        align   4
        dd      0x1BADB002
        dd      0x00
        dd      - (0x1BADB002+0x00)
        
global start
extern kmain            ; this function is gonna be located in our c code(kernel.c)
app_call_api:
        jmp os_start
        jmp print_string
        jmp print_ch
os_start:
        cli             ;clears the interrupts 
        call kmain      ;send processor to continue execution from the kamin funtion in c code
        hlt             ; halt the cpu(pause it from executing from this address

print_string:
        extern print
        call print

print_ch:
        extern printch
        call printch