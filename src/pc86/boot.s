
.ifdef	MODE_REAL
	.equ	MODE_REAL, 1
.else
	.equ	MODE_REAL, 0
.endif

.ifdef	MODE_PROTECTED
	.equ	MODE_PROTECTED, 1
.else
	.equ	MODE_PROTECTED, 0
.endif

.ifdef	BITS16
	.equ	BITS16, 1
.else
	.equ	BITS16, 0
.endif

.ifdef	BITS32
	.equ	BITS32, 1
.else
	.equ	BITS32, 0
.endif

.ifdef	BITS64
	.equ	BITS64, 1
.else
	.equ	BITS64, 0
.endif

.ifdef	MACHINE_IBMPC
	.equ	MACHINE_IBMPC, 1
.else
	.equ	MACHINE_IBMPC, 0
.endif

.ifdef	MACHINE_NECPC98
	.equ	MACHINE_NECPC98, 1
.else
	.equ	MACHINE_NECPC98, 0
.endif

.ifdef	MACHINE_NECPC88VA
	.equ	MACHINE_NECPC88VA, 1
.else
	.equ	MACHINE_NECPC88VA, 0
.endif

.macro	_descriptor	base, limit, attribute
	# Segment descriptors use an unusual layout
	.word	(\limit) & 0xFFFF
	.word	(\base) & 0xFFFF
	.word	(((\base) >> 16) & 0x00FF) | (((\attribute) & 0x00FF) << 8)
	.word	(((\limit) >> 16) & 0x000F) | (((\attribute) & 0xF000) >> 8) | (((\base) >> 16) & 0xFF00)
.endm

.macro	descriptor	base, limit, attributes
	# A more convenient way to handle segments that are larger than 64KiB
	.if	((\attributes) & 0x8000) != 0 || (\limit) > 0xFFFF
		_descriptor	\base, (\limit) >> 12, (\attributes) | 0x8000
	.else
		_descriptor	\base, \limit, (\attributes) & 0x7FFF
	.endif
.endm

.equ	DESC_CODE,  0x009A
.equ	DESC_DATA,  0x0092
.equ	DESC_CPL0,  0x0000
.equ	DESC_CPL3,  0x0060
.equ	DESC_16BIT, 0x0000
.equ	DESC_32BIT, 0x4000
.equ	DESC_64BIT, 0x2000 # only needed for the code segment

	.extern	sector_count
	.extern	kmain
	.extern	bss_start
	.extern	bss_size
	.extern	image_size

	.section	boot, "ax", @progbits
	.global	_start
	.code16

	# IBM PC: Start address at 0x07C00
	# NEC PC-98: Start address at 0x1FE00 or 0x1FC00
	# NEC PC-88 VA: Start address at 0xC0000
_start:
	# Skip over BIOS parameter block
	jmp	1f

	.rept	0x3e - 2
	# Skip over BIOS parameter block
	.byte	0
	.endr

1:
	# Set up temporary stack at 0x1000
	xorw	%ax, %ax
	movw	%ax, %ss
	movw	$0x1000, %sp

	# Move boot sector to 0:0x5000
.if MACHINE_IBMPC
	# From 0x7C00
	xorw	%ax, %ax
	movw	%ax, %ds
	movw	$0x7C00, %si
	movw	%ax, %es
.endif

.if MACHINE_NECPC98 || MACHINE_NECPC88VA
	movw	%cs, %ax
	movw	%ax, %ds
	xorw	%si, %si
	movw	%si, %es
.endif

	movw	$0x5000, %di
	movw	$0x100, %cx
	rep movsw
	ljmp	$0, $2f
2:
	# Make DS = CS for easier access
	movw	%cs, %ax
	movw	%ax, %ds
	movw	%ax, %es

.if MODE_PROTECTED
	# Check for at least 286 support, otherwise no protected mode is available
	pushfw
	popw	%ax
	andw	$0x0FFF, %ax
	pushw	%ax
	popfw
	pushfw
	popw	%ax
	movw	%ax, %cx
	and	$0xF000, %ax
	cmp	$0xF000, %ax
	je	error_old_cpu

.if BITS32
	# Check for at least 386 support, otherwise no 32-bit mode or registers are available
	or	$0xF000, %cx
	pushw	%cx
	popfw
	pushfw
	popw	%ax
	and	$0xF000, %ax
	jz	error_old_cpu

	xorw	%ax, %ax
	pushw	%ax
	popf

.if BITS64
	# Check for CPUID support
	pushfl
	popl	%eax
	movl	%eax, %ecx
	xorl	$0x00200000, %eax
	pushl	%eax
	popfl
	pushfl
	popl	%eax
	xorl	%eax, %ecx
	jz	error_old_cpu

	# Check for long mode support
	pushw	%dx
	movl	$0x80000001, %eax
	cpuid
	test	$0x20000000, %edx
	popw	%dx
	jz	error_old_cpu
.endif
.endif
.endif

