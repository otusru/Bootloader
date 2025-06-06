# Сборка UEFI
TARGET = BOOTX64.EFI
SRC = main.c elf_loader.c menu.c
OBJ = $(SRC:.c=.o)

EFIINC = /usr/include/efi
EFILIB = /usr/lib/gnuefi
CFLAGS = -I$(EFIINC) -I$(EFIINC)/x86_64 -fPIC -fshort-wchar -mno-red-zone -Wall -O2
LDFLAGS = -nostdlib -znocombreloc -T $(EFILIB)/elf_x86_64_efi.lds
CC = x86_64-linux-gnu-gcc
LD = x86_64-linux-gnu-ld
OBJCOPY = x86_64-linux-gnu-objcopy

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) -o boot.efi $(OBJ) $(EFILIB)/crt0-efi-x86_64.o -lefi -lgnuefi
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic \
	-j .dynsym -j .rel -j .rela -j .reloc \
	--target=efi-app-x86_64 boot.efi $@

run:
	qemu-system-x86_64 -bios /usr/share/OVMF/OVMF.fd \
	-drive format=raw,file=fat:rw:.

clean:
	rm -f *.o *.efi $(TARGET)
