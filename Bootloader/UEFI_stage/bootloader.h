// uefi_stage/bootloader.h
// Заголовочный файл: структуры, функции загрузки ядра и initrd

#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <efi.h>
#include <efilib.h>

// Структура BootInfo для передачи параметров ядру Otus OS
typedef struct {
    CHAR16 *cmdline;         // Командная строка ядра
    VOID   *initrd_addr;     // Адрес initrd в памяти
    UINTN   initrd_size;     // Размер initrd
} BootInfo;

typedef struct {
    CHAR16 *title;
    CHAR16 *kernel;
    CHAR16 *cmdline;
    CHAR16 *initrd;
    CHAR16 *windows_chainload;
} BootEntry;

INTN boot_menu_auto_or_manual(void);
BootEntry* get_boot_entry(INTN index);

// Тип для функции точки входа ядра
typedef void (*KernelMainFunc)(BootInfo*);

// Показывает меню выбора ОС и возвращает индекс выбранного пункта
INTN show_menu(void);

// Загружает ELF64-файл и возвращает точку входа
VOID* load_elf64(EFI_FILE *file);

// Загружает initrd по указанному пути
VOID* load_initrd(CHAR16 *path, UINTN *size);

// Открывает файл ядра по имени
EFI_FILE* open_kernel_file(CHAR16 *name);

// Загрузка Windows через chainload bootmgfw.efi
EFI_STATUS chainload_windows_bootmgr(void);

#endif // BOOTLOADER_H


/*
#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <efi.h>
#include <efilib.h>

#define MAX_KERNELS 10
#define MAX_CMDLINE_LENGTH 256

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289
#define MULTIBOOT2_HEADER_MAGIC     0xE85250D6
#define MULTIBOOT2_TAG_CMDLINE      1
#define MULTIBOOT2_TAG_MODULE       3
#define MULTIBOOT2_TAG_END          0

typedef struct {
    UINT32 type;
    UINT32 size;
} __attribute__((packed)) multiboot_tag;

typedef struct {
    multiboot_tag tag;
    CHAR8 string[128];
} __attribute__((packed)) multiboot_tag_cmdline;

typedef struct {
    multiboot_tag tag;
    UINT32 mod_start;
    UINT32 mod_end;
    CHAR8  cmdline[128];
} __attribute__((packed)) multiboot_tag_module;

typedef struct {
    multiboot_tag tag;
} __attribute__((packed)) multiboot_tag_end;


typedef struct {
    CHAR8 cmdline[MAX_CMDLINE_LENGTH];
    EFI_PHYSICAL_ADDRESS initrd_addr;
    UINTN initrd_size;
} BootInfo;

typedef struct {
    CHAR16* path;
    CHAR16* cmdline;
} KernelEntry;

EFI_STATUS LoadKernel(CHAR16* path);
EFI_STATUS StartKernel(CHAR16* cmdline);
EFI_STATUS ReadBootConfig(EFI_FILE_PROTOCOL* root, KernelEntry* kernels, UINTN* kernel_count);
EFI_STATUS ShowMenu(KernelEntry* kernels, UINTN kernel_count, UINTN* selection);
EFI_STATUS LoadInitrd(CHAR16* path);
EFI_STATUS ShowTimer(UINTN seconds);
EFI_STATUS OpenFile(CHAR16* path, EFI_FILE_PROTOCOL** file);
EFI_STATUS ReadFile(EFI_FILE_PROTOCOL* file, EFI_PHYSICAL_ADDRESS addr, UINTN size);
EFI_STATUS GetFileSize(EFI_FILE_PROTOCOL* file, UINTN* size);

#endif // BOOTLOADER_H
*/
