gdt_start:

; mandatory null segment
gdt_null:
    dd 0x00000000
    dd 0x00000000

; two overlapping code / data segments with kernel level privileges.

gdt_code:
    dw 0xffff       ; limit
    dw 0x0          ; base
    db 0x0
    db 10011010b    ; (present)1 (privilege)00 (type)1 (code)1 (conforming)0 (read)1 (accessed)0
    db 11001111b    ; (granularity)1 (32bit)1 (64bit)0 (avl)0 ()1 ()1 ()1 ()1
    db 0x0          ; base

gdt_data:           ; identical, but (code)0, (expand down)0 (writable)1
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start