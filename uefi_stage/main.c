// Основной загрузчик
// main.c — Главный файл UEFI-загрузчика
// Решение: вывод строки, чтение boot.cfg, отображение меню

/*
EFI_STATUS efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table) {
    InitializeLib(image_handle, system_table);

    Print(L"Добро пожаловать в UEFI-загрузчик!\n");

    // TODO: Чтение boot.cfg
    // TODO: Отображение меню выбора
    // TODO: Загрузка ELF64 ядра

    return EFI_SUCCESS;
}
*/

#include <efi.h>
#include <efilib.h>
#include "bootloader.h"

// Структура для хранения информации о загрузке ядра
typedef struct {
    CHAR16* kernel_path;
    CHAR16* cmdline;
} KernelEntry;

// Функция для чтения конфигурационного файла boot.cfg
EFI_STATUS ReadBootConfig(EFI_FILE_PROTOCOL* root, KernelEntry* entry) {
    EFI_FILE_PROTOCOL* file;
    EFI_STATUS status = root->Open(root, &file, L"boot.cfg", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось открыть boot.cfg\n");
        return status;
    }

    CHAR16 buffer[256];
    UINTN size;
    status = file->Read(file, &size, buffer);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка чтения boot.cfg\n");
        file->Close(file);
        return status;
    }

/**
  Преобразуем строку в параметры
  Предполагается, что файл имеет формат:
  kernel_path
  cmdline
   
  \EFI\OtusOS\kernel.elf
  root=/dev/sda1
  
*/
    buffer[size / sizeof(CHAR16)] = L'\0'; // Завершаем строку нулём

    // Разделяем строку на kernel_path и cmdline
    CHAR16* newline = StrStr(buffer, L"\n");
    if (newline != NULL) {
        *newline = L'\0';
        entry->kernel_path = buffer;
        entry->cmdline = newline + 1;
    }

    file->Close(file);
    return EFI_SUCCESS;
}

// Основная точка входа
EFI_STATUS EFIAPI efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table) {
    EFI_STATUS status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs;
    EFI_FILE_PROTOCOL* root;

    // Инициализация библиотеки
    InitializeLib(image_handle, system_table);

    // Получаем файловую систему
    status = system_table->BootServices->HandleProtocol(
        image_handle,
        &FileSystemProtocol,
        (VOID**)&fs
    );
    if (EFI_ERROR(status)) {
        Print(L"Не удалось получить файловую систему\n");
        return status;
    }

    // Открываем корневой каталог
    status = fs->OpenVolume(fs, &root);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось открыть корневой каталог\n");
        return status;
    }

    // Читаем конфигурационный файл
    KernelEntry entry;
    status = ReadBootConfig(root, &entry);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка чтения конфигурации\n");
        return status;
    }

    // Загружаем ядро
    status = LoadKernel(entry.kernel_path);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка загрузки ядра\n");
        return status;
    }

    // Запускаем ядро
    status = StartKernel(entry.cmdline);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка запуска ядра\n");
        return status;
    }

    return EFI_SUCCESS;
}
