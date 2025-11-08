
#include "../pc86/scancode.c" // TODO
#include "../pc86/keyproc.c"

static inline void keyboard_interrupt_handler(registers_t * registers)
{
	(void) registers;

	uint8_t scancode = ~CIAASDR;
	scancode = (scancode >> 1) | (scancode << 7);

	CIAACRA = 0x48;

	CIAATALO = 0x3D;
	CIAATAHI = 0x00;

	while((CIAAICR & 1) == 0)
		;

	CIAACRA &= ~0x40;

	screen_x = SCREEN_WIDTH - 2;
	screen_y = 1;
	screen_attribute = SCREEN_ATTR_CGA_FG_WHITE | SCREEN_ATTR_CGA_BG_BLUE;
	screen_puthex(scancode);
}

