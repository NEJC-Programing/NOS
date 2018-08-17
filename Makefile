COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding -fno-pic -fno-builtin -nostdlib -std=gnu99 -I ../include/
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T scr/src/link.ld

EMULATOR = qemu-system-i386
EMULATOR_FLAGS = -device isa-debug-exit,iobase=0xf4,iosize=0x04 -kernel

OBJS = scr/obj/kasm.o scr/obj/kc.o scr/obj/idt.o scr/obj/isr.o scr/obj/kb.o scr/obj/screen.o scr/obj/string.o scr/obj/system.o scr/obj/util.o scr/obj/shell.o scr/obj/fs.o scr/obj/graphics.o scr/obj/drivers.o scr/obj/iqr.o 

EXT2UTIL = ref\ stuff/ext2util/ext2util

OUTPUT = nos/kernel.bin
IMG = nos/nos.img
ELFOUT = nos/kernel.elf

FS = ext2


default: all
	mkdir nos/ -p
	make test
	clear

installdep:
	sudo apt update
	sudo apt install nasm gcc binutils

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
	$(COMPILER) $(CFLAGS) scr/src/iqr.c -o scr/obj/iqr.o
	make -C scr/src/drivers
	$(LINKER) $(LDFLAGS) -o $(ELFOUT) $(OBJS)
	strip $(ELFOUT)
	objcopy -O binary $(ELFOUT) $(OUTPUT)
	make boot
	make disk

bootloader:
	make boot
	make disk
	$(EMULATOR) -hdb $(IMG) -d guest_errors
	clear

boot:
	$(ASSEMBLER) -o nos/bootloader.bin scr/boot/bootloader_$(FS).asm
ifneq ($(FS),ext2)
	$(ASSEMBLER) -o nos/boot.bin scr/boot/boot_stage_2/stage_2_$(FS).asm
else
	gcc -w -fno-pic -fno-builtin -nostdlib -ffreestanding -std=gnu99 -m32 -c scr/boot/boot_stage_2/ext2-boot/main.c -o scr/obj/boot_main.o
	gcc -w -fno-pic -fno-builtin -nostdlib -ffreestanding -std=gnu99 -m32 -c scr/boot/boot_stage_2/ext2-boot/ext2.c -o scr/obj/boot_ext2.o
	gcc -w -fno-pic -fno-builtin -nostdlib -ffreestanding -std=gnu99 -m32 -c scr/boot/boot_stage_2/ext2-boot/lib.c -o scr/obj/boot_lib.o
	gcc -w -fno-pic -fno-builtin -nostdlib -ffreestanding -std=gnu99 -m32 -c scr/boot/boot_stage_2/ext2-boot/vga.c -o scr/obj/boot_vga.o
	gcc -w -fno-pic -fno-builtin -nostdlib -ffreestanding -std=gnu99 -m32 -c scr/boot/boot_stage_2/ext2-boot/elf.c -o scr/obj/boot_elf.o
	ld -m elf_i386 -N -e stage2_main -Ttext 0x00050000 -o nos/boot.bin scr/obj/boot_main.o scr/obj/boot_ext2.o scr/obj/boot_lib.o scr/obj/boot_vga.o scr/obj/boot_elf.o --oformat binary
endif

disk:
	dd if=/dev/zero of=$(IMG) bs=1k count=16k
	mkfs.ext2 $(IMG)
	dd if=nos/bootloader.bin of=$(IMG) conv=notrunc
	cd nos; ../$(EXT2UTIL) -x nos.img -wf boot.bin -i 5
	cd nos; ../$(EXT2UTIL) -x nos.img -wf kernel.elf
	cd nos; ../$(EXT2UTIL) -x nos.img -wf kernel.bin
	cd nos; ../$(EXT2UTIL) -x nos.img -wf boot.conf
	

test:
#	$(EMULATOR) $(EMULATOR_FLAGS) $(ELFOUT) -hdb $(IMG) -d guest_errors
	$(EMULATOR) -hdb $(IMG) -d guest_errors
	clear

clean:
	rm scr/obj/* nos/*bin nos/*elf 