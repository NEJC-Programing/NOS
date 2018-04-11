; Disassembly of file: util.o
; Thu Apr 12 09:53:24 2018
; Mode: 32 bits
; Syntax: YASM/NASM
; Instruction set: 80386


global memory_copy: 
global memory_set: 
global int_to_ascii: 
global int_to_string: 
global str_to_int: 
global malloc: 
global __x86.get_pc_thunk.ax: 
global __x86.get_pc_thunk.bx: 

extern __stack_chk_fail_local                           ; near
extern strlength                                        ; near
extern _GLOBAL_OFFSET_TABLE_                            ; byte


SECTION .text   align=1                         ; section number 1, code

memory_copy:;  begin
        push    ebp                                     ; 0000 _ 55
        mov     ebp, esp                                ; 0001 _ 89. E5
        sub     esp, 16                                 ; 0003 _ 83. EC, 10
        call    __x86.get_pc_thunk.ax                   ; 0006 _ E8, FFFFFFFC(rel)
        add     eax, _GLOBAL_OFFSET_TABLE_-$            ; 000B _ 05, 00000001(GOT r)
        mov     dword [ebp-4H], 0                       ; 0010 _ C7. 45, FC, 00000000
        jmp     ?_002                                   ; 0017 _ EB, 19

?_001:  mov     edx, dword [ebp-4H]                     ; 0019 _ 8B. 55, FC
        mov     eax, dword [ebp+8H]                     ; 001C _ 8B. 45, 08
        add     eax, edx                                ; 001F _ 01. D0
        mov     ecx, dword [ebp-4H]                     ; 0021 _ 8B. 4D, FC
        mov     edx, dword [ebp+0CH]                    ; 0024 _ 8B. 55, 0C
        add     edx, ecx                                ; 0027 _ 01. CA
        movzx   eax, byte [eax]                         ; 0029 _ 0F B6. 00
        mov     byte [edx], al                          ; 002C _ 88. 02
        add     dword [ebp-4H], 1                       ; 002E _ 83. 45, FC, 01
?_002:  mov     eax, dword [ebp-4H]                     ; 0032 _ 8B. 45, FC
        cmp     eax, dword [ebp+10H]                    ; 0035 _ 3B. 45, 10
        jl      ?_001                                   ; 0038 _ 7C, DF
        nop                                             ; 003A _ 90
        leave                                           ; 003B _ C9
        ret                                             ; 003C _ C3
; memory_copy End of 

memory_set:;  begin
        push    ebp                                     ; 003D _ 55
        mov     ebp, esp                                ; 003E _ 89. E5
        sub     esp, 20                                 ; 0040 _ 83. EC, 14
        call    __x86.get_pc_thunk.ax                   ; 0043 _ E8, FFFFFFFC(rel)
        add     eax, _GLOBAL_OFFSET_TABLE_-$            ; 0048 _ 05, 00000001(GOT r)
        mov     eax, dword [ebp+0CH]                    ; 004D _ 8B. 45, 0C
        mov     byte [ebp-14H], al                      ; 0050 _ 88. 45, EC
        mov     eax, dword [ebp+8H]                     ; 0053 _ 8B. 45, 08
        mov     dword [ebp-4H], eax                     ; 0056 _ 89. 45, FC
        jmp     ?_004                                   ; 0059 _ EB, 13

?_003:  mov     eax, dword [ebp-4H]                     ; 005B _ 8B. 45, FC
        lea     edx, [eax+1H]                           ; 005E _ 8D. 50, 01
        mov     dword [ebp-4H], edx                     ; 0061 _ 89. 55, FC
        movzx   edx, byte [ebp-14H]                     ; 0064 _ 0F B6. 55, EC
        mov     byte [eax], dl                          ; 0068 _ 88. 10
        sub     dword [ebp+10H], 1                      ; 006A _ 83. 6D, 10, 01
?_004:  cmp     dword [ebp+10H], 0                      ; 006E _ 83. 7D, 10, 00
        jnz     ?_003                                   ; 0072 _ 75, E7
        nop                                             ; 0074 _ 90
        leave                                           ; 0075 _ C9
        ret                                             ; 0076 _ C3
