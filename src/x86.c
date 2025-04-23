
#include "i8259.c"
#include "i8253.c"
#include "keyboard.c"

#if OS286 || OS386 || OS64
enum
{
	SEL_KERNEL_CS = 0x08,
	SEL_KERNEL_SS = 0x10, // must follow CS for SYSENTER and SYSCALL
#if OS286
	SEL_KERNEL_SCREEN = 0x18,
	SEL_KERNEL_SEGMENT1 = 0x20, // used as source segment for huge transfers
	SEL_KERNEL_SEGMENT2 = 0x28, // used as destination segment for huge transfers
	SEL_TSS = 0x30,
	SEL_LDT = 0x38,
	SEL_GDT_MAX = 0x40,

	SEL_CALL7 = 0x04,
	SEL_USER_CS = 0x0C,
	SEL_USER_SS = 0x14,
	SEL_LDT_MAX = 0x18,
#elif OS386
	SEL_USER_CS = 0x18, // must follow CS/SS for SYSEXIT
	SEL_USER_SS = 0x20, // must follow CS/SS for SYSEXIT
	SEL_TSS = 0x28,
	SEL_LDT = 0x30,
	SEL_GDT_MAX = 0x38,

	SEL_CALL7 = 0x04,
	SEL_LDT_MAX = 0x08,
#elif OS64
	SEL_USER_CS32 = 0x18, // must follow CS/SS for SYSEXIT
	SEL_USER_SS32 = 0x20, // must follow CS/SS for SYSEXIT
	SEL_USER_CS = 0x28, // must follow CS/SS for SYSEXIT and CS32/SS32 for SYSCALL
	SEL_USER_SS = 0x30, // must follow CS/SS for SYSEXIT
	SEL_TSS = 0x38, // takes up 2 slots
	SEL_LDT = 0x48, // takes up 2 slots
	SEL_GDT_MAX = 0x58,

	SEL_CALL7 = 0x04,
	SEL_LDT_MAX = 0x08,
#endif
};

descriptor_t gdt[SEL_GDT_MAX / 8];
descriptor_t ldt[SEL_LDT_MAX / 8];
task_state_segment_t tss;

#if !OS64
descriptor_t idt[256];
#else
descriptor64_t idt[256];
#endif

#endif

enum
{
	IRQ0 = 32,
	IRQ8 = IRQ0 + 8,
};

#if MODE_REAL
# define DEFINE_ISR_NO_ERROR_CODE(__hex) \
void isr##__hex(void); \
asm( \
	".global\tisr" #__hex "\n" \
	"isr" #__hex ":\n\t" \
	"pushw\t%ax\n\t" \
	"movw\t$" #__hex ", %ax\n\t" \
	"pushw\t%ax\n\t" \
	"jmp\tisr_common" \
);
// there are no error codes pushed to stack in real mode
# define DEFINE_ISR_ERROR_CODE DEFINE_ISR_NO_ERROR_CODE
#elif OS286
# define DEFINE_ISR_NO_ERROR_CODE(__hex) \
void isr##__hex(void); \
asm( \
	".global\tisr" #__hex "\n" \
	"isr" #__hex ":\n\t" \
	"pushw\t$0\n\t" \
	"pushw\t$" #__hex "\n\t" \
	"jmp\tisr_common" \
);
# define DEFINE_ISR_ERROR_CODE(__hex) \
void isr##__hex(void); \
asm( \
	".global\tisr" #__hex "\n" \
	"isr" #__hex ":\n\t" \
	"pushw\t$" #__hex "\n\t" \
	"jmp\tisr_common" \
);
#elif OS386
# define DEFINE_ISR_NO_ERROR_CODE(__hex) \
void isr##__hex(void); \
asm( \
	".global\tisr" #__hex "\n" \
	"isr" #__hex ":\n\t" \
	"pushl\t$0\n\t" \
	"pushl\t$" #__hex "\n\t" \
	"jmp\tisr_common" \
);
# define DEFINE_ISR_ERROR_CODE(__hex) \
void isr##__hex(void); \
asm( \
	".global\tisr" #__hex "\n" \
	"isr" #__hex ":\n\t" \
	"pushl\t$" #__hex "\n\t" \
	"jmp\tisr_common" \
);
#elif OS64
# define DEFINE_ISR_NO_ERROR_CODE(__hex) \
void isr##__hex(void); \
asm( \
	".global\tisr" #__hex "\n" \
	"isr" #__hex ":\n\t" \
	"pushq\t$0\n\t" \
	"pushq\t$" #__hex "\n\t" \
	"jmp\tisr_common" \
);
# define DEFINE_ISR_ERROR_CODE(__hex) \
void isr##__hex(void); \
asm( \
	".global\tisr" #__hex "\n" \
	"isr" #__hex ":\n\t" \
	"pushq\t$" #__hex "\n\t" \
	"jmp\tisr_common" \
);
#endif

