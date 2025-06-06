; bios_stage/boot.asm
; MBR-загрузчик (512 байт) для BIOS
; Загружает второй этап загрузки (stage2.bin) с диска

org 0x7C00                    ; адрес загрузки BIOS
bits 16

start:
    cli                       ; запретить прерывания
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00            ; стек

    ; Проверяем сигнатуру MBR
    mov si, mbr_signature_msg
    call print_string

    ; Загружаем сектор с stage2 (сектор 2, т.к. сектор 1 - MBR)
    mov bx, stage2_load_addr
    mov dl, [BOOT_DRIVE]
    mov ah, 0x02              ; функция BIOS - чтение с диска
    mov al, 1                 ; читать 1 сектор
    mov ch, 0                 ; цилиндр 0
    mov cl, 2                 ; сектор 2
    mov dh, 0                 ; головка 0
    int 0x13
    jc disk_error

    ; Переходим к stage2 (загрузчик второго этапа)
    jmp bx

disk_error:
    mov si, disk_error_msg
    call print_string
    hlt
    jmp $

print_string:
    mov ah, 0x0E
.next_char:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .next_char
.done:
    ret

BOOT_DRIVE db 0             ; BIOS заполняет при старте

stage2_load_addr dw 0x8000  ; адрес загрузки stage2

mbr_signature_msg db "MBR loader started...", 0
disk_error_msg db "Ошибка чтения диска", 0

times 510 - ($ - $$) db 0
dw 0xAA55                   ; сигнатура MBR (0x55AA в LE)
