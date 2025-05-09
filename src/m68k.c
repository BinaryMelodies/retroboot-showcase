
typedef struct registers_t registers_t;

#if !MACHINE_AMIGA && !MACHINE_MACINTOSH && !MACHINE_X68000 // TODO
#include "keyboard.c"
#endif

# define DEFINE_ISR(__hex) \
void isr##__hex(void); \
asm( \
	".global\tisr" #__hex "\n" \
	"isr" #__hex ":\n\t" \
	"move.w\t#" #__hex ", -(%sp)\n\t" \
	"braw\tisr_common" \
);

DEFINE_ISR(0x00)
DEFINE_ISR(0x01)
DEFINE_ISR(0x02)
DEFINE_ISR(0x03)
DEFINE_ISR(0x04)
DEFINE_ISR(0x05)
DEFINE_ISR(0x06)
DEFINE_ISR(0x07)
DEFINE_ISR(0x08)
DEFINE_ISR(0x09)
DEFINE_ISR(0x0A)
DEFINE_ISR(0x0B)
DEFINE_ISR(0x0C)
DEFINE_ISR(0x0D)
DEFINE_ISR(0x0E)
DEFINE_ISR(0x0F)
DEFINE_ISR(0x10)
DEFINE_ISR(0x11)
DEFINE_ISR(0x12)
DEFINE_ISR(0x13)
DEFINE_ISR(0x14)
DEFINE_ISR(0x15)
DEFINE_ISR(0x16)
DEFINE_ISR(0x17)
DEFINE_ISR(0x18)
DEFINE_ISR(0x19)
DEFINE_ISR(0x1A)
DEFINE_ISR(0x1B)
DEFINE_ISR(0x1C)
DEFINE_ISR(0x1D)
DEFINE_ISR(0x1E)
DEFINE_ISR(0x1F)
DEFINE_ISR(0x20)
DEFINE_ISR(0x21)
DEFINE_ISR(0x22)
DEFINE_ISR(0x23)
DEFINE_ISR(0x24)
DEFINE_ISR(0x25)
DEFINE_ISR(0x26)
DEFINE_ISR(0x27)
DEFINE_ISR(0x28)
DEFINE_ISR(0x29)
DEFINE_ISR(0x2A)
DEFINE_ISR(0x2B)
DEFINE_ISR(0x2C)
DEFINE_ISR(0x2D)
DEFINE_ISR(0x2E)
DEFINE_ISR(0x2F)
DEFINE_ISR(0x30)
DEFINE_ISR(0x31)
DEFINE_ISR(0x32)
DEFINE_ISR(0x33)
DEFINE_ISR(0x34)
DEFINE_ISR(0x35)
DEFINE_ISR(0x36)
DEFINE_ISR(0x37)
DEFINE_ISR(0x38)
DEFINE_ISR(0x39)
DEFINE_ISR(0x3A)
DEFINE_ISR(0x3B)
DEFINE_ISR(0x3C)
DEFINE_ISR(0x3D)
DEFINE_ISR(0x3E)
DEFINE_ISR(0x3F)
DEFINE_ISR(0x40)
DEFINE_ISR(0x41)
DEFINE_ISR(0x42)
DEFINE_ISR(0x43)
DEFINE_ISR(0x44)
DEFINE_ISR(0x45)
DEFINE_ISR(0x46)
DEFINE_ISR(0x47)
DEFINE_ISR(0x48)
DEFINE_ISR(0x49)
DEFINE_ISR(0x4A)
DEFINE_ISR(0x4B)
DEFINE_ISR(0x4C)
DEFINE_ISR(0x4D)
DEFINE_ISR(0x4E)
DEFINE_ISR(0x4F)
DEFINE_ISR(0x50)
DEFINE_ISR(0x51)
DEFINE_ISR(0x52)
DEFINE_ISR(0x53)
DEFINE_ISR(0x54)
DEFINE_ISR(0x55)
DEFINE_ISR(0x56)
DEFINE_ISR(0x57)
DEFINE_ISR(0x58)
DEFINE_ISR(0x59)
DEFINE_ISR(0x5A)
DEFINE_ISR(0x5B)
DEFINE_ISR(0x5C)
DEFINE_ISR(0x5D)
DEFINE_ISR(0x5E)
DEFINE_ISR(0x5F)
DEFINE_ISR(0x60)
DEFINE_ISR(0x61)
DEFINE_ISR(0x62)
DEFINE_ISR(0x63)
DEFINE_ISR(0x64)
DEFINE_ISR(0x65)
DEFINE_ISR(0x66)
DEFINE_ISR(0x67)
DEFINE_ISR(0x68)
DEFINE_ISR(0x69)
DEFINE_ISR(0x6A)
DEFINE_ISR(0x6B)
DEFINE_ISR(0x6C)
DEFINE_ISR(0x6D)
DEFINE_ISR(0x6E)
DEFINE_ISR(0x6F)
DEFINE_ISR(0x70)
DEFINE_ISR(0x71)
DEFINE_ISR(0x72)
DEFINE_ISR(0x73)
DEFINE_ISR(0x74)
DEFINE_ISR(0x75)
DEFINE_ISR(0x76)
DEFINE_ISR(0x77)
DEFINE_ISR(0x78)
DEFINE_ISR(0x79)
DEFINE_ISR(0x7A)
DEFINE_ISR(0x7B)
DEFINE_ISR(0x7C)
DEFINE_ISR(0x7D)
DEFINE_ISR(0x7E)
DEFINE_ISR(0x7F)
DEFINE_ISR(0x80)
DEFINE_ISR(0x81)
DEFINE_ISR(0x82)
DEFINE_ISR(0x83)
DEFINE_ISR(0x84)
DEFINE_ISR(0x85)
DEFINE_ISR(0x86)
DEFINE_ISR(0x87)
DEFINE_ISR(0x88)
DEFINE_ISR(0x89)
DEFINE_ISR(0x8A)
DEFINE_ISR(0x8B)
DEFINE_ISR(0x8C)
DEFINE_ISR(0x8D)
DEFINE_ISR(0x8E)
DEFINE_ISR(0x8F)
DEFINE_ISR(0x90)
DEFINE_ISR(0x91)
DEFINE_ISR(0x92)
DEFINE_ISR(0x93)
DEFINE_ISR(0x94)
DEFINE_ISR(0x95)
DEFINE_ISR(0x96)
DEFINE_ISR(0x97)
DEFINE_ISR(0x98)
DEFINE_ISR(0x99)
DEFINE_ISR(0x9A)
DEFINE_ISR(0x9B)
DEFINE_ISR(0x9C)
DEFINE_ISR(0x9D)
DEFINE_ISR(0x9E)
DEFINE_ISR(0x9F)
DEFINE_ISR(0xA0)
DEFINE_ISR(0xA1)
DEFINE_ISR(0xA2)
DEFINE_ISR(0xA3)
DEFINE_ISR(0xA4)
DEFINE_ISR(0xA5)
DEFINE_ISR(0xA6)
DEFINE_ISR(0xA7)
DEFINE_ISR(0xA8)
DEFINE_ISR(0xA9)
DEFINE_ISR(0xAA)
DEFINE_ISR(0xAB)
DEFINE_ISR(0xAC)
DEFINE_ISR(0xAD)
DEFINE_ISR(0xAE)
DEFINE_ISR(0xAF)
DEFINE_ISR(0xB0)
DEFINE_ISR(0xB1)
DEFINE_ISR(0xB2)
DEFINE_ISR(0xB3)
DEFINE_ISR(0xB4)
DEFINE_ISR(0xB5)
DEFINE_ISR(0xB6)
DEFINE_ISR(0xB7)
DEFINE_ISR(0xB8)
DEFINE_ISR(0xB9)
DEFINE_ISR(0xBA)
DEFINE_ISR(0xBB)
DEFINE_ISR(0xBC)
DEFINE_ISR(0xBD)
DEFINE_ISR(0xBE)
DEFINE_ISR(0xBF)
DEFINE_ISR(0xC0)
DEFINE_ISR(0xC1)
DEFINE_ISR(0xC2)
DEFINE_ISR(0xC3)
DEFINE_ISR(0xC4)
DEFINE_ISR(0xC5)
DEFINE_ISR(0xC6)
DEFINE_ISR(0xC7)
DEFINE_ISR(0xC8)
DEFINE_ISR(0xC9)
DEFINE_ISR(0xCA)
DEFINE_ISR(0xCB)
DEFINE_ISR(0xCC)
DEFINE_ISR(0xCD)
DEFINE_ISR(0xCE)
DEFINE_ISR(0xCF)
DEFINE_ISR(0xD0)
DEFINE_ISR(0xD1)
DEFINE_ISR(0xD2)
DEFINE_ISR(0xD3)
DEFINE_ISR(0xD4)
DEFINE_ISR(0xD5)
DEFINE_ISR(0xD6)
DEFINE_ISR(0xD7)
DEFINE_ISR(0xD8)
DEFINE_ISR(0xD9)
DEFINE_ISR(0xDA)
DEFINE_ISR(0xDB)
DEFINE_ISR(0xDC)
DEFINE_ISR(0xDD)
DEFINE_ISR(0xDE)
DEFINE_ISR(0xDF)
DEFINE_ISR(0xE0)
DEFINE_ISR(0xE1)
DEFINE_ISR(0xE2)
DEFINE_ISR(0xE3)
DEFINE_ISR(0xE4)
DEFINE_ISR(0xE5)
DEFINE_ISR(0xE6)
DEFINE_ISR(0xE7)
DEFINE_ISR(0xE8)
DEFINE_ISR(0xE9)
DEFINE_ISR(0xEA)
DEFINE_ISR(0xEB)
DEFINE_ISR(0xEC)
DEFINE_ISR(0xED)
DEFINE_ISR(0xEE)
DEFINE_ISR(0xEF)
DEFINE_ISR(0xF0)
DEFINE_ISR(0xF1)
DEFINE_ISR(0xF2)
DEFINE_ISR(0xF3)
DEFINE_ISR(0xF4)
DEFINE_ISR(0xF5)
DEFINE_ISR(0xF6)
DEFINE_ISR(0xF7)
DEFINE_ISR(0xF8)
DEFINE_ISR(0xF9)
DEFINE_ISR(0xFA)
DEFINE_ISR(0xFB)
DEFINE_ISR(0xFC)
DEFINE_ISR(0xFD)
DEFINE_ISR(0xFE)
DEFINE_ISR(0xFF)

