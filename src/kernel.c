
#if __ia16__ || __i386__ || __amd64__
# include "conio.h"
# include "i86.h"
#endif
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdnoreturn.h"
#include "string.h"

#ifndef __ia16__
# define far
#endif

#include "video.h"

#if defined OS86
# define OSNAME "OS/86"
# define COMMENT " running in real mode"
#elif defined OS286
# define OSNAME "OS/286"
# define COMMENT " running in 16-bit protected mode"
#elif defined OS386
# define OSNAME "OS/386"
# define COMMENT " running in 32-bit protected mode"
#elif defined OS64
# define OSNAME "OS/64"
# define COMMENT " running in 64-bit long mode"
#elif defined OS68000
# define OSNAME "OS/68000"
#endif

#ifndef COMMENT
# define COMMENT ""
#endif

#if defined IBMPC
# define SYSNAME "IBM PC"
#elif defined NECPC98
# define SYSNAME "NEC PC-98"
#elif defined ATARI
# define SYSNAME "Atari ST"
#elif defined X68000
# define SYSNAME "Sharp X68000"
#elif defined AMIGA
# define SYSNAME "Commodore Amiga"
#elif defined MACINTOSH
# define SYSNAME "Apple Macintosh"
#endif

#if __ia16__ || __i386__ || __amd64__
# define __X86__ 1
#endif

#if OS286 || OS386 || OS64
enum
{
	SEL_KERNEL_CS = 0x08,
	SEL_KERNEL_SS = 0x10, // must follow CS for SYSENTER and SYSCALL
#if OS286
	SEL_KERNEL_SCREEN = 0x18,
	SEL_KERNEL_SEGMENT1 = 0x20, // used as source segment for huge transfers
	SEL_KERNEL_SEGMENT2 = 0x28, // used as destination segment for huge transfers
	SEL_TSS = 0x30,
	SEL_LDT = 0x38,
	SEL_GDT_MAX = 0x40,

	SEL_CALL7 = 0x04,
	SEL_USER_CS = 0x0C,
	SEL_USER_SS = 0x14,
	SEL_LDT_MAX = 0x18,
#elif OS386
	SEL_USER_CS = 0x18, // must follow CS/SS for SYSEXIT
	SEL_USER_SS = 0x20, // must follow CS/SS for SYSEXIT
	SEL_TSS = 0x28,
	SEL_LDT = 0x30,
	SEL_GDT_MAX = 0x38,

	SEL_CALL7 = 0x04,
	SEL_LDT_MAX = 0x08,
#elif OS64
	SEL_USER_CS32 = 0x18, // must follow CS/SS for SYSEXIT
	SEL_USER_SS32 = 0x20, // must follow CS/SS for SYSEXIT
	SEL_USER_CS = 0x28, // must follow CS/SS for SYSEXIT and CS32/SS32 for SYSCALL
	SEL_USER_SS = 0x30, // must follow CS/SS for SYSEXIT
	SEL_TSS = 0x38, // takes up 2 slots
	SEL_LDT = 0x48, // takes up 2 slots
	SEL_GDT_MAX = 0x58,

	SEL_CALL7 = 0x04,
	SEL_LDT_MAX = 0x08,
#endif
};

descriptor_t gdt[SEL_GDT_MAX / 8];
descriptor_t ldt[SEL_LDT_MAX / 8];
task_state_segment_t tss;

#if !OS64
descriptor_t idt[256];
#else
descriptor64_t idt[256];
#endif

#endif

#if USE_VGA_EMULATION
# ifndef SCREEN_WIDTH_MAX
#  define SCREEN_WIDTH_MAX SCREEN_WIDTH
# endif
# ifndef SCREEN_HEIGHT_MAX
#  define SCREEN_HEIGHT_MAX SCREEN_HEIGHT
# endif

static uint8_t screen_buffer[SCREEN_WIDTH_MAX * SCREEN_HEIGHT_MAX * 2];

static inline void screen_init_palette(void)
{
	for(int c = 0; c < 16; c++)
	{
		screen_setpalette(c,
			((c & 4) ? 0xAA : 0) + ((c & 8) ? 0x55 : 0),
		c == 6 ? 0x55 :
			((c & 2) ? 0xAA : 0) + ((c & 8) ? 0x55 : 0),
			((c & 1) ? 0xAA : 0) + ((c & 8) ? 0x55 : 0)
		);
	}
}

#if !screen_drawchar_defined
static inline void screen_drawchar(int x, int y, int c, int a)
{
	char * address = screen_get_memory_base(x, y);
	for(int i = 0; i < FONT_HEIGHT; i++)
	{
#if FONT_HEIGHT == 8
		uint8_t pattern = font8x8[c * FONT_HEIGHT + i];
#elif FONT_HEIGHT == 16
		uint8_t pattern = font8x16[c * FONT_HEIGHT + i];
#endif
		for(int ch = 0; ch < 4; ch++)
		{
			int p = ((a >> ch) & 1) | ((a >> (4 + ch - 1)) & 2);
			switch(p)
			{
			case 0:
				p = 0;
				break;
			case 1:
				p = pattern;
				break;
			case 2:
				p = ~pattern;
				break;
			case 3:
				p = 0xFF;
				break;
			}
			address[screen_get_memory_offset(ch, i)] = p;
		}
	}
}
#endif

