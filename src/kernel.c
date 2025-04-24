
#if __ia16__ || __i386__ || __amd64__
# include "conio.h"
# include "i86.h"
#endif
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdnoreturn.h"
#include "string.h"
#include "system.h"

#ifndef __ia16__
# define far
#endif

#include "video.h"

#if __ia16__
# if MODE_REAL
#  define OSNAME "OS/86"
# elif MODE_PROTECTED
#  define OSNAME "OS/286"
# endif
#elif __i386__
# define OSNAME "OS/386"
#elif __amd64__
# define OSNAME "OS/64"
#elif __m68k__
# define OSNAME "OS/68000"
#endif

#if MODE_REAL
# if __ia16__
#  define COMMENT " running in 16-bit real mode"
# else
#  define COMMENT " running in 32-bit real mode" // note: not currently used
# endif
#elif MODE_PROTECTED
# if __ia16__
#  define COMMENT " running in 16-bit protected mode"
# else
#  define COMMENT " running in 32-bit protected mode"
# endif
#elif MODE_LONG
# define COMMENT " running in 64-bit long mode"
#endif

#ifndef COMMENT
# define COMMENT ""
#endif

#if MACHINE_IBMPC
# define SYSNAME "IBM PC"
#elif MACHINE_NECPC98
# define SYSNAME "NEC PC-98"
#elif MACHINE_NECPC88VA
# define SYSNAME "NEC PC-88 VA"
#elif MACHINE_ATARI
# define SYSNAME "Atari ST"
#elif MACHINE_X68000
# define SYSNAME "Sharp X68000"
#elif MACHINE_AMIGA
# define SYSNAME "Commodore Amiga"
#elif MACHINE_MACINTOSH
# define SYSNAME "Apple Macintosh"
#endif

noreturn void kmain(void);
extern char image_start;

static char system_stack[512] __attribute__((aligned(8)));
//static char user_stack[512] __attribute__((aligned(8)));

#if !screen_putchar_defined
static inline void screen_putchar(int c)
{
	switch(c)
	{
	case '\b':
		if(screen_x > 0)
		{
			screen_x --;
		}
		break;
	case '\n':
		screen_x = 0;
		screen_y ++;
		break;
	case '\t':
		screen_x = (screen_x + 8) & ~7;
		break;
	default:
		if(0x20 <= c && c <= 0x7E)
		{
			screen_setchar(c);
			screen_x ++;
		}
		break;
	}
	if(screen_x >= SCREEN_WIDTH)
	{
		screen_y += screen_x / SCREEN_WIDTH;
		screen_x %= SCREEN_WIDTH;
	}
	if(screen_y >= SCREEN_HEIGHT)
	{
		screen_scroll(screen_y + 1 - SCREEN_HEIGHT);
		screen_y = SCREEN_HEIGHT - 1;
	}
	screen_video_move_cursor();
}
#endif

static inline void screen_putstr(const char far * text)
{
	for(size_t i = 0; text[i] != '\0'; i++)
	{
		screen_putchar(text[i]);
	}
}

static inline void screen_puthex(size_t value)
{
	char buffer[sizeof(value) * 2 + 1];
	int ptr = sizeof(buffer);
	buffer[--ptr] = '\0';
	do
	{
		int d = value & 0xF;
		buffer[--ptr] = d < 10 ? '0' + d : 'A' + d - 10;
		value >>= 4;
	} while(value != 0);
	screen_putstr(&buffer[ptr]);
}

static inline void screen_putdec(ssize_t value)
{
	// this approximates the number of digits for around 10 bytes
	char buffer[(sizeof(value) * 5 + 1) / 2];
	int ptr = sizeof(buffer);
	if(value < 0)
	{
		screen_putchar('-');
		value = -value;
	}
	buffer[--ptr] = '\0';
	do
	{
		buffer[--ptr] = '0' + (value % 10);
		value /= 10;
	} while(value != 0);
	screen_putstr(&buffer[ptr]);
}

static volatile bool keyboard_shift = false;

#define KEYBOARD_BUFFER_SIZE 16
static volatile char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static volatile size_t keyboard_buffer_count;
static volatile size_t keyboard_buffer_pointer;
static volatile bool keyboard_used = false;

static inline void keyboard_buffer_push(char c)
{
	while(keyboard_used)
		;
	keyboard_used = true;
	if(keyboard_buffer_count < KEYBOARD_BUFFER_SIZE)
	{
#if !MACHINE_ATARI
		keyboard_buffer[(keyboard_buffer_pointer + keyboard_buffer_count++) % KEYBOARD_BUFFER_SIZE] = c;
#else
		keyboard_buffer[(keyboard_buffer_pointer + keyboard_buffer_count++) & (KEYBOARD_BUFFER_SIZE - 1)] = c;
#endif
	}
	keyboard_used = false;
}

