// uefi_stage/initrd_loader.c
// Загрузка initrd-файла по пути и возврат указателя на буфер

#include "bootloader.h"

VOID* load_initrd(CHAR16 *path, UINTN *size) {
    EFI_FILE_IO_INTERFACE *io;
    EFI_FILE *root, *file;
    EFI_STATUS status;

    EFI_LOADED_IMAGE *loaded_image;
    EFI_GUID LoadedImageProtocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_GUID FileSystemProtocol  = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    // Получаем интерфейс загруженного образа
    status = uefi_call_wrapper(BS->HandleProtocol,
        3, gImageHandle, &LoadedImageProtocol, (void **)&loaded_image);
    if (EFI_ERROR(status)) return NULL;

    // Получаем файловую систему загрузочного устройства
    status = uefi_call_wrapper(BS->HandleProtocol,
        3, loaded_image->DeviceHandle, &FileSystemProtocol, (void **)&io);
    if (EFI_ERROR(status)) return NULL;

    // Открываем корневую директорию
    status = io->OpenVolume(io, &root);
    if (EFI_ERROR(status)) return NULL;

    // Открываем initrd файл
    status = root->Open(root, &file, path, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) return NULL;

    EFI_FILE_INFO *info;
    UINTN info_size = sizeof(EFI_FILE_INFO) + 256;
    info = AllocatePool(info_size);
    file->GetInfo(file, &gEfiFileInfoGuid, &info_size, info);

    *size = info->FileSize;

    VOID *buffer = AllocatePages(EFI_SIZE_TO_PAGES(*size));
    file->Read(file, size, buffer);
    file->Close(file);

    return buffer;
}
