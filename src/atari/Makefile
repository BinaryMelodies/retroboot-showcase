
PLATFORM=atari
DISKFILE=disk.st

AS=m68k-elf-as
CC=m68k-elf-gcc

SECTOR_SIZE=512
SECTOR_SIZE_SHIFT=9
BOOT_BLOCK_SIZE=512

ASFLAGS=-m68000 --defsym MACHINE_ATARI=1 --defsym SECTOR_SIZE=$(SECTOR_SIZE) --defsym SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) --defsym BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -m68000 -mpcrel
CFLAGS+= -O2
CFLAGS+= -Isrc/atari -DMACHINE_ATARI=1 -DUSE_VGA_EMULATION=1
LDFLAGS=-ffreestanding -O2 -nostdlib -m68000 -mpcrel -lgcc -Wl,--defsym=SECTOR_SIZE=$(SECTOR_SIZE) -Wl,--defsym=SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) -Wl,--defsym=BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)

DISKSIZE=1440
FORMATFLAGS=-f 1440 -a
LDSCRIPT=src/atari/linker.ld
BOOT=src/atari/boot.s
FIXIMAGE=python3 makeboot.py -atari
NEEDFAT=1

INCLUDES=src/atari/video.h src/atari/vgasim.c src/pc86/colors.h src/m68k.c src/system.h src/pc86/scancode.c src/atari/keyboard.c src/pc86/keyproc.c src/atari/timer.c

include src/Makefile.template

