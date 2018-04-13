COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T scr/src/link.ld

EMULATOR = qemu/qemu-system-i386.exe
EMULATOR_FLAGS = -kernel

OBJS = scr/obj/kasm.o scr/obj/kc.o scr/obj/idt.o scr/obj/isr.o scr/obj/kb.o scr/obj/screen.o scr/obj/string.o scr/obj/system.o scr/obj/util.o scr/obj/shell.o #scr/obj/fs.o
OUTPUT = nos/kernel.bin

FS = fat16

all:
	make comp
	mkdir nos/ -p
	$(LINKER) $(LDFLAGS) -o $(OUTPUT) $(OBJS)
	make boot
	make test
	clear 

comp:
	$(ASSEMBLER) $(ASFLAGS) -o scr/obj/kasm.o scr/src/kernel.asm
	$(COMPILER) $(CFLAGS) scr/src/kernel.c -o scr/obj/kc.o 
	$(COMPILER) $(CFLAGS) scr/src/idt.c -o scr/obj/idt.o 
	$(COMPILER) $(CFLAGS) scr/src/kb.c -o scr/obj/kb.o
	$(COMPILER) $(CFLAGS) scr/src/isr.c -o scr/obj/isr.o
	$(COMPILER) $(CFLAGS) scr/src/screen.c -o scr/obj/screen.o
	$(COMPILER) $(CFLAGS) scr/src/string.c -o scr/obj/string.o
	$(COMPILER) $(CFLAGS) scr/src/system.c -o scr/obj/system.o
	$(COMPILER) $(CFLAGS) scr/src/util.c -o scr/obj/util.o
	$(COMPILER) $(CFLAGS) scr/src/shell.c -o scr/obj/shell.o
	$(COMPILER) $(CFLAGS) scr/src/fs.c -o scr/obj/fs.o

boot:
	$(ASSEMBLER) -o nos/bootloader.bin scr/boot/bootloader_$(FS).asm
	$(ASSEMBLER) -o nos/boot.bin scr/boot/boot_stage_2/stage_2_$(FS).asm

disk:
	$(ASSEMBLER) -o nos/nos.img scr/img.asm
	mkdosfs -F 16 nos/nos.img

test:
	$(EMULATOR) $(EMULATOR_FLAGS) $(OUTPUT)
	clear
