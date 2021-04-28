[bits 16]

print_string:
    ; SI = input
    mov ah, 0x0E
    mov bh, 0x00

    .loop:
        ; Load byte from SI, to AL, SI++
        lodsb

        ; null string termination
        cmp al, 0x00
        je .done

        ; print it
        int 10h

        jmp .loop
    .done:
    ret