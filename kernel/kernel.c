#include "bootinfo.h"

void kernel_main(BootInfo* info) {
    // Пример использования информации из загрузчика
    puts("Otus OS kernel запущено.\n");
    puts("Командная строка: ");
    puts(info->cmdline);
    puts("\n");

    // initrd
    printf("Initrd адрес: 0x%x, размер: %u байт\n", info->initrd_addr, info->initrd_size);

    while (1); // Зацикливаемся
}
