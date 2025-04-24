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

static uint16_t screen_port_base;

static inline void screen_video_move_cursor(void)
{
	uint16_t location = screen_y * SCREEN_WIDTH + screen_x;
	outp(screen_port_base,     0x0E);
	outp(screen_port_base + 1, location >> 8);
	outp(screen_port_base,     0x0F);
	outp(screen_port_base + 1, location);
}

static inline void screen_video_set_cursor_size(int start, int end)
{
	outb(screen_port_base,     0x0A);
	outb(screen_port_base + 1, (inb(screen_port_base + 1) & 0xC0) | start);
	outb(screen_port_base,     0x0B);
	outb(screen_port_base + 1, (inb(screen_port_base + 1) & 0xE0) | end);
}

static inline void screen_init(void)
{
	if(*(char *)0x0449 != 0x07)
	{
		screen_port_base =  0x3D4;
		//screen_video_set_cursor_size(6, 7); // TODO: this only works for CGA
	}
	else
	{
		screen_port_base =  0x3B4;
		screen_video_set_cursor_size(11, 12);
	}

#if MODE_REAL && __ia16__
	screen_buffer = (unsigned short far *)(*(char *)0x0449 != 0x07 ? 0xB8000000 : 0xB0000000);
#elif MODE_PROTECTED && __ia16__
	screen_buffer = (unsigned short far *)0x00180000;
#else
	screen_buffer = (unsigned short far *)(*(char *)0x0449 != 0x07 ? 0x000B8000L : 0x000B0000L);
#endif

	for(uint16_t offset = 0; offset < SCREEN_WIDTH * SCREEN_HEIGHT; offset++)
	{
		screen_buffer[offset] = (screen_attribute << 8) | ' ';
	}

	screen_video_move_cursor();
}

#endif // _VIDEO_H
