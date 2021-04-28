QEMU=qemu-system-x86_64
QEMUFLAGS=

BOOT_OFFSET=0x7c00
KERN_OFFSET=0x1000

export


all: bootloader.img

bootloader.img: boot kernel
	cat boot/boot.bin kernel/kernel.bin > bootloader.img

boot: FORCE
	@make -C boot/

kernel: FORCE
	@make -C kernel/

FORCE:


run: bootloader.img
	$(QEMU) $(QEMUFLAGS) -hda bootloader.img

gdb: bootloader.img
	$(QEMU) $(QEMUFLAGS) -hda bootloader.img -S -gdb tcp::9000

clean:
	rm -f bootloader.img
	@make -C kernel/ clean
	@make -C boot/ clean