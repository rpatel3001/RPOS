#ifndef _KEYBOARD_KEYMAP_H
#define _KEYBOARD_KEYMAP_H

// subarray indices as follows:
// 0 - unmodified
// 1 - shift
// 2 - control
// 3 - shift + control
// 4 - alt
// 5 - alt + shift
// 6 - alt + shift + control
// 7 - alt + control
uint8_t keymap[84][8] = {
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x00,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x01,
	{'1',   '!',    0,    0,    0,    0,    0,    0},    // 0x02,
	{'2',   '@',    0,    0,    0,    0,    0,    0},    // 0x03,
	{'3',   '#',    0,    0,    0,    0,    0,    0},    // 0x04,
	{'4',   '$',    0,    0,    0,    0,    0,    0},    // 0x05,
	{'5',   '%',    0,    0,    0,    0,    0,    0},    // 0x06,
	{'6',   '^',    0,    0,    0,    0,    0,    0},    // 0x07,
	{'7',   '&',    0,    0,    0,    0,    0,    0},    // 0x08,
	{'8',   '*',    0,    0,    0,    0,    0,    0},    // 0x09,
	{'9',   '(',    0,    0,    0,    0,    0,    0},    // 0x0A,
	{'0',   ')',    0,    0,    0,    0,    0,    0},    // 0x0B,
	{'-',   '_',    0,    0,    0,    0,    0,    0},    // 0x0C,
	{'=',   '+',    0,    0,    0,    0,    0,    0},    // 0x0D,
	{'\b',  '\b',   0,    0,    0,    0,    0,    0},    // 0x0E,
	{'\t',  '\t',   0,    0,    0,    0,    0,    0},    // 0x0F,
	{'q',   'Q',    0,    0,    0,    0,    0,    0},    // 0x10,
	{'w',   'W',    0,    0,    0,    0,    0,    0},    // 0x11,
	{'e',   'E',    0,    0,    0,    0,    0,    0},    // 0x12,
	{'r',   'R',    0,    0,    0,    0,    0,    0},    // 0x13,
	{'t',   'T',    0,    0,    0,    0,    0,    0},    // 0x14,
	{'y',   'Y',    0,    0,    0,    0,    0,    0},    // 0x15,
	{'u',   'U',    0,    0,    0,    0,    0,    0},    // 0x16,
	{'i',   'I',    0,    0,    0,    0,    0,    0},    // 0x17,
	{'o',   'O',    0,    0,    0,    0,    0,    0},    // 0x18,
	{'p',   'P',    0,    0,    0,    0,    0,    0},    // 0x19,
	{'[',   '{',    0,    0,    0,    0,    0,    0},    // 0x1A,
	{']',   '}',    0,    0,    0,    0,    0,    0},    // 0x1B,
	{'\n',  '\n',   0,    0,    0,    0,    0,    0},    // 0x1C,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x1D,
	{'a',   'A',    0,    0,    0,    0,    0,    0},    // 0x1E,
	{'s',   'S',    0,    0,    0,    0,    0,    0},    // 0x1F,
	{'d',   'D',    0,    0,    0,    0,    0,    0},    // 0x20,
	{'f',   'F',    0,    0,    0,    0,    0,    0},    // 0x21,
	{'g',   'G',    0,    0,    0,    0,    0,    0},    // 0x22,
	{'h',   'H',    0,    0,    0,    0,    0,    0},    // 0x23,
	{'j',   'J',    0,    0,    0,    0,    0,    0},    // 0x24,
	{'k',   'K',    0,    0,    0,    0,    0,    0},    // 0x25,
	{'l',   'L',    0,    0,    0,    0,    0,    0},    // 0x26,
	{';',   ':',    0,    0,    0,    0,    0,    0},    // 0x27,
	{'\'',  '"',    0,    0,    0,    0,    0,    0},    // 0x28,
	{'`',   '~',    0,    0,    0,    0,    0,    0},    // 0x29,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x2A,
	{'\\',  '|',    0,    0,    0,    0,    0,    0},    // 0x2B,
	{'z',   'Z',    0,    0,    0,    0,    0,    0},    // 0x2C,
	{'x',   'X',    0,    0,    0,    0,    0,    0},    // 0x2D,
	{'c',   'C',    0,    0,    0,    0,    0,    0},    // 0x2E,
	{'v',   'V',    0,    0,    0,    0,    0,    0},    // 0x2F,
	{'b',   'B',    0,    0,    0,    0,    0,    0},    // 0x30,
	{'n',   'N',    0,    0,    0,    0,    0,    0},    // 0x31,
	{'m',   'M',    0,    0,    0,    0,    0,    0},    // 0x32,
	{',',   '<',    0,    0,    0,    0,    0,    0},    // 0x33,
	{'.',   '>',    0,    0,    0,    0,    0,    0},    // 0x34,
	{'/',   '?',    0,    0,    0,    0,    0,    0},    // 0x35,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x36,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x37,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x38,
	{' ',     0,    0,    0,    0,    0,    0,    0},    // 0x39,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x3A,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x3B,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x3C,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x3D,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x3E,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x3F,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x40,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x41,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x42,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x43,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x44,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x45,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x46,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x47,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x48,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x49,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x4A,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x4B,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x4C,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x4D,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x4E,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x4F,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x50,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x51,
	{  0,     0,    0,    0,    0,    0,    0,    0},    // 0x52,
	{127,     0,    0,    0,    0,    0,    0,    0}     // 0x53
};

#endif