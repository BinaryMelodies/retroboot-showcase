
PLATFORM=mac
DISKFILE=disk.400

AS=m68k-elf-as
CC=m68k-elf-gcc

ASFLAGS=-m68000 --defsym OS68000=1 --defsym MACINTOSH=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -m68000
#CFLAGS+= -O2
CFLAGS+= -Isrc/mac -DOS68000=1 -DMACINTOSH=1 -DUSE_VGA_EMULATION=1
LDFLAGS=-ffreestanding -O2 -nostdlib -m68000 -lgcc

DISKSIZE=400
#FORMATFLAGS=-f 400 -a
LDSCRIPT=src/mac/linker.ld
BOOT=src/mac/boot.s
FIXIMAGE=./ignore
#NEEDFAT=0

include src/Makefile.template

