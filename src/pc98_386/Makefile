
PLATFORM=pc98_386
DISKFILE=disk.img

AS=i686-elf-as
CC=i686-elf-gcc

ASFLAGS=--defsym OS386=1 --defsym NECPC98=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -march=i386
CFLAGS+= -O2
CFLAGS+= -Isrc/pc98 -DOS386=1 -DNECPC98=1 
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc98/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=./ignore

include src/Makefile.template

