CC=i686-elf-gcc

CFLAGS?=-O2 -g -std=gnu11
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -isystem lib

LIBINC:=$(shell find . -name '*.h' | grep -v crosscompiler)
LIB_C_SRC:=$(shell find . -name '*.c' | grep -v crosscompiler)
LIB_ASM_SRC:=$(shell find . -name '*.asm' | grep -v crosscompiler)
LIB_C_OBJ:=$(patsubst %.c,%.c.o,$(LIB_C_SRC))
LIB_ASM_OBJ:=$(patsubst %.asm,%.asm.o,$(LIB_ASM_SRC))
LIBOBJ:=$(LIB_C_OBJ) $(LIB_ASM_OBJ)

all: rpos.iso

rpos.iso: rpos.bin
	mkdir -p isodir/boot/grub
	cp rpos.bin isodir/boot/rpos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o rpos.iso isodir

rpos.bin: $(LIBOBJ)
	$(CC) $(CFLAGS) -nostdlib -lgcc -T linker.ld -o rpos.bin $(LIBOBJ)
	grub-file --is-x86-multiboot2 rpos.bin

%.asm.o: %.asm
	nasm -felf32 -o $@ $<

%.c.o: %.c $(LIBINC)
	$(CC) $(CFLAGS) -c -o $@ $<

boot:
	qemu-system-x86_64 -cdrom rpos.iso -serial stdio

clean:
	find . -name '*.o' -delete
	-rm *.bin
	-rm -rf isodir
	-rm rpos.iso
