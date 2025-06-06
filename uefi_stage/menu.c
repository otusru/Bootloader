// Меню выбора

#include <efi.h>
#include <efilib.h>
#include "bootloader.h"

// Функция для отображения меню выбора
EFI_STATUS ShowMenu() {
    Print(L"1. Загрузить Otus OS\n");
    Print(L"2. Загрузить Linux\n");
    Print(L"3. Выход\n");

    EFI_INPUT_KEY key;
    EFI_STATUS status;

    while (TRUE) {
        status = gST->ConIn->ReadKeyStroke(gST
::contentReference[oaicite:0]{index=0}
 
EFI_STATUS ShowMenu(KernelEntry* kernels, UINTN kernel_count, UINTN* selection) {
    // Реализация отображения меню
    return EFI_SUCCESS;
}

EFI_STATUS ShowTimer(UINTN seconds) {
    // Реализация отображения таймера
    return EFI_SUCCESS;
}
