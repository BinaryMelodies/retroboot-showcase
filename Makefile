
all: bin/pc86/disk.img bin/pc286/disk.img bin/pc386/disk.img bin/pc64/disk.img bin/pc98/disk.img bin/pc98_286/disk.img bin/pc98_386/disk.img bin/atari/disk.st bin/x68000/disk.xdf

bin/%/disk.img: src/%/Makefile FORCE
	make -f $< $@

bin/atari/disk.st: FORCE
	make -f src/atari/Makefile $@

bin/x68000/disk.xdf: FORCE
	make -f src/x68000/Makefile $@

clean:
	rm -rf bin obj

distclean: clean
	find . -type f -name "*~" -exec rm {} \;

.PHONY: all clean distclean

FORCE: ;


