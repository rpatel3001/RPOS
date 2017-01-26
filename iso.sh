#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir/boot/grub

cp sysroot/boot/rpos.kernel isodir/boot/rpos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "rpos" {
	multiboot /boot/rpos.kernel
}
EOF
grub-mkrescue -o rpos.iso isodir
