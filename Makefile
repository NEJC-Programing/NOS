COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T scr/src/link.ld

EMULATOR = qemu-system-i386
EMULATOR_FLAGS = -kernel

OBJS = scr/obj/kasm.o scr/obj/kc.o scr/obj/idt.o scr/obj/isr.o scr/obj/kb.o scr/obj/screen.o scr/obj/string.o scr/obj/system.o scr/obj/util.o scr/obj/shell.o #scr/obj/fs.o
OUTPUT = nos/kernel.bin
ELFOUT = nos/kernel.elf

FS = fat32

all:
	make comp
	mkdir nos/ -p
	$(LINKER) $(LDFLAGS) -o $(ELFOUT) $(OBJS)
	objcopy -O binary $(ELFOUT) $(OUTPUT)
	make boot
	make disk
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
	dd if=/dev/zero of=nos/nos.img bs=16777216 count=1
	mkdosfs -F 16 nos/nos.img
	dd if=nos/bootloader.bin of=nos/nos.img bs=512 conv=notrunc

test:
	$(EMULATOR) $(EMULATOR_FLAGS) $(ELFOUT)
	clear
