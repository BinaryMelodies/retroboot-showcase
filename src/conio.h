#ifndef _CONIO_H
#define _CONIO_H

#if !__ia16__ && !__i386__ && !__amd64__
# error conio.h not supported outside x86
#endif

#include "stdint.h"

static inline void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile("outb %b0, %w1"
		:
		: "a"((uint8_t)value), "Nd"((uint16_t)port)
		: "memory");
}
#define outp outb

static inline void outw(uint16_t port, uint16_t value)
{
	__asm__ volatile("outw %b0, %w1"
		:
		: "a"((uint16_t)value), "Nd"((uint16_t)port)
		: "memory");
}
#define outpw outw

#ifndef __ia16__
static inline void outl(uint16_t port, uint32_t value)
{
	__asm__ volatile("outl %b0, %w1"
		:
		: "a"((uint32_t)value), "Nd"((uint16_t)port)
		: "memory");
}
#define outpl outl
#endif

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__("inb %w1, %b0"
		: "=a"(ret)
		: "Nd"((uint16_t)port)
		: "memory");
	return ret;
}
#define inp inb

static inline uint16_t inw(uint16_t port)
{
	uint16_t ret;
	__asm__("inw %w1, %b0"
		: "=a"(ret)
		: "Nd"((uint16_t)port)
		: "memory");
	return ret;
}
#define inpw inw

#ifndef __ia16__
static inline uint32_t inl(uint16_t port)
{
	uint32_t ret;
	__asm__("inl %w1, %b0"
		: "=a"(ret)
		: "Nd"((uint16_t)port)
		: "memory");
	return ret;
}
#define inpl inl
#endif

#endif // _CONIO_H
