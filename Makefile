CC=i686-elf-gcc

QEMU=qemu-system-i386 -cdrom build/rpos.iso -serial stdio -d guest_errors -m 64 -device isa-debug-exit,iobase=0xf4,iosize=0x04

CFLAGS?= -ggdb3 -std=gnu11
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -isystem lib

LIBINC:=$(shell find . -name '*.h' | grep -v crosscompiler)
LIB_C_SRC:=$(shell find . -name '*.c' | grep -v crosscompiler)
LIB_ASM_SRC:=$(shell find . -name '*.asm' | grep -v crosscompiler)
LIB_C_OBJ:=$(patsubst .%.c,build%.c.o,$(LIB_C_SRC))
LIB_ASM_OBJ:=$(patsubst .%.asm,build%.asm.o,$(LIB_ASM_SRC))
LIBOBJ:=$(LIB_C_OBJ) $(LIB_ASM_OBJ)

all: rpos.iso

rpos.iso: rpos.bin
	mkdir -p build/isodir/boot/grub
	cp build/rpos.bin build/isodir/boot/rpos.bin
	cp grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/rpos.iso build/isodir

rpos.bin: $(LIBOBJ)
	$(CC) $(CFLAGS) -nostdlib -lgcc -T linker.ld -o build/rpos.bin $(LIBOBJ)
	grub-file --is-x86-multiboot build/rpos.bin

build/%.asm.o: %.asm
	mkdir -p $(dir $@)
	nasm -g -F dwarf -felf32 -o $@ $<

build/%.c.o: %.c $(LIBINC)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

includes:
	-for target in $(LIB_C_SRC) ; do \
		include-what-you-use $(CFLAGS) $$target ; \
	done

boot:
	$(QEMU)

clean:
	-rm -rf build

list:
	tree -I "build|crosscompiler"

debug:
	$(QEMU) -s -S
