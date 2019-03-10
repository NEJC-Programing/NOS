CC = gcc
ASSEMBLER = nasm
CCF = -m32 -c -ffreestanding -fno-pic -fno-builtin -nostdlib -std=gnu99 -I ../../include/
ASFLAGS = -f elf32 -O0

all:
	mkdir objs/ -p
	$(CC) $(CCF) cmos.c -o objs/cmos.o
	$(CC) $(CCF) drivers.c -o objs/drivers.o
	$(CC) $(CCF) serial.c -o objs/serial.o
	$(CC) $(CCF) ide.c -o objs/ide.o
	$(CC) $(CCF) sound.c -o objs/sound.o
	$(CC) $(CCF) kb.c -o objs/kb.o
	mkdir system/objs/ -p
	$(CC) $(CCF) system/bios32.c -o system/objs/bios32.o
	$(CC) $(CCF) system/gdt.c -o system/objs/gdt.o
	$(CC) $(CCF) system/idt.c -o system/objs/idt.o
	$(CC) $(CCF) system/tss.c -o system/objs/tss.o
	$(ASSEMBLER) $(ASFLAGS) system/bios32.asm -o system/objs/asmbios32.o
	$(ASSEMBLER) $(ASFLAGS) system/helpers.asm -o system/objs/helpers.o
	make link

link:
	ld -m elf_i386 -r -o objs/sys.o system/objs/asmbios32.o system/objs/bios32.o system/objs/helpers.o system/objs/gdt.o system/objs/idt.o system/objs/tss.o
	ld -m elf_i386 -r -o ../../obj/drivers.o objs/cmos.o objs/serial.o objs/drivers.o objs/ide.o objs/sound.o objs/sys.o objs/kb.o