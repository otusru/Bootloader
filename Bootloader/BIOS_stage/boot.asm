; bootloader.asm — Загрузочный сектор BIOS: Stage1
; Решение: вывести сообщение и загрузить Stage2 (4 сектора, начиная с 2)

[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Показать сообщение
    mov si, msg
    call print_string

    ; Загрузить 4 сектора начиная с сектора 2 (Stage2)
    mov ah, 0x02        ; BIOS: читать секторы
    mov al, 4           ; кол-во секторов
    mov ch, 0
    mov cl, 2           ; сектор 2
    mov dh, 0
    mov dl, 0x80        ; жёсткий диск
    mov bx, 0x0600      ; адрес в памяти
    int 0x13
    jc disk_error

    ; Переход к загруженному Stage2
    jmp 0x0000:0x0600

disk_error:
    mov si, error
    call print_string
    jmp $

print_string:
    mov ah, 0x0E
.print_char:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .print_char
.done:
    ret

msg   db "Загрузка Otus OS Stage2...", 0
error db "Ошибка чтения Stage2!", 0

times 510 - ($ - $$) db 0
dw 0xAA55
