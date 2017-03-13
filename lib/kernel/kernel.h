#ifndef _KERNEL_H
#define _KERNEL_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	char sig[4];
	uint16_t version;
	char* oem_string;
	uint32_t capabilities;
	uint32_t video_mode_ptr; // TODO parse this
	uint16_t memory; // in 64KiB blocks
	uint16_t oem_revision;
	char* oem_vendor_name;
	char* oem_product_name;
	char* oem_product_revision;
	uint8_t reserved[222];
	uint8_t oem_data[256];
}  __attribute__ ((packed))vbe_control_info;

typedef struct {
	uint16_t mode_attr;
	uint8_t win_a_attr;
	uint8_t win_b_attr;
	uint16_t win_granularity;
	uint16_t win_size;
	uint16_t win_a_seg;
	uint16_t win_b_seg;
	uint32_t win_func_ptr;
	uint16_t bytes_per_scanline;
	uint16_t x_res;
	uint16_t y_res;
	uint8_t x_char_size;
	uint8_t y_char_size;
	uint8_t num_planes;
	uint8_t bits_per_pixel;
	uint8_t num_banks;
	uint8_t mem_model;
	uint8_t bank_size;
	uint8_t num_image_pages;
	uint8_t reserved1;
	uint8_t red_mask_size;
	uint8_t red_field_pos;
	uint8_t green_mask_size;;
	uint8_t green_field_pos;
	uint8_t blue_mask_size;
	uint8_t blue_field_pos;
	uint8_t reserved_mask_size;
	uint8_t reserved_field_pos;
	uint8_t direct_color_mode_info;
	uint32_t base_addr;
	uint32_t reserved2;
	uint16_t reserved3;
	// TODO fill in the rest
}  __attribute__ ((packed))vbe_mode_info;

typedef struct {
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
	uintptr_t mmap_addr;

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
	vbe_control_info* vbe_control_ptr;
	vbe_mode_info* vbe_mode_ptr;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_offs;
	uint16_t vbe_interface_len;
} __attribute__ ((packed)) multiboot_info;

//symbols from the linker script
extern char KERNEL_END[];
extern char KERNEL_VMA_OFFS[];
extern char KERNEL_LMA[];
extern char KERNEL_VMA[];

// check if cpuid is available
bool cpuid_supported(void);

// halt forever
void asm_halt(void);

uint32_t memsize_mb(void);

#endif