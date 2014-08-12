/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

#define CPL_MULTIPROC_STUB 1

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
//#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <values.h> header file. */
#define HAVE_VALUES_H 1

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

#define HAVE_LONG_LONG 1
#define HAVE_LIBZ      1

#if defined(UNIX_STDIO_64)

#ifndef VSI_FTELL64
#define VSI_FTELL64 ftell64
#endif
#ifndef VSI_FSEEK64
#define VSI_FSEEK64 fseek64
#endif
#ifndef VSI_FOPEN64
#define VSI_FOPEN64 fopen64
#endif
#ifndef VSI_STAT64
#define VSI_STAT64 stat64
#endif
#ifndef VSI_STAT64_T
#define VSI_STAT64_T stat64
#endif

#else /* not UNIX_STDIO_64 */

#ifndef VSI_FTELL64
#define VSI_FTELL64 ftell
#endif
#ifndef VSI_FSEEK64
#define VSI_FSEEK64 fseek
#endif
#ifndef VSI_FOPEN64
#define VSI_FOPEN64 fopen
#endif
#ifndef VSI_STAT64
#define VSI_STAT64 stat
#endif
#ifndef VSI_STAT64_T
#define VSI_STAT64_T stat
#endif

#endif

#ifdef flagWIN32
#define snprintf _snprintf
//#define vsnprintf _vsnprintf
#endif
