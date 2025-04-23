#ifndef _I86_H
#define _I86_H

#include "conio.h"
#include "stddef.h"

#if __ia16__
# define far __far
# define MK_FP(__s, __o) ((void far *)(((uint32_t)(uint16_t)(__s) << 16) | (uint16_t)(__o)))
# define FP_OFF(__p) ((void *)(uint16_t)(uint32_t)(void far *)(__p))
# define FP_SEG(__p) ((uint16_t)((uint32_t)(void far *)(__s) >> 16))
#endif

#if __ia16__ || __i386__
typedef uint32_t address_t;
#elif __amd64__
typedef uint64_t address_t;
#endif

#if CPU_8086 || OS286
typedef uint16_t limit_t;
#else
typedef uint32_t limit_t;
#endif

static inline void io_wait(void)
{
	outb(0x80, 0); // unused port
}

#if !MODE_REAL
typedef struct segment_descriptor_t
{
	uint16_t limit0;
	uint32_t base0 : 24;
	uint8_t access;
#if OS286
	uint16_t reserved;
#else
	uint8_t limit1 : 4;
	uint8_t flags : 4;
	uint8_t base1;
#endif
} __attribute__((packed)) segment_descriptor_t;

typedef union descriptor_t
{
	uint16_t w[4];
	uint32_t d[2];
	segment_descriptor_t segment;
	struct
	{
		uint16_t offset0;
		uint16_t selector;
		uint8_t parameter;
		uint8_t access;
#if OS286
		uint16_t reserved;
#else
		uint16_t offset1;
#endif
	} gate;
} descriptor_t;

typedef struct descriptor_table_t
{
	uint16_t limit;
	address_t base; // 32-bit for 286/386, 64-bit for x64
} __attribute__((packed)) descriptor_table_t;

#if OS64
typedef struct segment_descriptor64_t
{
	uint16_t limit0;
	uint32_t base0 : 24;
	uint8_t access;
	uint8_t limit1 : 4;
	uint8_t flags : 4;
	uint8_t base1;
	uint32_t base2;
	uint32_t reserved;
} __attribute__((packed)) segment_descriptor64_t;

typedef union
{
	uint16_t w[8];
	uint32_t d[4];
	segment_descriptor64_t segment;
	struct
	{
		uint16_t offset0;
		uint16_t selector;
		uint8_t parameter;
		uint8_t access;
		uint16_t offset1;
		uint32_t offset2;
		uint32_t reserved;
	} gate;
} descriptor64_t;
#endif

typedef struct task_state_segment16_t
{
	uint16_t link;
	uint16_t sp0;
	uint16_t ss0;
	uint16_t sp1;
	uint16_t ss1;
	uint16_t sp2;
	uint16_t ss2;
	uint16_t ip;
	uint16_t flags;
	uint16_t ax;
	uint16_t cx;
	uint16_t dx;
	uint16_t bx;
	uint16_t sp;
	uint16_t bp;
	uint16_t si;
	uint16_t di;
	uint16_t es;
	uint16_t cs;
	uint16_t ss;
	uint16_t ds;
	uint16_t ldtr;
} task_state_segment16_t;

typedef struct task_state_segment32_t
{
	uint16_t link;
	uint32_t esp0;
	uint16_t ss0;
	uint32_t esp1;
	uint16_t ss1;
	uint32_t esp2;
	uint16_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint16_t es;
	uint16_t _reserved0;
	uint16_t cs;
	uint16_t _reserved1;
	uint16_t ss;
	uint16_t _reserved2;
	uint16_t ds;
	uint16_t _reserved3;
	uint16_t fs;
	uint16_t _reserved4;
	uint16_t gs;
	uint16_t _reserved5;
	uint16_t ldtr;
	uint16_t _reserved6;
	uint16_t t : 1, : 0;
	uint16_t iopb_offset;
} task_state_segment32_t;

