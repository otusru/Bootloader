ASM = nasm
CC = i386-elf-gcc
LD = i386-elf-ld
OBJCOPY = i386-elf-objcopy

all: build/boot.img

build:
	mkdir -p build

build/stage1.bin: stage1.asm | build
	$(ASM) -f bin stage1.asm -o $@

build/stage2.bin: stage2.c stage2.ld | build
	$(CC) -m16 -ffreestanding -c stage2.c -o build/stage2.o
	$(LD) -T stage2.ld -o build/stage2.elf build/stage2.o
	$(OBJCOPY) -O binary build/stage2.elf $@

build/boot.img: build/stage1.bin build/stage2.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=build/stage1.bin of=$@ conv=notrunc
	dd if=build/stage2.bin of=$@ bs=512 seek=1 conv=notrunc

run: build/boot.img
	qemu-system-i386 -drive format=raw,file=$<

clean:
	rm -rf build
