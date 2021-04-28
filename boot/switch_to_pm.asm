[bits 16]

switch_to_pm:
    ; ignore interrupts for now
    cli
    ; load gdt data structure
    lgdt [gdt_descriptor]

    ; set first bit of CR0 control reg. to enable 32 bit protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; allow pipelining to catch up by long jumping
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    ; update segments now that we're in protected mode
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; move / empty stack
    mov ebp, 0x9000
    mov esp, ebp

    jmp BEGIN_PM ; never returns