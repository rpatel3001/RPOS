#ifndef _KERNEL_H
#define _KERNEL_H

#include <stdbool.h>
#include <stdint.h>

typedef struct multiboot_info {
	bool mem_present;
	uint32_t mem_lower;
	uint32_t mem_upper;

	bool bootdev_present;
	uint8_t* bootdev;

	bool cmdline_present;
	char* cmdline;

	bool mods_present;
	uint32_t mods_count;
	uint32_t* mods_addr;

	bool elf_syms_present;
	uint32_t elf_num;
	uint32_t elf_size;
	uint32_t* elf_addr;
	uint32_t elf_shndx;

	bool mmap_present;
	uint32_t mmap_len;
	uint32_t* mmap_addr;

	bool drives_present;
	uint32_t drives_len;
	uint32_t* drives_addr;

	bool config_present;
	uint32_t* config_addr;

	bool loader_name_present;
	char* loader_name;

	bool apm_present;
	uint32_t* apm_addr;

	bool vbe_present;
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_offs;
	uint16_t vbe_interface_len;
} multiboot_info;

//symbols from the linker script
extern char KERNEL_END[];
extern char KERNEL_VMA_OFFS[];
extern char KERNEL_LMA[];
extern char KERNEL_VMA[];

// load the ISR
void asm_load_idt(uintptr_t *idt_ptr);

// check if cpuid is available
bool cpuid_supported(void);

// halt forever
void asm_halt(void);

#endif