; memory_set End of 

int_to_ascii:;  begin
        push    ebp                                     ; 0077 _ 55
        mov     ebp, esp                                ; 0078 _ 89. E5
        sub     esp, 16                                 ; 007A _ 83. EC, 10
        call    __x86.get_pc_thunk.ax                   ; 007D _ E8, FFFFFFFC(rel)
        add     eax, _GLOBAL_OFFSET_TABLE_-$            ; 0082 _ 05, 00000001(GOT r)
        mov     eax, dword [ebp+8H]                     ; 0087 _ 8B. 45, 08
        mov     dword [ebp-4H], eax                     ; 008A _ 89. 45, FC
        cmp     dword [ebp-4H], 0                       ; 008D _ 83. 7D, FC, 00
        jns     ?_005                                   ; 0091 _ 79, 03
        neg     dword [ebp+8H]                          ; 0093 _ F7. 5D, 08
?_005:  mov     dword [ebp-8H], 0                       ; 0096 _ C7. 45, F8, 00000000
?_006:  mov     ecx, dword [ebp+8H]                     ; 009D _ 8B. 4D, 08
        mov     edx, 1717986919                         ; 00A0 _ BA, 66666667
        mov     eax, ecx                                ; 00A5 _ 89. C8
        imul    edx                                     ; 00A7 _ F7. EA
        sar     edx, 2                                  ; 00A9 _ C1. FA, 02
        mov     eax, ecx                                ; 00AC _ 89. C8
        sar     eax, 31                                 ; 00AE _ C1. F8, 1F
        sub     edx, eax                                ; 00B1 _ 29. C2
        mov     eax, edx                                ; 00B3 _ 89. D0
        shl     eax, 2                                  ; 00B5 _ C1. E0, 02
        add     eax, edx                                ; 00B8 _ 01. D0
        add     eax, eax                                ; 00BA _ 01. C0
        sub     ecx, eax                                ; 00BC _ 29. C1
        mov     edx, ecx                                ; 00BE _ 89. CA
        mov     eax, edx                                ; 00C0 _ 89. D0
        lea     ecx, [eax+30H]                          ; 00C2 _ 8D. 48, 30
        mov     eax, dword [ebp-8H]                     ; 00C5 _ 8B. 45, F8
        lea     edx, [eax+1H]                           ; 00C8 _ 8D. 50, 01
        mov     dword [ebp-8H], edx                     ; 00CB _ 89. 55, F8
        mov     edx, eax                                ; 00CE _ 89. C2
        mov     eax, dword [ebp+0CH]                    ; 00D0 _ 8B. 45, 0C
        add     eax, edx                                ; 00D3 _ 01. D0
        mov     edx, ecx                                ; 00D5 _ 89. CA
        mov     byte [eax], dl                          ; 00D7 _ 88. 10
        mov     ecx, dword [ebp+8H]                     ; 00D9 _ 8B. 4D, 08
        mov     edx, 1717986919                         ; 00DC _ BA, 66666667
        mov     eax, ecx                                ; 00E1 _ 89. C8
        imul    edx                                     ; 00E3 _ F7. EA
        sar     edx, 2                                  ; 00E5 _ C1. FA, 02
        mov     eax, ecx                                ; 00E8 _ 89. C8
        sar     eax, 31                                 ; 00EA _ C1. F8, 1F
        sub     edx, eax                                ; 00ED _ 29. C2
        mov     eax, edx                                ; 00EF _ 89. D0
        mov     dword [ebp+8H], eax                     ; 00F1 _ 89. 45, 08
        cmp     dword [ebp+8H], 0                       ; 00F4 _ 83. 7D, 08, 00
        jg      ?_006                                   ; 00F8 _ 7F, A3
        cmp     dword [ebp-4H], 0                       ; 00FA _ 83. 7D, FC, 00
        jns     ?_007                                   ; 00FE _ 79, 13
        mov     eax, dword [ebp-8H]                     ; 0100 _ 8B. 45, F8
        lea     edx, [eax+1H]                           ; 0103 _ 8D. 50, 01
        mov     dword [ebp-8H], edx                     ; 0106 _ 89. 55, F8
        mov     edx, eax                                ; 0109 _ 89. C2
        mov     eax, dword [ebp+0CH]                    ; 010B _ 8B. 45, 0C
        add     eax, edx                                ; 010E _ 01. D0
        mov     byte [eax], 45                          ; 0110 _ C6. 00, 2D
