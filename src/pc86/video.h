#ifndef _VIDEO_H
#define _VIDEO_H

enum
{
	SCREEN_WIDTH = 80,
	SCREEN_HEIGHT = 25,
};

enum
{
	SCREEN_ATTR_CGA_FG_BLACK         = 0x00,
	SCREEN_ATTR_CGA_FG_BLUE          = 0x01,
	SCREEN_ATTR_CGA_FG_GREEN         = 0x02,
	SCREEN_ATTR_CGA_FG_CYAN          = 0x03,
	SCREEN_ATTR_CGA_FG_RED           = 0x04,
	SCREEN_ATTR_CGA_FG_MAGENTA       = 0x05,
	SCREEN_ATTR_CGA_FG_BROWN         = 0x06,
	SCREEN_ATTR_CGA_FG_LIGHT_GRAY    = 0x07,

	SCREEN_ATTR_FG_LIGHT             = 0x08,

	SCREEN_ATTR_CGA_FG_DARK_GRAY     = SCREEN_ATTR_FG_LIGHT | SCREEN_ATTR_CGA_FG_BLACK,
	SCREEN_ATTR_CGA_FG_LIGHT_BLUE    = SCREEN_ATTR_FG_LIGHT | SCREEN_ATTR_CGA_FG_BLUE,
	SCREEN_ATTR_CGA_FG_LIGHT_GREEN   = SCREEN_ATTR_FG_LIGHT | SCREEN_ATTR_CGA_FG_GREEN,
	SCREEN_ATTR_CGA_FG_LIGHT_CYAN    = SCREEN_ATTR_FG_LIGHT | SCREEN_ATTR_CGA_FG_CYAN,
	SCREEN_ATTR_CGA_FG_LIGHT_RED     = SCREEN_ATTR_FG_LIGHT | SCREEN_ATTR_CGA_FG_RED,
	SCREEN_ATTR_CGA_FG_LIGHT_MAGENTA = SCREEN_ATTR_FG_LIGHT | SCREEN_ATTR_CGA_FG_MAGENTA,
	SCREEN_ATTR_CGA_FG_YELLOW        = SCREEN_ATTR_FG_LIGHT | SCREEN_ATTR_CGA_FG_BROWN,
	SCREEN_ATTR_CGA_FG_WHITE         = SCREEN_ATTR_FG_LIGHT | SCREEN_ATTR_CGA_FG_LIGHT_GRAY,

	SCREEN_ATTR_CGA_BG_BLACK         = SCREEN_ATTR_CGA_FG_BLACK << 4,
	SCREEN_ATTR_CGA_BG_BLUE          = SCREEN_ATTR_CGA_FG_BLUE << 4,
	SCREEN_ATTR_CGA_BG_GREEN         = SCREEN_ATTR_CGA_FG_GREEN << 4,
	SCREEN_ATTR_CGA_BG_CYAN          = SCREEN_ATTR_CGA_FG_CYAN << 4,
	SCREEN_ATTR_CGA_BG_RED           = SCREEN_ATTR_CGA_FG_RED << 4,
	SCREEN_ATTR_CGA_BG_MAGENTA       = SCREEN_ATTR_CGA_FG_MAGENTA << 4,
	SCREEN_ATTR_CGA_BG_BROWN         = SCREEN_ATTR_CGA_FG_BROWN << 4,
	SCREEN_ATTR_CGA_BG_LIGHT_GRAY    = SCREEN_ATTR_CGA_FG_LIGHT_GRAY << 4,
	// note: these are only available if blinking is disabled
	SCREEN_ATTR_BG_LIGHT             = 0x80,
	SCREEN_ATTR_CGA_BG_DARK_GRAY     = SCREEN_ATTR_CGA_FG_DARK_GRAY << 4,
	SCREEN_ATTR_CGA_BG_LIGHT_BLUE    = SCREEN_ATTR_CGA_FG_LIGHT_BLUE << 4,
	SCREEN_ATTR_CGA_BG_LIGHT_GREEN   = SCREEN_ATTR_CGA_FG_LIGHT_GREEN << 4,
	SCREEN_ATTR_CGA_BG_LIGHT_CYAN    = SCREEN_ATTR_CGA_FG_LIGHT_CYAN << 4,
	SCREEN_ATTR_CGA_BG_LIGHT_RED     = SCREEN_ATTR_CGA_FG_LIGHT_RED << 4,
	SCREEN_ATTR_CGA_BG_LIGHT_MAGENTA = SCREEN_ATTR_CGA_FG_LIGHT_MAGENTA << 4,
	SCREEN_ATTR_CGA_BG_YELLOW        = SCREEN_ATTR_CGA_FG_YELLOW << 4,
	SCREEN_ATTR_CGA_BG_WHITE         = SCREEN_ATTR_CGA_FG_WHITE << 4,

	// note: this is only availble if blinking is enabled
	SCREEN_ATTR_BLINK = 0x80,

	// only one of these should be used, but SCREEN_ATTR_FG_LIGHT and SCREEN_ATTR_BLINK/SCREEN_ATTR_BG_LIGHT can be or'd with it
	SCREEN_ATTR_MDA_INVISIBLE = 0x00,
	SCREEN_ATTR_MDA_UNDERLINED = 0x01,
	SCREEN_ATTR_MDA_NORMAL = 0x07,
	SCREEN_ATTR_MDA_INVERTED = 0x70,
};

#define DEFAULT_SCREEN_ATTRIBUTE_CGA (SCREEN_ATTR_CGA_FG_YELLOW | SCREEN_ATTR_CGA_BG_BLUE)
#define DEFAULT_SCREEN_ATTRIBUTE_MDA (SCREEN_ATTR_MDA_NORMAL | SCREEN_ATTR_FG_LIGHT)

enum
{
	SCREEN_MODE_MDA,
	SCREEN_MODE_CGA,
} screen_mode = SCREEN_MODE_CGA;

static uint8_t screen_x, screen_y;
static uint8_t screen_attribute = DEFAULT_SCREEN_ATTRIBUTE_CGA;
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
