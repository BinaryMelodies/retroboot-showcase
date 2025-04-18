#ifndef _SYSTEM_H
#define _SYSTEM_H

static inline bool is_system_mode(void)
{
#if __ia16__ || __i386__ || __amd64__
# if OS86
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
static inline void enter_usermode(void)
{
#if __m68k__
	asm volatile("\
	move.l	%sp, %usp\n\
	andi.w	#0xDFFF, %sr");
#endif
}
#endif

static inline void enable_interrupts(void)
{
#if __ia16__ || __i386__ || __amd64__
	asm volatile("sti");
#elif __m68k__
	asm volatile("andi.w\t#0xF8FF, %sr"); // TODO: this enables all interrupts
#endif
}

static inline void disable_interrupts(void)
{
#if __ia16__ || __i386__ || __amd64__
	asm volatile("cli");
#elif __m68k__
	asm volatile("ori.w\t#0x0700, %sr");
#endif
}

#endif // _SYSTEM_H