?_007:  mov     edx, dword [ebp-8H]                     ; 0113 _ 8B. 55, F8
        mov     eax, dword [ebp+0CH]                    ; 0116 _ 8B. 45, 0C
        add     eax, edx                                ; 0119 _ 01. D0
        mov     byte [eax], 0                           ; 011B _ C6. 00, 00
        nop                                             ; 011E _ 90
        leave                                           ; 011F _ C9
        ret                                             ; 0120 _ C3
; int_to_ascii End of 

int_to_string:;  begin
        push    ebp                                     ; 0121 _ 55
        mov     ebp, esp                                ; 0122 _ 89. E5
        push    ebx                                     ; 0124 _ 53
        sub     esp, 36                                 ; 0125 _ 83. EC, 24
        call    __x86.get_pc_thunk.bx                   ; 0128 _ E8, FFFFFFFC(rel)
        add     ebx, _GLOBAL_OFFSET_TABLE_-$            ; 012D _ 81. C3, 00000002(GOT r)
        sub     esp, 12                                 ; 0133 _ 83. EC, 0C
        push    50                                      ; 0136 _ 6A, 32
        call    malloc                                  ; 0138 _ E8, FFFFFFFC(rel)
        add     esp, 16                                 ; 013D _ 83. C4, 10
        mov     dword [ebp-10H], eax                    ; 0140 _ 89. 45, F0
        sub     esp, 8                                  ; 0143 _ 83. EC, 08
        push    dword [ebp-10H]                         ; 0146 _ FF. 75, F0
        push    dword [ebp+8H]                          ; 0149 _ FF. 75, 08
        call    int_to_ascii                            ; 014C _ E8, FFFFFFFC(rel)
        add     esp, 16                                 ; 0151 _ 83. C4, 10
        sub     esp, 12                                 ; 0154 _ 83. EC, 0C
        push    dword [ebp-10H]                         ; 0157 _ FF. 75, F0
        call    strlength                               ; 015A _ E8, FFFFFFFC(PLT r)
        add     esp, 16                                 ; 015F _ 83. C4, 10
        mov     dword [ebp-0CH], eax                    ; 0162 _ 89. 45, F4
        mov     dword [ebp-18H], 0                      ; 0165 _ C7. 45, E8, 00000000
        mov     eax, dword [ebp-0CH]                    ; 016C _ 8B. 45, F4
        sub     eax, 1                                  ; 016F _ 83. E8, 01
        mov     dword [ebp-14H], eax                    ; 0172 _ 89. 45, EC
        jmp     ?_009                                   ; 0175 _ EB, 39

?_008:  mov     edx, dword [ebp-18H]                    ; 0177 _ 8B. 55, E8
        mov     eax, dword [ebp-10H]                    ; 017A _ 8B. 45, F0
        add     eax, edx                                ; 017D _ 01. D0
        movzx   eax, byte [eax]                         ; 017F _ 0F B6. 00
        mov     byte [ebp-19H], al                      ; 0182 _ 88. 45, E7
        mov     edx, dword [ebp-14H]                    ; 0185 _ 8B. 55, EC
        mov     eax, dword [ebp-10H]                    ; 0188 _ 8B. 45, F0
        add     eax, edx                                ; 018B _ 01. D0
        mov     ecx, dword [ebp-18H]                    ; 018D _ 8B. 4D, E8
        mov     edx, dword [ebp-10H]                    ; 0190 _ 8B. 55, F0
        add     edx, ecx                                ; 0193 _ 01. CA
        movzx   eax, byte [eax]                         ; 0195 _ 0F B6. 00
        mov     byte [edx], al                          ; 0198 _ 88. 02
        mov     edx, dword [ebp-14H]                    ; 019A _ 8B. 55, EC
        mov     eax, dword [ebp-10H]                    ; 019D _ 8B. 45, F0
        add     edx, eax                                ; 01A0 _ 01. C2
        movzx   eax, byte [ebp-19H]                     ; 01A2 _ 0F B6. 45, E7
        mov     byte [edx], al                          ; 01A6 _ 88. 02
        add     dword [ebp-18H], 1                      ; 01A8 _ 83. 45, E8, 01
        sub     dword [ebp-14H], 1                      ; 01AC _ 83. 6D, EC, 01
