
// Intel 8253 programmable interval timer

#if MACHINE_IBMPC
#define PORT_PIT_DATA0    0x40
#define PORT_PIT_DATA1    (PORT_PIT_DATA0 + 1)
#define PORT_PIT_DATA2    (PORT_PIT_DATA0 + 2)
#define PORT_PIT_COMMAND  (PORT_PIT_DATA0 + 3)
#elif MACHINE_NECPC98
#define PORT_PIT_DATA0    0x71
#define PORT_PIT_DATA1    (PORT_PIT_DATA0 + 2)
#define PORT_PIT_DATA2    (PORT_PIT_DATA0 + 4)
#define PORT_PIT_COMMAND  (PORT_PIT_DATA0 + 6)
#endif

#define PIT_CHANNEL0 0x00
#define PIT_ACCESS_WORD 0x30
#define PIT_SQUARE_WAVE 0x06

static inline void i8253_setup(uint32_t frequency)
{
	uint16_t tick_interval = 1193180 / frequency;
	outb(PORT_PIT_COMMAND, PIT_CHANNEL0 | PIT_ACCESS_WORD | PIT_SQUARE_WAVE);
	outb(PORT_PIT_DATA0,   tick_interval & 0xFF);
	outb(PORT_PIT_DATA0,   tick_interval >> 8);
}

