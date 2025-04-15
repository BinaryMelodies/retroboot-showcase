
	.global _start

start:
	.ascii	"LK"
	bras	_start
	.long	0x4418

.macro	_Read
	# read immediately
	.word	0xA002 | (1 << 9)
.endm

.macro	_SysError
	.word	0xA9C9
.endm

_start:
# Load IO Parameter Block address
	move.l	%sp, %a0
	# check ROM version number
	.equ	ROMBase, 0x02ae
	move.l	ROMBase, %a1
	cmp.b	#0x69, 9(%a1)
	ble	1f
	# not needed for early Macs (128K)
	addq.l	#4, %a0
1:
	.equ	ioBuffer, 0x20
	lea	start + 1024(%pc), %a1
	movel	%a1, ioBuffer(%a0)
	.equ	ioReqCount, 0x24
	movel	#image_size - 1024, ioReqCount(%a0)
	.equ	ioPosOffset, 0x2E
	# must be a multiple of 512
	movel	#1024, ioPosOffset(%a0)
	_Read
2:
	bne	2b

	move.l	#stack_top, %sp
	# clear out bss
	movel.	#bss_start, %a0
	move.l	#bss_size, %d0
	lsr.l	#2, %d0
3:
	clr.l	(%a0)+
	dbf	%d1, 3b
	bsr	kmain

4:
	nop
	bras	4b

	.section	stack, "aw", @nobits
	.skip	0x200
stack_top:

