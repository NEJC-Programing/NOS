[bits 16]

global _start
_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x8000      ; Stack pointer at SS:SP = 0x0000:0x8000
    mov [BOOT_DRIVE], dl; Boot drive passed to us by the BIOS
    mov dh, 17          ; Number of sectors (kernel.bin) to read from disk
                        ; 17*512 allows for a kernel.bin up to 8704 bytes
    mov bx, 0x9000      ; Load Kernel to ES:BX = 0x0000:0x9000

    call load_kernel
    call enable_A20

;   call graphics_mode  ; Uncomment if you want to switch to graphics mode 0x13
    lgdt [gdtr]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp CODE_SEG:init_pm

graphics_mode:
    mov ax, 0013h
    int 10h
    ret

load_kernel:
                        ; load DH sectors to ES:BX from drive DL
    push dx             ; Store DX on stack so later we can recall
                        ; how many sectors were request to be read ,
                        ; even if it is altered in the meantime
    mov ah , 0x02       ; BIOS read sector function
    mov al , dh         ; Read DH sectors
    mov ch , 0x00       ; Select cylinder 0
    mov dh , 0x00       ; Select head 0
    mov cl , 0x02       ; Start reading from second sector ( i.e.
                        ; after the boot sector )
    int 0x13            ; BIOS interrupt
    jc disk_error       ; Jump if error ( i.e. carry flag set )
    pop dx              ; Restore DX from the stack
    cmp dh , al         ; if AL ( sectors read ) != DH ( sectors expected )
    jne disk_error      ; display error message
    ret
disk_error :
    mov bx , ERROR_MSG
    call print_string
    hlt

; prints a null - terminated string pointed to by EDX
print_string :
    pusha
    push es                   ;Save ES on stack and restore when we finish

    push VIDEO_MEMORY_SEG     ;Video mem segment 0xb800
    pop es
    xor di, di                ;Video mem offset (start at 0)
print_string_loop :
    mov al , [ bx ] ; Store the char at BX in AL
    mov ah , WHITE_ON_BLACK ; Store the attributes in AH
    cmp al , 0 ; if (al == 0) , at end of string , so
    je print_string_done ; jump to done
    mov word [es:di], ax ; Store char and attributes at current
        ; character cell.
    add bx , 1 ; Increment BX to the next char in string.
    add di , 2 ; Move to next character cell in vid mem.
    jmp print_string_loop ; loop around to print the next char.

print_string_done :
    pop es                    ;Restore ES that was saved on entry
    popa
    ret ; Return from the function

enable_A20:
    call check_a20
    cmp ax, 1
    je enabled
    call a20_bios
    call check_a20
    cmp ax, 1
    je enabled
    call a20_keyboard
    call check_a20
    cmp ax, 1
    je enabled
    call a20_fast
    call check_a20
    cmp ax, 1
    je enabled
    mov bx, [ERROR]
    call print_string
enabled:
    ret

check_a20:
    pushf
    push ds
    push es
    push di
    push si

    cli
    xor ax, ax ; ax = 0
    mov es, ax
    not ax ; ax = 0xFFFF
    mov ds, ax
    mov di, 0x0500
    mov si, 0x0510
    mov al, byte [es:di]
    push ax
    mov al, byte [ds:si]
    push ax
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF
    cmp byte [es:di], 0xFF
    pop ax
    mov byte [ds:si], al
    pop ax
    mov byte [es:di], al
    mov ax, 0
    je check_a20__exit
    mov ax, 1

check_a20__exit:
    pop si
    pop di
    pop es
    pop ds
    popf
    ret

a20_bios:
    mov ax, 0x2401
    int 0x15
    ret

a20_fast:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

    [bits 32]
    [section .text]

a20_keyboard:
    cli

    call    a20wait
    mov     al,0xAD
    out     0x64,al
    call    a20wait
    mov     al,0xD0
    out     0x64,al
    call    a20wait2
    in      al,0x60
    push    eax
    call    a20wait
    mov     al,0xD1
    out     0x64,al
    call    a20wait
    pop     eax
    or      al,2
    out     0x60,al
    call    a20wait
    mov     al,0xAE
    out     0x64,al
    call    a20wait
    sti
    ret

a20wait:
    in      al,0x64
    test    al,2
    jnz     a20wait
    ret

a20wait2:
    in      al,0x64
    test    al,1
    jz      a20wait2
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

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call 0x9000
    cli
loopend:                                ;Infinite loop when finished
    hlt
    jmp loopend

[bits 16]
; Variables
ERROR            db "A20 Error!" , 0
ERROR_MSG        db "Error!" , 0
BOOT_DRIVE:      db 0

VIDEO_MEMORY_SEG equ 0xb800
WHITE_ON_BLACK   equ 0x0f
