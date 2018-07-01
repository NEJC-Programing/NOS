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

FS = fat16

##################################################################
kernel.asm1 = $(call md5,scr/src/kernel.asm)
kernel.asm2 = $(shell cat md5/kernel.asm.md5)
kernel.c1 = $(call md5,scr/src/kernel.c)
kernel.c2 = $(shell cat md5/kernel.c.md5)
idt.c1 = $(call md5,scr/src/idt.c)
idt.c2 = $(shell cat md5/idt.c.md5)
kb.c1 = $(call md5,scr/src/kb.c)
kb.c2 = $(shell cat md5/kb.c.md5)
isr.c1 = $(call md5,scr/src/isr.c) 
isr.c2 = $(shell cat md5/isr.c.md5) 
screen.c1 = $(call md5,scr/src/screen.c) 
screen.c2 = $(shell cat md5/screen.c.md5) 
string.c1 = $(call md5,scr/src/string.c) 
string.c2 = $(shell cat md5/string.c.md5) 
system.c1 = $(call md5,scr/src/system.c) 
system.c2 = $(shell cat md5/system.c.md5) 
util.c1 = $(call md5,scr/src/util.c) 
util.c2 = $(shell cat md5/util.c.md5) 
shell.c1 = $(call md5,scr/src/shell.c) 
shell.c2 = $(shell cat md5/shell.c.md5) 
fs.c1 = $(call md5,scr/src/fs.c) 
fs.c2 = $(shell cat md5/fs.c.md5) 
##################################################################

default:
	make comp
	mkdir nos/ -p
	$(LINKER) $(LDFLAGS) -o $(ELFOUT) $(OBJS)
	objcopy -O binary $(ELFOUT) $(OUTPUT)
	make boot
	make disk
	make test
	clear 
all:
	$(ASSEMBLER) $(ASFLAGS) scr/src/kernel.asm -o scr/obj/kasm.o
	cat scr/src/kernel.asm | md5sum >md5/kernel.asm.md5
	$(COMPILER) $(CFLAGS) scr/src/kernel.c -o scr/obj/kc.o
	cat scr/src/kernel.c | md5sum >md5/kernel.c.md5

comp:
	@if [ "$(kernel.asm1)" = "$(kernel.asm2)" ]; then\
		echo "kernel.asm is not changed";\
	else\
		echo "$(ASSEMBLER) $(ASFLAGS) scr/src/kernel.asm -o scr/obj/kasm.o";\
		$(ASSEMBLER) $(ASFLAGS) scr/src/kernel.asm -o scr/obj/kasm.o;\
		cat scr/src/kernel.asm | md5sum >md5/kernel.asm.md5;\
	fi

	@if [ "$(kernel.c1)" = "$(kernel.c2)" ]; then\
		echo "kernel.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/kernel.c -o scr/obj/kc.o";\
		$(COMPILER) $(CFLAGS) scr/src/kernel.c -o scr/obj/kc.o;\
		cat scr/src/kernel.c | md5sum >md5/kernel.c.md5;\
	fi

	@if [ "$(idt.c1)" = "$(idt.c2)" ]; then\
		echo "idt.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/idt.c -o scr/obj/idt.o";\
		$(COMPILER) $(CFLAGS) scr/src/idt.c -o scr/obj/idt.o;\
		cat scr/src/idt.c | md5sum >md5/idt.c.md5;\
	fi
	
	@if [ "$(kb.c1)" = "$(kb.c2)" ]; then\
		echo "kb.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/kb.c -o scr/obj/kb.o";\
		$(COMPILER) $(CFLAGS) scr/src/kb.c -o scr/obj/kb.o;\
		cat scr/src/kb.c | md5sum >md5/kb.c.md5;\
	fi
	
	@if [ "$(isr.c1)" = "$(isr.c2)" ]; then\
		echo "isr.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/isr.c -o scr/obj/isr.o";\
		$(COMPILER) $(CFLAGS) scr/src/isr.c -o scr/obj/isr.o;\
		cat scr/src/isr.c | md5sum >md5/isr.c.md5;\
	fi

	@if [ "$(screen.c1)" = "$(screen.c2)" ]; then\
		echo "screen.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/screen.c -o scr/obj/screen.o";\
		$(COMPILER) $(CFLAGS) scr/src/screen.c -o scr/obj/screen.o;\
		cat scr/src/screen.c | md5sum >md5/screen.c.md5;\
	fi

	@if [ "$(string.c1)" = "$(string.c2)" ]; then\
		echo "string.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/string.c -o scr/obj/string.o";\
		$(COMPILER) $(CFLAGS) scr/src/string.c -o scr/obj/string.o;\
		cat scr/src/string.c | md5sum >md5/string.c.md5;\
	fi

	@if [ "$(system.c1)" = "$(system.c2)" ]; then\
		echo "system.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/system.c -o scr/obj/system.o";\
		$(COMPILER) $(CFLAGS) scr/src/system.c -o scr/obj/system.o;\
		cat scr/src/system.c | md5sum >md5/system.c.md5;\
	fi

	@if [ "$(util.c1)" = "$(util.c2)" ]; then\
		echo "util.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/util.c -o scr/obj/util.o";\
		$(COMPILER) $(CFLAGS) scr/src/util.c -o scr/obj/util.o;\
		cat scr/src/util.c | md5sum >md5/util.c.md5;\
	fi

	@if [ "$(shell.c1)" = "$(shell.c2)" ]; then\
		echo "shell.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/shell.c -o scr/obj/shell.o";\
		$(COMPILER) $(CFLAGS) scr/src/shell.c -o scr/obj/shell.o;\
		cat scr/src/shell.c | md5sum >md5/shell.c.md5;\
	fi

	@if [ "$(fs.c1)" = "$(fs.c2)" ]; then\
		echo "fs.c is not changed";\
	else\
		echo "$(COMPILER) $(CFLAGS) scr/src/fs.c -o scr/obj/fs.o";\
		$(COMPILER) $(CFLAGS) scr/src/fs.c -o scr/obj/fs.o;\
		cat scr/src/fs.c | md5sum >md5/fs.c.md5;\
	fi
	

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

functions:

define md5
$(shell cat $(1) | md5sum)
endef