#ifndef _VIDEO_H
#define _VIDEO_H

enum
{
	SCREEN_WIDTH = 80,
	SCREEN_HEIGHT = 25,
};

#include "colors.h"

#define DEFAULT_SCREEN_ATTRIBUTE_CGA (SCREEN_ATTR_CGA_FG_YELLOW | SCREEN_ATTR_CGA_BG_BLUE)
#define DEFAULT_SCREEN_ATTRIBUTE_MDA (SCREEN_ATTR_MDA_NORMAL | SCREEN_ATTR_FG_LIGHT)
#define DEFAULT_SCREEN_ATTRIBUTE DEFAULT_SCREEN_ATTRIBUTE_CGA

enum
{
	SCREEN_MODE_MDA,
	SCREEN_MODE_CGA,
} screen_mode = SCREEN_MODE_CGA;

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

static uint16_t screen_port_base = 0x3D4;

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
		//screen_mode = SCREEN_MODE_CGA; // already initalized
		//screen_port_base =  0x3D4; // already initialized
		//screen_video_set_cursor_size(6, 7); // TODO: this only works for CGA
		//screen_attribute = DEFAULT_SCREEN_ATTRIBUTE_CGA; // already initialized
	}
	else
	{
		screen_mode = SCREEN_MODE_MDA;
		screen_port_base =  0x3B4;
		screen_video_set_cursor_size(11, 12);
		screen_attribute = DEFAULT_SCREEN_ATTRIBUTE_MDA;
	}

#if MODE_REAL && __ia16__
	screen_buffer = (unsigned short far *)(screen_mode == SCREEN_MODE_CGA ? 0xB8000000 : 0xB0000000);
#elif MODE_PROTECTED && __ia16__
	screen_buffer = (unsigned short far *)0x00180000;
#else
	screen_buffer = (unsigned short far *)(screen_mode == SCREEN_MODE_CGA ? 0x000B8000L : 0x000B0000L);
#endif

	for(uint16_t offset = 0; offset < SCREEN_WIDTH * SCREEN_HEIGHT; offset++)
	{
		screen_buffer[offset] = (screen_attribute << 8) | ' ';
	}

	screen_video_move_cursor();
}

#endif // _VIDEO_H
