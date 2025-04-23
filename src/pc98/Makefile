
PLATFORM=pc98
DISKFILE=disk.img

AS=ia16-elf-as
CC=ia16-elf-gcc

ASFLAGS=--defsym MODE_REAL=1 --defsym CPU_8086=1 --defsym BITS16=1 --defsym MACHINE_NECPC98=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -fno-delete-null-pointer-checks
#CFLAGS+= -O2
CFLAGS+= -Isrc/pc98 -DMODE_REAL=1 -DCPU_8086=1 -DMACHINE_NECPC98=1
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc98/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=./ignore

include src/Makefile.template