DEFINE_ISR_NO_ERROR_CODE(0x00)
DEFINE_ISR_NO_ERROR_CODE(0x01)
DEFINE_ISR_NO_ERROR_CODE(0x02)
DEFINE_ISR_NO_ERROR_CODE(0x03)
DEFINE_ISR_NO_ERROR_CODE(0x04)
DEFINE_ISR_NO_ERROR_CODE(0x05)
DEFINE_ISR_NO_ERROR_CODE(0x06)
DEFINE_ISR_NO_ERROR_CODE(0x07)
DEFINE_ISR_ERROR_CODE(0x08)
DEFINE_ISR_NO_ERROR_CODE(0x09)
DEFINE_ISR_ERROR_CODE(0x0A)
DEFINE_ISR_ERROR_CODE(0x0B)
DEFINE_ISR_ERROR_CODE(0x0C)
DEFINE_ISR_ERROR_CODE(0x0D)
DEFINE_ISR_ERROR_CODE(0x0E)
DEFINE_ISR_NO_ERROR_CODE(0x0F)
DEFINE_ISR_NO_ERROR_CODE(0x10)
DEFINE_ISR_ERROR_CODE(0x11)
DEFINE_ISR_NO_ERROR_CODE(0x12)
DEFINE_ISR_NO_ERROR_CODE(0x13)
DEFINE_ISR_NO_ERROR_CODE(0x14)
DEFINE_ISR_ERROR_CODE(0x15)
DEFINE_ISR_NO_ERROR_CODE(0x16)
DEFINE_ISR_NO_ERROR_CODE(0x17)
DEFINE_ISR_NO_ERROR_CODE(0x18)
DEFINE_ISR_NO_ERROR_CODE(0x19)
DEFINE_ISR_NO_ERROR_CODE(0x1A)
DEFINE_ISR_NO_ERROR_CODE(0x1B)
DEFINE_ISR_NO_ERROR_CODE(0x1C)
DEFINE_ISR_ERROR_CODE(0x1D)
DEFINE_ISR_ERROR_CODE(0x1E)
DEFINE_ISR_NO_ERROR_CODE(0x1F)
DEFINE_ISR_NO_ERROR_CODE(0x20)
DEFINE_ISR_NO_ERROR_CODE(0x21)
DEFINE_ISR_NO_ERROR_CODE(0x22)
DEFINE_ISR_NO_ERROR_CODE(0x23)
DEFINE_ISR_NO_ERROR_CODE(0x24)
DEFINE_ISR_NO_ERROR_CODE(0x25)
DEFINE_ISR_NO_ERROR_CODE(0x26)
DEFINE_ISR_NO_ERROR_CODE(0x27)
DEFINE_ISR_NO_ERROR_CODE(0x28)
DEFINE_ISR_NO_ERROR_CODE(0x29)
DEFINE_ISR_NO_ERROR_CODE(0x2A)
DEFINE_ISR_NO_ERROR_CODE(0x2B)
DEFINE_ISR_NO_ERROR_CODE(0x2C)
DEFINE_ISR_NO_ERROR_CODE(0x2D)
DEFINE_ISR_NO_ERROR_CODE(0x2E)
DEFINE_ISR_NO_ERROR_CODE(0x2F)
DEFINE_ISR_NO_ERROR_CODE(0x30)
DEFINE_ISR_NO_ERROR_CODE(0x31)
DEFINE_ISR_NO_ERROR_CODE(0x32)
DEFINE_ISR_NO_ERROR_CODE(0x33)
DEFINE_ISR_NO_ERROR_CODE(0x34)
DEFINE_ISR_NO_ERROR_CODE(0x35)
DEFINE_ISR_NO_ERROR_CODE(0x36)
DEFINE_ISR_NO_ERROR_CODE(0x37)
DEFINE_ISR_NO_ERROR_CODE(0x38)
DEFINE_ISR_NO_ERROR_CODE(0x39)
DEFINE_ISR_NO_ERROR_CODE(0x3A)
DEFINE_ISR_NO_ERROR_CODE(0x3B)
DEFINE_ISR_NO_ERROR_CODE(0x3C)
DEFINE_ISR_NO_ERROR_CODE(0x3D)
DEFINE_ISR_NO_ERROR_CODE(0x3E)
DEFINE_ISR_NO_ERROR_CODE(0x3F)
DEFINE_ISR_NO_ERROR_CODE(0x40)
DEFINE_ISR_NO_ERROR_CODE(0x41)
DEFINE_ISR_NO_ERROR_CODE(0x42)
DEFINE_ISR_NO_ERROR_CODE(0x43)
DEFINE_ISR_NO_ERROR_CODE(0x44)
DEFINE_ISR_NO_ERROR_CODE(0x45)
DEFINE_ISR_NO_ERROR_CODE(0x46)
DEFINE_ISR_NO_ERROR_CODE(0x47)
DEFINE_ISR_NO_ERROR_CODE(0x48)
DEFINE_ISR_NO_ERROR_CODE(0x49)
DEFINE_ISR_NO_ERROR_CODE(0x4A)
DEFINE_ISR_NO_ERROR_CODE(0x4B)
DEFINE_ISR_NO_ERROR_CODE(0x4C)
DEFINE_ISR_NO_ERROR_CODE(0x4D)
DEFINE_ISR_NO_ERROR_CODE(0x4E)
DEFINE_ISR_NO_ERROR_CODE(0x4F)
DEFINE_ISR_NO_ERROR_CODE(0x50)
DEFINE_ISR_NO_ERROR_CODE(0x51)
DEFINE_ISR_NO_ERROR_CODE(0x52)
DEFINE_ISR_NO_ERROR_CODE(0x53)
DEFINE_ISR_NO_ERROR_CODE(0x54)
DEFINE_ISR_NO_ERROR_CODE(0x55)
DEFINE_ISR_NO_ERROR_CODE(0x56)
DEFINE_ISR_NO_ERROR_CODE(0x57)
DEFINE_ISR_NO_ERROR_CODE(0x58)
DEFINE_ISR_NO_ERROR_CODE(0x59)
DEFINE_ISR_NO_ERROR_CODE(0x5A)
DEFINE_ISR_NO_ERROR_CODE(0x5B)
DEFINE_ISR_NO_ERROR_CODE(0x5C)
DEFINE_ISR_NO_ERROR_CODE(0x5D)
DEFINE_ISR_NO_ERROR_CODE(0x5E)
DEFINE_ISR_NO_ERROR_CODE(0x5F)
DEFINE_ISR_NO_ERROR_CODE(0x60)
DEFINE_ISR_NO_ERROR_CODE(0x61)
DEFINE_ISR_NO_ERROR_CODE(0x62)
DEFINE_ISR_NO_ERROR_CODE(0x63)
DEFINE_ISR_NO_ERROR_CODE(0x64)
DEFINE_ISR_NO_ERROR_CODE(0x65)
DEFINE_ISR_NO_ERROR_CODE(0x66)
DEFINE_ISR_NO_ERROR_CODE(0x67)
DEFINE_ISR_NO_ERROR_CODE(0x68)
DEFINE_ISR_NO_ERROR_CODE(0x69)
DEFINE_ISR_NO_ERROR_CODE(0x6A)
DEFINE_ISR_NO_ERROR_CODE(0x6B)
DEFINE_ISR_NO_ERROR_CODE(0x6C)
DEFINE_ISR_NO_ERROR_CODE(0x6D)
DEFINE_ISR_NO_ERROR_CODE(0x6E)
DEFINE_ISR_NO_ERROR_CODE(0x6F)
DEFINE_ISR_NO_ERROR_CODE(0x70)
DEFINE_ISR_NO_ERROR_CODE(0x71)
DEFINE_ISR_NO_ERROR_CODE(0x72)
DEFINE_ISR_NO_ERROR_CODE(0x73)
DEFINE_ISR_NO_ERROR_CODE(0x74)
DEFINE_ISR_NO_ERROR_CODE(0x75)
DEFINE_ISR_NO_ERROR_CODE(0x76)
DEFINE_ISR_NO_ERROR_CODE(0x77)
DEFINE_ISR_NO_ERROR_CODE(0x78)
DEFINE_ISR_NO_ERROR_CODE(0x79)
DEFINE_ISR_NO_ERROR_CODE(0x7A)
DEFINE_ISR_NO_ERROR_CODE(0x7B)
DEFINE_ISR_NO_ERROR_CODE(0x7C)
DEFINE_ISR_NO_ERROR_CODE(0x7D)
DEFINE_ISR_NO_ERROR_CODE(0x7E)
DEFINE_ISR_NO_ERROR_CODE(0x7F)
DEFINE_ISR_NO_ERROR_CODE(0x80)
DEFINE_ISR_NO_ERROR_CODE(0x81)
DEFINE_ISR_NO_ERROR_CODE(0x82)
DEFINE_ISR_NO_ERROR_CODE(0x83)
DEFINE_ISR_NO_ERROR_CODE(0x84)
DEFINE_ISR_NO_ERROR_CODE(0x85)
DEFINE_ISR_NO_ERROR_CODE(0x86)
DEFINE_ISR_NO_ERROR_CODE(0x87)
DEFINE_ISR_NO_ERROR_CODE(0x88)
DEFINE_ISR_NO_ERROR_CODE(0x89)
DEFINE_ISR_NO_ERROR_CODE(0x8A)
DEFINE_ISR_NO_ERROR_CODE(0x8B)
DEFINE_ISR_NO_ERROR_CODE(0x8C)
DEFINE_ISR_NO_ERROR_CODE(0x8D)
DEFINE_ISR_NO_ERROR_CODE(0x8E)
DEFINE_ISR_NO_ERROR_CODE(0x8F)
DEFINE_ISR_NO_ERROR_CODE(0x90)
DEFINE_ISR_NO_ERROR_CODE(0x91)
DEFINE_ISR_NO_ERROR_CODE(0x92)
DEFINE_ISR_NO_ERROR_CODE(0x93)
DEFINE_ISR_NO_ERROR_CODE(0x94)
DEFINE_ISR_NO_ERROR_CODE(0x95)
DEFINE_ISR_NO_ERROR_CODE(0x96)
DEFINE_ISR_NO_ERROR_CODE(0x97)
DEFINE_ISR_NO_ERROR_CODE(0x98)
DEFINE_ISR_NO_ERROR_CODE(0x99)
DEFINE_ISR_NO_ERROR_CODE(0x9A)
DEFINE_ISR_NO_ERROR_CODE(0x9B)
DEFINE_ISR_NO_ERROR_CODE(0x9C)
DEFINE_ISR_NO_ERROR_CODE(0x9D)
DEFINE_ISR_NO_ERROR_CODE(0x9E)
DEFINE_ISR_NO_ERROR_CODE(0x9F)
DEFINE_ISR_NO_ERROR_CODE(0xA0)
DEFINE_ISR_NO_ERROR_CODE(0xA1)
DEFINE_ISR_NO_ERROR_CODE(0xA2)
DEFINE_ISR_NO_ERROR_CODE(0xA3)
DEFINE_ISR_NO_ERROR_CODE(0xA4)
DEFINE_ISR_NO_ERROR_CODE(0xA5)
DEFINE_ISR_NO_ERROR_CODE(0xA6)
DEFINE_ISR_NO_ERROR_CODE(0xA7)
DEFINE_ISR_NO_ERROR_CODE(0xA8)
DEFINE_ISR_NO_ERROR_CODE(0xA9)
DEFINE_ISR_NO_ERROR_CODE(0xAA)
DEFINE_ISR_NO_ERROR_CODE(0xAB)
DEFINE_ISR_NO_ERROR_CODE(0xAC)
DEFINE_ISR_NO_ERROR_CODE(0xAD)
DEFINE_ISR_NO_ERROR_CODE(0xAE)
DEFINE_ISR_NO_ERROR_CODE(0xAF)
DEFINE_ISR_NO_ERROR_CODE(0xB0)
DEFINE_ISR_NO_ERROR_CODE(0xB1)
DEFINE_ISR_NO_ERROR_CODE(0xB2)
DEFINE_ISR_NO_ERROR_CODE(0xB3)
DEFINE_ISR_NO_ERROR_CODE(0xB4)
DEFINE_ISR_NO_ERROR_CODE(0xB5)
DEFINE_ISR_NO_ERROR_CODE(0xB6)
DEFINE_ISR_NO_ERROR_CODE(0xB7)
DEFINE_ISR_NO_ERROR_CODE(0xB8)
DEFINE_ISR_NO_ERROR_CODE(0xB9)
DEFINE_ISR_NO_ERROR_CODE(0xBA)
DEFINE_ISR_NO_ERROR_CODE(0xBB)
DEFINE_ISR_NO_ERROR_CODE(0xBC)
DEFINE_ISR_NO_ERROR_CODE(0xBD)
DEFINE_ISR_NO_ERROR_CODE(0xBE)
DEFINE_ISR_NO_ERROR_CODE(0xBF)
DEFINE_ISR_NO_ERROR_CODE(0xC0)
DEFINE_ISR_NO_ERROR_CODE(0xC1)
DEFINE_ISR_NO_ERROR_CODE(0xC2)
DEFINE_ISR_NO_ERROR_CODE(0xC3)
DEFINE_ISR_NO_ERROR_CODE(0xC4)
DEFINE_ISR_NO_ERROR_CODE(0xC5)
DEFINE_ISR_NO_ERROR_CODE(0xC6)
DEFINE_ISR_NO_ERROR_CODE(0xC7)
DEFINE_ISR_NO_ERROR_CODE(0xC8)
DEFINE_ISR_NO_ERROR_CODE(0xC9)
DEFINE_ISR_NO_ERROR_CODE(0xCA)
DEFINE_ISR_NO_ERROR_CODE(0xCB)
DEFINE_ISR_NO_ERROR_CODE(0xCC)
DEFINE_ISR_NO_ERROR_CODE(0xCD)
DEFINE_ISR_NO_ERROR_CODE(0xCE)
DEFINE_ISR_NO_ERROR_CODE(0xCF)
DEFINE_ISR_NO_ERROR_CODE(0xD0)
DEFINE_ISR_NO_ERROR_CODE(0xD1)
DEFINE_ISR_NO_ERROR_CODE(0xD2)
DEFINE_ISR_NO_ERROR_CODE(0xD3)
DEFINE_ISR_NO_ERROR_CODE(0xD4)
DEFINE_ISR_NO_ERROR_CODE(0xD5)
DEFINE_ISR_NO_ERROR_CODE(0xD6)
DEFINE_ISR_NO_ERROR_CODE(0xD7)
DEFINE_ISR_NO_ERROR_CODE(0xD8)
DEFINE_ISR_NO_ERROR_CODE(0xD9)
DEFINE_ISR_NO_ERROR_CODE(0xDA)
DEFINE_ISR_NO_ERROR_CODE(0xDB)
DEFINE_ISR_NO_ERROR_CODE(0xDC)
DEFINE_ISR_NO_ERROR_CODE(0xDD)
DEFINE_ISR_NO_ERROR_CODE(0xDE)
DEFINE_ISR_NO_ERROR_CODE(0xDF)
DEFINE_ISR_NO_ERROR_CODE(0xE0)
DEFINE_ISR_NO_ERROR_CODE(0xE1)
DEFINE_ISR_NO_ERROR_CODE(0xE2)
DEFINE_ISR_NO_ERROR_CODE(0xE3)
DEFINE_ISR_NO_ERROR_CODE(0xE4)
DEFINE_ISR_NO_ERROR_CODE(0xE5)
DEFINE_ISR_NO_ERROR_CODE(0xE6)
DEFINE_ISR_NO_ERROR_CODE(0xE7)
DEFINE_ISR_NO_ERROR_CODE(0xE8)
DEFINE_ISR_NO_ERROR_CODE(0xE9)
DEFINE_ISR_NO_ERROR_CODE(0xEA)
DEFINE_ISR_NO_ERROR_CODE(0xEB)
DEFINE_ISR_NO_ERROR_CODE(0xEC)
DEFINE_ISR_NO_ERROR_CODE(0xED)
DEFINE_ISR_NO_ERROR_CODE(0xEE)
DEFINE_ISR_NO_ERROR_CODE(0xEF)
DEFINE_ISR_NO_ERROR_CODE(0xF0)
DEFINE_ISR_NO_ERROR_CODE(0xF1)
DEFINE_ISR_NO_ERROR_CODE(0xF2)
DEFINE_ISR_NO_ERROR_CODE(0xF3)
DEFINE_ISR_NO_ERROR_CODE(0xF4)
DEFINE_ISR_NO_ERROR_CODE(0xF5)
DEFINE_ISR_NO_ERROR_CODE(0xF6)
DEFINE_ISR_NO_ERROR_CODE(0xF7)
DEFINE_ISR_NO_ERROR_CODE(0xF8)
DEFINE_ISR_NO_ERROR_CODE(0xF9)
DEFINE_ISR_NO_ERROR_CODE(0xFA)
DEFINE_ISR_NO_ERROR_CODE(0xFB)
DEFINE_ISR_NO_ERROR_CODE(0xFC)
DEFINE_ISR_NO_ERROR_CODE(0xFD)
DEFINE_ISR_NO_ERROR_CODE(0xFE)
DEFINE_ISR_NO_ERROR_CODE(0xFF)

#if MODE_REAL
asm(
	".global\tisr_common\n\t"
	"isr_common:\n\t"
	"movw\t%ss, system_stack + 512 - 2\n\t"
	"movw\t%sp, system_stack + 512 - 4\n\t"
	"xorw\t%ax, %ax\n\t"
	"movw\t%ax, %ss\n\t"
	"movw\t$system_stack + 512 - 4, %sp\n\t"
	"pushw\t%cx\n\t"
	"pushw\t%dx\n\t"
	"pushw\t%bx\n\t"
	"pushw\t%bp\n\t"
	"pushw\t%si\n\t"
	"pushw\t%di\n\t"
	"pushw\t%es\n\t"
	"pushw\t%ds\n\t"
	"xorw\t%ax, %ax\n\t"
	"movw\t%ax, %ds\n\t"
	"movw\t%sp, %ax\n\t"
	"pushw\t%ax\n\t"
	"call\tinterrupt_handler\n\t"
	"popw\t%ax\n\t"
	"popw\t%ds\n\t"
	"popw\t%es\n\t"
	"popw\t%di\n\t"
	"popw\t%si\n\t"
	"popw\t%bp\n\t"
	"popw\t%bx\n\t"
	"popw\t%dx\n\t"
	"popw\t%cx\n\t"
	"popw\t%ax\n\t"
	"popw\t%ss\n\t"
	"movw\t%ax, %sp\n\t"
	"addw\t$2, %sp\n\t"
	"popw\t%ax\n\t"
	"iretw"
);
#elif OS286
asm(
	".global\tisr_common\n\t"
	"isr_common:\n\t"
	"pushaw\n\t"
	"pushw\t%es\n\t"
	"pushw\t%ds\n\t"
	"movw\t$0x10, %ax\n\t"
	"movw\t%ax, %ds\n\t"
	"movw\t%sp, %ax\n\t"
	"pushw\t%ax\n\t"
	"call\tinterrupt_handler\n\t"
	"popw\t%ax\n\t"
	"popw\t%ds\n\t"
	"popw\t%es\n\t"
	"popaw\n\t"
	"addw\t$4, %sp\n\t"
	"iretw"
);
#elif OS386
asm(
	".global\tisr_common\n\t"
	"isr_common:\n\t"
	"pushal\n\t"
	"pushl\t%es\n\t"
	"pushl\t%ds\n\t"
	"pushl\t%fs\n\t"
	"pushl\t%gs\n\t"
	"movw\t$0x10, %ax\n\t"
	"movw\t%ax, %ds\n\t"
	"movl\t%esp, %eax\n\t"
	"pushl\t%eax\n\t"
	"call\tinterrupt_handler\n\t"
	"popl\t%eax\n\t"
	"popl\t%gs\n\t"
	"popl\t%fs\n\t"
	"popl\t%ds\n\t"
	"popl\t%es\n\t"
	"popal\n\t"
	"addl\t$8, %esp\n\t"
	"iretl"
);
#elif OS64
asm(
	".global\tisr_common\n\t"
	"isr_common:\n\t"
	"pushq\t%rax\n\t"
	"pushq\t%rcx\n\t"
	"pushq\t%rdx\n\t"
	"pushq\t%rbx\n\t"
	"pushq\t%rbp\n\t"
	"pushq\t%rsi\n\t"
	"pushq\t%rdi\n\t"
	"pushq\t%r8\n\t"
	"pushq\t%r9\n\t"
	"pushq\t%r10\n\t"
	"pushq\t%r11\n\t"
	"pushq\t%r12\n\t"
	"pushq\t%r13\n\t"
	"pushq\t%r14\n\t"
	"pushq\t%r15\n\t"
	"movl\t%es, %eax\n\t"
	"pushq\t%rax\n\t"
	"movl\t%ds, %eax\n\t"
	"pushq\t%rax\n\t"
	"pushq\t%fs\n\t"
	"pushq\t%gs\n\t"
	"movw\t$0x10, %ax\n\t"
	"movw\t%ax, %ds\n\t"
	"movq\t%rsp, %rdi\n\t"
	"call\tinterrupt_handler\n\t"
	"popq\t%gs\n\t"
	"popq\t%fs\n\t"
	"popq\t%rax\n\t"
	"movl\t%eax, %ds\n\t"
	"popq\t%rax\n\t"
	"movl\t%eax, %es\n\t"
	"popq\t%r15\n\t"
	"popq\t%r14\n\t"
	"popq\t%r13\n\t"
	"popq\t%r12\n\t"
	"popq\t%r11\n\t"
	"popq\t%r10\n\t"
	"popq\t%r9\n\t"
	"popq\t%r8\n\t"
	"popq\t%rdi\n\t"
	"popq\t%rsi\n\t"
	"popq\t%rbp\n\t"
	"popq\t%rbx\n\t"
	"popq\t%rdx\n\t"
	"popq\t%rcx\n\t"
	"popq\t%rax\n\t"
	"addq\t$16, %rsp\n\t"
	"iretq"
);
#endif


