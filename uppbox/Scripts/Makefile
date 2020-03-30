.PHONY: all umk ide install clean distclean
SHELL = /bin/sh

all:
	+./domake

umk:
	+./domake umk

ide:
	+./domake ide

install:
	+./doinstall --verbose

uninstall:
	+./douninstall

clean:
	if [ -d "$(UPPOUT)" ] ; then \
		rm -fr "$(UPPOUT)" ; \
	else \
		rm -fr uppsrc/_out ; \
	fi
	rm -f uppsrc/ide.out
	rm -f uppsrc/umk.out

distclean: clean
	rm -f theide
	rm -f umk
	rm -f theide.desktop
	rm -f uppsrc/Makefile
	rm -f uppsrc/uMakefile
	rm -f uppsrc/build_info.h
