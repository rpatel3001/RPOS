#include <string.h>
#include <serial.h>
#include <kernel/util.h>
#include "keyboard_scancodes.h"
#include "keyboard.h"

void (*callback)(key_press);

static const key_press invalid_keypress = {false, false, false, false, false, 0};

bool super_down = false;
bool shift_down = false;
bool control_down = false;
bool alt_down = false;
bool capslock_down = false;
bool function_down = false;

// convert a scancode into a keypress event
key_press scan_to_kp(uint8_t scan[]) {
	uint8_t pause[] = {0xe1, 0x1d, 0x45, 0xe1, 0x9d, 0xc5};
	uint8_t controlpause[] = {0xe0, 0x46, 0xe0, 0xc6, 0x00, 0x00};

	if (!memcmp(scan, pause, 6)) {
		// pause
		return invalid_keypress;
	} else if (!memcmp(scan, controlpause, 6)) {
		// control+pause
		return invalid_keypress;
	}

	for (int i = 0; i < 6; i += 2) {
		if (scan[i] == 0 && scan[i+1] == 0) break;

		bool escaped = scan[i] == 0xE0;
		uint8_t code = escaped ? scan[i + 1] : scan[i];
		bool downevent = !(0x80 & code);

		if (!downevent) {
			code &= 0x7F;
		}

		switch (code) {
		//handle modifiers
		case SCAN_ALT:
			//left key if not escaped
			alt_down = downevent;
			break;
		case SCAN_LEFTSHIFT:
		case SCAN_RIGHTSHIFT:
			if (!escaped) {
				//not a fake event
				shift_down = downevent;
			}
			break;
		case SCAN_LEFTSUPER:
		case SCAN_RIGHTSUPER:
			if (escaped) {
				super_down = downevent;
			}
			break;
		case SCAN_CONTROL:
			//left key if not escaped
			control_down = downevent;
			break;
		case SCAN_SCROLLLOCK:
			if (escaped) {
				//control+break
			} else {
				//scroll lock
			}
			break;
		case SCAN_CAPSLOCK:
			if (downevent) {
				capslock_down = !capslock_down;
			}
			break;
		case SCAN_PRINTSCREEN:
			if (escaped) {
				//control + printscreen
			}
			break;
		//will get around to these eventually
		case SCAN_ESCAPE:
		case SCAN_HOME:
		case SCAN_PAGEUP:
		case SCAN_GRAYCENTER:
		case SCAN_END:
		case SCAN_PAGEDOWN:
		case SCAN_INSERT:
		case SCAN_F1:
		case SCAN_F2:
		case SCAN_F3:
		case SCAN_F4:
		case SCAN_F5:
		case SCAN_F6:
		case SCAN_F7:
		case SCAN_F8:
		case SCAN_F9:
		case SCAN_F10:
			break;
		//not keypresses, keyboard status codes
		case SCAN_ERROR1:
		case SCAN_BATOK:
		case SCAN_ECHO:
		case SCAN_ACK:
		case SCAN_BATERROR:
		case SCAN_INTERNALFAIL:
		case SCAN_RESEND:
		case SCAN_ERROR2:
			break;
		default:
			if (downevent) {
				key_press ret = {
					.shift = (!capslock_down && shift_down)
					|| (capslock_down && !shift_down),
					.control = control_down,
					.alt = alt_down,
					.super = super_down,
					.function = function_down,
					.keycode = code
				};
				return ret;
			}
		}
	}
	return invalid_keypress;
}

//return if a key is invalid;
static bool is_valid_key(key_press kp) {
	return kp.shift != false ||
	       kp.alt != false ||
	       kp.control != false ||
	       kp.super != false ||
	       kp.function != false ||
	       kp.keycode != 0;
}

// C keyboard ISR
void keyboard_ISR(void) {
	uint8_t scancode[6];
	size_t index = 0;

	memset(scancode, 0, 6);

	uint8_t status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	while (index < 6 && status & 0x01) {
		scancode[index++] = read_port(KEYBOARD_DATA_PORT);
		status = read_port(KEYBOARD_STATUS_PORT);
	}

	if (!scancode[0]) {
		return;
	}

	key_press kp = scan_to_kp(scancode);
	if (is_valid_key(kp)) {
		callback(kp);
	}
}

//initialize the keyboard
void kb_init(void (*handler)(key_press)) {
	callback = handler;
	serial_writestring("Keyboard Initialized\n");
}