
PLATFORM=pc98_286
DISKFILE=disk.img

AS=ia16-elf-as
CC=ia16-elf-gcc

ASFLAGS=--defsym OS286=1 --defsym CPU_80286=1 --defsym MODE_PROTECTED=1 --defsym MACHINE_NECPC98=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -march=i80286 -mprotected-mode
#CFLAGS+= -O2
CFLAGS+= -Isrc/pc98 -DOS286=1 -DCPU_80286=1 -DMODE_PROTECTED=1 -DMACHINE_NECPC98=1 
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc98/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=./ignore

include src/Makefile.template

