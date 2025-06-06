// boot_cfg.c
// Парсер boot.cfg и логика выбора записи по таймеру или вручную

#include "bootloader.h"

#define MAX_ENTRIES 10

typedef struct {
    CHAR16 *title;
    CHAR16 *kernel;
    CHAR16 *cmdline;
    CHAR16 *initrd;
    CHAR16 *windows_chainload;
} BootEntry;

static BootEntry entries[MAX_ENTRIES];
static UINTN entry_count = 0;
static UINTN default_entry = 0;
static UINTN timeout_seconds = 0;

static CHAR16* trim(CHAR16 *str) {
    while (*str == L' ' || *str == L'\t') str++;
    return str;
}

// Примитивный парсер boot.cfg
void parse_boot_cfg() {
    EFI_FILE *file = open_kernel_file(L"boot.cfg");
    if (!file) {
        Print(L"boot.cfg не найден, будет использована конфигурация по умолчанию\n");
        return;
    }

    CHAR16 line[256];
    UINTN size;
    BootEntry current = {0};
    CHAR16 *p;

    while (TRUE) {
        size = sizeof(line);
        if (file->Read(file, &size, line) != EFI_SUCCESS || size == 0)
            break;

        line[size / sizeof(CHAR16)] = 0;
        p = trim(line);

        if (*p == L'#' || *p == 0)
            continue;

        if (StrnCmp(p, L"timeout=", 8) == 0) {
            timeout_seconds = Atoi(p + 8);
        } else if (StrnCmp(p, L"default=", 8) == 0) {
            default_entry = Atoi(p + 8);
        } else if (StrnCmp(p, L"entry \"", 7) == 0) {
            if (entry_count < MAX_ENTRIES) {
                p += 7;
                CHAR16 *end = StrStr(p, L"\"");
                if (end) *end = 0;
                current.title = AllocateCopyPool(StrSize(p), p);
            }
        } else if (StrnCmp(p, L"kernel=", 7) == 0) {
            current.kernel = AllocateCopyPool(StrSize(p + 7), p + 7);
        } else if (StrnCmp(p, L"cmdline=", 8) == 0) {
            current.cmdline = AllocateCopyPool(StrSize(p + 8), p + 8);
        } else if (StrnCmp(p, L"initrd=", 7) == 0) {
            current.initrd = AllocateCopyPool(StrSize(p + 7), p + 7);
        } else if (StrnCmp(p, L"windows_chainload=", 18) == 0) {
            current.windows_chainload = AllocateCopyPool(StrSize(p + 18), p + 18);
        } else if (StrCmp(p, L"}") == 0) {
            if (entry_count < MAX_ENTRIES) {
                entries[entry_count++] = current;
                current = (BootEntry){0};
            }
        }
    }

    file->Close(file);
}

INTN boot_menu_auto_or_manual() {
    parse_boot_cfg();

    if (entry_count == 0) {
        Print(L"Нет записей в boot.cfg\n");
        return -1;
    }

    if (timeout_seconds > 0) {
        for (UINTN i = timeout_seconds; i > 0; i--) {
            ST->ConOut->ClearScreen(ST->ConOut);
            Print(L"Загрузка \"%s\" через %d секунд...\n", entries[default_entry].title, i);
            Print(L"Нажмите любую клавишу для меню выбора\n");

            EFI_INPUT_KEY key;
            UINTN index;
            EFI_EVENT events[1] = { ST->ConIn->WaitForKey };

            if (BS->WaitForEvent(1, events, &index) == EFI_SUCCESS) {
                if (ST->ConIn->ReadKeyStroke(ST->ConIn, &key) == EFI_SUCCESS)
                    break;
            }
        }
    }

    // Меню выбора вручную
    EFI_INPUT_KEY key;
    INTN current = (INTN)default_entry;

    while (TRUE) {
        ST->ConOut->ClearScreen(ST->ConOut);
        Print(L"Выберите систему:\n\n");

        for (UINTN i = 0; i < entry_count; i++) {
            if ((INTN)i == current)
                Print(L"> %s\n", entries[i].title);
            else
                Print(L"  %s\n", entries[i].title);
        }

        ST->ConIn->ReadKeyStroke(ST->ConIn, &key);

        if (key.ScanCode == SCAN_UP && current > 0)
            current--;
        else if (key.ScanCode == SCAN_DOWN && current < (INTN)entry_count - 1)
            current++;
        else if (key.UnicodeChar == CHAR_CARRIAGE_RETURN)
            return current;
    }
}

// Возвращает структуру BootEntry по индексу
BootEntry* get_boot_entry(INTN index) {
    if (index >= 0 && (UINTN)index < entry_count)
        return &entries[index];
    return NULL;
}
