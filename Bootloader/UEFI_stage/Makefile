EFI_TARGET = BOOTX64.EFI
EFI_OUTPUT = $(EFI_TARGET)
CC = x86_64-w64-mingw32-gcc
CFLAGS = -I. -std=c11 -Wall -Wextra -O2 -ffreestanding -fshort-wchar -mno-red-zone -m64
LDFLAGS = -nostdlib -Wl,--entry=efi_main -Wl,--subsystem=10 -Wl,--image-base=0x100000

OBJS = main.o elf_loader.o menu.o boot_cfg.o windows_chainload.o

all: $(EFI_OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EFI_OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -T linker.ld -o $@ $(OBJS)

iso: all
	mkdir -p ../iso/EFI/BOOT
	cp $(EFI_OUTPUT) ../iso/EFI/BOOT/BOOTX64.EFI
	cp boot.cfg ../iso/
	cp OtusOS.elf ../iso/
	cp initrd.img ../iso/
	cp vmlinuz ../iso/
	cp initrd-debian.img ../iso/
	grub-mkrescue -o ../iso/Otus.iso ../iso/

clean:
	rm -f *.o $(EFI_OUTPUT)
