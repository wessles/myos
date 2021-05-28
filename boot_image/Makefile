all: myos_boot.img

myos_boot.img:
	rm -f myos_boot.img
	dd if=/dev/zero of=myos_boot.img bs=512 count=93750
	parted myos_boot.img -s -a minimal mklabel gpt
	parted myos_boot.img -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted myos_boot.img -s -a minimal toggle 1 boot

	dd if=/dev/zero of=/tmp/part.img bs=512 count=91669
	mformat -i /tmp/part.img -h 32 -t 32 -n 64 -c 1

	mcopy -i /tmp/part.img STARTUP.NSH ::
	mmd -i /tmp/part.img EFI
	mmd -i /tmp/part.img EFI/BOOT
	mcopy -i /tmp/part.img ../uefi/BOOTx64.EFI ::EFI/BOOT/BOOTx64.EFI
	mcopy -i /tmp/part.img ../kernel/kernel.elf ::kernel.elf

	dd if=/tmp/part.img of=myos_boot.img bs=512 count=91669 seek=2048 conv=notrunc

clean:
	rm *.o
	rm *.efi
	rm *.img