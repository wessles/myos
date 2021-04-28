[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY   ; edx = start of video memory

print_string_pm_loop:
    mov al, [ebx]           ; store char &ebx in al
    mov ah, WHITE_ON_BLACK
    
    ; null termination
    cmp al, 0
    je print_string_pm_done

    mov [edx], ax           ; store char at current cel in vram

    add ebx, 1              ; Increment EBX
    add edx, 2              ; Move to next char cell

    jmp print_string_pm_loop
print_string_pm_done:
    popa
    ret