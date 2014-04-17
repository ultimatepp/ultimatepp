build: ide ide-nogtk umk

ifneq (,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
    JOBS:=$(patsubst parallel=%,%,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
else
    JOBS:=1
endif

SERIES:="`dpkg-parsechangelog -c1 | sed -n 's/^Distribution: //p;'`"
ARCH:="`dpkg-architecture -qDEB_HOST_ARCH`"
OPTS:=CC="cc -g" CXX="c++ -g" CFLAGS="-g" CXXFLAGS="-g" LDFLAGS="-Wl,--gc-sections -Wl,-O,2" TIME= COLOR=0 USEMAINCFG=0

ide:
	sed -i -e 's/IDE_VERSION[ \t]*"\([0-9]*\)[^"]*"/IDE_VERSION    "\1-'$(SERIES)-$(ARCH)'"/' $(CURDIR)/uppsrc/ide/version.h
	export CCACHE_DISABLE=1; $(MAKE) -f mkfile JOBS=$(JOBS) PKG=ide "NESTS=uppsrc" $(OPTS) "FLAGS=GUI GCC DEBUG DEBUG_FULL STACKTRACE" TARGET=`pwd`/$@

ide-nogtk:
	sed -i -e 's/IDE_VERSION[ \t]*"\([0-9]*\)[^"]*"/IDE_VERSION    "\1-'$(SERIES)-$(ARCH)'-nogtk"/' $(CURDIR)/uppsrc/ide/version.h
	export CCACHE_DISABLE=1; $(MAKE) -f mkfile JOBS=$(JOBS) PKG=ide "NESTS=uppsrc" $(OPTS) "FLAGS=GUI NOGTK GCC DEBUG DEBUG_FULL STACKTRACE" TARGET=`pwd`/$@

umk:
	export CCACHE_DISABLE=1; $(MAKE) -f mkfile JOBS=$(JOBS) PKG=umk "NESTS=uppsrc" $(OPTS) "FLAGS=GCC DEBUG DEBUG_FULL STACKTRACE" TARGET=`pwd`/$@

install:
	# this is just a trick to rename files
	install -D ide $(DESTDIR)/theide
	install -D ide-nogtk $(DESTDIR)/nogtk/theide
	install -D -m 644 uppsrc/ide/theide-48.png $(DESTDIR)/theide.png

clean:
	rm -f ide ide-nogtk umk

distclean: clean
