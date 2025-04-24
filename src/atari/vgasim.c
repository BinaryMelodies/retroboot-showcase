
#ifndef SCREEN_WIDTH_MAX
# define SCREEN_WIDTH_MAX SCREEN_WIDTH
#endif
#ifndef SCREEN_HEIGHT_MAX
# define SCREEN_HEIGHT_MAX SCREEN_HEIGHT
#endif

static uint8_t screen_buffer[SCREEN_WIDTH_MAX * SCREEN_HEIGHT_MAX * 2];

static inline void screen_init_palette(void)
{
	for(int c = 0; c < 16; c++)
	{
		screen_setpalette(c,
			((c & 4) ? 0xAA : 0) + ((c & 8) ? 0x55 : 0),
		c == 6 ? 0x55 :
			((c & 2) ? 0xAA : 0) + ((c & 8) ? 0x55 : 0),
			((c & 1) ? 0xAA : 0) + ((c & 8) ? 0x55 : 0)
		);
	}
}

#if !screen_drawchar_defined
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
	}
}
#endif

static inline void screen_setchar(uint8_t c)
{
	screen_buffer[screen_x + screen_y * SCREEN_WIDTH] = c;
	screen_buffer[screen_x + screen_y * SCREEN_WIDTH + SCREEN_WIDTH * SCREEN_HEIGHT] = screen_attribute;
	screen_drawchar(screen_x, screen_y, c, screen_attribute);
}

static inline void screen_clear(void)
{
	memset(screen_buffer, ' ', SCREEN_HEIGHT * SCREEN_WIDTH);
	memset(screen_buffer + SCREEN_HEIGHT * SCREEN_WIDTH, screen_attribute, SCREEN_HEIGHT * SCREEN_WIDTH);
	screen_video_clear();
}

static inline void screen_scroll(int lines)
{
	if(lines > SCREEN_HEIGHT)
	{
		lines = SCREEN_HEIGHT;
	}
	memcpy(screen_buffer, &screen_buffer[(SCREEN_HEIGHT - lines) * SCREEN_WIDTH], lines * SCREEN_WIDTH);
	memset(&screen_buffer[(SCREEN_HEIGHT - lines) * SCREEN_WIDTH], ' ', lines * SCREEN_WIDTH);
	memcpy(&screen_buffer[SCREEN_HEIGHT * SCREEN_WIDTH], &screen_buffer[(SCREEN_HEIGHT - lines) * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_WIDTH], lines * SCREEN_WIDTH);
	memset(&screen_buffer[(SCREEN_HEIGHT - lines) * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_WIDTH], screen_attribute, lines * SCREEN_WIDTH);
	screen_video_scroll(lines);
}

