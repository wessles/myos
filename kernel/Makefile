ASM=nasm
ASMFLAGS=-O0 -f elf32 -F dwarf -g -dBOOT_OFFSET=$(BOOT_OFFSET) -dKERN_OFFSET=$(KERN_OFFSET)

CC=gcc
CCFLAGS=-Wall -m32 -fno-pie -ffreestanding -g

all: kernel.o kernel_entry.elf kernel.bin

kernel_entry.elf: kernel_entry.asm
	$(ASM) $(ASMFLAGS) -o $@ $^

kernel.o: kernel.c
	$(CC) $(CCFLAGS) -o $@ -c $^

kernel.bin: kernel_entry.elf kernel.o
	ld -Ttext $(KERN_OFFSET) -m elf_i386 --oformat binary -o kernel.bin kernel_entry.elf kernel.o

clean:
	rm -f *.o *.elf *.bin