
static volatile uint32_t timer_tick;

#define IRQ_TIMER (IRQ0 + 0)

static inline void timer_interrupt_handler(registers_t * registers)
{
	(void) registers;

	timer_tick ++;

	screen_x = SCREEN_WIDTH - 1;
	screen_y = 0;
#if MACHINE_IBMPC
	screen_attribute =
		screen_mode == SCREEN_MODE_MDA
			? SCREEN_ATTR_MDA_NORMAL | SCREEN_ATTR_FG_LIGHT
			: SCREEN_ATTR_CGA_FG_WHITE | SCREEN_ATTR_CGA_BG_BLACK;
#elif MACHINE_NECPC98
	screen_attribute = SCREEN_ATTR_WHITE;
#endif
	screen_putchar("/-\\|"[timer_tick & 3]);
}

