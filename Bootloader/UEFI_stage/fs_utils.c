#include "bootloader.h"

// вспомогательные функции работы с файлами

// Открытие файла по пути
EFI_STATUS OpenFile(CHAR16* path, EFI_FILE_PROTOCOL** file) {
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs;
    EFI_FILE_PROTOCOL* root;
    EFI_STATUS status;

    status = gBS->HandleProtocol(gImageHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&fs);
    if (EFI_ERROR(status)) return status;

    status = fs->OpenVolume(fs, &root);
    if (EFI_ERROR(status)) return status;

    status = root->Open(root, file, path, EFI_FILE_MODE_READ, 0);
    return status;
}

// Получение размера файла
EFI_STATUS GetFileSize(EFI_FILE_PROTOCOL* file, UINTN* size) {
    EFI_FILE_INFO* fileInfo;
    UINTN infoSize = sizeof(EFI_FILE_INFO) + 256;

    fileInfo = AllocateZeroPool(infoSize);
    if (!fileInfo) return EFI_OUT_OF_RESOURCES;

    EFI_STATUS status = file->GetInfo(file, &gEfiFileInfoGuid, &infoSize, fileInfo);
    if (EFI_ERROR(status)) {
        FreePool(fileInfo);
        return status;
    }

    *size = fileInfo->FileSize;
    FreePool(fileInfo);
    return EFI_SUCCESS;
}

// Чтение файла в память
EFI_STATUS ReadFile(EFI_FILE_PROTOCOL* file, EFI_PHYSICAL_ADDRESS addr, UINTN size) {
    UINTN readSize = size;
    EFI_STATUS status = file->Read(file, &readSize, (VOID*)(UINTN)addr);
    return status;
}
