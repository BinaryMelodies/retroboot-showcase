
#define USART_DATA_REGISTER (*(uint8_t *)0xE8802F)

#include "../x68000/scancode.c"
#include "../pc86/keyproc.c"

#define IRQ_KEYBOARD 0x4C

static inline void keyboard_interrupt_handler(registers_t * registers)
{
	(void) registers;

	uint8_t scancode = USART_DATA_REGISTER;

	if(scancode != 0xFF)
	{
		screen_x = SCREEN_WIDTH - 2;
		screen_y = 1;
		screen_attribute = SCREEN_ATTR_CGA_FG_WHITE | SCREEN_ATTR_CGA_BG_BLUE;
		screen_puthex(scancode);

		keyboard_interrupt_process(scancode);
	}
}