#if MODE_REAL
typedef struct call_frame_t
{
	uint16_t interrupt_number;
	uint16_t ax;
	uint16_t ip;
	uint16_t cs;
	uint16_t flags;
} call_frame_t;
typedef struct registers_t
{
	uint16_t ds;
	uint16_t es;
	uint16_t di;
	uint16_t si;
	uint16_t bp;
	uint16_t bx;
	uint16_t dx;
	uint16_t cx;
	union
	{
		struct
		{
			uint16_t sp;
			uint16_t ss;
		};
		call_frame_t far * frame;
	};
} registers_t;
# define FLD_INTERRUPT_NUMBER frame->interrupt_number
# define FLD_CS frame->cs
# define FLD_IP frame->ip
#elif OS286
typedef struct registers_t
{
	uint16_t ds;
	uint16_t es;
	uint16_t di;
	uint16_t si;
	uint16_t bp;
	uint16_t _sp;
	uint16_t bx;
	uint16_t dx;
	uint16_t cx;
	uint16_t ax;
	uint16_t interrupt_number;
	uint16_t error_code;
	uint16_t ip;
	uint16_t cs;
	uint16_t flags;
	uint16_t ss;
	uint16_t sp;
} registers_t;
# define FLD_INTERRUPT_NUMBER interrupt_number
# define FLD_CS cs
# define FLD_IP ip
#elif OS386
typedef struct registers_t
{
	uint32_t gs;
	uint32_t fs;
	uint32_t ds;
	uint32_t es;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t _esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t interrupt_number;
	uint32_t error_code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} registers_t;
