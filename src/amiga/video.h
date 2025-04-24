#ifndef _VIDEO_H
#define _VIDEO_H

#define USE_VGA_EMULATION 1

#include "../pc86/colors.h"

#define CUSTOM 0xDFF000
#define COP1LCH (*(volatile uint16_t *)(CUSTOM + 0x080))
#define COPJMP1 (*(volatile uint16_t *)(CUSTOM + 0x088))
#define DIWSTRT (*(volatile uint16_t *)(CUSTOM + 0x08E))
#define DIWSTOP (*(volatile uint16_t *)(CUSTOM + 0x090))
#define DDFSTRT (*(volatile uint16_t *)(CUSTOM + 0x092))
#define DDFSTOP (*(volatile uint16_t *)(CUSTOM + 0x094))
#define DMACON (*(volatile uint16_t *)(CUSTOM + 0x096))
#define INTENA (*(volatile uint16_t *)(CUSTOM + 0x09A))
#define INTREQ (*(volatile uint16_t *)(CUSTOM + 0x09C))
#define BPLCON0 (*(volatile uint16_t *)(CUSTOM + 0x100))
#define BPLCON1 (*(volatile uint16_t *)(CUSTOM + 0x102))
#define BPL1MOD (*(volatile uint16_t *)(CUSTOM + 0x108))
#define BPL2MOD (*(volatile uint16_t *)(CUSTOM + 0x10A))
#define COLOR0 (*(volatile uint16_t *)(CUSTOM + 0x180))
#define COLOR1 (*(volatile uint16_t *)(CUSTOM + 0x182))

enum
{
	SCREEN_WIDTH = 40,
	SCREEN_HEIGHT = 25, // TODO: PAL offered 32 lines

	FONT_WIDTH = 8,
};
#define FONT_HEIGHT 8

#define LINE_BYTES (SCREEN_WIDTH * FONT_WIDTH / 8)
#define PLANE_BYTES (SCREEN_HEIGHT * FONT_HEIGHT * LINE_BYTES)

static uint16_t copper[1024] __attribute__((section("chipram,\"aw\",@nobits\n#")));
static uint8_t screen_memory[PLANE_BYTES * 4] __attribute__((section("chipram,\"aw\",@nobits\n#")));

# define DEFAULT_SCREEN_ATTRIBUTE (SCREEN_ATTR_CGA_FG_YELLOW | SCREEN_ATTR_CGA_BG_BLUE)

static uint8_t screen_x, screen_y;
static uint8_t screen_attribute = DEFAULT_SCREEN_ATTRIBUTE;

static inline void screen_init_palette(void);

#include "../font8x8.h"

static inline void screen_setpalette(int c, uint8_t r, uint8_t g, uint8_t b)
{
	copper[16 + 2 * c + 1] = ((r & 0xF0) << 4) + (g & 0xF0) + (b >> 4);
}

static inline void screen_video_clear(void)
{
	for(int ch = 0; ch < 4; ch++)
	{
		uint8_t pattern;
		pattern = screen_attribute & (1 << ch) ? 0x00 : 0xFF;
		memset(&screen_memory[PLANE_BYTES * ch], pattern, PLANE_BYTES);
	}
}

static inline void screen_video_scroll(int count)
{
	for(int ch = 0; ch < 4; ch++)
	{
		memcpy(
			&screen_memory[PLANE_BYTES * ch],
			&screen_memory[PLANE_BYTES * ch + count * (FONT_HEIGHT * LINE_BYTES)],
			(SCREEN_HEIGHT - count) * FONT_HEIGHT * LINE_BYTES);
	}

	for(int ch = 0; ch < 4; ch++)
	{
		uint8_t pattern;
		pattern = screen_attribute & (1 << ch) ? 0x00 : 0xFF;
		memset(&screen_memory[PLANE_BYTES * ch + (SCREEN_HEIGHT - count) * FONT_HEIGHT * LINE_BYTES], pattern, count * FONT_HEIGHT * LINE_BYTES);
	}
}

static inline void screen_video_move_cursor(void)
{
	// TODO
}

static inline void screen_clear(void);

static inline void screen_init(void)
{
	BPLCON0 = 0x4200;
	BPLCON1 = 0;
	BPL1MOD = 0;
	BPL2MOD = 0;
	DIWSTRT = 0x2C81;
	DIWSTOP = 0xF4C1;
	DDFSTRT = 0x0038;
	DDFSTOP = 0x00D0;

	DMACON = 0x8180;
	DMACON = 0x0007;
	INTENA = 0xC000;
	INTENA = 0x3FFF;

	for(int i = 0; i < 4; i++)
	{
		copper[0 + 4 * i] = 0x00E0 + 4 * i;
		copper[1 + 4 * i] = ((size_t)screen_memory + PLANE_BYTES * i) >> 16;
		copper[2 + 4 * i] = 0x00E2 + 4 * i;
		copper[3 + 4 * i] = (size_t)screen_memory + PLANE_BYTES * i;
	}

	for(int i = 0; i < 16; i++)
	{
		copper[16 + 2 * i] = 0x0180 + 2 * i;
		copper[16 + 2 * i + 1] = 0; // color needs to be set
	}

	copper[48] = 0xFFFF;
	copper[49] = 0xFFFE;

	*(uint32_t *)&COP1LCH = (size_t)copper;
	COPJMP1 = 0;

	screen_init_palette();
	screen_clear();
}

static inline char * screen_get_memory_base(uint8_t x, uint8_t y)
{
	return (char *)screen_memory + ((x * FONT_WIDTH) >> 3) + (y * FONT_HEIGHT) * LINE_BYTES;
}

static inline size_t screen_get_memory_offset(uint8_t channel, uint8_t line)
{
	return PLANE_BYTES * channel + LINE_BYTES * line;
}

# include "../atari/vgasim.c"

#endif // _VIDEO_H
