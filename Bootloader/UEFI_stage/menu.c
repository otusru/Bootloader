// uefi_stage/menu.c
// Отображение меню выбора операционной системы

#include <efi.h>
#include <efilib.h>
#include "bootloader.h"

INTN show_menu() {
    CHAR16 *options[] = {
        L"Otus OS",
        L"Debian 12",
        L"Windows 11"
    };

    const UINTN option_count = sizeof(options) / sizeof(CHAR16*);
    INTN current = 0;
    EFI_INPUT_KEY key;

    while (1) {
        ST->ConOut->ClearScreen(ST->ConOut);
        Print(L"Выберите операционную систему:\n\n");

        for (UINTN i = 0; i < option_count; i++) {
            if (i == current) {
                Print(L"> %s\n", options[i]);
            } else {
                Print(L"  %s\n", options[i]);
            }
        }

        UINTN index;
        ST->ConIn->ReadKeyStroke(ST->ConIn, &key);

        if (key.ScanCode == SCAN_UP) {
            if (current > 0) current--;
        } else if (key.ScanCode == SCAN_DOWN) {
            if (current < option_count - 1) current++;
        } else if (key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
            return current;
        }
    }
}


/*
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
*/
