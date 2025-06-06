; Этап 1: Загрузочный сектор для BIOS
; stage1.asm — Этап 1: Загрузочный сектор для BIOS
; Решение: загрузить второй этап загрузчика из следующих секторов

BITS 16
ORG 0x7C00

start:
    cli                 ; Отключаем прерывания
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00      ; Настраиваем стек

    ; Выводим сообщение
    mov si, message
    call print_string

    ; Загружаем 4 сектора начиная с сектора 2 (Stage 2)
    mov ah, 0x02        ; Функция BIOS — читать сектора
    mov al, 4           ; Количество секторов
    mov ch, 0
    mov cl, 2           ; Сектор 2
    mov dh, 0
    mov dl, 0x80        ; Жёсткий диск
    mov bx, 0x0600      ; Адрес в памяти (0x0000:0x0600)
    int 0x13
    jc disk_error

    ; Переход к загруженному второму этапу
    jmp 0x0000:0x0600

disk_error:
    mov si, error
    call print_string
    jmp $

print_string:
    mov ah, 0x0E
.next:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next
.done:
    ret

message db "Загрузка второго этапа...", 0
error   db "Ошибка чтения диска!", 0

times 510 - ($ - $$) db 0
dw 0xAA55  ; Сигнатура загрузочного сектора
