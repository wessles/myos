[bits 16]

load_kernel:
    mov bx, KERN_OFFSET     ; KERN_OFFSET is a macro supplied by Makefile
    mov dh, 32              ; load 32 sectors. Should be enough, maybe.
    mov dl, [BOOT_DRIVE]
    call disk_load
after_load:
    ret

%include "disk_load.asm"