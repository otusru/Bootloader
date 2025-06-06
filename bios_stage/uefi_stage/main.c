// main.c — Главный файл UEFI-загрузчика
// Результат: вывод строки, чтение boot.cfg, отображение меню

#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* system_table) {
    InitializeLib(image_handle, system_table);

    Print(L"Добро пожаловать в UEFI-загрузчик!\n");

    // TODO: Чтение boot.cfg
    // TODO: Отображение меню выбора
    // TODO: Загрузка ELF64 ядра

    return EFI_SUCCESS;
}
