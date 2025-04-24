
typedef struct acia_t
{
	uint8_t control, _0, data, _1;
} acia_t;
#define acia (*(volatile acia_t *)0xFFFFFC00)
#define ACIA_IRQ 0x80
#define MFP_BASE ((volatile uint8_t *)0xFFFFFA00)

#include "../pc86/scancode.c"
#include "../pc86/keyproc.c"

#define IRQ_KEYBOARD 0x46

static inline void keyboard_interrupt_handler(registers_t * registers)
{
	(void) registers;

	do
	{
		if((acia.control & ACIA_IRQ) != 0)
		{
			uint8_t scancode = acia.data;

			screen_x = SCREEN_WIDTH - 2;
			screen_y = 1;
			screen_attribute = SCREEN_ATTR_CGA_FG_WHITE | SCREEN_ATTR_CGA_BG_BLUE;
			screen_puthex(scancode);

			keyboard_interrupt_process(scancode);
		}
	} while(!(MFP_BASE[1] & 0x10));
	MFP_BASE[0x11] = 0xBF;
}