#if OS64
typedef struct task_state_segment64_t
{
	uint32_t _reserved0;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t _reserved1;
	uint64_t ist_rsp1;
	uint64_t ist_rsp2;
	uint64_t ist_rsp3;
	uint64_t ist_rsp4;
	uint64_t ist_rsp5;
	uint64_t ist_rsp6;
	uint64_t ist_rsp7;
	uint64_t _reserved2;
	uint16_t t : 1, : 0;
	uint16_t iopb_offset;
} __attribute__((packed)) task_state_segment64_t;
#endif

#if OS286
typedef task_state_segment16_t task_state_segment_t;
#elif OS386
typedef task_state_segment32_t task_state_segment_t;
#elif OS64
typedef task_state_segment64_t task_state_segment_t;
#endif

enum
{
	DESCRIPTOR_ACCESS_CPL0 = 0x0000,
	DESCRIPTOR_ACCESS_CPL1 = 0x0020,
	DESCRIPTOR_ACCESS_CPL2 = 0x0040,
	DESCRIPTOR_ACCESS_CPL3 = 0x0060,

	DESCRIPTOR_ACCESS_TSS16 = 0x0081,
	DESCRIPTOR_ACCESS_TSS32 = 0x0089,
	DESCRIPTOR_ACCESS_TSS64 = 0x0089, // same as TSS32, only option in long mode
	DESCRIPTOR_ACCESS_LDT = 0x0082,
	DESCRIPTOR_ACCESS_CALLGATE16 = 0x0084,
	DESCRIPTOR_ACCESS_CALLGATE32 = 0x008C,
	DESCRIPTOR_ACCESS_CALLGATE64 = 0x008C, // same as INTGATE32, only option in long mode
	DESCRIPTOR_ACCESS_TASKGATE = 0x0085, // invalid in long mode
	DESCRIPTOR_ACCESS_INTGATE16 = 0x0086,
	DESCRIPTOR_ACCESS_INTGATE32 = 0x008E,
	DESCRIPTOR_ACCESS_INTGATE64 = 0x008E, // same as INTGATE32, only option in long mode
	DESCRIPTOR_ACCESS_TRAPGATE16 = 0x0087,
	DESCRIPTOR_ACCESS_TRAPGATE32 = 0x008F,
	DESCRIPTOR_ACCESS_TRAPGATE64 = 0x008F, // same as INTGATE32, only option in long mode

	DESCRIPTOR_ACCESS_DATA = 0x0092,
	DESCRIPTOR_ACCESS_CODE = 0x009A,

	DESCRIPTOR_FLAGS_16BIT = 0x00,
	DESCRIPTOR_FLAGS_32BIT = 0x40,
	DESCRIPTOR_FLAGS_64BIT = 0x20,

	DESCRIPTOR_FLAGS_G = 0x80,
};

#if OS286
// 286 does not use the flags parameter, drop the argument
# define descriptor_set_segment(__descriptor, __base, __limit, __attributes, __flags) descriptor_set_segment(__descriptor, __base, __limit, __attributes)
#endif

static inline void descriptor_set_segment(descriptor_t * descriptor, uint32_t base, limit_t limit, uint8_t access, uint8_t flags)
{
#if !OS286
	if(limit > 0xFFF)
	{
		flags |= DESCRIPTOR_FLAGS_G;
		limit >>= 12;
	}
	else
	{
		flags &= ~DESCRIPTOR_FLAGS_G;
	}
#endif
	descriptor->segment.limit0 = limit;
	descriptor->segment.base0 = base;
	descriptor->segment.access = access;
#if OS286
	descriptor->segment.reserved = 0;
#else
	descriptor->segment.limit1 = limit >> 16;
	descriptor->segment.flags = flags >> 4;
	descriptor->segment.base1 = base >> 24;
#endif
}

#if OS64
static inline void descriptor_set_segment64(descriptor64_t * descriptor, uint64_t base, limit_t limit, uint8_t access, uint8_t flags)
{
	if(limit > 0xFFF)
	{
		flags |= DESCRIPTOR_FLAGS_G;
		limit >>= 12;
	}
	else
	{
		flags &= ~DESCRIPTOR_FLAGS_G;
	}
	descriptor->segment.limit0 = limit;
	descriptor->segment.base0 = base;
	descriptor->segment.access = access;
	descriptor->segment.limit1 = limit >> 16;
	descriptor->segment.flags = flags >> 4;
	descriptor->segment.base1 = base >> 24;
	descriptor->segment.base2 = base >> 32;
	descriptor->segment.reserved = 0;
}
#endif

