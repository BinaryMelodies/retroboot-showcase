#ifndef _VIDEO_H
#define _VIDEO_H

enum
{
#if USE_VGA_EMULATION
	SCREEN_WIDTH = 128,
	SCREEN_HEIGHT = 32,
#else
	SCREEN_WIDTH = 80,
	SCREEN_HEIGHT = 25,
#endif

	FONT_WIDTH = 8,
};
#define FONT_HEIGHT 16

static inline void bios_putchar(int c)
{
	register unsigned short _c __asm__("%d1") = c;
	asm volatile(
		"and.w\t#0xFF, %%d1\n\t"
		"moveq\t#0x20, %%d0\n\t"
		"trap\t#15"
		: : "r"(_c));
}

#if USE_VGA_EMULATION
# define DEFAULT_SCREEN_ATTRIBUTE 0x1E // yellow on blue

static uint8_t screen_x, screen_y;
static uint8_t screen_attribute = DEFAULT_SCREEN_ATTRIBUTE;
static uint16_t * screen_memory;

static inline void screen_init_palette(void);

#include "../font8x16.h"

static inline void screen_setpalette(int c, uint8_t r, uint8_t g, uint8_t b)
{
	((uint16_t *)0xE82200)[c] = ((g >> 3) << 11) | ((r >> 3) << 6) | ((b >> 3) << 1);
}

static inline void screen_video_clear(void)
{
	for(int ch = 0; ch < 4; ch++)
	{
		char * memory = (char *)screen_memory + ch * 0x020000;
		uint8_t pattern = screen_attribute & (1 << ch) ? 0x00 : 0xFF;
		for(int i = 0; i < SCREEN_HEIGHT * FONT_HEIGHT; i ++)
		{
			memset(memory, pattern, SCREEN_WIDTH * FONT_WIDTH / 8);
			memory += 128;
		}
	}
}

static inline void screen_video_scroll(int count)
{
	for(int ch = 0; ch < 4; ch++)
	{
		char * memory = (char *)screen_memory + ch * 0x020000;
		memcpy(memory, &memory[128 * count * FONT_HEIGHT], 128 * (SCREEN_HEIGHT - count) * FONT_HEIGHT);
	}

	for(int ch = 0; ch < 4; ch++)
	{
		char * memory = (char *)screen_memory + ch * 0x020000;
		uint8_t pattern = screen_attribute & (1 << ch) ? 0x00 : 0xFF;
		memory += 128 * (SCREEN_HEIGHT - count) * FONT_HEIGHT;
		for(int i = (SCREEN_HEIGHT - count) * FONT_HEIGHT; i < SCREEN_HEIGHT * FONT_HEIGHT; i ++)
		{
			memset(memory, pattern, SCREEN_WIDTH * FONT_WIDTH / 8);
			memory += 128;
		}
	}
}

static inline void screen_video_move_cursor(void)
{
	// TODO
}

static inline void screen_clear(void);

static inline void screen_init(void)
{
	screen_memory = ((uint16_t *)0xE00000);
	screen_init_palette();
	screen_clear();
}

static inline char * screen_get_memory_base(uint8_t x, uint8_t y)
{
	return ((char *)screen_memory + ((x * FONT_WIDTH) >> 3) + (y * FONT_HEIGHT) * 128);
}

static inline size_t screen_get_memory_offset(uint8_t channel, uint8_t line)
{
	return 0x020000 * channel + 128 * line;
}

#else

static inline void screen_init(void)
{
}

static inline void screen_putchar(int c)
{
	if(c == '\n')
	{
		bios_putchar('\r');
	}
	bios_putchar(c);
}

#endif

#endif // _VIDEO_H
