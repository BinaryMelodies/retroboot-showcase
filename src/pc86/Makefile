
PLATFORM=pc86
DISKFILE=disk.img

AS=ia16-elf-as
CC=ia16-elf-gcc

ASFLAGS=--defsym OS86=1 --defsym IBMPC=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -fno-delete-null-pointer-checks
#CFLAGS+= -O2
CFLAGS+= -Isrc/pc86 -DOS86=1 -DIBMPC=1 
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc86/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=python3 makeboot.py -ibm

include src/Makefile.template

