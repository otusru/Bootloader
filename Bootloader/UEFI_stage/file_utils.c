// uefi_stage/file_utils.c
// Вспомогательная функция: открытие файла ядра по имени

#include "bootloader.h"

EFI_FILE* open_kernel_file(CHAR16 *name) {
    EFI_FILE_IO_INTERFACE *io;
    EFI_FILE *root, *file;
    EFI_STATUS status;

    EFI_LOADED_IMAGE *loaded_image;
    EFI_GUID LoadedImageProtocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_GUID FileSystemProtocol  = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    status = uefi_call_wrapper(BS->HandleProtocol,
        3, gImageHandle, &LoadedImageProtocol, (void **)&loaded_image);
    if (EFI_ERROR(status)) return NULL;

    status = uefi_call_wrapper(BS->HandleProtocol,
        3, loaded_image->DeviceHandle, &FileSystemProtocol, (void **)&io);
    if (EFI_ERROR(status)) return NULL;

    status = io->OpenVolume(io, &root);
    if (EFI_ERROR(status)) return NULL;

    status = root->Open(root, &file, name, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) return NULL;

    return file;
}
