global _start
global _end

extern start

[bits 32]

MODULEALIGN       equ     1<<0
MEMINFO           equ     1<<1
FLAGS             equ     MODULEALIGN | MEMINFO
MAGIC             equ     0x1BADB002
CHECKSUM          equ     -(MAGIC + FLAGS)


section .multiboot

align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .text

_start:
  mov esp, stack_top
  cli
  call start
  jmp _end

_end: ; should never get here
  hlt
  jmp _end

  section .bss
  align 16
  stack_bottom:
  resb 16384 ; 16 KiB
  stack_top:
