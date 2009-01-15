# $Id: GNUmakefile 10656 2007-01-19 01:31:01Z mloskot $
#
# Makefile to build zlib using GNU Make and GCC.
#
include ../../GDALmake.opt

OBJ = \
	adler32.o \
	compress.o \
	crc32.o \
	deflate.o \
	gzio.o \
	infback.o \
	inffast.o \
	inflate.o \
	inftrees.o \
	trees.o \
	uncompr.o \
	zutil.o

default:	$(OBJ)

clean:
	rm -f *.o

install-obj:	$(O_OBJ)
