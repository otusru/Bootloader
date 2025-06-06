#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <efi.h>
#include <efilib.h>

#define MAX_KERNELS 10
#define MAX_CMDLINE_LENGTH 256

typedef struct {
    CHAR16* path;
    CHAR16* cmdline;
} KernelEntry;

EFI_STATUS LoadKernel(CHAR16* path);
EFI_STATUS StartKernel(CHAR16* cmdline);
EFI_STATUS ReadBootConfig(EFI_FILE_PROTOCOL* root, KernelEntry* kernels, UINTN* kernel_count);
EFI_STATUS ShowMenu(KernelEntry* kernels, UINTN kernel_count, UINTN* selection);
EFI_STATUS LoadInitrd(CHAR16* path);
EFI_STATUS ShowTimer(UINTN seconds);

#endif // BOOTLOADER_H
