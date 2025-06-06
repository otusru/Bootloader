// uefi_stage/main.c
// Главный файл UEFI-загрузчика Otus OS с меню выбора систем
// Использует UEFI SimpleTextInput/Output, загружает ядра по выбору

#include <efi.h>
#include <efilib.h>
#include <efi_protocols.h>
#include "bootloader.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    Print(L"\nДобро пожаловать в загрузчик Otus OS (UEFI)\n\n");

    EFI_FILE *kernel_file = NULL;
    CHAR16 *cmdline = NULL;
    CHAR16 *initrd_path = NULL;

    INTN choice = show_menu();

    switch (choice) {
        case 0:
            Print(L"Вы выбрали Otus OS\n");
            kernel_file = open_kernel_file(L"OtusOS.elf");
            cmdline = L"root=/dev/sda1 quiet";
            initrd_path = L"initrd.img";
            break;

        case 1:
            Print(L"Вы выбрали Debian 12\n");
            kernel_file = open_kernel_file(L"vmlinuz");
            cmdline = L"root=/dev/sda2 ro quiet splash";
            initrd_path = L"initrd-debian.img";
            break;

        case 2:
            Print(L"Вы выбрали Windows 11\n");
            Print(L"Запуск Windows 11 через UEFI chainload...\n");
            return chainload_windows_bootmgr();
    }

    if (!kernel_file) {
        Print(L"Ошибка: ядро не найдено\n");
        return EFI_NOT_FOUND;
    }

    void *entry = load_elf64(kernel_file);
    if (!entry) {
        Print(L"Ошибка загрузки ELF\n");
        return EFI_LOAD_ERROR;
    }

    BootInfo *bootinfo = AllocatePool(sizeof(BootInfo));
    bootinfo->cmdline = cmdline;
    bootinfo->initrd_addr = load_initrd(initrd_path, &bootinfo->initrd_size);

    Print(L"Передача управления ядру...\n");
    ((KernelMainFunc)entry)(bootinfo);  // kernel_main(BootInfo*)

    return EFI_SUCCESS;
}

// ------------------------

/*
// Основной загрузчик
// main.c — Главный файл UEFI-загрузчика
// Решение: вывод строки, чтение boot.cfg, отображение меню

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
EFI_STATUS EFIAPI efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table) {
    EFI_STATUS status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs;
    EFI_FILE_PROTOCOL* root;
    KernelEntry kernels[MAX_KERNELS];
    UINTN kernel_count = 0;
    UINTN selection = 0;

    Print(L"Загрузчик Otus OS (UEFI)\n");

    // Читаем конфиг boot.cfg, парсим ядра
    // Отображаем меню выбора ядра
    // Загружаем выбранное ядро (ELF64)
    // Загружаем initrd если есть
    // Формируем BootInfo
    // Передаем управление ядру (kernel_main или multiboot_main)

    // Инициализация библиотеки
    InitializeLib(image_handle, system_table);

    status = system_table->BootServices->HandleProtocol(image_handle, &FileSystemProtocol, (VOID**)&fs);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось получить файловую систему\n");
        return status;
    }
    
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
    status = StartKernel(kernels[selection].cmdline);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка запуска ядра\n");
        return status;
    }
   
    status = fs->OpenVolume(fs, &root);
    if (EFI_ERROR(status)) {
        Print(L"Не удалось открыть корневой каталог\n");
        return status;
    }

    status = ReadBootConfig(root, kernels, &kernel_count);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка чтения конфигурации\n");
        return status;
    }

    status = ShowMenu(kernels, kernel_count, &selection);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка отображения меню\n");
        return status;
    }

    status = LoadKernel(kernels[selection].path);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка загрузки ядра\n");
        return status;
    }

    status = LoadInitrd(L"\\EFI\\Otus\\initrd.img");
    if (EFI_ERROR(status)) {
        Print(L"Ошибка загрузки initrd\n");
        return status;
    }
    
    // Запускаем ядро
    status = StartKernel(kernels[selection].cmdline);
    if (EFI_ERROR(status)) {
        Print(L"Ошибка запуска ядра\n");
        return status;
    }

    Print(L"Добро пожаловать в UEFI-загрузчик!\n");

    return EFI_SUCCESS;
}
*/
