// bios_stage/BootInfo.h
// Определение структуры BootInfo для передачи ядру

#ifndef BOOTINFO_H
#define BOOTINFO_H

#include <stdint.h>

typedef struct {
    const char* cmdline;       // Указатель на строку командной строки
    uint32_t initrd_addr;      // Адрес initrd в памяти (0 если нет)
    uint32_t initrd_size;      // Размер initrd
    // Можно добавить другую информацию: память, видео, диск и др.
} BootInfo;

#endif // BOOTINFO_H