asm(
	".global\tisr_common\n\t"
	"isr_common:\n\t"
	"subq\t#4, %sp\n\t"
	"moveml\t%d0-%d7/%a0-%a6, -(%sp)\n\t"
	"movel\t%usp, %a0\n\t"
	"movel\t%a0, 0x3C(%sp)\n\t"
	"pea\t(%sp)\n\t"
	"bsrw\tinterrupt_handler\n\t"
	"addql\t#4, %sp\n\t"
	"movel\t0x3C(%sp), %a0\n\t"
	"movel\t%a0, %usp\n\t"
	"moveml\t(%sp)+, %d0-%d7/%a0-%a6\n\t"
	"addql\t#6, %sp\n\t"
	"rte"
);

typedef struct registers_t
{
	uint32_t d[8];
	uint32_t a[8];
	uint16_t interrupt_number;
	uint16_t sr;
	uint32_t pc;
} __attribute__((packed)) registers_t;

static inline void set_interrupt(uint8_t interrupt_number, void * offset)
{
	*(void **)(interrupt_number * 4) = offset;
}

void interrupt_handler(registers_t * registers)
{
	uint8_t old_screen_x = screen_x;
	uint8_t old_screen_y = screen_y;
	uint8_t old_screen_attribute = screen_attribute;

	screen_x = 0;
	screen_y = SCREEN_HEIGHT - 1;
	screen_attribute = SCREEN_ATTR_CGA_FG_YELLOW | SCREEN_ATTR_CGA_BG_RED;

	screen_putstr("Interrupt 0x");
	screen_puthex(registers->interrupt_number);
	screen_putstr(" called from 0x");
	screen_puthex(registers->pc);
	screen_putchar(' ');

	switch(registers->interrupt_number)
	{
#if 0
	case IRQ0 + 0: // timer interrupt
		timer_interrupt_handler(registers);
		break;
#endif
#ifdef IRQ_KEYBOARD
	case IRQ_KEYBOARD:
		keyboard_interrupt_handler(registers);
		break;
#endif
	}

	screen_x = old_screen_x;
	screen_y = old_screen_y;
	screen_attribute = old_screen_attribute;

	screen_video_move_cursor();
}

