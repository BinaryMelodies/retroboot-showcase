#ifndef _VIDEO_H
#define _VIDEO_H

enum
{
	SCREEN_WIDTH = 80,
	SCREEN_HEIGHT = 25,
};

#define DEFAULT_SCREEN_ATTRIBUTE 0xC1 // yellow on black

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
	// TODO
}

static inline void screen_init(void)
{
#if MODE_REAL
	screen_buffer = (unsigned short far *)0xA0000000;
#elif defined OS286
	screen_buffer = (unsigned short far *)0x00180000;
#elif defined OS386 || defined OS64
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
}

#endif // _VIDEO_H
