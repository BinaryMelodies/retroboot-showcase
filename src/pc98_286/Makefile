
PLATFORM=pc98_286
DISKFILE=disk.img

AS=ia16-elf-as
CC=ia16-elf-gcc

SECTOR_SIZE=512
SECTOR_SIZE_SHIFT=9
BOOT_BLOCK_SIZE=512

ASFLAGS=--defsym MODE_PROTECTED=1 --defsym BITS16=1 --defsym MACHINE_NECPC98=1 --defsym SECTOR_SIZE=$(SECTOR_SIZE) --defsym SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) --defsym BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -march=i80286 -mprotected-mode
#CFLAGS+= -O2
CFLAGS+= -Isrc/pc98 -DMODE_PROTECTED=1 -DMACHINE_NECPC98=1 
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc -Wl,--defsym=SECTOR_SIZE=$(SECTOR_SIZE) -Wl,--defsym=SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) -Wl,--defsym=BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc98/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=./ignore

INCLUDES=src/pc98/video.h src/x86.c src/i8259.c src/i8253.c src/i86.h src/system.h src/pc98/scancode.c src/pc98/keyboard.c src/pc86/keyproc.c src/pc86/timer.c

include src/Makefile.template

