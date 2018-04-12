COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding 
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T scr/link.ld
EMULATOR = qemu/qemu-system-i386.exe
EMULATOR_FLAGS = -kernel
kernel_file = nos\\kernel.bin

OBJS = temp/obj/kasm.o temp/obj/kc.o
OUTPUT = nos/kernel.bin temp/obj/system.o temp/obj/string.o temp/obj/screen.o  temp/obj/kb.o temp/obj/util.o temp/obj/idt.o temp/obj/isr.o 

all: kernel test

kernel: functions
	$(ASSEMBLER) $(ASFLAGS) -o temp/obj/kasm.o scr/kernel.asm
	$(COMPILER) $(CFLAGS) scr/kernel.c -o temp/obj/kc.o 
	$(COMPILER) $(CFLAGS) scr/functions/screen.c -o temp/obj/screen.o 
	$(COMPILER) $(CFLAGS) scr/functions/string.c -o temp/obj/string.o 
	$(COMPILER) $(CFLAGS) scr/functions/system.c -o temp/obj/system.o 
	$(COMPILER) $(CFLAGS) scr/functions/kb.c -o temp/obj/kb.o 
	$(COMPILER) $(CFLAGS) scr/functions/util.c -o temp/obj/util.o 
	$(COMPILER) $(CFLAGS) scr/functions/isr.c -o temp/obj/isr.o 
	$(COMPILER) $(CFLAGS) scr/functions/idt.c -o temp/obj/idt.o 
	
	$(LINKER) $(LDFLAGS) -o $(OUTPUT) $(OBJS)
	
functions:
	mkdir temp/obj -p

test:
	$(EMULATOR) $(EMULATOR_FLAGS) $(kernel_file)
	clear