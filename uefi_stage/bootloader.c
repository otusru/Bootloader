// Загрузка ELF64

#include <efi.h>
#include <efilib.h>
#include "bootloader.h"

EFI_STATUS EFIAPI StartKernel(CHAR16* cmdline) {
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

// Функция для загрузки ELF64 ядра
EFI_STATUS LoadKernel(CHAR16* path) {
    EFI_STATUS status;
    EFI_FILE_PROTOCOL* file;
    EFI_LOADED_IMAGE_PROTOCOL* loaded_image;
    EFI_PHYSICAL_ADDRESS kernel_base;
    UINTN kernel_size;

    // Открываем файл ядра
    status = OpenFile(path, &file);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось открыть файл ядра\n");
        return status;
    }

    // Получаем размер файла
    status = GetFileSize(file, &kernel_size);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось получить размер файла\n");
        file->Close(file);
        return status;
    }

    // Выделяем память для ядра
    status = AllocatePages(EFI_SIZE_TO_PAGES(kernel_size), &kernel_base);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось выделить память для ядра\n");
        file->Close(file);
        return status;
    }

    // Читаем содержимое файла в память
    status = ReadFile(file, kernel_base, kernel_size);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось прочитать файл в память\n");
        FreePages(kernel_base, EFI_SIZE_TO_PAGES(kernel_size));
        file->Close(file);
        return status;
    }

    // Закрываем файл
    file->Close(file);

    // Получаем информацию о загруженном образе
    status = gBS->HandleProtocol(
        image_handle,
        &LoadedImageProtocol,
        (VOID**)&loaded_image
    );
    if (EFI_ERROR(status)) {
        Print(L"Не удалось получить информацию о загруженном образе\n");
        FreePages(kernel_base, EFI_SIZE_TO_PAGES(kernel_size));
        return status;
    }

    // Устанавливаем точку входа ядра
    loaded_image->EntryPoint = (EFI_IMAGE_ENTRY_POINT)kernel_base;

    return EFI_SUCCESS;
}

EFI_STATUS StartKernel(CHAR16* cmdline) {
    // Реализация запуска ядра
    typedef void (*KernelEntry)(void);

    KernelEntry entry_point = (KernelEntry)0x100000; // Предположим, адрес точки входа

    Print(L"Перед запуском ядра Otus OS...\n");
    Print(L"Командная строка: %s\n", cmdline);

    gBS->ExitBootServices(gImageHandle, 0);

    entry_point(); // Передача управления ядру

    return EFI_SUCCESS;
}

EFI_STATUS LoadInitrd(CHAR16* path) {
    // Реализация загрузки initrd
    return EFI_SUCCESS;
}

EFI_STATUS StartKernel(CHAR16* cmdline) {
    // Выделяем память под структуру BootInfo
    BootInfo* info;
    EFI_PHYSICAL_ADDRESS addr;
    EFI_STATUS status;

    status = gBS->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, &addr);
    if (EFI_ERROR(status)) return status;

    info = (BootInfo*)(UINTN)addr;

    // Копируем cmdline
    CHAR8 ascii_cmdline[MAX_CMDLINE_LENGTH];
    UnicodeStrToAsciiStrS(cmdline, ascii_cmdline, MAX_CMDLINE_LENGTH);
    AsciiStrCpyS(info->cmdline, MAX_CMDLINE_LENGTH, ascii_cmdline);

    // Предположим, что initrd уже загружен по адресу:
    info->initrd_addr = 0x800000; // Пример адреса
    info->initrd_size = 0x100000; // Пример размера

    // Адрес точки входа ядра
    void (*kernel_entry)(BootInfo*) = (void (*)(BootInfo*))0x100000;

    // Завершаем работу служб UEFI
    UINTN mmap_size = 0, map_key, desc_size;
    UINT32 desc_version;
    gBS->GetMemoryMap(&mmap_size, NULL, &map_key, &desc_size, &desc_version);
    void* mem_map = AllocatePool(mmap_size);
    gBS->GetMemoryMap(&mmap_size, mem_map, &map_key, &desc_size, &desc_version);
    gBS->ExitBootServices(gImageHandle, map_key);

    // Передаём управление ядру
    kernel_entry(info);

    return EFI_SUCCESS;
}
