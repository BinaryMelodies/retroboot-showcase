#ifndef _VIDEO_H
#define _VIDEO_H

enum
{
	SCREEN_WIDTH = 80,
	SCREEN_HEIGHT = 25,
};

#define DEFAULT_SCREEN_ATTRIBUTE 0x1E // yellow on blue

static uint8_t screen_x, screen_y;
static uint8_t screen_attribute = DEFAULT_SCREEN_ATTRIBUTE;
static uint16_t far * screen_buffer;

static inline void screen_setchar(uint8_t c)
{
	screen_buffer[screen_y * SCREEN_WIDTH + screen_x] = (screen_attribute << 8) | c;
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
	for(int i = 0; i < SCREEN_WIDTH * count; i++)
	{
		screen_buffer[i + SCREEN_WIDTH * (SCREEN_HEIGHT - count)] = (screen_attribute << 8) | ' ';
	}
}

static inline void screen_video_move_cursor(void)
{
	uint16_t location = screen_y * SCREEN_WIDTH + screen_x;
	outp(0x3D4, 0x0E);
	outp(0x3D5, location >> 8);
	outp(0x3D4, 0x0F);
	outp(0x3D5, location);
}

static inline void screen_init(void)
{
#if defined OS86
	screen_buffer = (unsigned short far *)(*(char *)0x0449 != 0x07 ? 0xB8000000 : 0xB0000000);
#elif defined OS286
	screen_buffer = (unsigned short far *)0x00180000;
#elif defined OS386 || defined OS64
	screen_buffer = (unsigned short far *)(*(char *)0x0449 != 0x07 ? 0x000B8000L : 0x000B0000L);
#endif

	for(uint16_t offset = 0; offset < SCREEN_WIDTH * SCREEN_HEIGHT; offset++)
	{
		screen_buffer[offset] = (screen_attribute << 8) | ' ';
	}

	screen_video_move_cursor();
}

#endif // _VIDEO_H
