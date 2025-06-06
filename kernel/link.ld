ENTRY(multiboot_main)

SECTIONS {
    . = 1M;

    .multiboot : {
        *(.multiboot)
    }

    .text : {
        *(.text*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }
}
