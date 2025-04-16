# RetroBoot Showcase

An example code base to show how to boot on several systems and set up basic functionality, such as text display.

# Features

The current implementation is very preliminary.
However, it shows how a boot loader can load the kernel image from disk and use (or simulate) a VGA text display.

The current code base supports the following systems:

* [Intel 8086/8088](https://en.wikipedia.org/wiki/Intel_8086) based systems
    * Supports running in 16-bit [real mode](https://en.wikipedia.org/wiki/Real_mode), 16-bit [protected mode](https://en.wikipedia.org/wiki/Protected_mode), 32-bit protected mode and (for IBM PC) 64-bit [long mode](https://en.wikipedia.org/wiki/Long_mode).
    * [IBM PC](https://en.wikipedia.org/wiki/IBM_Personal_Computer) with either [MDA](https://en.wikipedia.org/wiki/IBM_Monochrome_Display_Adapter) (monochrome) or [CGA](https://en.wikipedia.org/wiki/Color_Graphics_Adapter) (color) display (or compatible)
    * [NEC PC-98](https://en.wikipedia.org/wiki/PC-98)
* [Motorola 68000](https://en.wikipedia.org/wiki/Motorola_68000) based systems
    * [Atari ST](https://en.wikipedia.org/wiki/Atari_ST)
    * [Apple Macintosh 128K](https://en.wikipedia.org/wiki/Macintosh_128K), [Macintosh Plus](https://en.wikipedia.org/wiki/Macintosh_Plus), [Macintosh II](https://en.wikipedia.org/wiki/Macintosh_II)
    * [Commodore Amiga](https://en.wikipedia.org/wiki/Amiga)
    * [Sharp X68000](https://en.wikipedia.org/wiki/X68000)

# Compilation

The floppy images can be compiled using the `make` command, and they are placed under `bin/<platform name>/disk.<extension>`.

The following tools are required for compilation:

* i16-elf, i686-elf, x86_64-elf, m68k-elf binutils and GCC cross compilers (to compile the sources)
* Make
* Mtools (to generate BIOS parameter block)
* Python 3 (to make disk image bootable)
* Due to copyright restrictions, fonts cannot be shared in this repository. The files `src/font8x8.h` and `src/font8x16.h` must be provided by the user for proper functioning of the system. These files contain a byte array called `font8x8` and `font8x16`, each byte representing a row of the bitmap corresponding to each character in the font. A sample format for these files is as follows:
        #ifndef _FONT8X8_H
        #define _FONT8X8_H
        static const uint8_t font8x8[256 * 8] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ...
        };
        #endif // _FONT8X8_H

# Execution

The script `run` is provided for easy execution of the emulators.
The following options are provided (check `run` for most up to date information):

* x86 16-bit real mode: `run 16` (requires [QEMU](https://www.qemu.org/)), `run mda 16`, `run cga 16`, `run pc98 16` (requires [DOSBox-X](https://dosbox-x.com/))
* x86 16-bit protected mode: `run pm` (requires [QEMU](https://www.qemu.org/)), `run mda pm`, `run cga pm`, `run pc98 pm` (requires [DOSBox-X](https://dosbox-x.com/))
* x86 32-bit protected mode: `run 32` (requires [QEMU](https://www.qemu.org/)), `run mda 32`, `run cga 32`, `run pc98 32` (requires [DOSBox-X](https://dosbox-x.com/))
* x86 64-bit long mode: `run 64` (requires [QEMU](https://www.qemu.org/))
* Atari ST: `run atari` (requires [Hatari](https://hatari-emu.org/))
* Apple Macintosh: `run mac`, `run mac plus`, `run mac ii` (requires [Mini vMac](https://www.gryphel.com/c/minivmac/index.html))
* Commodore Amiga: `run amiga` (requires [fs-uae](https://fs-uae.net/))
* Sharp X68000: `run x68000` (requires [PX68k](https://github.com/libretro/px68k-libretro))

These emulators may also require the appropriate ROM images.

Other emulators will probably work as well.

# Inspiration and other links

This project would not have been possible without the many resources online and manuals for these platforms.
Links to some online resources:

* [OSDev.org](https://wiki.osdev.org/Expanded_Main_Page)
* [sandpile.org](https://sandpile.org/)
* [Learn Assembly Programming with ChibiAkumas](https://www.chibiakumas.com/68000/)
* NEC PC-98 references
    * [NEC PC98 Basic Reference](https://radioc.web.fc2.com/column/pc98bas/index_en.htm)
    * [The Differences between PC-9800 and PC/AT](https://newsonya.narod.ru/pc9821/diff-at/diff-at-overvw.html)
    * [PC98 DOS Programming - Hardware Edition](https://web.archive.org/web/20041225164712/http://www2.muroran-it.ac.jp/circle/mpc/program/pc98dos/index.html)

Some links to other similar projects:

* [EmuTOS for the Atari ST and Commodore Amiga](https://emutos.sourceforge.io/)
* [CP/M-68K port to the Commodore Amiga](https://github.com/juollila/cpm68k-amiga)
* [Tiny bootloader for early 68K Macs](https://github.com/notvelleda/tiny-mac-bootloader)

