/* bios_stage/linker.ld */
/* Линковка для BIOS stage2, загрузка в 0x8000 */

OUTPUT_FORMAT("elf32-i386")
ENTRY(start)

SECTIONS
{
  . = 0x8000;
  .text : {
    *(.text)
  }
  .data : {
    *(.data)
  }
  .bss : {
    *(.bss)
  }
}
