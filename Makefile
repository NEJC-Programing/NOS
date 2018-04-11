COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T scr/link.ld
EMULATOR = /mnt/c/qemu/qemu-system-i386.exe
EMULATOR_FLAGS = -kernel
kernel_file = C:\\Users\\noam\\Desktop\\NOS\\nos\\kernel.bin

OBJS = temp/obj/kasm.o temp/obj/kc.o
OUTPUT = nos/kernel.bin temp/obj/system.o temp/obj/string.o temp/obj/screen.o 

all: kernel test

kernel: functions
	$(ASSEMBLER) $(ASFLAGS) -o temp/obj/kasm.o scr/kernel.asm
	$(COMPILER) $(CFLAGS) scr/kernel.c -o temp/obj/kc.o 
	$(COMPILER) $(CFLAGS) scr/functions/screen.c -o temp/obj/screen.o 
	$(COMPILER) $(CFLAGS) scr/functions/string.c -o temp/obj/string.o 
	$(COMPILER) $(CFLAGS) scr/functions/system.c -o temp/obj/system.o 
	
	$(LINKER) $(LDFLAGS) -o $(OUTPUT) $(OBJS)
	
functions:
	mkdir temp/obj -p

test:
	$(EMULATOR) $(EMULATOR_FLAGS) $(kernel_file)
	clear