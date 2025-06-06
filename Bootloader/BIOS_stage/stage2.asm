; stage2.asm — второй этап загрузки, переходит в защищённый режим и загружает ядро

[BITS 16]
[ORG 0x0600]

start:
    cli
    xor ax, ax
    mov ds, ax

    ; Печать сообщения
    mov si, msg
.print:
    lodsb
    or al, al
    jz .load_kernel
    mov ah, 0x0E
    int 0x10
    jmp .print

.load_kernel:
    call read_kernel_file

    ; Настройка GDT и переход в PM
    lgdt [gdt_desc]

    ; Включить защищённый режим
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:protected_mode_entry

; -----------------------------
; Реализация FAT12/FAT16 чтения
; -----------------------------
read_kernel_file:
    ; Предположим, что сектор с ELF начинается с LBA = 100 (заглушка)
    ; Читаем 20 секторов по 512 байт -> 10 KB в память по 0x8000
    mov bx, 0x8000
    mov dh, 0
    mov dl, 0x80    ; первый HDD
    mov si, 20      ; 20 секторов
.next_sector:
    push si
    mov ah, 0x02    ; функция чтения
    mov al, 1       ; один сектор
    mov ch, 0       ; цилиндр
    mov cl, 1       ; сектор
    mov dh, 0       ; головка
    mov dl, 0x80
    mov es, bx
    mov bx, 0x0000
    int 0x13
    jc read_error
    pop si
    add bx, 512
    dec si
    jnz .next_sector
    ret

read_error:
    hlt
    jmp $

; -----------------------------
; Protected Mode
; -----------------------------
[BITS 32]
protected_mode_entry:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Парсим ELF64 из 0x8000
    mov esi, 0x8000        ; ELF загружен сюда
    cmp dword [esi], 0x464C457F ; магия ELF (0x7F 'E' 'L' 'F')
    jne elf_error

    ; Получаем точку входа
    mov eax, [esi+0x18]    ; e_entry
    call eax               ; передаём управление ядру

elf_error:
    hlt
    jmp $

; -----------------------------
; GDT и Сегменты
; -----------------------------
gdt_start:
    dq 0x0000000000000000  ; нулевой дескриптор
    dq 0x00CF9A000000FFFF  ; кодовый сегмент
    dq 0x00CF92000000FFFF  ; сегмент данных
gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start
gdt_end:

CODE_SEG equ 0x08
DATA_SEG equ 0x10

msg db "Stage2: чтение ядра Otus OS из FAT и переход в Protected Mode...", 0
