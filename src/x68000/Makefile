
PLATFORM=x68000
DISKFILE=disk.xdf

AS=m68k-elf-as
CC=m68k-elf-gcc

SECTOR_SIZE=1024
SECTOR_SIZE_SHIFT=10
BOOT_BLOCK_SIZE=1024

ASFLAGS=-m68000 --defsym MACHINE_X68000=1 --defsym SECTOR_SIZE=1024 --defsym SECTOR_SIZE=$(SECTOR_SIZE) --defsym SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) --defsym BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -m68000
CFLAGS+= -O2
CFLAGS+= -Isrc/x68000 -DMACHINE_X68000=1 -DUSE_VGA_EMULATION=1
LDFLAGS=-ffreestanding -O2 -nostdlib -m68000 -lgcc -Wl,--defsym=SECTOR_SIZE=$(SECTOR_SIZE) -Wl,--defsym=SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) -Wl,--defsym=BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)

DISKSIZE=1440
FORMATFLAGS=-f 720 -M 1024
LDSCRIPT=src/x68000/linker.ld
BOOT=src/x68000/boot.s
FIXIMAGE=./ignore

INCLUDES=src/x68000/video.h src/atari/vgasim.c src/pc86/colors.h src/m68k.c src/system.h src/pc86/scancode.c src/x68000/keyboard.c src/pc86/keyproc.c

include src/Makefile.template

