all:
	make asm
	make kernel
	make link

asm:
	nasm -felf32 boot.asm -o boot.o

kernel:
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

link:
	i686-elf-gcc -T linker.ld -o rpos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	grub-file --is-x86-multiboot rpos.bin

boot:
	qemu-system-i386 -kernel rpos.bin

clean:
	rm *.o
	rm *.bin
