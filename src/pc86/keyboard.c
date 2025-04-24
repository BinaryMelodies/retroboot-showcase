
#define PORT_PS2_DATA 0x60

#include "scancode.c"
#include "keyproc.c"

#define IRQ_KEYBOARD (IRQ0 + 1)

typedef struct registers_t registers_t;
static inline void keyboard_interrupt_handler(registers_t * registers)
{
	(void) registers;

	uint8_t scancode;
	scancode = inp(PORT_PS2_DATA);

	screen_x = SCREEN_WIDTH - 2;
	screen_y = 1;
	screen_attribute =
		screen_mode == SCREEN_MODE_MDA
			? SCREEN_ATTR_MDA_NORMAL | SCREEN_ATTR_FG_LIGHT
			: SCREEN_ATTR_CGA_FG_WHITE | SCREEN_ATTR_CGA_BG_BLUE;
	screen_puthex(scancode);

	keyboard_interrupt_process(scancode);
}

