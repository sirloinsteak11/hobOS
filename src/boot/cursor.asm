get_cursor:
    pusha

    mov ah, 0x03
    int 10h
    call print_hex

    popa
    ret