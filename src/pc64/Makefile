
PLATFORM=pc64
DISKFILE=disk.img

AS=x86_64-elf-as
CC=x86_64-elf-gcc

SECTOR_SIZE=512
SECTOR_SIZE_SHIFT=9
BOOT_BLOCK_SIZE=512

ASFLAGS=--defsym MODE_LONG=1 --defsym BITS64=1 --defsym MACHINE_IBMPC=1 --defsym SECTOR_SIZE=$(SECTOR_SIZE) --defsym SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) --defsym BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -march=x86-64 -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2
CFLAGS+= -O2
CFLAGS+= -Isrc/pc86 -DMODE_LONG=1 -DMACHINE_IBMPC=1
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc -Wl,--defsym=SECTOR_SIZE=$(SECTOR_SIZE) -Wl,--defsym=SECTOR_SIZE_SHIFT=$(SECTOR_SIZE_SHIFT) -Wl,--defsym=BOOT_BLOCK_SIZE=$(BOOT_BLOCK_SIZE)

DISKSIZE=1440
FORMATFLAGS=-f 1440
LDSCRIPT=src/pc86/linker.ld
BOOT=src/pc86/boot.s
FIXIMAGE=python3 makeboot.py -ibm

INCLUDES=src/pc86/video.h src/pc86/colors.h src/x86.c src/pc86/irq.c src/i8259.c src/i8253.c src/i86.h src/system.h src/pc86/scancode.c src/pc86/keyboard.c src/pc86/keyproc.c src/pc86/timer.c

include src/Makefile.template

