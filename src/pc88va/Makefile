
PLATFORM=pc88va
DISKFILE=disk.img

AS=ia16-elf-as
CC=ia16-elf-gcc

SECTOR_SIZE=1024
SECTOR_SIZE_SHIFT=10
BOOT_BLOCK_SIZE=1024

ASFLAGS=--defsym MODE_REAL=1 --defsym BITS16=1 --defsym MACHINE_NECPC88VA=1 --defsym SECTOR_SIZE=$(SECTOR_SIZE) --defsym SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) --defsym BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -fno-delete-null-pointer-checks
#CFLAGS+= -O2
CFLAGS+= -Isrc/pc88va -DMODE_REAL=1 -DMACHINE_NECPC88VA=1
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc -Wl,--defsym=SECTOR_SIZE=$(SECTOR_SIZE) -Wl,--defsym=SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT)

DISKSIZE=1232
FORMATFLAGS=-h 2 -t 77 -s 8
LDSCRIPT=src/pc88va/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=./ignore

#INCLUDES=src/pc88va/video.h src/x86.c src/i8259.c src/i8253.c src/i86.h src/system.h src/pc88va/scancode.c src/pc88va/keyboard.c src/pc86/keyproc.c src/pc86/timer.c
INCLUDES=src/pc88va/video.h src/x86.c src/i8259.c src/i8253.c src/i86.h src/system.h

include src/Makefile.template

