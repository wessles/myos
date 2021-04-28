[bits 16]

disk_load:
    mov ah, 0x02 ; BIOS read sector function
    mov al, dh   ; Read DH # of sectors
    mov ch, 0x00 ; Cylinder 0
    mov dh, 0x00 ; Head 0
    mov cl, 0x02 ; read from sector 2 (after the bootstrapper)
    push bx
    xor bx, bx     ; es = 0x0000
    mov es, bx
    pop bx          ; BX = load offset (supplied to function)
                    ; we load it at address ES=0x0000:BX=___

    int 0x13
    jc .disk_load_error
    or ah, ah
    jnz .disk_load_error

    ret

    .disk_load_error:
        mov al, ah
        ; call print_byte
        mov si, disk_load_error_str
        call print_string
        jmp $

success_str: db "Success!",0
disk_load_error_str: db ": Error reading additional boot sectors.",0