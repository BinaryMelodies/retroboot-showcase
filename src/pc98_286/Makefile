
PLATFORM=pc98_286
DISKFILE=disk.img

AS=ia16-elf-as
CC=ia16-elf-gcc

ASFLAGS=--defsym MODE_PROTECTED=1 --defsym BITS16=1 --defsym MACHINE_NECPC98=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -march=i80286 -mprotected-mode
#CFLAGS+= -O2
CFLAGS+= -Isrc/pc98 -DMODE_PROTECTED=1 -DMACHINE_NECPC98=1 
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc98/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=./ignore

INCLUDES=src/pc98/video.h src/x86.c src/i8259.c src/i8253.c src/keyboard.c src/i86.h src/system.h

include src/Makefile.template

