/* Линковка UEFI загрузчика Otus OS */

OUTPUT_FORMAT("efi-app-x86_64")
ENTRY(efi_main)

SECTIONS {
    . = 0;
    .text : ALIGN(4096) {
        *(.text*)
    }
    .data : ALIGN(4096) {
        *(.data*)
    }
    .rodata : ALIGN(4096) {
        *(.rodata*)
    }
    .bss : ALIGN(4096) {
        *(.bss*)
        *(COMMON)
    }
}
