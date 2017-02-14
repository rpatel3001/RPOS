ENTRY(_start)

KERNEL_VMA_OFFS = 0;
KERNEL_LMA = 1M;

SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = KERNEL_VMA_OFFS;
	. += KERNEL_LMA;
	kernel_start = .;
	.multiboot ALIGN(4K) : AT(ADDR(.multiboot)-KERNEL_VMA_OFFS)
	{
		*(.multiboot)
	}

	.text ALIGN(4K) : AT(ADDR(.text)-KERNEL_VMA_OFFS)
	{
		*(.text)
	}

	/* Read-only data. */
	.rodata ALIGN(4K) : AT(ADDR(.rodata)-KERNEL_VMA_OFFS)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data)-KERNEL_VMA_OFFS)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss)-KERNEL_VMA_OFFS)
	{
		*(COMMON)
		*(.bss)
	}
	kernel_end = .;
}
