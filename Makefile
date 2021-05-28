QEMU=qemu-system-x86_64
QEMUFLAGS= \
		-cpu qemu64 \
		-drive if=pflash,format=raw,unit=0,file=OVMF/OVMF_CODE.fd,readonly=on \
		-drive if=pflash,format=raw,unit=1,file=OVMF/OVMF_VARS.fd \
		-drive file=boot_image/myos_boot.img,if=ide,format=raw,index=0,media=disk \
		-net none

all: boot_image

boot_image: uefi kernel
	@make -C boot_image/

uefi: kernel
	@make -C uefi/

kernel: FORCE
	@make -C kernel/

FORCE:

run: boot_image
	$(QEMU) $(QEMUFLAGS)

# gdb: uefi.img
# 	$(QEMU) $(QEMUFLAGS) -S -gdb tcp::9000

clean:
	@make -C kernel/ clean
	@make -C uefi/ clean
	@make -C boot_image