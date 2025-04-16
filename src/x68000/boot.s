
	.extern	image_size
	.extern	sector_count

	.section	boot, "ax", @progbits
	.global _start

_start:
	# skip over BIOS parameter block
	bras	1f

	.rept	0x3e - 2
	# skip over BIOS parameter block
	.byte	0
	.endr

1:
	# set up stack
	move.l	#stack_top, %sp

	# get boot information
	move.l	#0xFFFFFF8E, %d0
	trap	#15
	lsl.l	#8, %d0
	move.l	%d0, %d7

	# sector (bytes: sector (1 to 8), side (0 or 1), track (0 to 76))
	# high byte: 128 times power of 2 (only 1024 byte sectors supported)
	move.l	#0x03000002, %d2
	# count
	move.l	#image_size - 1024, %d3
	# buffer
	lea	_start + 1024(%pc), %a1
	# PDA (boot unit number) & mode
	move.w	#0x70, %d1
	or.w	%d7, %d1
	move.w	#0x46, %d0
	trap	#15

2:
	# clear out bss
	move.l	#bss_start, %a0
	move.l	#bss_size - 4, %d0
	lsr.l	#2, %d0
3:
	clr.l	(%a0)+
	dbf	%d1, 3b

	bsr	kmain
1:
	bras	1b

	.section	stack, "aw", @nobits
	.skip	0x200
stack_top:

