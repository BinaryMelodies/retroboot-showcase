
	.extern	image_size
	.extern	memory_size
	.extern	sector_count

	.section	boot, "ax", @progbits
	.global _start

start:
	.ascii	"LK"
	bras	_start
	.long	0x4418

.macro	_Read
	# read immediately
	.word	0xA002 | (1 << 9)
.endm

_start:
	# Load IO Parameter Block address
	move.l	%sp, %a0
	# Check ROM version number
	.equ	ROMBase, 0x02ae
	move.l	ROMBase, %a1
	cmp.b	#0x69, 9(%a1)
	ble	1f
	# Not needed for early Macs (128K)
	addq.l	#4, %a0
1:
	.equ	ioBuffer, 0x20
	movel	#start + 1024, %a1
	movel	%a1, ioBuffer(%a0)
	.equ	ioReqCount, 0x24
	movel	#image_size - 1024, ioReqCount(%a0)
	.equ	ioPosOffset, 0x2E
	# Must be a multiple of 512
	movel	#1024, ioPosOffset(%a0)
	_Read
2:
	bne	2b

	jmp	(3f)

	.section	.text
3:
	move.l	#stack_top, %sp
	# Clear out bss
	move.l	#bss_start, %a0
	move.l	#bss_size, %d0
	lsr.l	#2, %d0
4:
	clr.l	(%a0)+
	dbf	%d0, 4b
	bsr	kmain

5:
	nop
	bras	5b

	.section	stack, "aw", @nobits
	.skip	0x200
stack_top:

