
PLATFORM=mac
DISKFILE=disk.400

AS=m68k-elf-as
CC=m68k-elf-gcc

SECTOR_SIZE=512
SECTOR_SIZE_SHIFT=9
BOOT_BLOCK_SIZE=1024

ASFLAGS=-m68000 --defsym MACHINE_MACINTOSH=1 --defsym SECTOR_SIZE=$(SECTOR_SIZE) --defsym SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) --defsym BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -m68000
CFLAGS+= -O2
CFLAGS+= -Isrc/mac -DMACHINE_MACINTOSH=1 -DUSE_VGA_EMULATION=1
LDFLAGS=-ffreestanding -O2 -nostdlib -m68000 -lgcc -lgcc -Wl,--defsym=SECTOR_SIZE=$(SECTOR_SIZE) -Wl,--defsym=SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) -Wl,--defsym=BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)

DISKSIZE=400
#FORMATFLAGS=-f 400 -a
LDSCRIPT=src/mac/linker.ld
BOOT=src/mac/boot.s
FIXIMAGE=./ignore
#NEEDFAT=0

INCLUDES=src/mac/video.h src/atari/vgasim.c src/pc86/colors.h src/m68k.c src/system.h

include src/Makefile.template

