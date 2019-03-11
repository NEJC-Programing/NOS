global _start
global _end

extern start

[bits 32]

MODULEALIGN       equ     1<<0
MEMINFO           equ     1<<1
FLAGS             equ     MODULEALIGN | MEMINFO
MAGIC             equ     0x1BADB002
CHECKSUM          equ     -(MAGIC + FLAGS)

section .text

align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

_start:
  cli
  call start
  jmp _end

_end:
  hlt
  jmp _end
