
typedef struct acia_t
{
	uint8_t control, _0, data, _1;
} acia_t;
#define acia (*(volatile acia_t *)0xFFFFFC00)
#define ACIA_IRQ 0x80
#define MFP_BASE ((volatile uint8_t *)0xFFFFFA00)

#include "../pc86/scancode.c"
#include "../pc86/keyproc.c"

