#ifndef _VIDEO_H
#define _VIDEO_H

enum
{
	SCREEN_WIDTH = 80,
	SCREEN_HEIGHT = 25,
};

enum
{
	SCREEN_ATTR_VISIBLE       = 0x01,
	SCREEN_ATTR_BLINK         = 0x02 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_INVERTED      = 0x04 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_UNDERLINED    = 0x08 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_VERTICAL_LINE = 0x10 | SCREEN_ATTR_VISIBLE,

	SCREEN_ATTR_BLACK         = 0x00 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_BLUE          = 0x20 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_RED           = 0x40 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_MAGENTA       = 0x60 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_GREEN         = 0x80 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_CYAN          = 0xA0 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_YELLOW        = 0xC0 | SCREEN_ATTR_VISIBLE,
	SCREEN_ATTR_WHITE         = 0xE0 | SCREEN_ATTR_VISIBLE,
};

#define DEFAULT_SCREEN_ATTRIBUTE SCREEN_ATTR_YELLOW

static uint8_t screen_x, screen_y;
static uint8_t screen_attribute = DEFAULT_SCREEN_ATTRIBUTE;

static uint16_t far * screen_buffer;

static inline void screen_setchar(uint8_t c)
{
	screen_buffer[screen_y * SCREEN_WIDTH + screen_x] = c;
	screen_buffer[0x1000 + screen_y * SCREEN_WIDTH + screen_x] = screen_attribute;
}

static inline void screen_scroll(int count)
{
	if(count > SCREEN_HEIGHT)
	{
		count = SCREEN_HEIGHT;
	}
	for(int i = 0; i < SCREEN_WIDTH * (SCREEN_HEIGHT - count); i++)
	{
		screen_buffer[i] = screen_buffer[i + SCREEN_WIDTH * count];
	}
	for(int i = 0; i < SCREEN_WIDTH * (SCREEN_HEIGHT - count); i++)
	{
		screen_buffer[0x1000 + i] = screen_buffer[0x1000 + i + SCREEN_WIDTH * count];
	}
	for(int i = 0; i < SCREEN_WIDTH * count; i++)
	{
		screen_buffer[i + SCREEN_WIDTH * (SCREEN_HEIGHT - count)] = ' ';
	}
	for(int i = 0; i < SCREEN_WIDTH * count; i++)
	{
		screen_buffer[0x1000 + i + SCREEN_WIDTH * (SCREEN_HEIGHT - count)] = screen_attribute;
	}
}

static inline void screen_video_move_cursor(void)
{
	uint16_t location = screen_y * SCREEN_WIDTH + screen_x;
	outb(0x62, 0x49); // cursor position specify
	outb(0x60, location & 0xFF);
	outb(0x60, location >> 8);
}

static inline void screen_video_set_cursor_size(int start, int end)
{
	outb(0x62, 0x4B); // cursor & character characteristics
	outb(0x60, 0x8F); // display cursor (0x80), 15 lines per character row
	outb(0x60, 0x00 | (start & 0x1F)); // rrBttttt (top, blink, rate.low)
	outb(0x60, 0x0A | ((end & 0x1F) << 3)); // bbbbbrrr (rate.high, bottom)
}

static inline void screen_init(void)
{
	screen_video_set_cursor_size(0, 15);

#if MODE_REAL && __ia16__
	screen_buffer = (unsigned short far *)0xA0000000;
#elif MODE_PROTECTED && __ia16__
	screen_buffer = (unsigned short far *)0x00180000;
#else
	screen_buffer = (unsigned short far *)0x000A0000;
#endif

	for(uint16_t offset = 0; offset < SCREEN_WIDTH * SCREEN_HEIGHT; offset++)
	{
		screen_buffer[offset] = ' ';
	}

	for(uint16_t offset = 0x1000; offset < 0x1000 + SCREEN_WIDTH * SCREEN_HEIGHT; offset++)
	{
		screen_buffer[offset] = screen_attribute;
	}

	screen_video_move_cursor();
}

#endif // _VIDEO_H
