
PLATFORM=pc64
DISKFILE=disk.img

AS=x86_64-elf-as
CC=x86_64-elf-gcc

ASFLAGS=--defsym OS64=1 --defsym IBMPC=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -march=x86-64 -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2
CFLAGS+= -O2
CFLAGS+= -Isrc/pc86 -DOS64=1 -DIBMPC=1 
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc86/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=python3 makeboot.py -ibm

include src/Makefile.template