static inline void descriptor_set_gate32(descriptor_t * descriptor, uint16_t selector, size_t offset, uint8_t access)
{
	descriptor->gate.offset0 = offset;
	descriptor->gate.selector = selector;
	descriptor->gate.access = access;
#if OS286
	descriptor->gate.reserved = 0;
#else
	descriptor->gate.offset1 = offset >> 16;
#endif
}

#if OS64
static inline void descriptor_set_gate64(descriptor64_t * descriptor, uint16_t selector, size_t offset, uint8_t access)
{
	descriptor->gate.offset0 = offset;
	descriptor->gate.selector = selector;
	descriptor->gate.access = access;
	descriptor->gate.offset1 = offset >> 16;
	descriptor->gate.offset2 = offset >> 32;
}
#endif

#if !OS64
# define descriptor_set_gate descriptor_set_gate32
#else
# define descriptor_set_gate descriptor_set_gate64
#endif

static inline void load_gdt(descriptor_t * table, uint16_t size, uint16_t cs, uint16_t ss, uint16_t ds)
{
	descriptor_table_t gdtr;
	gdtr.limit = size - 1;
	gdtr.base = (size_t)table;

	/* after calling LGDT, a long return is used to load CS:[E/R]IP, and then the remaining registers are filled with SEL_KERNEL_SS */

#if __ia16__
	/* The 80286 only has the SS, DS and ES registers */
	asm volatile(
		"pushw\t%1\n\t"
		"pushw\t$1f\n\t"
		"lgdt\t(%0)\n\t"
		"lretw\n"
		"1:\n\t"
		"movw\t%w2, %%ss\n\t"
		"movw\t%w3, %%ds\n\t"
		"movw\t%w3, %%es"
		:
		: "B"(&gdtr), "ri"(cs), "r"(ss), "r"(ds)
		: "memory");
#elif __i386__
	asm volatile(
		"pushl\t%1\n\t"
		"pushl\t$1f\n\t"
		"lgdt\t(%0)\n\t"
		"lretl\n"
		"1:\n\t"
		"movw\t%w2, %%ss\n\t"
		"movw\t%w3, %%ds\n\t"
		"movw\t%w3, %%es\n\t"
		"movw\t%w3, %%fs\n\t"
		"movw\t%w3, %%gs\n\t"
		:
		: "r"(&gdtr), "ri"((uint32_t)cs), "r"(ss), "r"(ds)
		: "memory");
#elif __amd64__
	asm volatile(
		"pushq\t%1\n\t"
		"pushq\t$1f\n\t"
		"lgdt\t(%0)\n\t"
		"lretq\n"
		"1:\n\t"
		"movw\t%w2, %%ss\n\t"
		"movw\t%w3, %%ds\n\t"
		"movw\t%w3, %%es\n\t"
		"movw\t%w3, %%fs\n\t"
		"movw\t%w3, %%gs\n\t"
		:
		: "r"(&gdtr), "ri"((uint64_t)cs), "r"(ss), "r"(ds)
		: "memory");
#endif
}

#if !OS64
static inline void load_idt(descriptor_t * table, uint16_t size)
#else
static inline void load_idt(descriptor64_t * table, uint16_t size)
#endif
{
	descriptor_table_t idtr;
	idtr.limit = size - 1;
	idtr.base = (size_t)table;

#if OS286
	asm volatile("lidt\t(%0)" : : "B"(&idtr) : "memory");
#else
	asm volatile("lidt\t(%0)" : : "r"(&idtr) : "memory");
#endif
}

static inline void load_ldt(uint16_t selector)
{
	asm volatile("lldt\t%0" : : "r"(selector));
}

static inline void load_tss(uint16_t selector)
{
	asm volatile("ltr\t%0" : : "r"(selector));
}
#endif

#endif // _I86_H