static inline void setup_tables(void)
{
#if MACHINE_ATARI
	acia.control = 3; // reset
	acia.control = 0x96;
#endif

/*
	set_interrupt(0x00, isr0x00);
	set_interrupt(0x01, isr0x01);
*/
	set_interrupt(0x02, isr0x02);
	set_interrupt(0x03, isr0x03);
	set_interrupt(0x04, isr0x04);
	set_interrupt(0x05, isr0x05);
	set_interrupt(0x06, isr0x06);
	set_interrupt(0x07, isr0x07);
	set_interrupt(0x08, isr0x08);
	set_interrupt(0x09, isr0x09);
	set_interrupt(0x0A, isr0x0A);
	set_interrupt(0x0B, isr0x0B);
	set_interrupt(0x0C, isr0x0C);
	set_interrupt(0x0D, isr0x0D);
	set_interrupt(0x0E, isr0x0E);
	set_interrupt(0x0F, isr0x0F);
	set_interrupt(0x10, isr0x10);
	set_interrupt(0x11, isr0x11);
	set_interrupt(0x12, isr0x12);
	set_interrupt(0x13, isr0x13);
	set_interrupt(0x14, isr0x14);
	set_interrupt(0x15, isr0x15);
	set_interrupt(0x16, isr0x16);
	set_interrupt(0x17, isr0x17);
	set_interrupt(0x18, isr0x18);
	set_interrupt(0x19, isr0x19);
	set_interrupt(0x1A, isr0x1A);
	set_interrupt(0x1B, isr0x1B);
#if !MACHINE_ATARI
	set_interrupt(0x1C, isr0x1C); // TODO: vblank, unable to return
#endif
	set_interrupt(0x1D, isr0x1D);
	set_interrupt(0x1E, isr0x1E);
	set_interrupt(0x1F, isr0x1F);
	set_interrupt(0x20, isr0x20);
	set_interrupt(0x21, isr0x21);
	set_interrupt(0x22, isr0x22);
	set_interrupt(0x23, isr0x23);
	set_interrupt(0x24, isr0x24);
	set_interrupt(0x25, isr0x25);
	set_interrupt(0x26, isr0x26);
	set_interrupt(0x27, isr0x27);
	set_interrupt(0x28, isr0x28);
	set_interrupt(0x29, isr0x29);
	set_interrupt(0x2A, isr0x2A);
	set_interrupt(0x2B, isr0x2B);
	set_interrupt(0x2C, isr0x2C);
	set_interrupt(0x2D, isr0x2D);
	set_interrupt(0x2E, isr0x2E);
	set_interrupt(0x2F, isr0x2F);
	set_interrupt(0x30, isr0x30);
	set_interrupt(0x31, isr0x31);
	set_interrupt(0x32, isr0x32);
	set_interrupt(0x33, isr0x33);
	set_interrupt(0x34, isr0x34);
	set_interrupt(0x35, isr0x35);
	set_interrupt(0x36, isr0x36);
	set_interrupt(0x37, isr0x37);
	set_interrupt(0x38, isr0x38);
	set_interrupt(0x39, isr0x39);
	set_interrupt(0x3A, isr0x3A);
	set_interrupt(0x3B, isr0x3B);
	set_interrupt(0x3C, isr0x3C);
	set_interrupt(0x3D, isr0x3D);
	set_interrupt(0x3E, isr0x3E);
	set_interrupt(0x3F, isr0x3F);
	set_interrupt(0x40, isr0x40);
	set_interrupt(0x41, isr0x41);
	set_interrupt(0x42, isr0x42);
	set_interrupt(0x43, isr0x43);
	set_interrupt(0x44, isr0x44);
	set_interrupt(0x45, isr0x45);
	set_interrupt(0x46, isr0x46);
	set_interrupt(0x47, isr0x47);
	set_interrupt(0x48, isr0x48);
	set_interrupt(0x49, isr0x49);
	set_interrupt(0x4A, isr0x4A);
	set_interrupt(0x4B, isr0x4B);
	set_interrupt(0x4C, isr0x4C);
	set_interrupt(0x4D, isr0x4D);
	set_interrupt(0x4E, isr0x4E);
	set_interrupt(0x4F, isr0x4F);
	set_interrupt(0x50, isr0x50);
	set_interrupt(0x51, isr0x51);
	set_interrupt(0x52, isr0x52);
	set_interrupt(0x53, isr0x53);
	set_interrupt(0x54, isr0x54);
	set_interrupt(0x55, isr0x55);
	set_interrupt(0x56, isr0x56);
	set_interrupt(0x57, isr0x57);
	set_interrupt(0x58, isr0x58);
	set_interrupt(0x59, isr0x59);
	set_interrupt(0x5A, isr0x5A);
	set_interrupt(0x5B, isr0x5B);
	set_interrupt(0x5C, isr0x5C);
	set_interrupt(0x5D, isr0x5D);
	set_interrupt(0x5E, isr0x5E);
	set_interrupt(0x5F, isr0x5F);
	set_interrupt(0x60, isr0x60);
	set_interrupt(0x61, isr0x61);
	set_interrupt(0x62, isr0x62);
	set_interrupt(0x63, isr0x63);
	set_interrupt(0x64, isr0x64);
	set_interrupt(0x65, isr0x65);
	set_interrupt(0x66, isr0x66);
	set_interrupt(0x67, isr0x67);
	set_interrupt(0x68, isr0x68);
	set_interrupt(0x69, isr0x69);
	set_interrupt(0x6A, isr0x6A);
	set_interrupt(0x6B, isr0x6B);
	set_interrupt(0x6C, isr0x6C);
	set_interrupt(0x6D, isr0x6D);
	set_interrupt(0x6E, isr0x6E);
	set_interrupt(0x6F, isr0x6F);
	set_interrupt(0x70, isr0x70);
	set_interrupt(0x71, isr0x71);
	set_interrupt(0x72, isr0x72);
	set_interrupt(0x73, isr0x73);
	set_interrupt(0x74, isr0x74);
	set_interrupt(0x75, isr0x75);
	set_interrupt(0x76, isr0x76);
	set_interrupt(0x77, isr0x77);
	set_interrupt(0x78, isr0x78);
	set_interrupt(0x79, isr0x79);
	set_interrupt(0x7A, isr0x7A);
	set_interrupt(0x7B, isr0x7B);
	set_interrupt(0x7C, isr0x7C);
	set_interrupt(0x7D, isr0x7D);
	set_interrupt(0x7E, isr0x7E);
	set_interrupt(0x7F, isr0x7F);
	set_interrupt(0x80, isr0x80);
	set_interrupt(0x81, isr0x81);
	set_interrupt(0x82, isr0x82);
	set_interrupt(0x83, isr0x83);
	set_interrupt(0x84, isr0x84);
	set_interrupt(0x85, isr0x85);
	set_interrupt(0x86, isr0x86);
	set_interrupt(0x87, isr0x87);
	set_interrupt(0x88, isr0x88);
	set_interrupt(0x89, isr0x89);
	set_interrupt(0x8A, isr0x8A);
	set_interrupt(0x8B, isr0x8B);
	set_interrupt(0x8C, isr0x8C);
	set_interrupt(0x8D, isr0x8D);
	set_interrupt(0x8E, isr0x8E);
	set_interrupt(0x8F, isr0x8F);
	set_interrupt(0x90, isr0x90);
	set_interrupt(0x91, isr0x91);
	set_interrupt(0x92, isr0x92);
	set_interrupt(0x93, isr0x93);
	set_interrupt(0x94, isr0x94);
	set_interrupt(0x95, isr0x95);
	set_interrupt(0x96, isr0x96);
	set_interrupt(0x97, isr0x97);
	set_interrupt(0x98, isr0x98);
	set_interrupt(0x99, isr0x99);
	set_interrupt(0x9A, isr0x9A);
	set_interrupt(0x9B, isr0x9B);
	set_interrupt(0x9C, isr0x9C);
	set_interrupt(0x9D, isr0x9D);
	set_interrupt(0x9E, isr0x9E);
	set_interrupt(0x9F, isr0x9F);
	set_interrupt(0xA0, isr0xA0);
	set_interrupt(0xA1, isr0xA1);
	set_interrupt(0xA2, isr0xA2);
	set_interrupt(0xA3, isr0xA3);
	set_interrupt(0xA4, isr0xA4);
	set_interrupt(0xA5, isr0xA5);
	set_interrupt(0xA6, isr0xA6);
	set_interrupt(0xA7, isr0xA7);
	set_interrupt(0xA8, isr0xA8);
	set_interrupt(0xA9, isr0xA9);
	set_interrupt(0xAA, isr0xAA);
	set_interrupt(0xAB, isr0xAB);
	set_interrupt(0xAC, isr0xAC);
	set_interrupt(0xAD, isr0xAD);
	set_interrupt(0xAE, isr0xAE);
	set_interrupt(0xAF, isr0xAF);
	set_interrupt(0xB0, isr0xB0);
	set_interrupt(0xB1, isr0xB1);
	set_interrupt(0xB2, isr0xB2);
	set_interrupt(0xB3, isr0xB3);
	set_interrupt(0xB4, isr0xB4);
	set_interrupt(0xB5, isr0xB5);
	set_interrupt(0xB6, isr0xB6);
	set_interrupt(0xB7, isr0xB7);
	set_interrupt(0xB8, isr0xB8);
	set_interrupt(0xB9, isr0xB9);
	set_interrupt(0xBA, isr0xBA);
	set_interrupt(0xBB, isr0xBB);
	set_interrupt(0xBC, isr0xBC);
	set_interrupt(0xBD, isr0xBD);
	set_interrupt(0xBE, isr0xBE);
	set_interrupt(0xBF, isr0xBF);
	set_interrupt(0xC0, isr0xC0);
	set_interrupt(0xC1, isr0xC1);
	set_interrupt(0xC2, isr0xC2);
	set_interrupt(0xC3, isr0xC3);
	set_interrupt(0xC4, isr0xC4);
	set_interrupt(0xC5, isr0xC5);
	set_interrupt(0xC6, isr0xC6);
	set_interrupt(0xC7, isr0xC7);
	set_interrupt(0xC8, isr0xC8);
	set_interrupt(0xC9, isr0xC9);
	set_interrupt(0xCA, isr0xCA);
	set_interrupt(0xCB, isr0xCB);
	set_interrupt(0xCC, isr0xCC);
	set_interrupt(0xCD, isr0xCD);
	set_interrupt(0xCE, isr0xCE);
	set_interrupt(0xCF, isr0xCF);
	set_interrupt(0xD0, isr0xD0);
	set_interrupt(0xD1, isr0xD1);
	set_interrupt(0xD2, isr0xD2);
	set_interrupt(0xD3, isr0xD3);
	set_interrupt(0xD4, isr0xD4);
	set_interrupt(0xD5, isr0xD5);
	set_interrupt(0xD6, isr0xD6);
	set_interrupt(0xD7, isr0xD7);
	set_interrupt(0xD8, isr0xD8);
	set_interrupt(0xD9, isr0xD9);
	set_interrupt(0xDA, isr0xDA);
	set_interrupt(0xDB, isr0xDB);
	set_interrupt(0xDC, isr0xDC);
	set_interrupt(0xDD, isr0xDD);
	set_interrupt(0xDE, isr0xDE);
	set_interrupt(0xDF, isr0xDF);
	set_interrupt(0xE0, isr0xE0);
	set_interrupt(0xE1, isr0xE1);
	set_interrupt(0xE2, isr0xE2);
	set_interrupt(0xE3, isr0xE3);
	set_interrupt(0xE4, isr0xE4);
	set_interrupt(0xE5, isr0xE5);
	set_interrupt(0xE6, isr0xE6);
	set_interrupt(0xE7, isr0xE7);
	set_interrupt(0xE8, isr0xE8);
	set_interrupt(0xE9, isr0xE9);
	set_interrupt(0xEA, isr0xEA);
	set_interrupt(0xEB, isr0xEB);
	set_interrupt(0xEC, isr0xEC);
	set_interrupt(0xED, isr0xED);
	set_interrupt(0xEE, isr0xEE);
	set_interrupt(0xEF, isr0xEF);
	set_interrupt(0xF0, isr0xF0);
	set_interrupt(0xF1, isr0xF1);
	set_interrupt(0xF2, isr0xF2);
	set_interrupt(0xF3, isr0xF3);
	set_interrupt(0xF4, isr0xF4);
	set_interrupt(0xF5, isr0xF5);
	set_interrupt(0xF6, isr0xF6);
	set_interrupt(0xF7, isr0xF7);
	set_interrupt(0xF8, isr0xF8);
	set_interrupt(0xF9, isr0xF9);
	set_interrupt(0xFA, isr0xFA);
	set_interrupt(0xFB, isr0xFB);
	set_interrupt(0xFC, isr0xFC);
	set_interrupt(0xFD, isr0xFD);
	set_interrupt(0xFE, isr0xFE);
	set_interrupt(0xFF, isr0xFF);
}