# define FLD_INTERRUPT_NUMBER interrupt_number
# define FLD_CS cs
# define FLD_IP eip
#elif OS64
typedef struct registers_t
{
	uint64_t gs;
	uint64_t fs;
	uint64_t ds;
	uint64_t es;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rbp;
	uint64_t rbx;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rax;
	uint64_t interrupt_number;
	uint64_t error_code;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} registers_t;
# define FLD_INTERRUPT_NUMBER interrupt_number
# define FLD_CS cs
# define FLD_IP rip
#endif

#if MODE_REAL
// real mode provides no access flags, drop the argument
# define set_interrupt(__interrupt_number, __selector, __offset, __access) set_interrupt(__interrupt_number, __selector, __offset)
#endif

static char system_stack[512] __attribute__((aligned(8)));
//static char user_stack[512] __attribute__((aligned(8)));

static inline void set_interrupt(uint8_t interrupt_number, uint16_t selector, void * offset, uint8_t access)
{
#if MODE_REAL
	*(uint16_t *)MK_FP(0, interrupt_number * 4)     = (size_t)offset;
	*(uint16_t *)MK_FP(0, interrupt_number * 4 + 2) = selector;
#elif !OS64
	descriptor_set_gate(&idt[interrupt_number], selector, (size_t)offset, access);
#else
	descriptor_set_gate(&idt[interrupt_number], selector, (size_t)offset, access);
#endif
}

static inline void timer_interrupt_handler(registers_t * registers);
static inline void keyboard_interrupt_handler(registers_t * registers);

void interrupt_handler(registers_t * registers)
{
	if(IRQ8 <= registers->FLD_INTERRUPT_NUMBER && registers->FLD_INTERRUPT_NUMBER < IRQ8 + 8)
	{
		outp(PORT_PIC2_COMMAND, PIC_EOI);
		outp(PORT_PIC1_COMMAND, PIC_EOI);
	}
	else if(IRQ0 <= registers->FLD_INTERRUPT_NUMBER && registers->FLD_INTERRUPT_NUMBER < IRQ0 + 8)
	{
		outp(PORT_PIC1_COMMAND, PIC_EOI);
	}

	uint8_t old_screen_x = screen_x;
	uint8_t old_screen_y = screen_y;
	uint8_t old_screen_attribute = screen_attribute;

	screen_x = 0;
	screen_y = SCREEN_HEIGHT - 1;
#if MACHINE_IBMPC
	screen_attribute = 0x4E; // yellow on red
#elif MACHINE_NECPC98
	screen_attribute = 0x41; // red on black
#endif

	screen_putstr("Interrupt 0x");
	screen_puthex(registers->FLD_INTERRUPT_NUMBER);
#if !MODE_REAL
	screen_putstr(" with error code ");
	screen_puthex(registers->error_code);
#endif
	screen_putstr(" called from 0x");
	screen_puthex(registers->FLD_CS);
	screen_putstr(":0x");
	screen_puthex(registers->FLD_IP);
	screen_putchar(' ');

	switch(registers->FLD_INTERRUPT_NUMBER)
	{
#if MACHINE_IBMPC || MACHINE_NECPC98
	case IRQ0 + 0: // timer interrupt
		timer_interrupt_handler(registers);
		break;
#endif
#if MACHINE_IBMPC || MACHINE_NECPC98
	case IRQ0 + 1: // keyboard interrupt
		keyboard_interrupt_handler(registers);
		break;
#endif
	}

	screen_x = old_screen_x;
	screen_y = old_screen_y;
	screen_attribute = old_screen_attribute;

	screen_video_move_cursor();
}

static inline void enter_usermode(void)
{
#if OS286
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
	movw	%%ax, %%es\n" : : "g"(SEL_USER_SS|3), "g"(SEL_USER_CS|3));
#elif OS386
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
	movl	%%eax, %%gs\n" : : "g"(SEL_USER_SS|3), "g"(SEL_USER_CS|3));
#elif OS64
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
	movl	%%eax, %%gs\n" : : "g"(SEL_USER_SS|3), "g"(SEL_USER_CS|3));
#endif
}

static volatile uint32_t timer_tick;

static inline void timer_interrupt_handler(registers_t * registers)
{
	(void) registers;

	timer_tick ++;

	screen_x = SCREEN_WIDTH - 1;
	screen_y = 0;
#if MACHINE_IBMPC
	screen_attribute = 0x0F; // white on black
#elif MACHINE_NECPC98
	screen_attribute = 0xE1; // white on black
#endif
	screen_putchar("/-\\|"[timer_tick & 3]);
}

static inline void keyboard_interrupt_handler(registers_t * registers)
{
	(void) registers;

	uint8_t scancode;
#if MACHINE_IBMPC || MACHINE_NECPC98
	scancode = inp(PORT_PS2_DATA);
#endif

	screen_x = SCREEN_WIDTH - 2;
	screen_y = 1;
#if MACHINE_IBMPC
	screen_attribute = 0x2F; // white on blue
#elif MACHINE_NECPC98
	screen_attribute = 0x25; // black on blue
#endif
	screen_puthex(scancode);

	keyboard_interrupt_process(scancode);
}

