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

#if !MODE_REAL && (__ia16__ || __i386__ || __amd64__)
static inline void enter_usermode(uint16_t user_cs, uint16_t user_ss)
{
#if __ia16__
	asm volatile("\
	movw	%%sp, %%ax\n\
	pushw	%0\n\
	pushw	%%ax\n\
	pushfw\n\
	popw	%%ax\n\
	orw	$0x3000, %%ax # set IOPL to 3 to allow I/O\n\
	pushw	%%ax\n\
	pushw	%1\n\
	pushw	$1f\n\
	iretw\n\
1:\n\
	movw	%%ss, %%ax\n\
	movw	%%ax, %%ds\n\
	movw	%%ax, %%es\n" : : "g"(user_ss), "g"(user_cs));
#elif __i386__
	asm volatile("\
	movl	%%esp, %%eax\n\
	pushl	%0\n\
	pushl	%%eax\n\
	pushfl\n\
	popl	%%eax\n\
	orw	$0x3000, %%ax # set IOPL to 3 to allow I/O\n\
	pushl	%%eax\n\
	pushl	%1\n\
	pushl	$1f\n\
	iretl\n\
1:\n\
	movl	%%ss, %%eax\n\
	movl	%%eax, %%ds\n\
	movl	%%eax, %%es\n\
	movl	%%eax, %%fs\n\
	movl	%%eax, %%gs\n" : : "g"(user_ss), "g"(user_cs));
#elif __amd64__
	asm volatile("\
	movq	%%rsp, %%rax\n\
	pushq	%0\n\
	pushq	%%rax\n\
	pushfq\n\
	popq	%%rax\n\
	orw	$0x3000, %%ax # set IOPL to 3 to allow I/O\n\
	pushq	%%rax\n\
	pushq	%1\n\
	pushq	$1f\n\
	iretq\n\
1:\n\
	movl	%%ss, %%eax\n\
	movl	%%eax, %%ds\n\
	movl	%%eax, %%es\n\
	movl	%%eax, %%fs\n\
	movl	%%eax, %%gs\n" : : "g"(user_ss), "g"(user_cs));
#endif
}

#elif __m68k__
static inline void enter_usermode(uint32_t system_stack_top)
{
	asm volatile(
		"move.l\t%%sp, %%usp\n\t"
		"move.l\t%0, %%sp\n\t"
		"andi.w\t#0xDFFF, %%sr"
		: : "r"(system_stack_top));
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
