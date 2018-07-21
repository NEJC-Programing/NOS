bits    32 
section         .text
        align   4
        dd      0x1BADB002
        dd      0x00
        dd      - (0x1BADB002+0x00)
        
global start
global entering_v86 
extern isr_install
extern irq_install
extern kmain            ; this function is gonna be located in our c code(kernel.c)
start:
    cli             ;clears the interrupts 
    call isr_install
    call irq_install
    mov esp, sys_stack
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


;iqr
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; 32: IRQ0
irq0:
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

; 33: IRQ1
irq1:
	cli
	push byte 0
	push byte 33
	jmp irq_common_stub

; 34: IRQ2
irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub

; 35: IRQ3
irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub

; 36: IRQ4
irq4:
	cli
	push byte 0
	push byte 36
	jmp irq_common_stub

; 37: IRQ5
irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub

; 38: IRQ6
irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub

; 39: IRQ7
irq7:
	cli
	push byte 0
	push byte 39
	jmp irq_common_stub

; 40: IRQ8
irq8:
	cli
	push byte 0
	push byte 40
	jmp irq_common_stub

; 41: IRQ9
irq9:
	cli
	push byte 0
	push byte 41
	jmp irq_common_stub

; 42: IRQ10
irq10:
	cli
	push byte 0
	push byte 42
	jmp irq_common_stub

; 43: IRQ11
irq11:
	cli
	push byte 0
	push byte 43
	jmp irq_common_stub

; 44: IRQ12
irq12:
	cli
	push byte 0
	push byte 44
	jmp irq_common_stub

; 45: IRQ13
irq13:
	cli
	push byte 0
	push byte 45
	jmp irq_common_stub

; 46: IRQ14
irq14:
	cli
	push byte 0
	push byte 46
	jmp irq_common_stub

; 47: IRQ15
irq15:
	cli
	push byte 0
	push byte 47
	jmp irq_common_stub

extern irq_handler

irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp

	push eax
	mov eax, irq_handler
	call eax
	pop eax

	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret


SECTION .bss
	resb 8192         ; This reserves 8KBytes of memory here
sys_stack: