
PLATFORM=pc386
DISKFILE=disk.img

AS=i686-elf-as
CC=i686-elf-gcc

ASFLAGS=--defsym OS386=1 --defsym IBMPC=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -march=i386
CFLAGS+= -O2
CFLAGS+= -Isrc/pc86 -DOS386=1 -DIBMPC=1 
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc86/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=python3 makeboot.py -ibm

include src/Makefile.template