static inline void setup_tables(void)
{
#if OS286
	descriptor_set_segment(&gdt[SEL_KERNEL_CS / 8], 0, 0xFFFF, DESCRIPTOR_ACCESS_CODE | DESCRIPTOR_ACCESS_CPL0, DESCRIPTOR_FLAGS_16BIT);
	descriptor_set_segment(&gdt[SEL_KERNEL_SS / 8], 0, 0xFFFF, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL0, DESCRIPTOR_FLAGS_16BIT);
# if MACHINE_IBMPC
	if(*(char *)0x0449 != 0x07)
		descriptor_set_segment(&gdt[SEL_KERNEL_SCREEN / 8], 0x0B8000, 0xFFFF, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_16BIT);
	else
		descriptor_set_segment(&gdt[SEL_KERNEL_SCREEN / 8], 0x0B0000, 0xFFFF, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_16BIT);
# elif MACHINE_NECPC98
	descriptor_set_segment(&gdt[SEL_KERNEL_SCREEN / 8], 0x0A0000, 0xFFFF, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_16BIT);
# endif
	descriptor_set_segment(&gdt[SEL_KERNEL_SEGMENT1 / 8], 0, 0xFFFF, DESCRIPTOR_ACCESS_CODE | DESCRIPTOR_ACCESS_CPL0, DESCRIPTOR_FLAGS_16BIT);
	descriptor_set_segment(&gdt[SEL_KERNEL_SEGMENT2 / 8], 0, 0xFFFF, DESCRIPTOR_ACCESS_CODE | DESCRIPTOR_ACCESS_CPL0, DESCRIPTOR_FLAGS_16BIT);
	descriptor_set_segment(&gdt[SEL_TSS / 8], (size_t)&tss, sizeof tss - 1, DESCRIPTOR_ACCESS_TSS16 | DESCRIPTOR_ACCESS_CPL0, 0);
	descriptor_set_segment(&gdt[SEL_LDT / 8], (size_t)ldt, sizeof ldt - 1, DESCRIPTOR_ACCESS_LDT | DESCRIPTOR_ACCESS_CPL0, 0);

	descriptor_set_gate(&ldt[SEL_CALL7 / 8], SEL_KERNEL_CS, 0/* TODO */, DESCRIPTOR_ACCESS_CALLGATE16 | DESCRIPTOR_ACCESS_CPL3);
	descriptor_set_segment(&ldt[SEL_USER_CS / 8], 0, 0xFFFF, DESCRIPTOR_ACCESS_CODE | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_16BIT);
	descriptor_set_segment(&ldt[SEL_USER_SS / 8], 0, 0xFFFF, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_16BIT);
#elif OS386
	descriptor_set_segment(&gdt[SEL_KERNEL_CS / 8], 0, 0xFFFFFFFF, DESCRIPTOR_ACCESS_CODE | DESCRIPTOR_ACCESS_CPL0, DESCRIPTOR_FLAGS_32BIT);
	descriptor_set_segment(&gdt[SEL_KERNEL_SS / 8], 0, 0xFFFFFFFF, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL0, DESCRIPTOR_FLAGS_32BIT);
	descriptor_set_segment(&gdt[SEL_USER_CS / 8], 0, 0xFFFFFFFF, DESCRIPTOR_ACCESS_CODE | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_32BIT);
	descriptor_set_segment(&gdt[SEL_USER_SS / 8], 0, 0xFFFFFFFF, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_32BIT);
	descriptor_set_segment(&gdt[SEL_TSS / 8], (size_t)&tss, sizeof tss - 1, DESCRIPTOR_ACCESS_TSS32 | DESCRIPTOR_ACCESS_CPL0, 0);
	descriptor_set_segment(&gdt[SEL_LDT / 8], (size_t)ldt, sizeof ldt - 1, DESCRIPTOR_ACCESS_LDT | DESCRIPTOR_ACCESS_CPL0, 0);

	descriptor_set_gate(&ldt[SEL_CALL7 / 8], SEL_KERNEL_CS, 0/* TODO */, DESCRIPTOR_ACCESS_CALLGATE32 | DESCRIPTOR_ACCESS_CPL3);
#elif OS64
	descriptor_set_segment(&gdt[SEL_KERNEL_CS / 8], 0, 0, DESCRIPTOR_ACCESS_CODE | DESCRIPTOR_ACCESS_CPL0, DESCRIPTOR_FLAGS_64BIT);
	descriptor_set_segment(&gdt[SEL_KERNEL_SS / 8], 0, 0, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL0, 0);
	descriptor_set_segment(&gdt[SEL_USER_CS32 / 8], 0, 0, DESCRIPTOR_ACCESS_CODE | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_32BIT);
	descriptor_set_segment(&gdt[SEL_USER_SS32 / 8], 0, 0, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_32BIT);
	descriptor_set_segment(&gdt[SEL_USER_CS / 8], 0, 0, DESCRIPTOR_ACCESS_CODE | DESCRIPTOR_ACCESS_CPL3, DESCRIPTOR_FLAGS_64BIT);
	descriptor_set_segment(&gdt[SEL_USER_SS / 8], 0, 0, DESCRIPTOR_ACCESS_DATA | DESCRIPTOR_ACCESS_CPL3, 0);
	descriptor_set_segment64((descriptor64_t *)&gdt[SEL_TSS / 8], (size_t)&tss, sizeof tss, DESCRIPTOR_ACCESS_TSS64 | DESCRIPTOR_ACCESS_CPL0, 0);
	descriptor_set_segment64((descriptor64_t *)&gdt[SEL_LDT / 8], (size_t)ldt, sizeof ldt, DESCRIPTOR_ACCESS_LDT | DESCRIPTOR_ACCESS_CPL0, 0);

	descriptor_set_gate64((descriptor64_t *)&ldt[SEL_CALL7 / 8], SEL_KERNEL_CS, 0/* TODO */, DESCRIPTOR_ACCESS_CALLGATE64 | DESCRIPTOR_ACCESS_CPL3);
#endif

#if OS286 || OS386 || OS64
	load_gdt(gdt, sizeof gdt, SEL_KERNEL_CS, SEL_KERNEL_SS, SEL_KERNEL_SS);
	load_ldt(SEL_LDT);
#endif

#if MODE_REAL
# define DESCRIPTOR_ACCESS_INTGATE 0
# define KERNEL_SEGMENT 0
#elif OS286
# define DESCRIPTOR_ACCESS_INTGATE DESCRIPTOR_ACCESS_INTGATE16 | DESCRIPTOR_ACCESS_CPL3
# define KERNEL_SEGMENT SEL_KERNEL_CS
#elif OS386
# define DESCRIPTOR_ACCESS_INTGATE DESCRIPTOR_ACCESS_INTGATE32 | DESCRIPTOR_ACCESS_CPL3
# define KERNEL_SEGMENT SEL_KERNEL_CS
#elif OS64
# define DESCRIPTOR_ACCESS_INTGATE DESCRIPTOR_ACCESS_INTGATE64 | DESCRIPTOR_ACCESS_CPL3
# define KERNEL_SEGMENT SEL_KERNEL_CS
#endif

	set_interrupt(0x00, KERNEL_SEGMENT, isr0x00, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x01, KERNEL_SEGMENT, isr0x01, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x02, KERNEL_SEGMENT, isr0x02, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x03, KERNEL_SEGMENT, isr0x03, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x04, KERNEL_SEGMENT, isr0x04, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x05, KERNEL_SEGMENT, isr0x05, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x06, KERNEL_SEGMENT, isr0x06, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x07, KERNEL_SEGMENT, isr0x07, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x08, KERNEL_SEGMENT, isr0x08, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x09, KERNEL_SEGMENT, isr0x09, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x0A, KERNEL_SEGMENT, isr0x0A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x0B, KERNEL_SEGMENT, isr0x0B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x0C, KERNEL_SEGMENT, isr0x0C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x0D, KERNEL_SEGMENT, isr0x0D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x0E, KERNEL_SEGMENT, isr0x0E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x0F, KERNEL_SEGMENT, isr0x0F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x10, KERNEL_SEGMENT, isr0x10, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x11, KERNEL_SEGMENT, isr0x11, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x12, KERNEL_SEGMENT, isr0x12, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x13, KERNEL_SEGMENT, isr0x13, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x14, KERNEL_SEGMENT, isr0x14, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x15, KERNEL_SEGMENT, isr0x15, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x16, KERNEL_SEGMENT, isr0x16, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x17, KERNEL_SEGMENT, isr0x17, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x18, KERNEL_SEGMENT, isr0x18, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x19, KERNEL_SEGMENT, isr0x19, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x1A, KERNEL_SEGMENT, isr0x1A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x1B, KERNEL_SEGMENT, isr0x1B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x1C, KERNEL_SEGMENT, isr0x1C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x1D, KERNEL_SEGMENT, isr0x1D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x1E, KERNEL_SEGMENT, isr0x1E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x1F, KERNEL_SEGMENT, isr0x1F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x20, KERNEL_SEGMENT, isr0x20, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x21, KERNEL_SEGMENT, isr0x21, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x22, KERNEL_SEGMENT, isr0x22, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x23, KERNEL_SEGMENT, isr0x23, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x24, KERNEL_SEGMENT, isr0x24, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x25, KERNEL_SEGMENT, isr0x25, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x26, KERNEL_SEGMENT, isr0x26, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x27, KERNEL_SEGMENT, isr0x27, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x28, KERNEL_SEGMENT, isr0x28, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x29, KERNEL_SEGMENT, isr0x29, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x2A, KERNEL_SEGMENT, isr0x2A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x2B, KERNEL_SEGMENT, isr0x2B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x2C, KERNEL_SEGMENT, isr0x2C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x2D, KERNEL_SEGMENT, isr0x2D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x2E, KERNEL_SEGMENT, isr0x2E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x2F, KERNEL_SEGMENT, isr0x2F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x30, KERNEL_SEGMENT, isr0x30, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x31, KERNEL_SEGMENT, isr0x31, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x32, KERNEL_SEGMENT, isr0x32, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x33, KERNEL_SEGMENT, isr0x33, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x34, KERNEL_SEGMENT, isr0x34, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x35, KERNEL_SEGMENT, isr0x35, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x36, KERNEL_SEGMENT, isr0x36, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x37, KERNEL_SEGMENT, isr0x37, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x38, KERNEL_SEGMENT, isr0x38, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x39, KERNEL_SEGMENT, isr0x39, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x3A, KERNEL_SEGMENT, isr0x3A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x3B, KERNEL_SEGMENT, isr0x3B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x3C, KERNEL_SEGMENT, isr0x3C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x3D, KERNEL_SEGMENT, isr0x3D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x3E, KERNEL_SEGMENT, isr0x3E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x3F, KERNEL_SEGMENT, isr0x3F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x40, KERNEL_SEGMENT, isr0x40, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x41, KERNEL_SEGMENT, isr0x41, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x42, KERNEL_SEGMENT, isr0x42, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x43, KERNEL_SEGMENT, isr0x43, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x44, KERNEL_SEGMENT, isr0x44, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x45, KERNEL_SEGMENT, isr0x45, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x46, KERNEL_SEGMENT, isr0x46, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x47, KERNEL_SEGMENT, isr0x47, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x48, KERNEL_SEGMENT, isr0x48, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x49, KERNEL_SEGMENT, isr0x49, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x4A, KERNEL_SEGMENT, isr0x4A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x4B, KERNEL_SEGMENT, isr0x4B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x4C, KERNEL_SEGMENT, isr0x4C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x4D, KERNEL_SEGMENT, isr0x4D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x4E, KERNEL_SEGMENT, isr0x4E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x4F, KERNEL_SEGMENT, isr0x4F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x50, KERNEL_SEGMENT, isr0x50, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x51, KERNEL_SEGMENT, isr0x51, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x52, KERNEL_SEGMENT, isr0x52, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x53, KERNEL_SEGMENT, isr0x53, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x54, KERNEL_SEGMENT, isr0x54, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x55, KERNEL_SEGMENT, isr0x55, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x56, KERNEL_SEGMENT, isr0x56, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x57, KERNEL_SEGMENT, isr0x57, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x58, KERNEL_SEGMENT, isr0x58, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x59, KERNEL_SEGMENT, isr0x59, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x5A, KERNEL_SEGMENT, isr0x5A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x5B, KERNEL_SEGMENT, isr0x5B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x5C, KERNEL_SEGMENT, isr0x5C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x5D, KERNEL_SEGMENT, isr0x5D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x5E, KERNEL_SEGMENT, isr0x5E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x5F, KERNEL_SEGMENT, isr0x5F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x60, KERNEL_SEGMENT, isr0x60, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x61, KERNEL_SEGMENT, isr0x61, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x62, KERNEL_SEGMENT, isr0x62, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x63, KERNEL_SEGMENT, isr0x63, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x64, KERNEL_SEGMENT, isr0x64, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x65, KERNEL_SEGMENT, isr0x65, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x66, KERNEL_SEGMENT, isr0x66, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x67, KERNEL_SEGMENT, isr0x67, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x68, KERNEL_SEGMENT, isr0x68, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x69, KERNEL_SEGMENT, isr0x69, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x6A, KERNEL_SEGMENT, isr0x6A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x6B, KERNEL_SEGMENT, isr0x6B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x6C, KERNEL_SEGMENT, isr0x6C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x6D, KERNEL_SEGMENT, isr0x6D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x6E, KERNEL_SEGMENT, isr0x6E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x6F, KERNEL_SEGMENT, isr0x6F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x70, KERNEL_SEGMENT, isr0x70, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x71, KERNEL_SEGMENT, isr0x71, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x72, KERNEL_SEGMENT, isr0x72, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x73, KERNEL_SEGMENT, isr0x73, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x74, KERNEL_SEGMENT, isr0x74, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x75, KERNEL_SEGMENT, isr0x75, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x76, KERNEL_SEGMENT, isr0x76, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x77, KERNEL_SEGMENT, isr0x77, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x78, KERNEL_SEGMENT, isr0x78, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x79, KERNEL_SEGMENT, isr0x79, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x7A, KERNEL_SEGMENT, isr0x7A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x7B, KERNEL_SEGMENT, isr0x7B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x7C, KERNEL_SEGMENT, isr0x7C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x7D, KERNEL_SEGMENT, isr0x7D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x7E, KERNEL_SEGMENT, isr0x7E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x7F, KERNEL_SEGMENT, isr0x7F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x80, KERNEL_SEGMENT, isr0x80, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x81, KERNEL_SEGMENT, isr0x81, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x82, KERNEL_SEGMENT, isr0x82, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x83, KERNEL_SEGMENT, isr0x83, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x84, KERNEL_SEGMENT, isr0x84, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x85, KERNEL_SEGMENT, isr0x85, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x86, KERNEL_SEGMENT, isr0x86, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x87, KERNEL_SEGMENT, isr0x87, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x88, KERNEL_SEGMENT, isr0x88, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x89, KERNEL_SEGMENT, isr0x89, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x8A, KERNEL_SEGMENT, isr0x8A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x8B, KERNEL_SEGMENT, isr0x8B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x8C, KERNEL_SEGMENT, isr0x8C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x8D, KERNEL_SEGMENT, isr0x8D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x8E, KERNEL_SEGMENT, isr0x8E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x8F, KERNEL_SEGMENT, isr0x8F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x90, KERNEL_SEGMENT, isr0x90, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x91, KERNEL_SEGMENT, isr0x91, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x92, KERNEL_SEGMENT, isr0x92, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x93, KERNEL_SEGMENT, isr0x93, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x94, KERNEL_SEGMENT, isr0x94, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x95, KERNEL_SEGMENT, isr0x95, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x96, KERNEL_SEGMENT, isr0x96, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x97, KERNEL_SEGMENT, isr0x97, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x98, KERNEL_SEGMENT, isr0x98, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x99, KERNEL_SEGMENT, isr0x99, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x9A, KERNEL_SEGMENT, isr0x9A, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x9B, KERNEL_SEGMENT, isr0x9B, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x9C, KERNEL_SEGMENT, isr0x9C, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x9D, KERNEL_SEGMENT, isr0x9D, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x9E, KERNEL_SEGMENT, isr0x9E, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0x9F, KERNEL_SEGMENT, isr0x9F, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA0, KERNEL_SEGMENT, isr0xA0, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA1, KERNEL_SEGMENT, isr0xA1, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA2, KERNEL_SEGMENT, isr0xA2, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA3, KERNEL_SEGMENT, isr0xA3, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA4, KERNEL_SEGMENT, isr0xA4, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA5, KERNEL_SEGMENT, isr0xA5, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA6, KERNEL_SEGMENT, isr0xA6, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA7, KERNEL_SEGMENT, isr0xA7, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA8, KERNEL_SEGMENT, isr0xA8, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xA9, KERNEL_SEGMENT, isr0xA9, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xAA, KERNEL_SEGMENT, isr0xAA, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xAB, KERNEL_SEGMENT, isr0xAB, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xAC, KERNEL_SEGMENT, isr0xAC, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xAD, KERNEL_SEGMENT, isr0xAD, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xAE, KERNEL_SEGMENT, isr0xAE, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xAF, KERNEL_SEGMENT, isr0xAF, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB0, KERNEL_SEGMENT, isr0xB0, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB1, KERNEL_SEGMENT, isr0xB1, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB2, KERNEL_SEGMENT, isr0xB2, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB3, KERNEL_SEGMENT, isr0xB3, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB4, KERNEL_SEGMENT, isr0xB4, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB5, KERNEL_SEGMENT, isr0xB5, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB6, KERNEL_SEGMENT, isr0xB6, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB7, KERNEL_SEGMENT, isr0xB7, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB8, KERNEL_SEGMENT, isr0xB8, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xB9, KERNEL_SEGMENT, isr0xB9, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xBA, KERNEL_SEGMENT, isr0xBA, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xBB, KERNEL_SEGMENT, isr0xBB, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xBC, KERNEL_SEGMENT, isr0xBC, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xBD, KERNEL_SEGMENT, isr0xBD, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xBE, KERNEL_SEGMENT, isr0xBE, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xBF, KERNEL_SEGMENT, isr0xBF, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC0, KERNEL_SEGMENT, isr0xC0, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC1, KERNEL_SEGMENT, isr0xC1, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC2, KERNEL_SEGMENT, isr0xC2, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC3, KERNEL_SEGMENT, isr0xC3, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC4, KERNEL_SEGMENT, isr0xC4, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC5, KERNEL_SEGMENT, isr0xC5, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC6, KERNEL_SEGMENT, isr0xC6, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC7, KERNEL_SEGMENT, isr0xC7, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC8, KERNEL_SEGMENT, isr0xC8, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xC9, KERNEL_SEGMENT, isr0xC9, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xCA, KERNEL_SEGMENT, isr0xCA, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xCB, KERNEL_SEGMENT, isr0xCB, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xCC, KERNEL_SEGMENT, isr0xCC, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xCD, KERNEL_SEGMENT, isr0xCD, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xCE, KERNEL_SEGMENT, isr0xCE, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xCF, KERNEL_SEGMENT, isr0xCF, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD0, KERNEL_SEGMENT, isr0xD0, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD1, KERNEL_SEGMENT, isr0xD1, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD2, KERNEL_SEGMENT, isr0xD2, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD3, KERNEL_SEGMENT, isr0xD3, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD4, KERNEL_SEGMENT, isr0xD4, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD5, KERNEL_SEGMENT, isr0xD5, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD6, KERNEL_SEGMENT, isr0xD6, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD7, KERNEL_SEGMENT, isr0xD7, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD8, KERNEL_SEGMENT, isr0xD8, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xD9, KERNEL_SEGMENT, isr0xD9, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xDA, KERNEL_SEGMENT, isr0xDA, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xDB, KERNEL_SEGMENT, isr0xDB, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xDC, KERNEL_SEGMENT, isr0xDC, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xDD, KERNEL_SEGMENT, isr0xDD, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xDE, KERNEL_SEGMENT, isr0xDE, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xDF, KERNEL_SEGMENT, isr0xDF, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE0, KERNEL_SEGMENT, isr0xE0, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE1, KERNEL_SEGMENT, isr0xE1, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE2, KERNEL_SEGMENT, isr0xE2, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE3, KERNEL_SEGMENT, isr0xE3, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE4, KERNEL_SEGMENT, isr0xE4, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE5, KERNEL_SEGMENT, isr0xE5, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE6, KERNEL_SEGMENT, isr0xE6, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE7, KERNEL_SEGMENT, isr0xE7, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE8, KERNEL_SEGMENT, isr0xE8, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xE9, KERNEL_SEGMENT, isr0xE9, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xEA, KERNEL_SEGMENT, isr0xEA, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xEB, KERNEL_SEGMENT, isr0xEB, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xEC, KERNEL_SEGMENT, isr0xEC, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xED, KERNEL_SEGMENT, isr0xED, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xEE, KERNEL_SEGMENT, isr0xEE, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xEF, KERNEL_SEGMENT, isr0xEF, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF0, KERNEL_SEGMENT, isr0xF0, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF1, KERNEL_SEGMENT, isr0xF1, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF2, KERNEL_SEGMENT, isr0xF2, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF3, KERNEL_SEGMENT, isr0xF3, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF4, KERNEL_SEGMENT, isr0xF4, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF5, KERNEL_SEGMENT, isr0xF5, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF6, KERNEL_SEGMENT, isr0xF6, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF7, KERNEL_SEGMENT, isr0xF7, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF8, KERNEL_SEGMENT, isr0xF8, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xF9, KERNEL_SEGMENT, isr0xF9, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xFA, KERNEL_SEGMENT, isr0xFA, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xFB, KERNEL_SEGMENT, isr0xFB, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xFC, KERNEL_SEGMENT, isr0xFC, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xFD, KERNEL_SEGMENT, isr0xFD, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xFE, KERNEL_SEGMENT, isr0xFE, DESCRIPTOR_ACCESS_INTGATE);
	set_interrupt(0xFF, KERNEL_SEGMENT, isr0xFF, DESCRIPTOR_ACCESS_INTGATE);
#if !MODE_REAL
	load_idt(idt, sizeof idt);
#endif

#if MODE_REAL
	(void) system_stack; // suppress warning

#else
	//memset(&tss, 0, sizeof tss);

#if OS286
	tss.sp0 = (size_t)system_stack + sizeof system_stack;
	tss.ss0 = SEL_KERNEL_SS;
#elif OS386
	tss.esp0 = (size_t)system_stack + sizeof system_stack;
	tss.ss0 = SEL_KERNEL_SS;
	tss.cs = SEL_USER_CS|3;
	tss.ds = tss.es = tss.fs = tss.gs = SEL_USER_SS|3;
#elif OS64
	tss.rsp0 = (size_t)system_stack + sizeof system_stack;
#endif

	load_tss(SEL_TSS);
#endif

	i8259_setup(IRQ0, IRQ8);
	i8253_setup(20);
}

