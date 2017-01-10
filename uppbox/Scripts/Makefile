.PHONY: all install clean distclean
SHELL = /bin/sh

all:
	+$(SHELL) domake

install:
	$(SHELL) doinstall

clean:
	rm -fr uppsrc/_out
	rm -f  uppsrc/ide.out
	rm -f  uppsrc/umk.out

distclean: clean
	rm -f GCC.bm
	rm -f CLANG.bm
	rm -f uppsrc/Makefile
	rm -f uppsrc/uMakefile
	rm -f uppsrc/build_info.h
