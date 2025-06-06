#include "bootloader.h"

EFI_STATUS
EFIAPI
StartKernel(CHAR16* cmdline) {
    EFI_STATUS status;
    EFI_PHYSICAL_ADDRESS kernel_addr;
    BootInfo* info;
    CHAR8 ascii_cmdline[MAX_CMDLINE_LENGTH];

    // Выделяем память под структуру BootInfo
    status = gBS->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, &kernel_addr);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось выделить память для BootInfo\n");
        return status;
    }

    info = (BootInfo*)(UINTN)kernel_addr;

    // Копируем командную строку
    UnicodeStrToAsciiStrS(cmdline, ascii_cmdline, MAX_CMDLINE_LENGTH);
    AsciiStrCpyS(info->cmdline, MAX_CMDLINE_LENGTH, ascii_cmdline);

    // Устанавливаем адрес и размер initrd (если имеется)
    info->initrd_addr = 0x100000; // Примерный адрес
    info->initrd_size = 0x200000; // Примерный размер

    // Печатаем информацию
    Print(L"Cmdline: %a\n", info->cmdline);
    Print(L"Initrd: 0x%lx, %lu байт\n", info->initrd_addr, info->initrd_size);

    // Здесь должен быть код загрузки ядра и передача управления

    return EFI_SUCCESS;
}
