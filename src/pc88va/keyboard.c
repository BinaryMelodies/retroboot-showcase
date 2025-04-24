
#define PORT_PS2_DATA 0x1C1

#include "../pc98/scancode.c"
#include "../pc86/keyproc.c"

#define IRQ_KEYBOARD (IRQ0 + 1)

static inline void keyboard_interrupt_handler(registers_t * registers)
{
	(void) registers;

	uint8_t scancode;
	scancode = inp(PORT_PS2_DATA);

	screen_x = SCREEN_WIDTH - 2;
	screen_y = 1;
	screen_attribute = SCREEN_ATTR_BLUE | SCREEN_ATTR_INVERTED;
	screen_puthex(scancode);

	keyboard_interrupt_process(scancode);
}

