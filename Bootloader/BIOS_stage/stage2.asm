; bios_stage/stage2.asm
; Второй этап BIOS-загрузчика
; Читает FAT12/16, ищет KERNEL.BIN, загружает ELF64 ядро Otus OS
; Загружает все сегменты, подготавливает BootInfo и запускает ядро

[org 0x8000]                ; адрес загрузки stage2

bits 16

; Стандартные данные
kernel_load_addr equ 0x100000   ; 1MB - место загрузки ядра
bootinfo_addr equ 0x7000        ; место BootInfo

start:
    cli
    call init_segment_registers
    call load_fat12_kernel
    call load_elf_segments
    call setup_bootinfo

    ; Запускаем ядро: kernel_main(BootInfo*)
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, bootinfo_addr
    call kernel_main

    hlt
    jmp $

; ======================
; Инициализация сегментов
init_segment_registers:
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9000
    sti
    ret

; ======================
; Чтение FAT12 и поиск KERNEL.BIN
load_fat12_kernel:
    ; Реализовать чтение FAT, поиск файла KERNEL.BIN
    ; Загрузить файл в kernel_load_addr
    ; Для примера - заглушка
    ret

; ======================
; Загрузка всех ELF сегментов из kernel_load_addr
load_elf_segments:
    ; Анализируем ELF64 загруженный файл
    ; Загружаем каждый PT_LOAD сегмент по адресу
    ; См. спецификацию ELF64
    ret

; ======================
; Настройка структуры BootInfo по адресу bootinfo_addr
setup_bootinfo:
    ; Заполнение структуры BootInfo с параметрами загрузчика
    ret

; ======================
; Вызов kernel_main(BootInfo*)
kernel_main:
    ; Здесь адрес перехода в ядро
    ret

times 512*4 - ($ - $$) db 0 ; размер stage2 ~2KB
