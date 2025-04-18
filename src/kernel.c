
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

#include "system.c"

noreturn void kmain(void);

extern char image_start;

static inline void test_long_line(void)
{
	for(int i = 0; i <= SCREEN_WIDTH; i += 17)
		screen_putstr(" a very long line"); // 17 is a prime number, very unlikely to divide SCREEN_WIDTH evenly
	screen_putchar('\n');
}

static inline void test_puthex(void)
{
	// truncated to fit integer size
	screen_puthex((size_t)0x1A2B3C4D);
	screen_putchar('\n');

	screen_putstr("Starting address for image: ");
	screen_puthex((size_t)&image_start);
	screen_putchar('\n');
}

static inline void test_putdec(void)
{
	screen_putdec(-12345);
	screen_putchar('\n');
}

static inline void test_scroll(void)
{
	screen_putstr("This line will be erased\n");
	screen_putstr("This line will not be erased\n");
	for(int i = 2; i < SCREEN_HEIGHT; i++)
		screen_putstr("scroll\n");

#if USE_VGA_EMULATION || IBMPC || NECPC98
	screen_y = 2;
#endif
}

static inline void test_interrupts(void)
{
#if __ia16__ || __i386__ || __amd64__
	asm volatile("int $3");
	asm volatile("int $4");
#endif
}

noreturn void kmain(void)
{
#if __ia16__ || __i386__ || __amd64__
	disable_interrupts();

	setup_tables();
#endif

	screen_init();

	test_scroll();

	screen_putstr("Greetings! " OSNAME " for " SYSNAME COMMENT "\n");

	test_long_line();
	test_puthex();
#if !ATARI // TODO: 68000 pcrel modsi3 does not work
	test_putdec();
#endif

#if !__m68k__ // TODO: this does not work yet
	enter_usermode();
#endif

	if(is_system_mode())
		screen_putstr("Running in system mode\n");
	else
		screen_putstr("Running in user mode\n");

	test_interrupts();

	for(;;)
		;
}

