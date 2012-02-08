build: ide ide-nogtk umk

ifneq (,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
	JOBS=-j$(patsubst parallel=%,%,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
else
	JOBS= 
endif

SERIES="`dpkg-parsechangelog -c1 | sed -n 's/^Distribution: //p;'`"
ARCH="`dpkg-architecture -qDEB_HOST_ARCH`"

ide:
	sed -i -e 's/IDE_VERSION[ \t]*"\([0-9]*\)[^"]*"/IDE_VERSION    "\1-'$(SERIES)-$(ARCH)'"/' $(CURDIR)/uppsrc/ide/version.h
	$(MAKE) -f mkfile $(JOBS) PKG=ide "NESTS=uppsrc" "FLAGS=GUI GCC" TARGET=$@ TIME= COLOR=0

ide-nogtk:
	sed -i -e 's/IDE_VERSION[ \t]*"\([0-9]*\)[^"]*"/IDE_VERSION    "\1-'$(SERIES)-$(ARCH)'-nogtk"/' $(CURDIR)/uppsrc/ide/version.h
	$(MAKE) -f mkfile $(JOBS) PKG=ide "NESTS=uppsrc" "FLAGS=GUI NOGTK GCC " TARGET=$@ TIME= COLOR=0

umk:
	$(MAKE) -f mkfile $(JOBS) PKG=umk "NESTS=uppsrc" "FLAGS=GCC" TARGET=$@ TIME= COLOR=0

install:
	# this is just a trick to rename files
	install -D ide $(DESTDIR)/theide
	install -D ide-nogtk $(DESTDIR)/nogtk/theide
	install -D -m 644 uppsrc/ide/theide-48.png $(DESTDIR)/theide.png

clean:
	rm -f ide ide-nogtk umk
