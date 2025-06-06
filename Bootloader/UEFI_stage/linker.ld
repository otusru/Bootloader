/* uefi_stage/linker.ld */
/* Линковка UEFI загрузчика Otus OS */

OUTPUT_FORMAT("efi-app-x86_64")
ENTRY(efi_main)

SECTIONS
{
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
