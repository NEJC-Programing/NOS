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
    jmp kmain      ;send processor to continue execution from the kamin funtion in c code
    extern die
    call die

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


global restoreflags
restoreflags:
push      ebp
mov       ebp,esp
mov	  eax,[ebp+8]
push	  eax
popf	
pop       ebp
ret 
global storeflags
storeflags:
push      ebp
mov       ebp,esp
push      ecx
pushf	
pop	 eax
mov	  [ebp-4],eax
mov       eax,[ebp-4]
mov       edx,[ebp+8]
mov       [edx],eax
pop       ecx
pop       ebp
ret 

global inw
global inl
global outw
global outl

inw:
        xor eax, eax
        xor edx, edx
        mov dx, word [esp + 4]
        in ax, dx
        ret

inl:
        xor eax, eax
        xor edx, edx
        mov dx, word [esp + 4]
        in eax, dx
        ret
outw:
        xor eax, eax
        xor edx, edx
        mov dx, word [esp + 4]
        mov ax, word [esp + 8]
        out dx, ax
        ret

outl:
        xor eax, eax
        xor edx, edx
        mov dx, word [esp + 4]
        mov eax, [esp + 8]
        out dx, eax
        ret
