#ifndef _TERMINAL_H
#define _TERMINAL_H

/* Hardware text mode color constants. */
enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

//initialize the terminal
void terminal_init(void);

//set the background and foreground color
void terminal_setcolor(enum vga_color fg, enum vga_color bg);

//set the foreground color
void terminal_setfgcolor(enum vga_color fg);

//set the background color
void terminal_setbgcolor(enum vga_color bg);

//write a null terminated string
void terminal_writestring(const char* data);

//put a character at a specific location onscreen
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

//put a character at the current cursor position
void terminal_putchar(char c);

//write a given number of characters
void terminal_write(const char* data, size_t size);

#endif