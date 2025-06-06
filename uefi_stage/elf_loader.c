// Загрузка ELF64

#include <efi.h>
#include <efilib.h>
#include "bootloader.h"

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
    return EFI_SUCCESS;
}

EFI_STATUS LoadInitrd(CHAR16* path) {
    // Реализация загрузки initrd
    return EFI_SUCCESS;
}