static inline void screen_setchar(uint8_t c)
{
	screen_buffer[screen_x + screen_y * SCREEN_WIDTH] = c;
	screen_buffer[screen_x + screen_y * SCREEN_WIDTH + SCREEN_WIDTH * SCREEN_HEIGHT] = screen_attribute;
	screen_drawchar(screen_x, screen_y, c, screen_attribute);
}

static inline void screen_clear(void)
{
	memset(screen_buffer, ' ', SCREEN_HEIGHT * SCREEN_WIDTH);
	memset(screen_buffer + SCREEN_HEIGHT * SCREEN_WIDTH, screen_attribute, SCREEN_HEIGHT * SCREEN_WIDTH);
	screen_video_clear();
}

static inline void screen_scroll(int lines)
{
	if(lines > SCREEN_HEIGHT)
	{
		lines = SCREEN_HEIGHT;
	}
	memcpy(screen_buffer, &screen_buffer[(SCREEN_HEIGHT - lines) * SCREEN_WIDTH], lines * SCREEN_WIDTH);
	memset(&screen_buffer[(SCREEN_HEIGHT - lines) * SCREEN_WIDTH], ' ', lines * SCREEN_WIDTH);
	memcpy(&screen_buffer[SCREEN_HEIGHT * SCREEN_WIDTH], &screen_buffer[(SCREEN_HEIGHT - lines) * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_WIDTH], lines * SCREEN_WIDTH);
	memset(&screen_buffer[(SCREEN_HEIGHT - lines) * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_WIDTH], screen_attribute, lines * SCREEN_WIDTH);
	screen_video_scroll(lines);
}
#endif

#if defined IBMPC || defined NECPC98 || USE_VGA_EMULATION
static inline void screen_putchar(int c)
{
	switch(c)
	{
	case '\b':
		if(screen_x > 0)
		{
			screen_x --;
		}
		break;
	case '\n':
		screen_x = 0;
		screen_y ++;
		break;
	case '\t':
		screen_x = (screen_x + 8) & ~7;
		break;
	default:
		if(0x20 <= c && c <= 0x7E)
		{
			screen_setchar(c);
			screen_x ++;
		}
		break;
	}
	if(screen_x >= SCREEN_WIDTH)
	{
		screen_y += screen_x / SCREEN_WIDTH;
		screen_x %= SCREEN_WIDTH;
	}
	if(screen_y >= SCREEN_HEIGHT)
	{
		screen_scroll(screen_y + 1 - SCREEN_HEIGHT);
		screen_y = SCREEN_HEIGHT - 1;
	}
	screen_video_move_cursor();
}
#endif

static inline void screen_putstr(const char far * text)
{
	for(size_t i = 0; text[i] != '\0'; i++)
	{
		screen_putchar(text[i]);
	}
}

static inline void screen_puthex(size_t value)
{
	char buffer[sizeof(value) * 2 + 1];
	int ptr = sizeof(buffer);
	buffer[--ptr] = '\0';
	do
	{
		int d = value & 0xF;
		buffer[--ptr] = d < 10 ? '0' + d : 'A' + d - 10;
		value >>= 4;
	} while(value != 0);
	screen_putstr(&buffer[ptr]);
}

static inline void screen_putdec(ssize_t value)
{
	// this approximates the number of digits for around 10 bytes
	char buffer[(sizeof(value) * 5 + 1) / 2];
	int ptr = sizeof(buffer);
	if(value < 0)
	{
		screen_putchar('-');
		value = -value;
	}
	buffer[--ptr] = '\0';
	do
	{
		buffer[--ptr] = '0' + (value % 10);
		value /= 10;
	} while(value != 0);
	screen_putstr(&buffer[ptr]);
}

#if __ia16__ || __i386__ || __amd64__
#include "x86.c"
#endif

noreturn void kmain(void);

extern char image_start;

void test_scroll(void)
{
	screen_putstr("This line will be erased\n");
	screen_puthex((size_t)&image_start);
	screen_putstr("\n");
	screen_putstr("Greetings! " OSNAME " for " SYSNAME COMMENT);
	screen_putstr("\n");
	screen_puthex((size_t)0x1A2B3C4D);
#if !ATARI // TODO: 68000 pcrel modsi3 does not work
	screen_putdec(-12345);
#endif
	screen_putstr("\n");
	for(int i = 4; i < SCREEN_HEIGHT; i++)
		screen_putstr("scroll\n");
}

noreturn void kmain(void)
{
#if __X86__
	disable_interrupts();

	setup_tables();
#endif

	screen_init();

//	screen_putstr("Greetings! " OSNAME " for " SYSNAME COMMENT);

	test_scroll();

#if __ia16__ || __i386__ || __amd64__
#if OS286 || OS386 || OS64
	enter_usermode();
#endif

	asm volatile("int $3");
	asm volatile("int $4");
#endif

	for(;;)
		;
}