static inline bool keyboard_buffer_empty(void)
{
	return keyboard_buffer_count == 0;
}

static inline int keyboard_buffer_remove(void)
{
	if(keyboard_buffer_empty())
	{
		return -1;
	}
	else
	{
		while(keyboard_used)
			;
		keyboard_used = true;
		int c = keyboard_buffer[keyboard_buffer_pointer];
#if !MACHINE_ATARI
		keyboard_buffer_pointer = (keyboard_buffer_pointer + 1) % KEYBOARD_BUFFER_SIZE;
#else
		keyboard_buffer_pointer = (keyboard_buffer_pointer + 1) & (KEYBOARD_BUFFER_SIZE - 1);
#endif
		keyboard_buffer_count --;
		keyboard_used = false;
		return c;
	}
}

static inline bool keyboard_kbhit(void)
{
	return !keyboard_buffer_empty();
}

static inline int keyboard_getch(void)
{
	while(keyboard_buffer_empty())
		;
	return keyboard_buffer_remove();
}

#if __ia16__ || __i386__ || __amd64__
#include "x86.c"
#elif __m68k__
#include "m68k.c"
#endif

static inline void test_long_line(void)
{
	for(int i = 0; i <= SCREEN_WIDTH; i += 17)
		screen_putstr(" a very long line"); // 17 is a prime number, very unlikely to divide SCREEN_WIDTH evenly
	screen_putchar('\n');
}

static inline void test_puthex(void)
{
	// truncated to fit integer size
	screen_puthex((size_t)0x1A2B3C4D);
	screen_putchar('\n');

	screen_putstr("Starting address for image: ");
	screen_puthex((size_t)&image_start);
	screen_putchar('\n');
}

static inline void test_putdec(void)
{
	screen_putdec(-12345);
	screen_putchar('\n');
}

static inline void test_scroll(void)
{
	screen_putstr("This line will be erased\n");

	screen_putstr("Starting address for image: ");
	screen_puthex((size_t)&image_start);
	screen_putchar('\n');

	for(int i = 2; i < SCREEN_HEIGHT; i++)
		screen_putstr("scroll\n");

#if USE_VGA_EMULATION || MACHINE_IBMPC || MACHINE_NECPC98 || MACHINE_NECPC88VA
	screen_y = 2;
#endif
}

static inline void test_interrupts(void)
{
#if __ia16__ || __i386__ || __amd64__
	asm volatile("int $3");
	asm volatile("int $4");
	asm volatile("int $0x80");
#endif
}

noreturn void kmain(void)
{
	uint16_t intval = disable_interrupts();

#if __ia16__ && MODE_REAL
	(void) system_stack; // suppress warning
#endif

#if !MACHINE_AMIGA && !MACHINE_X68000 && !MACHINE_MACINTOSH // TODO
	setup_tables();
#endif

	screen_init();

	test_scroll();

	screen_putstr("Greetings! " OSNAME " for " SYSNAME COMMENT "\n");

	test_long_line();
	test_puthex();
#if !MACHINE_ATARI // TODO: 68000 pcrel modsi3 does not work
	test_putdec();
#endif

#if !MACHINE_AMIGA && !MACHINE_X68000 && !MACHINE_MACINTOSH // TODO
	enable_interrupts(intval);
#else
	(void) intval;
#endif

#if MACHINE_MACINTOSH
	if(screen_depth == 1) // TODO: switching to user mode does not work yet on Macintosh II, this check does not work on a MacII with a monochrome display
#endif
#if !MODE_REAL && (__ia16__ || __i386__ || __amd64__)
	enter_usermode(SEL_USER_CS|3, SEL_USER_SS|3);
#elif __m68k__
	enter_usermode((size_t)system_stack + sizeof system_stack);
#endif

	if(is_system_mode())
		screen_putstr("Running in system mode\n");
	else
		screen_putstr("Running in user mode\n");

	test_interrupts();

	for(;;)
	{
#if !MACHINE_AMIGA && !MACHINE_X68000 && !MACHINE_MACINTOSH // TODO
		screen_putchar(keyboard_getch());
		if(screen_y == SCREEN_HEIGHT - 1)
		{
			screen_scroll(1);
			screen_y --;
		}
#endif
	}
}

