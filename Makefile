CC=i686-elf-gcc

CFLAGS?=-O2 -g -std=gnu11
CFLAGS:=$(CFLAGS) -ffreestanding -Werror -Wall -Wextra

all: rpos.bin

rpos.bin: kernel.o boot.o
	$(CC) $(CFLAGS) -nostdlib -lgcc -T linker.ld -o rpos.bin boot.o kernel.o
	grub-file --is-x86-multiboot rpos.bin

boot.o: boot.asm
	nasm -felf32 boot.asm -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

boot:
	qemu-system-i386 -kernel rpos.bin 2> /dev/null

clean:
	-rm *.o
	-rm *.bin
