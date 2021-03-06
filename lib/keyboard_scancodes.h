// mostly taken from
// https://msdn.microsoft.com/en-us/library/aa299374
// and
// https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html

#ifndef _KEYBORD_SCANCODES_H
#define _KEYBORD_SCANCODES_H

// keyboard scancodes for translated set 2 (pretty much set 1)
#define SCAN_ESCAPE         0x01
#define SCAN_1              0x02
#define SCAN_2              0x03
#define SCAN_3              0x04
#define SCAN_4              0x05
#define SCAN_5              0x06
#define SCAN_6              0x07
#define SCAN_7              0x08
#define SCAN_8              0x09
#define SCAN_9              0x0A
#define SCAN_0              0x0B
#define SCAN_MINUS          0x0C
#define SCAN_EQUALS         0x0D
#define SCAN_BACKSPACE      0x0E
#define SCAN_TAB            0x0F
#define SCAN_Q              0x10
#define SCAN_W              0x11
#define SCAN_E              0x12
#define SCAN_R              0x13
#define SCAN_T              0x14
#define SCAN_Y              0x15
#define SCAN_U              0x16
#define SCAN_I              0x17
#define SCAN_O              0x18
#define SCAN_P              0x19
#define SCAN_LEFTSQUARE     0x1A
#define SCAN_RIGHTSQUARE    0x1B
#define SCAN_ENTER          0x1C
#define SCAN_CONTROL        0x1D
#define SCAN_A              0x1E
#define SCAN_S              0x1F
#define SCAN_D              0x20
#define SCAN_F              0x21
#define SCAN_G              0x22
#define SCAN_H              0x23
#define SCAN_J              0x24
#define SCAN_K              0x25
#define SCAN_L              0x26
#define SCAN_SEMICOLON      0x27
#define SCAN_SINGLEQUOTE    0x28
#define SCAN_BACKTICK       0x29
#define SCAN_LEFTSHIFT      0x2A
#define SCAN_BACKSLASH      0x2B
#define SCAN_Z              0x2C
#define SCAN_X              0x2D
#define SCAN_C              0x2E
#define SCAN_V              0x2F
#define SCAN_B              0x30
#define SCAN_N              0x31
#define SCAN_M              0x32
#define SCAN_COMMA          0x33
#define SCAN_PERIOD         0x34
#define SCAN_SLASH          0x35
#define SCAN_RIGHTSHIFT     0x36
#define SCAN_PRINTSCREEN    0x37
#define SCAN_ALT            0x38
#define SCAN_SPACE          0x39
#define SCAN_CAPSLOCK       0x3A
#define SCAN_F1             0x3B
#define SCAN_F2             0x3C
#define SCAN_F3             0x3D
#define SCAN_F4             0x3E
#define SCAN_F5             0x3F
#define SCAN_F6             0x40
#define SCAN_F7             0x41
#define SCAN_F8             0x42
#define SCAN_F9             0x43
#define SCAN_F10            0x44
#define SCAN_NUMLOCK        0x45
#define SCAN_SCROLLLOCK     0x46
#define SCAN_HOME           0x47
#define SCAN_UP             0x48
#define SCAN_PAGEUP         0x49
#define SCAN_GRAYMINUS      0x4A
#define SCAN_LEFTARROW      0x4B
#define SCAN_GRAYCENTER     0x4C
#define SCAN_RIGHTARROW     0x4D
#define SCAN_GRAYPLUS       0x4E
#define SCAN_END            0x4F
#define SCAN_DOWNARROW      0x50
#define SCAN_PAGEDOWN       0x51
#define SCAN_INSERT         0x52
#define SCAN_DELETE         0x53

// status codes
#define SCAN_ERROR1         0x00
#define SCAN_BATOK          0xAA
#define SCAN_ECHO           0xEE
#define SCAN_ACK            0xFA
#define SCAN_BATERROR       0xFC
#define SCAN_INTERNALFAIL   0xFD
#define SCAN_RESEND         0xFE
#define SCAN_ERROR2         0xFF

// escaped scancodes
#define SCAN_LEFTSUPER      0x5B
#define SCAN_RIGHTSUPER     0x5C

#endif