read_sectors:
.if MACHINE_IBMPC
	# ES:BX contains the destination buffer, 0:0x5000 + BOOT_BLOCK_SIZE
	movw	$_start + BOOT_BLOCK_SIZE, %bx
	# AH contains the BIOS function number 0x02, AL contains the sector count
	movw	$0x0200 + sector_count - 1, %ax
	# Access is according to cylinder:head:sector
	# CH contains the cylinder number, CL the 1 based sector number
	# First sector is already in memory, so we start from 2
	movw	$0x0002, %cx
	# DH contains the head number
	movb	$0x00, %dh
	# DL contains the drive number, the value is received by the boot sector code
	int	$0x13

	jnc	.success
	# There was a failure, reset the disk system and try again
	movb	$0, %al
	int	$0x13
	jmp	read_sectors
.success:
	cmpb	$sector_count - 1, %al
	je	.complete

	# TODO: read the remaining sectors

.complete:
.endif
.if MACHINE_NECPC98
	# ES:BP contains the destination buffer, 0:0x5000 + BOOT_BLOCK_SIZE
	movw	$_start + BOOT_BLOCK_SIZE, %bp
	# BX contains the read size
	movw	$image_size - BOOT_BLOCK_SIZE, %bx
	# Access is according to cylinder:head:sector
	# DH contains the head number, DL the 1 based sector number
	# First sector is already in memory, so we start from 2
	movw	$0x0002, %dx
	# CH contains the sector size (0: 128, 1: 256, 2: 512, 3: 1024), CL contains the cylinder number in the low 6 bits
	movb	$SECTOR_SIZE_SHIFT - 7, %ch
	movb	$0, %cl
	# AH contains the BIOS function number 0x06, AL contains the device type, 0x30
	movw	$0x0630, %ax
	int	$0x1B
.endif
.if MACHINE_NECPC88VA
	# ES:BP contains the destination buffer, 0:0x5000 + BOOT_BLOCK_SIZE
	movw	$_start + BOOT_BLOCK_SIZE, %bp
	# AL contains the sector count, AH contains the BIOS function number 0x01
	movw	$0x0100 + sector_count - 1, %ax
	# Access is according to cylinder:head:sector
	# DH contains the 1 based sector number number, DL the sector size (0: 128, 1: 256, 2: 512, 3: 1024)
	# First sector is already in memory, so we start from 2
	movw	$0x0200 | (SECTOR_SIZE_SHIFT - 7), %dx
	# CH contains the drive number, CL the track number (cylinder * 2 + head)
	movw	$0x0000, %cx
	int	$0x80
.endif

.if !MODE_REAL
	# Enable the A20 line
.if MACHINE_IBMPC
	inb	$0x92, %al
	orb	$0x02, %al
	outb	%al, $0x92
.endif
.if MACHINE_NECPC98
	movb	$0x00, %al
	outb	%al, $0xF2
.endif
.endif

.if MODE_PROTECTED && BITS16
.if MACHINE_IBMPC
	# Check for MDA screen, modify GDT selector base 0x18 to 0xB0000
	cmpb	$0x07, (0x0449)
	jne	1f
	movw	$0, (gdt + 0x1A)
1:
.endif
	# Turn off interrupts while setting up protected mode
	cli
	# The GDTR is needed to set up protected mode segments
	lgdt	gdtr
	# Enter protected mode in a 286 compatible way, there's no CR0, only the MSW which contains the low bits
	smsw	%ax
	orb	$1, %al
	lmsw	%ax
	# Load CS with a protected mode descriptor
	ljmp	$0x08, $pm_start
.endif
.if BITS32
	# Turn off interrupts while setting up protected mode
	cli
	lgdt	gdtr
	# Enter protected mode
	movl	%cr0, %eax
	orb	$1, %al
	movl	%eax, %cr0
	# Load CS with a protected mode descriptor
	jmp	$0x08, $pm_start
.endif
.if BITS64
	# Entering long mode directly from real mode, based on https://wiki.osdev.org/Entering_Long_Mode_Directly
	# Set up identity paging tables from 0x1000
	movw	$0x1000, %di
	pushw	%di
	movl	$0x1000, %ecx
	xorl	%eax, %eax
	movw	%ax, %es
	cld
	rep stosl
	popw	%di

	pushw	%di
	movw	$3, %cx
.setup_directories:
	lea	0x1000(%di), %eax
	orw	$7, %ax
	movl	%eax, (%di)
	addw	$0x1000, %di
	loop	.setup_directories

	movl	$7, %eax
	movl	$0x200, %ecx
.setup_id_paging:
	movl	%eax, (%di)
	addl	$0x1000, %eax
	addw	$8, %di
	loop	.setup_id_paging

	xorl	%edi, %edi
	popw	%di

	# Turn off interrupts while setting up protected mode
	cli
	# The GDTR is needed to set up protected mode segments
	lgdt	gdtr
	# Enable paging extensions and PAE
	movl	$0x000000a0, %eax
	movl	%eax, %cr4
	# Load page tables
	movl	%edi, %cr3
	# Enable long mode
	movl	$0xC0000080, %ecx
	rdmsr
	orw	$0x0100, %ax
	wrmsr
	# Enter protected mode and enable paging
	movl	%cr0, %eax
	orl	$0x80000001, %eax
	movl	%eax, %cr0
	# Load CS with a protected mode descriptor
	ljmp	$0x08, $pm_start
