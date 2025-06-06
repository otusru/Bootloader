// uefi_stage/windows_chainload.c
// Загрузка Windows Boot Manager (bootmgfw.efi) через UEFI chainload

#include "bootloader.h"

EFI_STATUS chainload_windows_bootmgr(void) {
    EFI_STATUS status;
    EFI_LOADED_IMAGE *loaded_image;
    EFI_FILE_IO_INTERFACE *io;
    EFI_FILE *root, *file;
    EFI_HANDLE image_handle = NULL;
    EFI_GUID LoadedImageProtocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_GUID FileSystemProtocol  = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    // Получаем интерфейс загруженного образа
    status = uefi_call_wrapper(BS->HandleProtocol,
        3, gImageHandle, &LoadedImageProtocol, (void **)&loaded_image);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка: не удалось получить LoadedImageProtocol\n");
        return status;
    }

    // Получаем интерфейс файловой системы
    status = uefi_call_wrapper(BS->HandleProtocol,
        3, loaded_image->DeviceHandle, &FileSystemProtocol, (void **)&io);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка: не удалось получить FileSystemProtocol\n");
        return status;
    }

    // Открываем корневую директорию
    status = io->OpenVolume(io, &root);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка открытия тома\n");
        return status;
    }

    // Путь к загрузчику Windows
    CHAR16 *path = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";

    // Открываем загрузчик Windows
    status = root->Open(root, &file, path, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось найти bootmgfw.efi\n");
        return status;
    }

    // Загружаем EFI-файл
    status = uefi_call_wrapper(BS->LoadImage, 6, FALSE,
        gImageHandle, NULL, file, 0, &image_handle);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка загрузки bootmgfw.efi\n");
        return status;
    }

    // Передаем управление Windows Boot Manager
    status = uefi_call_wrapper(BS->StartImage, 3, image_handle, NULL, NULL);
    return status;
}
