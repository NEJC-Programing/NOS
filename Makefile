COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T scr/src/link.ld

EMULATOR = qemu/qemu-system-i386.exe
EMULATOR_FLAGS = -kernel

OUTPUT = nos/kernel.bin

all:
	cd scr/src
	$(COMPILER) $(CFLAGS) *.c
	$(ASSEMBLER) $(ASFLAGS) -o kasm.o asm/kernel.asm
	cd ..
	cd ..
	mkdir nos/ -p
	$(LINKER) $(LDFLAGS) -o $(OUTPUT) *.o
	make test
	clear

test:
	$(EMULATOR) $(EMULATOR_FLAGS) $(OUTPUT)
	clear