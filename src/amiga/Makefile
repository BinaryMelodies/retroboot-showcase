
PLATFORM=amiga
DISKFILE=disk.adf

AS=m68k-elf-as
CC=m68k-elf-gcc

SECTOR_SIZE=512
SECTOR_SIZE_SHIFT=9
BOOT_BLOCK_SIZE=1024

ASFLAGS=-m68000 --defsym MACHINE_AMIGA=1 --defsym SECTOR_SIZE=$(SECTOR_SIZE) --defsym SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) --defsym BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -m68000
CFLAGS+= -O2
CFLAGS+= -Isrc/amiga -DMACHINE_AMIGA=1 -DUSE_VGA_EMULATION=1
LDFLAGS=-ffreestanding -O2 -nostdlib -m68000 -lgcc -Wl,--defsym=SECTOR_SIZE=$(SECTOR_SIZE) -Wl,--defsym=SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) -Wl,--defsym=BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)

DISKSIZE=880
#FORMATFLAGS=-f 880 -a
LDSCRIPT=src/amiga/linker.ld
BOOT=src/amiga/boot.s
FIXIMAGE=python3 makeboot.py -amiga
#NEEDFAT=0

INCLUDES=src/amiga/amiga.h src/amiga/video.h src/atari/vgasim.c src/pc86/colors.h src/m68k.c src/system.h src/pc86/scancode.c src/amiga/keyboard.c src/pc86/keyproc.c src/amiga/timer.c

include src/Makefile.template

