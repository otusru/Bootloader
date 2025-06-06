#ifndef BOOTINFO_H
#define BOOTINFO_H

#include <stdint.h>

typedef struct {
    char cmdline[256];
    uint64_t initrd_addr;
    uint64_t initrd_size;
} BootInfo;

#endif // BOOTINFO_H
