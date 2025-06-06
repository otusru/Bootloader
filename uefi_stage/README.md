## Описание Otus OS

Загрузчик для операционной системы Otus OS, поддерживающий:

- Множественный выбор ядер с конфигурацией через boot.cfg
- Отображение таймера
- Загрузку initrd
- Структуру передачи параметров ядру (через multiboot или cmdline)

## Сборка UEFI-загрузчика

``` bash

cd bootloader/uefi_stage
make
qemu-system-x86_64 -bios /usr/share/OVMF/OVMF.fd -drive format=raw,file=fat:rw:.

```
