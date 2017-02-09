ENTRY(_start)

KERNEL_VMA = 0;

SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = KERNEL_VMA;
	. += 1M;

	.multiboot ALIGN(4K) : AT(ADDR(.multiboot)-KERNEL_VMA)
	{
		*(.multiboot)
	}

	.text ALIGN(4K) : AT(ADDR(.text)-KERNEL_VMA)
	{
		*(.text)
	}

	/* Read-only data. */
	.rodata ALIGN(4K) : AT(ADDR(.rodata)-KERNEL_VMA)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data)-KERNEL_VMA)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss)-KERNEL_VMA)
	{
		*(COMMON)
		*(.bss)
	}
}
