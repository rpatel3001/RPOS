void keyboard_handler(void);
void load_idt(uint32_t *idt_ptr);

bool cpuid_supported(void);
bool longmode_supported(void);