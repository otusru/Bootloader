# Сборка UEFI-загрузчика

``` bash

cd bootloader/uefi_stage
make
qemu-system-x86_64 -bios /usr/share/OVMF/OVMF.fd -drive format=raw,file=fat:rw:.

```
