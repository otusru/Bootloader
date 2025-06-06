CC = x86_64-elf-gcc
LD = x86_64-elf-ld
CFLAGS = -O2 -Wall -ffreestanding -nostdlib -mno-red-zone -fno-pic
LDFLAGS = -Tlink.ld -nostdlib

SOURCES = kernel.c
OBJECTS = $(SOURCES:.c=.o)

all: kernel.elf

kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o kernel.elf
