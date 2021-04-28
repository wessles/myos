; KERN_OFFSET is a macro supplied by the Makefile

[bits 16]
    ; save boot drive for later
    mov [ BOOT_DRIVE ], dl

    mov bp, 0x9000
    mov sp, bp

    mov si, MSG_LOADING_KERNEL
    call print_string

    call load_kernel

    mov si, MSG_PM_SWITCH
    call print_string

    call switch_to_pm

    jmp $

MSG_LOADING_KERNEL: db 'Loading kernel...',0x0a,0x0d,0
MSG_PM_SWITCH: db 'Switching to protected mode...',0x0a,0x0d,0

BOOT_DRIVE: db 0x00

%include "print_string.asm"
%include "load_kernel.asm"
%include "switch_to_pm.asm"
%include "gdt_data.asm"

[bits 32]

BEGIN_PM:
    mov ebx, MSG_SUCCESS
    call print_string_pm
    
kern_jump:
    push KERN_OFFSET
    ret

    jmp $

%include "print_string_pm.asm"

MSG_SUCCESS: db 'Successful!',0

; end boot sector 0
times 510-($-$$) db 0
dw 0xaa55 ; magic num