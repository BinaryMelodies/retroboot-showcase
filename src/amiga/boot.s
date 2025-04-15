
	.extern	image_start
	.extern	image_size
	.section	boot, "ax", @progbits
	.global _start

	.equ	BLOCK_SIZE, 1024
	.equ	CMD_READ, 2
	.equ	io_Command, 32
	.equ	io_Length, 36
	.equ	io_Data, 40
	.equ	io_Offset, 44
	.equ	DoIO, -456
	.equ	SuperState, -150

	.ascii	"DOS\0"
	# checksum
	.long	0
	# number of 1024 byte blocks
	.long	880

_start:
# A6: SysBase
# A1: IoStdReq

	# first two 512 byte sectors are already loaded
	move.l	#image_start + BLOCK_SIZE, %a2
	move.l	#CMD_READ, io_Command(%a1)
	# must be a multiple of 512
	move.l	#image_size - BLOCK_SIZE, io_Length(%a1)
	# must be word aligned
	move.l	%a2, io_Data(%a1)
	# must be a multiple of 512
	move.l	#BLOCK_SIZE, io_Offset(%a1)
	jsr	DoIO(%a6)

	tst.l	%d0
1:
	bne	1b

	jsr	SuperState(%a6)

	jmp	(2f).l

	.section	.text
2:
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
	bras	4b

	.section	stack, "aw", @nobits
	.skip	0x200
stack_top:

