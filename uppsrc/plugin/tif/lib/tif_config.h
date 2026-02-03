/* clang-format off */
/* clang-format disabled because CMake scripts are very sensitive to the
 * formatting of this file. configure_file variables of type "@VAR@" are
 * modified by clang-format and won't be substituted.
 */

/* libtiff/tif_config.h.in.  Not generated, but originated from autoheader.  */

#include "tiffconf.h"

#include <Core/config.h>

/* Support CCITT Group 3 & 4 algorithms */
#undef CCITT_SUPPORT

/* Pick up YCbCr subsampling info from the JPEG data stream to support files
   lacking the tag (default enabled). */
#define CHECK_JPEG_YCBCR_SUBSAMPLING 1

/* enable partial strip reading for large strips (experimental) */
#define CHUNKY_STRIP_READ_SUPPORT 1

/* Support C++ stream API (requires C++ compiler) */
// #undef CXX_SUPPORT

/* enable deferred strip/tile offset/size loading */
#define DEFER_STRILE_LOAD 1

/* Define to 1 if you have the <assert.h> header file. */
#define HAVE_ASSERT_H 1

/* Define to 1 if you have the declaration of `optarg', and to 0 if you don't.
   */
//#undef HAVE_DECL_OPTARG

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if fseeko (and presumably ftello) exists and is declared. */
// #undef HAVE_FSEEKO

/* Define to 1 if you have the `getopt' function. */
// #undef HAVE_GETOPT

/* Define to 1 if you have the <GLUT/glut.h> header file. */
// #undef HAVE_GLUT_GLUT_H

/* Define to 1 if you have the <GL/glut.h> header file. */
// #undef HAVE_GL_GLUT_H

/* Define to 1 if you have the <GL/glu.h> header file. */
// #undef HAVE_GL_GLU_H

/* Define to 1 if you have the <GL/gl.h> header file. */
// #undef HAVE_GL_GL_H

/* Define to 1 if you have the <io.h> header file. */
#define HAVE_IO_H

/* Define to 1 if you have the `jbg_newlen' function. */
// #undef HAVE_JBG_NEWLEN

/* Define to 1 if you have the `mmap' function. */
// #undef HAVE_MMAP

/* Define to 1 if you have the <OpenGL/glu.h> header file. */
// #undef HAVE_OPENGL_GLU_H

/* Define to 1 if you have the <OpenGL/gl.h> header file. */
// #undef HAVE_OPENGL_GL_H

/* Define to 1 if you have the `setmode' function. */
#define HAVE_SETMODE 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <strings.h> header file. */
// #undef HAVE_STRINGS_H

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
// #undef HAVE_UNISTD_H

/* 8/12 bit libjpeg dual mode enabled */
// #undef JPEG_DUAL_MODE_8_12

/* 8/12 bit dual mode JPEG built into libjpeg-turbo 3.0+ */
// #undef HAVE_JPEGTURBO_DUAL_MODE_8_12

/* Support LERC compression */
// #define LERC_SUPPORT 1

/* Define to 1 when building a static libtiff with LERC enabled. */
// #define LERC_STATIC 1

/* 12bit libjpeg primary include file with path */
// #undef LIBJPEG_12_PATH

/* Support LZMA2 compression */
// #undef LZMA_SUPPORT

/* Name of package */
// #undef PACKAGE

/* Define to the address where bug reports for this package should be sent. */
// #undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
// #undef PACKAGE_NAME

/* Define to the one symbol short name of this package. */
// #undef PACKAGE_TARNAME

/* Define to the home page for this package. */
// #undef PACKAGE_URL


#ifdef CPU_32
#define SIZEOF_SIZE_T 4
#else
#define SIZEOF_SIZE_T 8
#endif

/* The size of `size_t', as computed by sizeof. */

/* Default size of the strip in bytes (when strip chopping enabled) */
// #undef STRIP_SIZE_DEFAULT

/* Maximum number of TIFF IFDs that libtiff can iterate through in a file. */
// #undef TIFF_MAX_DIR_COUNT

/* define to use win32 IO system */
// #undef USE_WIN32_FILEIO

/* Support webp compression */
// #undef WEBP_SUPPORT

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
#  undef WORDS_BIGENDIAN
# endif
#endif

/* Support zstd compression */
// #undef ZSTD_SUPPORT

/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
// #undef _FILE_OFFSET_BITS

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
// #undef _LARGEFILE_SOURCE

/* Define for large files, on AIX-style hosts. */
// #undef _LARGE_FILES

#if !defined(__MINGW32__)
#  define TIFF_SIZE_FORMAT "zu"
#endif
#if SIZEOF_SIZE_T == 8
#  define TIFF_SSIZE_FORMAT PRId64
#  if defined(__MINGW32__)
#    define TIFF_SIZE_FORMAT PRIu64
#  endif
#elif SIZEOF_SIZE_T == 4
#  define TIFF_SSIZE_FORMAT PRId32
#  if defined(__MINGW32__)
#    define TIFF_SIZE_FORMAT PRIu32
#  endif
#else
#  error "Unsupported size_t size; please submit a bug report"
#endif

/* clang-format on */
