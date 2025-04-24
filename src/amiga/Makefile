
PLATFORM=amiga
DISKFILE=disk.adf

AS=m68k-elf-as
CC=m68k-elf-gcc

ASFLAGS=-m68000 --defsym MACHINE_AMIGA=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -m68000
CFLAGS+= -O2
CFLAGS+= -Isrc/amiga -DMACHINE_AMIGA=1 -DUSE_VGA_EMULATION=1
LDFLAGS=-ffreestanding -O2 -nostdlib -m68000 -lgcc

DISKSIZE=880
#FORMATFLAGS=-f 880 -a
LDSCRIPT=src/amiga/linker.ld
BOOT=src/amiga/boot.s
FIXIMAGE=python3 makeboot.py -amiga
#NEEDFAT=0

INCLUDES=src/amiga/video.h src/atari/vgasim.c src/pc86/colors.h src/m68k.c src/system.h

include src/Makefile.template

