
#if defined IBMPC
# define PORT_PS2_DATA     0x60
#elif defined NECPC98
# define PORT_PS2_DATA     0x41
#endif

static const struct
{
	char normal;
	char shifted;
} keyboard_scancode_table[256] =
{
#if defined IBMPC || defined ATARI
	[0x01] = { '\33', '\33' },
	[0x02] = { '1', '!' },
	[0x03] = { '2', '@' },
	[0x04] = { '3', '#' },
	[0x05] = { '4', '$' },
	[0x06] = { '5', '%' },
	[0x07] = { '6', '^' },
	[0x08] = { '7', '&' },
	[0x09] = { '8', '*' },
	[0x0A] = { '9', '(' },
	[0x0B] = { '0', ')' },
	[0x0C] = { '-', '_' },
	[0x0D] = { '=', '+' },
	[0x0E] = { '\b', '\b' },
	[0x0F] = { '\t', '\t' },
	[0x10] = { 'q', 'Q' },
	[0x11] = { 'w', 'W' },
	[0x12] = { 'e', 'E' },
	[0x13] = { 'r', 'R' },
	[0x14] = { 't', 'T' },
	[0x15] = { 'y', 'Y' },
	[0x16] = { 'u', 'U' },
	[0x17] = { 'i', 'I' },
	[0x18] = { 'o', 'O' },
	[0x19] = { 'p', 'P' },
	[0x1A] = { '[', '{' },
	[0x1B] = { ']', '}' },
	[0x1C] = { '\n', '\n' },

	[0x1E] = { 'a', 'A' },
	[0x1F] = { 's', 'S' },
	[0x20] = { 'd', 'D' },
	[0x21] = { 'f', 'F' },
	[0x22] = { 'g', 'G' },
	[0x23] = { 'h', 'H' },
	[0x24] = { 'j', 'J' },
	[0x25] = { 'k', 'K' },
	[0x26] = { 'l', 'L' },
	[0x27] = { ';', ':' },
	[0x28] = { '\'', '"' },
	[0x29] = { '`', '~' },

	[0x2B] = { '\\', '|' },
	[0x2C] = { 'z', 'Z' },
	[0x2D] = { 'x', 'X' },
	[0x2E] = { 'c', 'C' },
	[0x2F] = { 'v', 'V' },
	[0x30] = { 'b', 'B' },
	[0x31] = { 'n', 'N' },
	[0x32] = { 'm', 'M' },
	[0x33] = { ',', '<' },
	[0x34] = { '.', '>' },
	[0x35] = { '/', '?' },

	[0x39] = { ' ', ' ' },
#elif defined NECPC98
	[0x00] = { '`', '~' },
	[0x01] = { '1', '!' },
	[0x02] = { '2', '@' },
	[0x03] = { '3', '#' },
	[0x04] = { '4', '$' },
	[0x05] = { '5', '%' },
	[0x06] = { '6', '^' },
	[0x07] = { '7', '&' },
	[0x08] = { '8', '*' },
	[0x09] = { '9', '(' },
	[0x0A] = { '0', ')' },
	[0x0B] = { '-', '_' },
	[0x0C] = { '=', '+' },

	[0x0E] = { '\b', '\b' },
	[0x0F] = { '\t', '\t' },
	[0x10] = { 'q', 'Q' },
	[0x11] = { 'w', 'W' },
	[0x12] = { 'e', 'E' },
	[0x13] = { 'r', 'R' },
	[0x14] = { 't', 'T' },
	[0x15] = { 'y', 'Y' },
	[0x16] = { 'u', 'U' },
	[0x17] = { 'i', 'I' },
	[0x18] = { 'o', 'O' },
	[0x19] = { 'p', 'P' },
	[0x1A] = { '[', '{' },
	[0x1B] = { ']', '}' },
	[0x1C] = { '\n', '\n' },
	[0x1D] = { 'a', 'A' },
	[0x1E] = { 's', 'S' },
	[0x1F] = { 'd', 'D' },
	[0x20] = { 'f', 'F' },
	[0x21] = { 'g', 'G' },
	[0x22] = { 'h', 'H' },
	[0x23] = { 'j', 'J' },
	[0x24] = { 'k', 'K' },
	[0x25] = { 'l', 'L' },
	[0x26] = { ';', ':' },
	[0x27] = { '\'', '"' },
	[0x28] = { '\\', '|' },
	[0x29] = { 'z', 'Z' },
	[0x2A] = { 'x', 'X' },
	[0x2B] = { 'c', 'C' },
	[0x2C] = { 'v', 'V' },
	[0x2D] = { 'b', 'B' },
	[0x2E] = { 'n', 'N' },
	[0x2F] = { 'm', 'M' },
	[0x30] = { ',', '<' },
	[0x31] = { '.', '>' },
	[0x32] = { '/', '?' },

	[0x34] = { ' ', ' ' },

	[0x60] = { '\33', '\33' },
#endif
};

static volatile bool keyboard_shift = false;

#define KEYBOARD_BUFFER_SIZE 16
static volatile char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static volatile size_t keyboard_buffer_count;
static volatile size_t keyboard_buffer_pointer;
static volatile bool keyboard_used = false;

static inline void keyboard_buffer_push(char c)
{
	while(keyboard_used)
		;
	keyboard_used = true;
	if(keyboard_buffer_count < KEYBOARD_BUFFER_SIZE)
	{
#if !ATARI
		keyboard_buffer[(keyboard_buffer_pointer + keyboard_buffer_count++) % KEYBOARD_BUFFER_SIZE] = c;
#else
		keyboard_buffer[(keyboard_buffer_pointer + keyboard_buffer_count++) & (KEYBOARD_BUFFER_SIZE - 1)] = c;
#endif
	}
	keyboard_used = false;
}

static inline bool keyboard_buffer_empty(void)
{
	return keyboard_buffer_count == 0;
}

static inline int keyboard_buffer_remove(void)
{
	if(keyboard_buffer_empty())
	{
		return -1;
	}
	else
	{
		while(keyboard_used)
			;
		keyboard_used = true;
		int c = keyboard_buffer[keyboard_buffer_pointer];
#if !ATARI
		keyboard_buffer_pointer = (keyboard_buffer_pointer + 1) % KEYBOARD_BUFFER_SIZE;
#else
		keyboard_buffer_pointer = (keyboard_buffer_pointer + 1) & (KEYBOARD_BUFFER_SIZE - 1);
#endif
		keyboard_buffer_count --;
		keyboard_used = false;
		return c;
	}
}

#if defined IBMPC
# define PS2_SCANCODE_LSHIFT 0x2A
# define PS2_SCANCODE_RSHIFT 0x36
# define PS2_SCANCODE_RELEASED 0x80
#elif defined NECPC98
# define PS2_SCANCODE_LSHIFT 0x70
# define PS2_SCANCODE_RSHIFT 0x70
# define PS2_SCANCODE_RELEASED 0x80
#endif

#if defined ATARI
typedef struct acia_t
{
	uint8_t control, _0, data, _1;
} acia_t;
#define acia (*(volatile acia_t *)0xFFFFFC00)
#define ACIA_IRQ 0x80
#define MFP_BASE ((volatile uint8_t *)0xFFFFFA00)

# define PS2_SCANCODE_RELEASED 0x80
# define PS2_SCANCODE_LSHIFT 0x2A
# define PS2_SCANCODE_RSHIFT 0x36
#endif

static inline void keyboard_interrupt_process(uint8_t scancode)
{
#if !AMIGA && !MACINTOSH && !X68000 // TODO
	if((scancode & PS2_SCANCODE_RELEASED) == 0)
	{
		// key pressed
		if(scancode == PS2_SCANCODE_LSHIFT || scancode == PS2_SCANCODE_RSHIFT)
		{
			// shift
			keyboard_shift = true;
		}
		else
		{
			int c = keyboard_shift ? keyboard_scancode_table[scancode].shifted : keyboard_scancode_table[scancode].normal;
			keyboard_buffer_push(c);
		}
	}
	else
	{
		// key released
		if(scancode == (PS2_SCANCODE_RELEASED|PS2_SCANCODE_LSHIFT) || scancode == (PS2_SCANCODE_RELEASED|PS2_SCANCODE_RSHIFT))
		{
			// shift
			keyboard_shift = false;
		}
	}
#endif
}

static inline bool keyboard_kbhit(void)
{
	return !keyboard_buffer_empty();
}

static inline int keyboard_getch(void)
{
	while(keyboard_buffer_empty())
		;
	return keyboard_buffer_remove();
}

