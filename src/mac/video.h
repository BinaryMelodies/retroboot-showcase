#ifndef _VIDEO_H
#define _VIDEO_H

#define USE_VGA_EMULATION 1

static uint16_t screen_width, screen_height;
static uint8_t screen_depth;

# define ScreenRow (*(uint16_t volatile *)0x0106)
# define ScrnBase (*(uint8_t * volatile *)0x0824)
# define screen_row (*(uint16_t volatile *)0x0106)
# define JScrnSize(__width, __height) asm volatile( \
	"move.l\t%0, -(%%sp)\n\t" \
	"move.l\t%1, -(%%sp)\n\t" \
	"move.l\t0x0810, %%a0\n\t" \
	"jsr\t(%%a0)" : : "g"(&(__width)), "g"(&(__height)) : "%a0")

enum
{
# define SCREEN_WIDTH (screen_width / FONT_WIDTH)
# define SCREEN_HEIGHT (screen_height / FONT_HEIGHT)

	FONT_WIDTH = 8,
};
#define FONT_HEIGHT 16

# define SCREEN_WIDTH_MAX (640/FONT_WIDTH) // Macintosh II
# define SCREEN_HEIGHT_MAX (480/FONT_HEIGHT) // Macintosh II

# define DEFAULT_SCREEN_ATTRIBUTE 0x1E // yellow on blue

static uint8_t screen_x, screen_y;
static uint8_t screen_attribute = DEFAULT_SCREEN_ATTRIBUTE;

static inline void screen_init_palette(void);

#include "../font8x16.h"

static inline void screen_setpalette(int c, uint8_t r, uint8_t g, uint8_t b)
{
	(void) c;
	(void) r;
	(void) g;
	(void) b;
	// TODO
}

static inline void screen_video_clear(void)
{
	uint8_t pattern;
	pattern = (screen_attribute & 1) != 0 ? 0x00 : 0xFF;
	for(int i = 0; i < ScreenRow * screen_height; i++)
		ScrnBase[i] = pattern;
}

static inline void screen_video_scroll(int count)
{
	memcpy(ScrnBase, &ScrnBase[ScreenRow * FONT_HEIGHT * count], ScreenRow * FONT_HEIGHT * (SCREEN_HEIGHT - count));

	uint8_t pattern;
	pattern = (screen_attribute & 1) != 0 ? 0x00 : 0xFF;
	for(int i = ScreenRow * FONT_HEIGHT * (SCREEN_HEIGHT - count); i < ScreenRow * FONT_HEIGHT * SCREEN_HEIGHT; i++)
		ScrnBase[i] = pattern;
}

static inline void screen_video_move_cursor(void)
{
	// TODO
}

static inline void screen_clear(void);

static inline void screen_init(void)
{
	// On the Macintosh 128K, this contains the wrong value
	if(ScreenRow == 0xFFFF)
		ScreenRow = 512 / 8;

	JScrnSize(screen_width, screen_height);
	screen_depth = ScreenRow * 8 / screen_width;

	//screen_init_palette();
	screen_clear();
}

static inline char * screen_get_memory_base(uint8_t x, uint8_t y)
{
	switch(screen_depth)
	{
	case 1:
		return (char *)ScrnBase + ((x * FONT_WIDTH) >> 3) + (y * FONT_HEIGHT) * ScreenRow;
	case 8:
		return (char *)ScrnBase + x * FONT_WIDTH + (y * FONT_HEIGHT) * ScreenRow;
	default:
		// invalid
		return 0;
	}
}

static inline size_t screen_get_memory_offset(uint8_t channel, uint8_t line)
{
	(void) channel;
	return ScreenRow * line;
}

#define screen_drawchar_defined 1
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

		switch(screen_depth)
		{
		case 1:
			pattern = ~pattern; // TODO: this can be removed when palettes can be fixed (maybe only for color Macs?)
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
			break;
		case 8:
			for(int j = 0; j < 8; j++)
				address[screen_get_memory_offset(0, i) + j] = ((pattern >> (7 - j)) & 1) != 0 ? 0x00 : 0xFF;
			break;
		default:
			// invalid
			return;
		}
	}
}

#endif // _VIDEO_H
