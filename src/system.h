#ifndef _SYSTEM_H
#define _SYSTEM_H

static inline bool is_system_mode(void)
{
#if __ia16__ || __i386__ || __amd64__
# if MODE_REAL
	// real mode is always in system mode
	return true;
# else
	uint16_t cs;
	asm("movw\t%%cs, %0" : "=g"(cs));
	// get the privilege level from the selector, 3 is user mode, 0 is system mode
	return (cs & 3) == 0;
# endif
#elif __m68k__
	uint16_t sr;
	// note: this instruction will trap if not in system mode
	asm("move.w\t%%sr, %0" : "=g"(sr));
	return (sr & 0x2000) != 0;
#else
	return true;
#endif
}

#if !__ia16__ && !__i386__ && !__amd64__ // defined in x86.c
static char system_stack[512] __attribute__((aligned(8)));
static inline void enter_usermode(void)
{
#if __m68k__
	asm volatile(
		"move.l\t%%sp, %%usp\n\t"
		"move.l\t%0, %%sp\n\t"
		"andi.w\t#0xDFFF, %%sr"
		: : "r"((size_t)system_stack + sizeof system_stack));
#endif
}
#endif

static inline void enable_interrupts(uint16_t sr)
{
#if __ia16__ || __i386__ || __amd64__
	(void) sr;
	asm volatile("sti");
#elif __m68k__
	asm volatile(
		"move.w\t%%sr, %%d0\n\t"
		"andi.w\t#0xF8FF, %%d0\n\t"
		"andi.w\t#0x0700, %0\n\t"
		"or.w\t%0, %%d0\n\t"
		"move.w\t%%d0, %%sr" : : "d"(sr) : "%d0");
#endif
}

static inline uint16_t disable_interrupts(void)
{
#if __ia16__ || __i386__ || __amd64__
	asm volatile("cli");
	return 0;
#elif __m68k__
	uint16_t sr;
	asm volatile(
		"move.w\t%%sr, %0\n\t"
		"ori.w\t#0x0700, %%sr"
		: "=r"(sr));
	return sr;
#endif
}

#endif // _SYSTEM_H
