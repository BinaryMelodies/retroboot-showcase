#ifndef _VIDEO_H
#define _VIDEO_H

enum
{
	SCREEN_WIDTH = 80,
	SCREEN_HEIGHT = 20,
};

enum
{
	SCREEN_ATTR_INVISIBLE     = 0x01,
	SCREEN_ATTR_BLINK         = 0x02,
	SCREEN_ATTR_INVERTED      = 0x04,
	SCREEN_ATTR_UNDERLINED    = 0x08,

	SCREEN_ATTR_BLACK         = 0x00,
	SCREEN_ATTR_BLUE          = 0x10,
	SCREEN_ATTR_RED           = 0x20,
	SCREEN_ATTR_MAGENTA       = 0x30,
	SCREEN_ATTR_GREEN         = 0x40,
	SCREEN_ATTR_CYAN          = 0x50,
	SCREEN_ATTR_YELLOW        = 0x60,
	SCREEN_ATTR_WHITE         = 0x70,
	SCREEN_ATTR_DARK          = 0x80,
};

#define DEFAULT_SCREEN_ATTRIBUTE SCREEN_ATTR_YELLOW

static uint8_t screen_x, screen_y;
static uint8_t screen_attribute = DEFAULT_SCREEN_ATTRIBUTE;

#define screen_buffer ((uint16_t far *)0xA0000000)

static inline void screen_setchar(uint8_t c)
{
	screen_buffer[screen_y * SCREEN_WIDTH + screen_x] = c;
	screen_buffer[0x4000 + screen_y * SCREEN_WIDTH + screen_x] = screen_attribute;
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
		screen_buffer[0x4000 + i] = screen_buffer[0x4000 + i + SCREEN_WIDTH * count];
	}
	for(int i = 0; i < SCREEN_WIDTH * count; i++)
	{
		screen_buffer[i + SCREEN_WIDTH * (SCREEN_HEIGHT - count)] = ' ';
	}
	for(int i = 0; i < SCREEN_WIDTH * count; i++)
	{
		screen_buffer[0x4000 + i + SCREEN_WIDTH * (SCREEN_HEIGHT - count)] = screen_attribute;
	}
}

static inline void screen_video_move_cursor(void)
{
	// TODO
}

static inline void screen_init(void)
{
	for(uint16_t offset = 0; offset < SCREEN_WIDTH * SCREEN_HEIGHT; offset++)
	{
		screen_buffer[offset] = ' ';
	}

	for(uint16_t offset = 0x1000; offset < 0x1000 + SCREEN_WIDTH * SCREEN_HEIGHT; offset++)
	{
		screen_buffer[offset] = screen_attribute;
	}
}

#endif // _VIDEO_H
