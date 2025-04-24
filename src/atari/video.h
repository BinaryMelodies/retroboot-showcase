#ifndef _VIDEO_H
#define _VIDEO_H

enum
{
	SCREEN_WIDTH = 40,
	SCREEN_HEIGHT = 25,

	FONT_WIDTH = 8,
};
#define FONT_HEIGHT 8

static inline void bios_putchar(int c)
{
	asm volatile(
		"move.w\t%0, -(%%sp)\n\t"
		"move.w\t#2, -(%%sp)\n\t"
		"move.w\t#3, -(%%sp)\n\t"
		"trap\t#13\n\t"
		"addq.l\t#6, %%sp"
		: : "g"((uint16_t)c) : "%d0", "%d1", "%d2", "%a0", "%a1", "%a2");
}

#if USE_VGA_EMULATION

#include "../pc86/colors.h"

# define DEFAULT_SCREEN_ATTRIBUTE (SCREEN_ATTR_CGA_FG_YELLOW | SCREEN_ATTR_CGA_BG_BLUE)

static uint8_t screen_x, screen_y;
static uint8_t screen_attribute = DEFAULT_SCREEN_ATTRIBUTE;

static uint16_t * screen_memory;

static inline void screen_init_palette(void);

#include "../font8x8.h"
#define font font8x8

static inline void screen_setpalette(int c, uint8_t r, uint8_t g, uint8_t b)
{
	((uint16_t *)0xFF8240)[c] = ((r >> 5) << 8) | ((g >> 5) << 4) | (b >> 5);
}

static inline void screen_video_clear(void)
{
	char * memory = (char *)screen_memory;
	uint16_t pattern[4];
	for(int ch = 0; ch < 4; ch++)
	{
		pattern[ch] = screen_attribute & (1 << ch) ? 0x0000 : 0xFFFF;
	}
	for(int i = 0; i < SCREEN_HEIGHT * FONT_HEIGHT; i ++)
	{
		for(int j = 0; j < SCREEN_WIDTH * FONT_WIDTH / 2; j += 8)
		{
			memcpy(&memory[j], pattern, 8);
		}
		memory += 160;
	}
}

static inline void screen_video_scroll(int count)
{
	char * memory = (char *)screen_memory;
	memcpy(memory, &memory[160 * count * FONT_HEIGHT], 160 * (SCREEN_HEIGHT - count) * FONT_HEIGHT);
	uint16_t pattern[4];
	for(int ch = 0; ch < 4; ch++)
	{
		pattern[ch] = screen_attribute & (1 << ch) ? 0x0000 : 0xFFFF;
	}
	memory += 160 * (SCREEN_HEIGHT - count) * FONT_HEIGHT;
	for(int i = (SCREEN_HEIGHT - count) * FONT_HEIGHT; i < SCREEN_HEIGHT * FONT_HEIGHT; i ++)
	{
		for(int j = 0; j < SCREEN_WIDTH * FONT_WIDTH / 2; j += 8)
		{
			memcpy(&memory[j], pattern, 8);
		}
		memory += 160;
	}
}

static inline void screen_video_move_cursor(void)
{
	// TODO
}

static inline void screen_clear(void);

static inline void screen_init(void)
{
	uint32_t address = *(uint32_t *)0xFF8200;
	screen_memory = (void *)((address & 0x00FF0000) | ((address & 0xFF) << 8));
	screen_init_palette();
	screen_clear();
}

static inline char * screen_get_memory_base(uint8_t x, uint8_t y)
{
	return ((char *)screen_memory + (((x * FONT_WIDTH) >> 1) & ~7) + (((x * FONT_WIDTH) >> 3) & 1) + (y * FONT_HEIGHT) * 160);
}

static inline size_t screen_get_memory_offset(uint8_t channel, uint8_t line)
{
	return 2 * channel + 160 * line;
}

# include "vgasim.c"

#else

static inline void screen_init(void)
{
}

# define screen_putchar_defined 1
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
