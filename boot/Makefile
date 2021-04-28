ASM=nasm
ASMFLAGS=-O0 -f elf32 -F dwarf -g -dBOOT_OFFSET=$(BOOT_OFFSET) -dKERN_OFFSET=$(KERN_OFFSET)

all: boot.elf boot.bin

boot.elf: boot.asm
	$(ASM) $(ASMFLAGS) -o $@ $^

boot.bin: boot.elf
	ld -Ttext=$(BOOT_OFFSET) -m elf_i386 --oformat binary -o boot.bin boot.elf

clean:
	rm -f *.elf *.bin