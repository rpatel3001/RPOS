ENTRY(_start)

KERNEL_VMA_OFFS = 0xE0000000;
KERNEL_LMA = 1M;

SECTIONS
{
	KERNEL_VMA = KERNEL_LMA + KERNEL_VMA_OFFS;
	. = KERNEL_VMA;

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
	KERNEL_END = .;
}
