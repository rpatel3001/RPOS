CC=i686-elf-gcc

CFLAGS?=-O2 -g -std=gnu11
CFLAGS:=$(CFLAGS) -ffreestanding -Werror -Wall -Wextra -isystem lib

LIBINC:=$(wildcard lib/*.h)
LIBSRC:=$(wildcard lib/*.c)
LIBOBJ:=$(patsubst %.c,%.o,$(LIBSRC))

all: rpos.bin

rpos.bin: kernel.o boot.o $(LIBOBJ)
	$(CC) $(CFLAGS) -nostdlib -lgcc -T linker.ld -o rpos.bin boot.o kernel.o $(LIBOBJ)
	grub-file --is-x86-multiboot rpos.bin

boot.o: boot.asm
	nasm -felf32 boot.asm -o boot.o

%.o: %.c $(LIBINC)
	$(CC) -c -o $@ $< $(CFLAGS)

boot:
	qemu-system-i386 -kernel rpos.bin -serial stdio 

clean:
	find . -name '*.o' -delete
	-rm *.bin
