# bios_stage/Makefile
# Сборка BIOS загрузчика Otus OS

AS=nasm
LD=ld

ASFLAGS=-f elf32
LDFLAGS=-m elf_i386 -T linker.ld

all: stage2.bin boot.bin

stage2.o: stage2.asm
    $(AS) $(ASFLAGS) -o stage2.o stage2.asm

stage2.bin: stage2.o
    $(LD) $(LDFLAGS) -o stage2.elf stage2.o
    objcopy -O binary stage2.elf stage2.bin

boot.bin: boot.asm
    $(AS) -f bin -o boot.bin boot.asm

clean:
    rm -f *.o *.bin *.elf
