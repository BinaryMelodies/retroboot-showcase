
PLATFORM=x68000
DISKFILE=disk.xdf

AS=m68k-elf-as
CC=m68k-elf-gcc

ASFLAGS=--defsym OS68000=1 --defsym X68000=1 --defsym SECTOR_SIZE=1024
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -m68000
CFLAGS+= -O2
CFLAGS+= -Isrc/x68000 -DOS68000=1 -DX68000=1 -DUSE_VGA_EMULATION=1
LDFLAGS=-ffreestanding -O2 -nostdlib -m68000 -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 720 -M 1024
LDSCRIPT=src/x68000/linker.ld
BOOT=src/x68000/boot.s
FIXIMAGE=./ignore

include src/Makefile.template

