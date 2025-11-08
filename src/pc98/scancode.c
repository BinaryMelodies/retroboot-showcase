
// This is a mapping based on the position of the keys on a keyboard and their analoguous keys on an American IBM keyboard
// The symbolic names are chosen according to the DOSBox-X emulator

enum
{
	_KBD_ESCAPE,
	_KBD_1,
	_KBD_2,
	_KBD_3,
	_KBD_4,
	_KBD_5,
	_KBD_6,
	_KBD_7,
	_KBD_8,
	_KBD_9,
	_KBD_0,
	_KBD_MINUS,
	_KBD_EQUALS, // actually ^
	_KBD_CARET, // actually ¥
	_KBD_BACKSPACE,
	_KBD_TAB,
	_KBD_Q,
	_KBD_W,
	_KBD_E,
	_KBD_R,
	_KBD_T,
	_KBD_Y,
	_KBD_U,
	_KBD_I,
	_KBD_O,
	_KBD_P,
	_KBD_OPEN_BRACKET, // actually @
	_KBD_CLOSE_BRACKET, // actually [
	_KBD_ENTER,
	_KBD_A,
	_KBD_S,
	_KBD_D,
	_KBD_F,
	_KBD_G,
	_KBD_H,
	_KBD_J,
	_KBD_K,
	_KBD_L,
	_KBD_SEMICOLON,
	_KBD_APOSTROPHE, // actually :
	_KBD_BACKSLASH, // actually ]
	_KBD_Z,
	_KBD_X,
	_KBD_C,
	_KBD_V,
	_KBD_B,
	_KBD_N,
	_KBD_M,
	_KBD_COMMA,
	_KBD_PERIOD,
	_KBD_SLASH,
	_KBD_UNDERSCORE, // Japanese specific
	_KBD_SPACE,
	_KBD_XFER, // Japanese specific
	_KBD_PGDN,
	_KBD_PGUP,
	_KBD_INS,
	_KBD_DEL,
	_KBD_UP,
	_KBD_LEFT,
	_KBD_RIGHT,
	_KBD_DOWN,
	_KBD_HOME,
	_KBD_END, // actually HELP
	_KBD_MINUS_2, // numpad
	_KBD_SLASH_2, // numpad
	_KBD_HOME_2, // numpad
	_KBD_UP_2, // numpad
	_KBD_PGUP_2, // numpad
	_KBD_ASTERISK, // numpad
	_KBD_LEFT_2, // numpad
	_KBD_NUM5, // numpad
	_KBD_RIGHT_2, // numpad
	_KBD_PLUS, // numpad
	_KBD_END_2, // numpad
	_KBD_DOWN_2, // numpad
	_KBD_PGDN_2, // numpad
	_KBD_EQUALS_2, // numpad
	_KBD_INS_2, // numpad
	_KBD_COMMA_2, // numpad
	_KBD_DEL_2 = 0x50, // numpad
	_KBD_NFER, // Japanese specific
#if MACHINE_NECPC98
	_KBD_F11,
	_KBD_F12,
	_KBD_F13,
	_KBD_F14,
	_KBD_F15,

	_KBD_HOME_3 = 0x5E,
#endif

	_KBD_STOP = 0x60,
	_KBD_COPY,
	_KBD_F1,
	_KBD_F2,
	_KBD_F3,
	_KBD_F4,
	_KBD_F5,
	_KBD_F6,
	_KBD_F7,
	_KBD_F8,
	_KBD_F9,
	_KBD_F10,

	_KBD_SHIFT = 0x70,
	_KBD_CAPSLOCK,
	_KBD_KANA,
	// TODO: is this a typo, or are PC-88VA scancodes actually shifted by 1 compared to PC-98 scancodes?
	_KBD_ALT, // actually GRPH
	_KBD_CTRL,

#if MACHINE_IBMPC88VA
	_KBD_SHIFT_2 = 0x78,
	_KBD_ENTER_2,
	_KBD_PC,
	_KBD_FULLWIDTH,
#endif
};

static const struct
{
	char normal;
	char shifted;
} keyboard_scancode_table[128] =
{
	[_KBD_ESCAPE] =
		{ '\33', '\33' },
	[_KBD_1] =
		{ '1', '!' },
	[_KBD_2] =
		{ '2', '@' },
	[_KBD_3] =
		{ '3', '#' },
	[_KBD_4] =
		{ '4', '$' },
	[_KBD_5] =
		{ '5', '%' },
	[_KBD_6] =
		{ '6', '^' },
	[_KBD_7] =
		{ '7', '&' },
	[_KBD_8] =
		{ '8', '*' },
	[_KBD_9] =
		{ '9', '(' },
	[_KBD_0] =
		{ '0', ')' },
	[_KBD_MINUS] =
		{ '-', '_' },
	[_KBD_EQUALS] =
		{ '=', '+' },
	[_KBD_CARET] =
		{ '^', '^' },
	[_KBD_BACKSPACE] =
		{ '\b', '\b' },
	[_KBD_TAB] =
		{ '\t', '\t' },
	[_KBD_Q] =
		{ 'q', 'Q' },
	[_KBD_W] =
		{ 'w', 'W' },
	[_KBD_E] =
		{ 'e', 'E' },
	[_KBD_R] =
		{ 'r', 'R' },
	[_KBD_T] =
		{ 't', 'T' },
	[_KBD_Y] =
		{ 'y', 'Y' },
	[_KBD_U] =
		{ 'u', 'U' },
	[_KBD_I] =
		{ 'i', 'I' },
	[_KBD_O] =
		{ 'o', 'O' },
	[_KBD_P] =
		{ 'p', 'P' },
	[_KBD_OPEN_BRACKET] =
		{ '[', '{' },
	[_KBD_CLOSE_BRACKET] =
		{ ']', '}' },
	[_KBD_ENTER] =
		{ '\n', '\n' },
	[_KBD_A] =
		{ 'a', 'A' },
	[_KBD_S] =
		{ 's', 'S' },
	[_KBD_D] =
		{ 'd', 'D' },
	[_KBD_F] =
		{ 'f', 'F' },
	[_KBD_G] =
		{ 'g', 'G' },
	[_KBD_H] =
		{ 'h', 'H' },
	[_KBD_J] =
		{ 'j', 'J' },
	[_KBD_K] =
		{ 'k', 'K' },
	[_KBD_L] =
		{ 'l', 'L' },
	[_KBD_SEMICOLON] =
		{ ';', ':' },
	[_KBD_APOSTROPHE] =
		{ '\'', '"' },
	[_KBD_BACKSLASH] =
		{ '\\', '|' },
	[_KBD_Z] =
		{ 'z', 'Z' },
	[_KBD_X] =
		{ 'x', 'X' },
	[_KBD_C] =
		{ 'c', 'C' },
	[_KBD_V] =
		{ 'v', 'V' },
	[_KBD_B] =
		{ 'b', 'B' },
	[_KBD_N] =
		{ 'n', 'N' },
	[_KBD_M] =
		{ 'm', 'M' },
	[_KBD_COMMA] =
		{ ',', '<' },
	[_KBD_PERIOD] =
		{ '.', '>' },
	[_KBD_SLASH] =
		{ '/', '?' },

	[_KBD_SPACE] =
		{ ' ', ' ' },

	[_KBD_MINUS_2] =
		{ '-', '-' },
	[_KBD_SLASH_2] =
		{ '/', '/' },
	[_KBD_HOME_2] =
		{ '7', '7' },
	[_KBD_UP_2] =
		{ '8', '8' },
	[_KBD_PGUP_2] =
		{ '9', '9' },
	[_KBD_ASTERISK] =
		{ '*', '*' },
	[_KBD_LEFT_2] =
		{ '4', '4' },
	[_KBD_NUM5] =
		{ '5', '5' },
	[_KBD_RIGHT_2] =
		{ '6', '6' },
	[_KBD_PLUS] =
		{ '+', '+' },
	[_KBD_END_2] =
		{ '1', '1' },
	[_KBD_DOWN_2] =
		{ '2', '2' },
	[_KBD_PGDN_2] =
		{ '3', '3' },
	[_KBD_EQUALS_2] =
		{ '=', '=' },
	[_KBD_INS_2] =
		{ '0', '0' },
	[_KBD_COMMA_2] =
		{ ',', ',' },
	[_KBD_DEL_2] =
		{ '.', '.' },

#if MACHINE_IBMPC88VA
	[_KBD_ENTER_2] =
		{ '\n', '\n' },
#endif
};

#define PS2_SCANCODE_LSHIFT _KBD_SHIFT
#define PS2_SCANCODE_RSHIFT _KBD_SHIFT
#define PS2_SCANCODE_RELEASED 0x80

