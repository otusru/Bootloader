#include "bootinfo.h"
#include "multiboot2.h"

// Multiboot2 заголовок
__attribute__((section(".multiboot")))
volatile const unsigned int multiboot2_header[] = {
    MULTIBOOT2_HEADER_MAGIC,
    MULTIBOOT2_HEADER_ARCHITECTURE,
    MULTIBOOT2_HEADER_LENGTH,
    MULTIBOOT2_HEADER_CHECKSUM
};

// Простая функция вывода (заглушка)
void puts(const char* s) {
    while (*s) {
        *((volatile char*)0xb8000) = *s++; // VGA Text Mode
    }
}

// Точка входа с кастомной структурой
void kernel_main(BootInfo* info) {
    //  информации из загрузчика
    puts("Otus OS kernel запущено (BootInfo mode).\n");
    puts("Командная строка: ");
    puts(info->cmdline);
    puts("\n");

    // initrd
    printf("Initrd адрес: 0x%x, размер: %u байт\n", info->initrd_addr, info->initrd_size);

    while (1); // Зацикливаемся
}

// Точка входа Multiboot2
void multiboot_main(uint32_t magic, uint32_t addr) {
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        puts("Неверная сигнатура Multiboot2!\n");
        while (1);
    }

    puts("Otus OS kernel (Multiboot2 mode)\n");

    char* cmdline = NULL;

    uint8_t* p = (uint8_t*)addr;
    while (1) {
        uint32_t type = *(uint32_t*)p;
        uint32_t size = *(uint32_t*)(p + 4);

        if (type == 0) break; // END

        if (type == 1) { // Cmdline
            cmdline = (char*)(p + 8);
        }

        p += (size + 7) & ~7; // 8-byte aligned
    }

    if (cmdline) {
        puts("Cmdline: ");
        puts(cmdline);
    }

    while (1);
}
