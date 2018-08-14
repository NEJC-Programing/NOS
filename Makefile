COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T scr/src/link.ld

EMULATOR = qemu-system-i386
EMULATOR_FLAGS = -device isa-debug-exit,iobase=0xf4,iosize=0x04 -kernel

OBJS = scr/obj/kasm.o scr/obj/kc.o scr/obj/idt.o scr/obj/isr.o scr/obj/kb.o scr/obj/screen.o scr/obj/string.o scr/obj/system.o scr/obj/util.o scr/obj/shell.o scr/obj/fs.o scr/obj/graphics.o scr/obj/serial.o scr/obj/drivers.o scr/obj/iqr.o \

EXT2UTIL = ref\ stuff/ext2util/ext2util

OUTPUT = nos/kernel.bin
IMG = nos/nos.img
ELFOUT = nos/kernel.elf

FS = ext2


default: all
	mkdir nos/ -p
	make test
	clear


all:
	mkdir nos/ -p
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
	$(COMPILER) $(CFLAGS) scr/src/graphics.c -o scr/obj/graphics.o
	$(COMPILER) $(CFLAGS) scr/src/serial.c -o scr/obj/serial.o
	$(COMPILER) $(CFLAGS) scr/src/drivers.c -o scr/obj/drivers.o
	$(COMPILER) $(CFLAGS) scr/src/iqr.c -o scr/obj/iqr.o
	$(LINKER) $(LDFLAGS) -o $(ELFOUT) $(OBJS)
	strip $(ELFOUT)
	objcopy -O binary $(ELFOUT) $(OUTPUT)
	make boot
	make disk

boot:
	$(ASSEMBLER) -o nos/bootloader.bin scr/boot/bootloader_$(FS).asm
ifneq ($(FS),ext2)
	$(ASSEMBLER) -o nos/boot.bin scr/boot/boot_stage_2/stage_2_$(FS).asm
else
	
	cp scr/boot/boot_stage_2/ext2_stage2 nos/boot.bin
endif

disk:
	dd if=/dev/zero of=$(IMG) bs=1k count=16k
	mkfs.ext2 $(IMG)
	dd if=nos/bootloader.bin of=$(IMG) conv=notrunc
	cd nos; ../$(EXT2UTIL) -x nos.img -wf boot.bin -i 5
	cd nos; ../$(EXT2UTIL) -x nos.img -wf kernel.elf
	cd nos; ../$(EXT2UTIL) -x nos.img -wf boot.conf
	

test:
	$(EMULATOR) $(EMULATOR_FLAGS) $(ELFOUT) -hdb $(IMG)
	$(EMULATOR) -hdb $(IMG)
	clear

clean:
	rm scr/obj/* nos/*bin nos/*elf 