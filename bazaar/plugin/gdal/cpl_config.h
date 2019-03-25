#include <Core/config.h> // we want PLATFORM and COMPILER defines

#define CPL_DISABLE_DLL
#define DISABLE_CPLID // ALLOW BLITZ

#define GDAL_COMPILATION



#ifndef _gdal_cpl_config_h_
#define _gdal_cpl_config_h_

#define RENAME_INTERNAL_LIBTIFF_SYMBOLS
#define RENAME_INTERNAL_LIBGEOTIFF_SYMBOLS

// added to embed in U++ to fix plugin/tif interoperability
#define TIFFSwabArrayOfDouble gdal_TIFFSwabArrayOfDouble
#define TIFFSwabArrayOfLong gdal_TIFFSwabArrayOfLong
#define TIFFSwabArrayOfShort gdal_TIFFSwabArrayOfShort
#define TIFFSwabArrayOfTriples gdal_TIFFSwabArrayOfTriples
#define TIFFSwabDouble gdal_TIFFSwabDouble
#define TIFFSwabLong gdal_TIFFSwabLong
#define TIFFSwabShort gdal_TIFFSwabShort
#define _TIFFSwab16BitData gdal__TIFFSwab16BitData
#define _TIFFSwab24BitData gdal__TIFFSwab24BitData
#define _TIFFSwab32BitData gdal__TIFFSwab32BitData
#define _TIFFSwab64BitData gdal__TIFFSwab64BitData

#define TIFFSwabArrayOfFloat gdal_TIFFSwabArrayOfFloat
#define TIFFSwabArrayOfLong8 gdal_TIFFSwabArrayOfLong8
#define TIFFSwabFloat gdal_TIFFSwabFloat
#define TIFFSwabLong8 gdal_TIFFSwabLong8

/* We define this here in general so that a VC++ build will publically

   declare STDCALL interfaces even if an application is built against it
   using MinGW */

#ifdef PLATFORM_WIN32
#ifndef CPL_DISABLE_STDCALL
#  define CPL_STDCALL __stdcall
#endif
#endif

#define HAVE_LIBZ 1

/* Define if you don't have vprintf but do have _doprnt.  */
#undef HAVE_DOPRNT

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1
#define HAVE_VSNPRINTF 1
#define HAVE_SNPRINTF 1
#if defined(_MSC_VER) && (_MSC_VER < 1500)
#  define vsnprintf _vsnprintf
#endif
#if defined(_MSC_VER) && (_MSC_VER < 1900)
#  define snprintf _snprintf
#endif

#define HAVE_GETCWD 1

/* Define if you have the ANSI C header files.  */
#ifndef STDC_HEADERS
#  define STDC_HEADERS 1
#endif

/* Define to 1 if you have the <assert.h> header file. */
#define HAVE_ASSERT_H 1

/* Define to 1 if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <unistd.h> header file.  */
#undef HAVE_UNISTD_H

/* Define if you have the <stdint.h> header file.  */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

#undef HAVE_LIBDL 

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

#define HAVE_FLOAT_H 1

#define HAVE_ERRNO_H 1

#define HAVE_SEARCH_H 1

/* Define to 1 if you have the <direct.h> header file. */
// #define HAVE_DIRECT_H

/* Define to 1 if you have the `localtime_r' function. */
#undef HAVE_LOCALTIME_R

#undef HAVE_DLFCN_H
#undef HAVE_DBMALLOC_H
#undef HAVE_LIBDBMALLOC
#undef WORDS_BIGENDIAN

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of a `unsigned long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG 4

/* The size of `void*', as computed by sizeof. */
#ifdef _WIN64
# define SIZEOF_VOIDP 8
#else
# define SIZEOF_VOIDP 4
#endif

/* Set the native cpu bit order */
#define HOST_FILLORDER FILLORDER_LSB2MSB

/* Define as 0 or 1 according to the floating point format suported by the
   machine */
#define HAVE_IEEEFP 1

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#  ifndef inline
#    define inline __inline
#  endif
#endif

#define lfind _lfind

#ifdef PLATFORM_POSIX
#define VSI_STAT64 stat64
#define VSI_STAT64_T stat64
#else
#define VSI_STAT64 _stat64
#define VSI_STAT64_T __stat64
#endif

/* VC6 doesn't known intptr_t */
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
    typedef int intptr_t;
#endif

#pragma warning(disable: 4786)

/* #define CPL_DISABLE_DLL */

#endif
