CC = gcc
ASSEMBLER = nasm
CCF = -m32 -c -ffreestanding -fno-pic -fno-builtin -nostdlib -std=gnu99
ASFLAGS = -f elf32 -O0


all:
	mkdir objs/ -p
	$(CC) $(CCF) cmos.c -o objs/cmos.o
	$(CC) $(CCF) drivers.c -o objs/drivers.o
	$(CC) $(CCF) serial.c -o objs/serial.o
	$(CC) $(CCF) ide.c -o objs/ide.o
	$(CC) $(CCF) sound.c -o objs/sound.o
	mkdir bios/objs/ -p
	#$(CC) $(CCF) bios/bios32.c -o bios/objs/bios32.o
	#$(ASSEMBLER) $(ASFLAGS) bios/bios32.asm -o bios/objs/asmbios32.o
	make link

link:
	ld -m elf_i386 -r -o objs/bios32.o bios/objs/asmbios32.o bios/objs/bios32.o
	ld -m elf_i386 -r -o ../../obj/drivers.o objs/cmos.o objs/serial.o objs/drivers.o objs/ide.o objs/sound.o objs/bios32.o