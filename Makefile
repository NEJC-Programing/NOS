CC = gcc
AS = nasm
LD = ld

CCF = -m32 -c -ffreestanding -fno-pic -fno-builtin -nostdlib -std=gnu99 -I src/include/

ASF = -f elf32
LDF = -m elf_i386 -T src/link.ld

EMULATOR = qemu-system-i386
EMULATOR_FLAGS = -device isa-debug-exit,iobase=0xf4,iosize=0x04 -kernel


default:
	mkdir nos/ -p
	make all
	make test
	clear

all:
	make kernel
	#make boot

KOP = obj/kernel/
KSP = src/kernel/
KELF = nos/kernel.elf
KOBJ = $(KOP)start.asm.o $(KOP)start.c.o $(KOP)main.c.o $(KOP)system/ports.c.o $(KOP)screen/textmode.c.o $(KOP)system/libk.c.o \
$(KOP)screen/screen.c.o $(KOP)screen/printf.c.o $(KOP)system/bios_32.asm.o $(KOP)screen/VESA/vesa.c.o


kernel:
	mkdir $(KOP) -p
	$(AS) $(ASF) $(KSP)start.asm -o $(KOP)start.asm.o
	$(CC) $(CCF) $(KSP)start.c -o $(KOP)start.c.o
	$(CC) $(CCF) $(KSP)main.c -o $(KOP)main.c.o
	#screen
	mkdir $(KOP)/screen/ -p
	$(CC) $(CCF) $(KSP)screen/textmode.c -o $(KOP)screen/textmode.c.o
	$(CC) $(CCF) $(KSP)screen/screen.c -o $(KOP)screen/screen.c.o
	$(CC) $(CCF) $(KSP)screen/printf.c -o $(KOP)screen/printf.c.o
	#VESA
	mkdir $(KOP)/screen/VESA/ -p
	$(CC) $(CCF) $(KSP)screen/VESA/vesa.c -o $(KOP)screen/VESA/vesa.c.o
	#system
	mkdir $(KOP)/system/ -p
	$(CC) $(CCF) $(KSP)system/ports.c -o $(KOP)system/ports.c.o
	$(CC) $(CCF) $(KSP)system/libk.c -o $(KOP)system/libk.c.o
	$(AS) $(ASF) $(KSP)system/bios_32.asm -o $(KOP)system/bios_32.asm.o
	#link
	$(LD) $(LDF) $(KOBJ) -o $(KELF)


test:
	$(EMULATOR) $(EMULATOR_FLAGS) $(KELF)
