build: ide ide-nogtk umk

ifneq (,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
	JOBS=-j$(patsubst parallel=%,%,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
else
	JOBS= 
endif

ide:
	sed -i -e 's/IDE_VERSION[ \t]*"\([0-9]*\)[^"]*"/IDE_VERSION    "\1-'$(SERIES)-$(ARCH)'"/' $(CURDIR)/uppsrc/ide/version.h
	$(MAKE) -f mkfile $(JOBS) PKG=ide $(compiler) "NESTS=uppsrc" "FLAGS=GUI MT GCC SHARED" BINEXT= "BINPREFIX="
	#cp /usr/bin/theide $@

ide-nogtk:
	sed -i -e 's/IDE_VERSION[ \t]*"\([0-9]*\)[^"]*"/IDE_VERSION    "\1-'$(SERIES)-$(ARCH)'-nogtk"/' $(CURDIR)/uppsrc/ide/version.h
	$(MAKE) -f mkfile $(JOBS) PKG=ide $(compiler) "NESTS=uppsrc" "FLAGS=GUI MT NOGTK GCC SHARED" BINEXT=-nogtk "BINPREFIX="
	#cp /usr/bin/theide $@

umk:
	$(MAKE) -f mkfile $(JOBS) PKG=umk $(compiler) "NESTS=uppsrc" "FLAGS=GCC SHARED" BINEXT= "BINPREFIX="
	#cp /usr/bin/theide $@

install:
	# this is just a trick to rename files
	install -D ide $(DESTDIR)/theide
	install -D ide-nogtk $(DESTDIR)/nogtk/theide
	install -D uppsrc/ide/theide-48.png $(DESTDIR)/theide.png

clean:
	rm -f ide ide-nogtk umk