.endif

.if MODE_REAL
	# Set up stack
	movw	$stack_top, %sp

	# Clear bss
	movw	$bss_start, %di
	movw	$bss_size, %cx
	movb	$0, %al
	rep	stosb
.endif
.if MODE_PROTECTED && BITS16
pm_start:
 	# Now we are in protected mode
	# Set up stack and other segment registers with protected mode selectors
	movw	$stack_top, %sp
	movw	$0x10, %ax
	movw	%ax, %ss
	movw	%ax, %ds
	movw	%ax, %es

	# Clear bss
	movw	$bss_start, %di
	movw	$bss_size, %cx
	movb	$0, %al
	rep	stosb
.endif
.if MODE_PROTECTED && BITS32
	.code32
pm_start:
	# Now we are in 32-bit protected mode
	# Set up stack and other segment registers with protected mode selectors
	movl	$stack_top, %esp
	movw	$0x10, %ax
	movw	%ax, %ss
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs

	# Clear bss
	movl	$bss_start, %edi
	movl	$bss_size, %ecx
	movb	$0, %al
	rep	stosb
.endif
.if BITS64
	.code64
pm_start:
	# Now we are in 64-bit protected mode
	# Set up stack and other segment registers with protected mode selectors
	movq	$stack_top, %rsp
	movw	$0x10, %ax
	movw	%ax, %ss
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs

	# Clear bss
	movq	$bss_start, %rdi
	movq	$bss_size, %rcx
	movb	$0, %al
	rep	stosb
.endif

	jmp	kmain
0:
	hlt
	jmp	0b

.if !MODE_REAL
	.code16
error_old_cpu:
.if MACHINE_IBMPC
	# DS=0
	cmpb	$0x07, 0x449
	jne	.cga
.mda:
	movw	$0xB000, %di
	movb	$0x0F, %ah
	jmp	1f
.cga:
	movw	$0xB800, %di
	movb	$0x4E, %ah
1:
	movw	%di, %es
	xorw	%di, %di
	movw	$message_old_cpu, %si
	movw	$length_message_old_cpu, %cx
2:
	lodsb
	stosw
	loop	2b
.endif
.if MACHINE_NECPC98
	movw	$0xA000, %di
	movw	%di, %es
	xorw	%di, %di
	movw	$message_old_cpu, %si
	movw	$length_message_old_cpu, %cx
	movb	$0x00, %ah
1:
	lodsb
	stosw
	loop	1b

	# set attributes
	movw	$length_message_old_cpu, %cx
	movw	$0x2000, %di
	movw	$0x0041, %ax
2:
	stosw
	loop	2b
.endif
0:
	hlt
	jmp	0b
.endif

.if MODE_PROTECTED && BITS16
message_old_cpu:
	.ascii	"Intel 80286 or newer expected"
.endif
.if BITS32
message_old_cpu:
	.ascii	"Intel 80386 or newer expected"
.endif
.if BITS64
message_old_cpu:
	.ascii	"AMD64 or Intel64 expected"
.endif
.if !MODE_REAL
	.equ	length_message_old_cpu, . - message_old_cpu
.endif

.if MODE_PROTECTED && BITS16
	.align	4, 0
gdtr:
	.word	gdt_end - gdt - 1
	.long	gdt

	.align	8, 0
gdt:
	.word	0, 0, 0, 0
	descriptor	0, 0xFFFF, DESC_CODE | DESC_16BIT
	descriptor	0, 0xFFFF, DESC_DATA | DESC_16BIT
	# A separate selector to access the VGA text buffer
.if MACHINE_IBMPC
	descriptor	0xB8000, 0xFFFF, DESC_DATA | DESC_16BIT
.endif
.if MACHINE_NECPC98
	descriptor	0xA0000, 0xFFFF, DESC_DATA | DESC_16BIT
.endif
gdt_end:
.endif
.if MODE_PROTECTED && BITS32
	.align	4, 0
gdtr:
	.word	gdt_end - gdt - 1
	.long	gdt

	.align	8, 0
gdt:
	.word	0, 0, 0, 0
	descriptor	0, 0xFFFFFFFF, DESC_CODE | DESC_32BIT
	descriptor	0, 0xFFFFFFFF, DESC_DATA | DESC_32BIT
gdt_end:
.endif
.if BITS64
	.align	4, 0
gdtr:
	.word	gdt_end - gdt - 1
	.long	gdt

	.align	8, 0
gdt:
	.word	0, 0, 0, 0
	# 64-bit descriptors don't need a limit, and only the code segment needs to be set to 64-bit
	descriptor	0, 0, DESC_CODE | DESC_64BIT
	descriptor	0, 0, DESC_DATA
gdt_end:
.endif

	.section	.data
	# A data section is required for the linker script

	.section	stack, "aw", @nobits
	.skip	0x200
stack_top:

