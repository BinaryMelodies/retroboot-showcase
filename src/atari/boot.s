
	.extern	image_size
	.extern	memory_size
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
	lea	_start(%pc), %sp

	# first, initialize device
	# device
	move.w	(0x0446).w, -(%sp)
	move.w	#7, -(%sp)
	trap	#13
	addq.l	#4, %sp

	# allocate memory for image, ignoring sector 1
	move.l	#memory_size - 512, -(%sp)
	move.w	#0x48, -(%sp)
	trap	#1
	addq.l	#6, %sp

	tst.l	%d0
0:
	# if allocation failed, enter infinite loop
	beq	0b

	# set up stack
	move.l	%d0, %a0
	add.l	#stack_top - 512, %a0
	move.l	%a0, %sp

	# store buffer address in memory
	move.l	%d0, -(%sp)

	# load sectors into memory, from sector 1
	# device
	move.w	(0x0446).w, -(%sp)
	# sector
	move.w	#1, -(%sp)
	# count
	move.w	#sector_count - 1, -(%sp)
	# buffer
	move.l	%d0, %d1
	move.l	%d1, -(%sp)
	# flags (0)
	clr.w	-(%sp)
	move.w	#4, -(%sp)
	trap	#13
	lea	14(%sp), %sp

	# jump to same location in memory buffer
	add.l	#2f - 512, (%sp)
	rts

	.section	.text
2:
	# clear out bss
	lea	bss_start(%pc), %a0
	move.l	#bss_size, %d0
	lsr.l	#2, %d0
3:
	clr.l	(%a0)+
	dbf	%d1, 3b

	# simulate a bsr kmain
	lea	4f(%pc), %a0
	move.l	%a0, -(%sp)
	lea	kmain(%pc), %a0
	jmp	(%a0)
4:
	bras	4b

	.section	stack, "aw", @nobits
	.skip	0x200
stack_top:

