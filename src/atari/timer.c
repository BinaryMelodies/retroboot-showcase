
static volatile uint32_t timer_tick;

#define IRQ_TIMER 0x45

static inline void timer_interrupt_handler(registers_t * registers)
{
	(void) registers;

	// TODO: needs processing, otherwise no more ticks will arrive

	timer_tick ++;

	screen_x = SCREEN_WIDTH - 1;
	screen_y = 0;
	screen_attribute = SCREEN_ATTR_CGA_FG_WHITE | SCREEN_ATTR_CGA_BG_BLACK;
	screen_putchar("/-\\|"[timer_tick & 3]);
}

