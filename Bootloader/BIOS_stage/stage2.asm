; stage2.asm — второй этап загрузки, переходит в защищённый режим и загружает ядро

[BITS 16]
[ORG 0x0600]

start_stage2:
    cli
    xor ax, ax
    mov ds, ax

    ; Печать сообщения
    mov si, msg
.print:
    lodsb
    or al, al
    jz .setup_pm
    mov ah, 0x0E
    int 0x10
    jmp .print

.setup_pm:
    ; Настройка GDT
    lgdt [gdt_desc]

    ; Включение защищённого режима
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:init_pm

[BITS 32]
init_pm:
    ; Устанавливаем сегменты
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Печать текста (пример, можно убрать)
    ; Здесь нужно реализовать чтение ядра с диска (например, ELF)
    ; и передать управление

.hang:
    jmp $

; GDT таблица
gdt_start:
    dq 0x0000000000000000  ; нулевой дескриптор
    dq 0x00CF9A000000FFFF  ; сегмент кода
    dq 0x00CF92000000FFFF  ; сегмент данных

gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start

gdt_end:

msg db "Otus OS Stage2 загружен. Переход в Protected Mode...", 0

CODE_SEG equ 0x08
DATA_SEG equ 0x10
