/* Define to 1 if you have the <assert.h> header file. */
#define HAVE_ASSERT_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define as 0 or 1 according to the floating point format suported by the
   machine */
#define HAVE_IEEEFP 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <io.h> header file. */
#define HAVE_IO_H 1

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* Define to 1 if you have the <search.h> header file. */
#define HAVE_SEARCH_H 1
#define HAVE_STRING_H 1

/* Set the native cpu bit order */
#define HOST_FILLORDER FILLORDER_LSB2MSB

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

#ifdef COMPILER_MSC
	#define TIFF_INT64_T __int64
	#define TIFF_UINT64_T __int64 unsigned
#else
	#define TIFF_INT64_T long long int
	#define TIFF_UINT64_T long long int unsigned;
#endif

#define HAVE_INT64
#define HAVE_UINT64

#ifdef COMPILER_MSC
	#define int64 __int64
	#define uint64 __int64 unsigned
#else
	#define int64 long long int
	#define uint64 long long int unsigned
#endif

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#define inline __inline
#endif
