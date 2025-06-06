ASM = nasm
CC = i386-elf-gcc
LD = i386-elf-ld
OBJCOPY = i386-elf-objcopy

# Выбор варианта Stage2: c или asm
STAGE2_TYPE ?= c

# Основной таргет
all: build/boot.img

# Папка для сборки
build:
	mkdir -p build

# Stage1: всегда NASM
build/stage1.bin: stage1.asm | build
	$(ASM) -f bin stage1.asm -o $@

# Stage2 на C
build/stage2_c.bin: stage2.c stage2.ld | build
	$(CC) -m16 -ffreestanding -c stage2.c -o build/stage2.o
	$(LD) -T stage2.ld -o build/stage2.elf build/stage2.o
	$(OBJCOPY) -O binary build/stage2.elf $@

# Stage2 на ASM
build/stage2_asm.bin: stage2.asm | build
	$(ASM) -f bin stage2.asm -o $@

# Выбор нужного бинарника Stage2
build/stage2.bin: build/stage2_$(STAGE2_TYPE).bin
	cp $< $@

# Финальный образ (флоппи 1.44 МБ)
build/boot.img: build/stage1.bin build/stage2.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=build/stage1.bin of=$@ conv=notrunc
	dd if=build/stage2.bin of=$@ bs=512 seek=1 conv=notrunc

# Запуск через QEMU
run: build/boot.img
	qemu-system-i386 -drive format=raw,file=$<

# Очистка
clean:
	rm -rf build
