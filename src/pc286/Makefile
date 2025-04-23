
PLATFORM=pc286
DISKFILE=disk.img

AS=ia16-elf-as
CC=ia16-elf-gcc

ASFLAGS=--defsym OS286=1 --defsym CPU_80286=1 --defsym MODE_PROTECTED=1 --defsym BITS16=1 --defsym MACHINE_IBMPC=1
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -march=i80286 -mprotected-mode
#CFLAGS+= -O2
CFLAGS+= -Isrc/pc86 -DOS286=1 -DCPU_80286=1 -DMODE_PROTECTED=1 -DMACHINE_IBMPC=1
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc86/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=python3 makeboot.py -ibm

include src/Makefile.template

