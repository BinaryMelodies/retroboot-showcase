
PLATFORM=atari
DISKFILE=disk.st

AS=m68k-elf-as
CC=m68k-elf-gcc

ASFLAGS=-m68000 --defsym MACHINE_ATARI=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -m68000 -mpcrel
CFLAGS+= -O2
CFLAGS+= -Isrc/atari -DMACHINE_ATARI=1 -DUSE_VGA_EMULATION=1
LDFLAGS=-ffreestanding -O2 -nostdlib -m68000 -mpcrel -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 1440 -a
LDSCRIPT=src/atari/linker.ld
BOOT=src/atari/boot.s
FIXIMAGE=python3 makeboot.py -atari
NEEDFAT=1

include src/Makefile.template