?_009:  mov     eax, dword [ebp-0CH]                    ; 01B0 _ 8B. 45, F4
        mov     edx, eax                                ; 01B3 _ 89. C2
        shr     edx, 31                                 ; 01B5 _ C1. EA, 1F
        add     eax, edx                                ; 01B8 _ 01. D0
        sar     eax, 1                                  ; 01BA _ D1. F8
        mov     ecx, eax                                ; 01BC _ 89. C1
        mov     eax, dword [ebp-0CH]                    ; 01BE _ 8B. 45, F4
        cdq                                             ; 01C1 _ 99
        shr     edx, 31                                 ; 01C2 _ C1. EA, 1F
        add     eax, edx                                ; 01C5 _ 01. D0
        and     eax, 01H                                ; 01C7 _ 83. E0, 01
        sub     eax, edx                                ; 01CA _ 29. D0
        add     eax, ecx                                ; 01CC _ 01. C8
        cmp     dword [ebp-18H], eax                    ; 01CE _ 39. 45, E8
        jl      ?_008                                   ; 01D1 _ 7C, A4
        mov     eax, dword [ebp-10H]                    ; 01D3 _ 8B. 45, F0
        mov     ebx, dword [ebp-4H]                     ; 01D6 _ 8B. 5D, FC
        leave                                           ; 01D9 _ C9
        ret                                             ; 01DA _ C3
; int_to_string End of 

str_to_int:;  begin
        push    ebp                                     ; 01DB _ 55
        mov     ebp, esp                                ; 01DC _ 89. E5
        push    ebx                                     ; 01DE _ 53
        sub     esp, 20                                 ; 01DF _ 83. EC, 14
        call    __x86.get_pc_thunk.ax                   ; 01E2 _ E8, FFFFFFFC(rel)
        add     eax, _GLOBAL_OFFSET_TABLE_-$            ; 01E7 _ 05, 00000001(GOT r)
        mov     dword [ebp-18H], 0                      ; 01EC _ C7. 45, E8, 00000000
        mov     dword [ebp-14H], 1                      ; 01F3 _ C7. 45, EC, 00000001
        sub     esp, 12                                 ; 01FA _ 83. EC, 0C
        push    dword [ebp+8H]                          ; 01FD _ FF. 75, 08
        mov     ebx, eax                                ; 0200 _ 89. C3
        call    strlength                               ; 0202 _ E8, FFFFFFFC(PLT r)
        add     esp, 16                                 ; 0207 _ 83. C4, 10
        mov     dword [ebp-0CH], eax                    ; 020A _ 89. 45, F4
        mov     eax, dword [ebp-0CH]                    ; 020D _ 8B. 45, F4
        sub     eax, 1                                  ; 0210 _ 83. E8, 01
        mov     dword [ebp-10H], eax                    ; 0213 _ 89. 45, F0
        jmp     ?_011                                   ; 0216 _ EB, 2B

?_010:  mov     edx, dword [ebp-10H]                    ; 0218 _ 8B. 55, F0
        mov     eax, dword [ebp+8H]                     ; 021B _ 8B. 45, 08
        add     eax, edx                                ; 021E _ 01. D0
        movzx   eax, byte [eax]                         ; 0220 _ 0F B6. 00
        movsx   eax, al                                 ; 0223 _ 0F BE. C0
        sub     eax, 48                                 ; 0226 _ 83. E8, 30
        imul    eax, dword [ebp-14H]                    ; 0229 _ 0F AF. 45, EC
        add     dword [ebp-18H], eax                    ; 022D _ 01. 45, E8
        mov     edx, dword [ebp-14H]                    ; 0230 _ 8B. 55, EC
        mov     eax, edx                                ; 0233 _ 89. D0
        shl     eax, 2                                  ; 0235 _ C1. E0, 02
        add     eax, edx                                ; 0238 _ 01. D0
        add     eax, eax                                ; 023A _ 01. C0
        mov     dword [ebp-14H], eax                    ; 023C _ 89. 45, EC
        sub     dword [ebp-10H], 1                      ; 023F _ 83. 6D, F0, 01
?_011:  cmp     dword [ebp-10H], 0                      ; 0243 _ 83. 7D, F0, 00
        jns     ?_010                                   ; 0247 _ 79, CF
        mov     eax, dword [ebp-18H]                    ; 0249 _ 8B. 45, E8
        mov     ebx, dword [ebp-4H]                     ; 024C _ 8B. 5D, FC
        leave                                           ; 024F _ C9
        ret                                             ; 0250 _ C3
; str_to_int End of 

malloc: ;  begin
        push    ebp                                     ; 0251 _ 55
        mov     ebp, esp                                ; 0252 _ 89. E5
        push    ebx                                     ; 0254 _ 53
        sub     esp, 20                                 ; 0255 _ 83. EC, 14
        call    __x86.get_pc_thunk.ax                   ; 0258 _ E8, FFFFFFFC(rel)
        add     eax, _GLOBAL_OFFSET_TABLE_-$            ; 025D _ 05, 00000001(GOT r)
; Note: Absolute memory address without relocation
        mov     eax, dword [gs:14H]                     ; 0262 _ 65: A1, 00000014
        mov     dword [ebp-0CH], eax                    ; 0268 _ 89. 45, F4
        xor     eax, eax                                ; 026B _ 31. C0
        mov     eax, esp                                ; 026D _ 89. E0
        mov     ecx, eax                                ; 026F _ 89. C1
        mov     eax, dword [ebp+8H]                     ; 0271 _ 8B. 45, 08
        lea     edx, [eax-1H]                           ; 0274 _ 8D. 50, FF
        mov     dword [ebp-14H], edx                    ; 0277 _ 89. 55, EC
        mov     edx, eax                                ; 027A _ 89. C2
        mov     eax, 16                                 ; 027C _ B8, 00000010
        sub     eax, 1                                  ; 0281 _ 83. E8, 01
        add     eax, edx                                ; 0284 _ 01. D0
        mov     ebx, 16                                 ; 0286 _ BB, 00000010
        mov     edx, 0                                  ; 028B _ BA, 00000000
        div     ebx                                     ; 0290 _ F7. F3
        imul    eax, eax, 16                            ; 0292 _ 6B. C0, 10
        sub     esp, eax                                ; 0295 _ 29. C4
        mov     eax, esp                                ; 0297 _ 89. E0
        add     eax, 0                                  ; 0299 _ 83. C0, 00
        mov     dword [ebp-10H], eax                    ; 029C _ 89. 45, F0
        mov     eax, 0                                  ; 029F _ B8, 00000000
        mov     esp, ecx                                ; 02A4 _ 89. CC
        mov     ebx, dword [ebp-0CH]                    ; 02A6 _ 8B. 5D, F4
; Note: Absolute memory address without relocation
        xor     ebx, dword [gs:14H]                     ; 02A9 _ 65: 33. 1D, 00000014
        jz      ?_012                                   ; 02B0 _ 74, 05
       ; call    __stack_chk_fail_local                  ; 02B2 _ E8, FFFFFFFC(rel)
?_012:  mov     ebx, dword [ebp-4H]                     ; 02B7 _ 8B. 5D, FC
        leave                                           ; 02BA _ C9
        ret                                             ; 02BB _ C3
; malloc End of 


SECTION .data   align=1                       ; section number 2, data


SECTION .bss    align=1                       ; section number 3, bss


SECTION .text.__x86.get_pc_thunk.ax align=1     ; section number 4, code

__x86.get_pc_thunk.ax:;  begin
        mov     eax, dword [esp]                        ; 0000 _ 8B. 04 24
        ret                                             ; 0003 _ C3
; __x86.get_pc_thunk.ax End of 


SECTION .text.__x86.get_pc_thunk.bx align=1     ; section number 5, code

__x86.get_pc_thunk.bx:;  begin
        mov     ebx, dword [esp]                        ; 0000 _ 8B. 1C 24
        ret                                             ; 0003 _ C3
; __x86.get_pc_thunk.bx End of 


SECTION .eh_frame align=4                     ; section number 6, const

        db 14H, 00H, 00H, 00H, 00H, 00H, 00H, 00H       ; 0000 _ ........
        db 01H, 7AH, 52H, 00H, 01H, 7CH, 08H, 01H       ; 0008 _ .zR..|..
        db 1BH, 0CH, 04H, 04H, 88H, 01H, 00H, 00H       ; 0010 _ ........
        db 1CH, 00H, 00H, 00H, 1CH, 00H, 00H, 00H       ; 0018 _ ........
        dd memory_copy-$-20H                            ; 0020 _ 00000000 (rel)
        dd 0000003DH, 080E4100H                         ; 0024 _ 61 135151872 
        dd 0D420285H, 0CC57905H                         ; 002C _ 222429829 214268165 
        dd 00000404H, 0000001CH                         ; 0034 _ 1028 28 
        dd 0000003CH                                    ; 003C _ 60 
        dd memory_copy-$-3H                             ; 0040 _ 0000003D (rel)
        dd 0000003AH, 080E4100H                         ; 0044 _ 58 135151872 
        dd 0D420285H, 0CC57605H                         ; 004C _ 222429829 214267397 
        dd 00000404H, 0000001CH                         ; 0054 _ 1028 28 
        dd 0000005CH                                    ; 005C _ 92 
        dd memory_copy-$+17H                            ; 0060 _ 00000077 (rel)
        dd 000000AAH, 080E4100H                         ; 0064 _ 170 135151872 
        dd 0D420285H, 0C5A60205H                        ; 006C _ 222429829 -978976251 
        dd 0004040CH, 00000020H                         ; 0074 _ 263180 32 
        dd 0000007CH                                    ; 007C _ 124 
        dd memory_copy-$+0A1H                           ; 0080 _ 00000121 (rel)
        dd 000000BAH, 080E4100H                         ; 0084 _ 186 135151872 
        dd 0D420285H, 03834405H                         ; 008C _ 222429829 58934277 
        dd 0C3C5B202H, 0004040CH                        ; 0094 _ -1010454014 263180 
        dd 00000020H, 000000A0H                         ; 009C _ 32 160 
        dd memory_copy-$+137H                           ; 00A4 _ 000001DB (rel)
        dd 00000076H, 080E4100H                         ; 00A8 _ 118 135151872 
        dd 0D420285H, 03834405H                         ; 00B0 _ 222429829 58934277 
        dd 0C3C56E02H, 0004040CH                        ; 00B8 _ -1010471422 263180 
        dd 00000020H, 000000C4H                         ; 00C0 _ 32 196 
        dd memory_copy-$+189H                           ; 00C8 _ 00000251 (rel)
        dd 0000006BH, 080E4100H                         ; 00CC _ 107 135151872 
        dd 0D420285H, 03834405H                         ; 00D4 _ 222429829 58934277 
        dd 0C3C56302H, 0004040CH                        ; 00DC _ -1010474238 263180 
        dd 00000010H, 000000E8H                         ; 00E4 _ 16 232 
        dd __x86.get_pc_thunk.ax-$-0ECH                 ; 00EC _ 00000000 (rel)
        dd 00000004H, 00000000H                         ; 00F0 _ 4 0 
        dd 00000010H, 000000FCH                         ; 00F8 _ 16 252 
        dd __x86.get_pc_thunk.bx-$-100H                 ; 0100 _ 00000000 (rel)
        dd 00000004H, 00000000H                         ; 0104 _ 4 0 


