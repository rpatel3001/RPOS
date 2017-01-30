#include <stddef.h>
#include <stdint.h>
#include "terminal.h"

extern size_t strlen(const char* str);
extern void linecpy(uint16_t* to, const uint16_t* from, size_t len);

//give a vga buffer value for a character and color combo
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

//initialize the terminal
void terminal_init(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; ++y) {
    for (size_t x = 0; x < VGA_WIDTH; ++x) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

//set the background and foreground color
void terminal_setcolor(enum vga_color fg, enum vga_color bg) {
  terminal_color = fg | bg << 4;
}

//set the foreground color
void terminal_setfgcolor(enum vga_color fg) {
  terminal_color &= 0xF0;
  terminal_color |= fg;
}

//set the background color
void terminal_setbgcolor(enum vga_color bg) {
  terminal_color &= 0x0F;
  terminal_color |= bg << 4;
}

//get the foreground color
enum vga_color terminal_getfgcolor(void) {
  return terminal_color & 0x0F;
}

//get the background color
enum vga_color terminal_getbgcolor(void ) {
  return terminal_color >> 4;
}

//put a character at a specific location onscreen
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

//scroll up the terminal by one line
void terminal_scroll() {
  --terminal_row;
  for (size_t row = 1; row < VGA_HEIGHT; ++row) {
    linecpy(&terminal_buffer[(row-1) * VGA_WIDTH], &terminal_buffer[row * VGA_WIDTH], VGA_WIDTH);
  }
  for (size_t col = 0; col < VGA_WIDTH; ++col) {
    terminal_putentryat(' ', terminal_color, col, VGA_HEIGHT - 1);
  }
}

//put a character at the current cursor position
void terminal_putchar(char c) {
  const size_t index = terminal_row * VGA_WIDTH + terminal_column;
  if ((c >= 0 && c <= 6) || (c == 11) || (c >= 14 && c <= 31)) {
    //nonprintable
  } else if (c == 7) {
    //bell
  } else if (c == 8) {
    //backspace
    if (terminal_column != 0) {
      --terminal_column;
      linecpy(&terminal_buffer[index-1], &terminal_buffer[index], index - index / VGA_WIDTH * terminal_row);
    }
  } else if (c == 9) {
    //tab
    for (; terminal_column % 8 != 0;) {
      terminal_putchar(' ');
    }
  } else if (c == 10) {
    //newline
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
      terminal_scroll();
  } else if (c == 12) {
    //form feed
    terminal_init();
  } else if (c == 13) {
    //carriage return
    terminal_column = 0;
  } else if (c == 127) {
    //delete
    if (terminal_column != 0) {
      --terminal_column;
      linecpy(&terminal_buffer[index], &terminal_buffer[index+1], index - index / VGA_WIDTH * terminal_row);
    }
  } else {
    //printable character
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      if (++terminal_row == VGA_HEIGHT)
        terminal_scroll();
    }
  }
}

//write a given number of characters
void terminal_write(const char* data, size_t size) {
  for (size_t i = 0; i < size; ++i)
    terminal_putchar(data[i]);
}

//write a null terminated string
void terminal_writestring(const char* data) {
  terminal_write(data, strlen(data));
}