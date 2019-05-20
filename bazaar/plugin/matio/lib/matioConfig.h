/*
 * Copyright (c) 2012-2019, Christopher C. Hulbert
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Debug enabled */
#undef DEBUG

/* Extended sparse matrix data types */
#define EXTENDED_SPARSE 1

/* Define to dummy `main' function (if any) required to link to the Fortran
   libraries. */
#undef FC_DUMMY_MAIN

/* Define if F77 and FC dummy `main' functions are identical. */
#undef FC_DUMMY_MAIN_EQ_F77

/* Define to a macro mangling the given C identifier (in lower and upper
   case), which must not contain underscores, for linking with Fortran. */
#undef FC_FUNC

/* As FC_FUNC, but for C identifiers containing underscores. */
#undef FC_FUNC_

/* Define to 1 if you have the `asprintf' function. */
#undef HAVE_ASPRINTF

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if the system has the type `intmax_t'. */
#if defined(_MSC_VER) && _MSC_VER >= 1600
#define HAVE_INTMAX_T 1
#else
#undef HAVE_INTMAX_T
#endif

/* Define to 1 if you have the <inttypes.h> header file. */
#if defined(_MSC_VER) && _MSC_VER >= 1800
#define HAVE_INTTYPES_H 1
#else
#undef HAVE_INTTYPES_H
#endif

/* Define to 1 if you have the `m' library (-lm). */
#undef HAVE_LIBM

/* Define to 1 if you have the `localeconv' function. */
#define HAVE_LOCALECONV 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if the system has the type `long double'. */
#if defined(_MSC_VER) && _MSC_VER >= 1300
#define HAVE_LONG_DOUBLE 1
#else
#undef HAVE_LONG_DOUBLE
#endif

/* Define to 1 if the system has the type `long long int'. */
#if defined(_MSC_VER) && _MSC_VER >= 1300
#define HAVE_LONG_LONG_INT 1
#else
#undef HAVE_LONG_LONG_INT
#endif

/* Have MAT int16 */
#define HAVE_MAT_INT16_T 1

/* Have MAT int32 */
#define HAVE_MAT_INT32_T 1

/* Have MAT int64 */
#define HAVE_MAT_INT64_T 1

/* Have MAT int8 */
#define HAVE_MAT_INT8_T 1

/* Have MAT uint16 */
#define HAVE_MAT_UINT16_T 1

/* Have MAT uint32 */
#define HAVE_MAT_UINT32_T 1

/* Have MAT uint64 */
#define HAVE_MAT_UINT64_T 1

/* Have MAT uint8 */
#define HAVE_MAT_UINT8_T 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if the system has the type `ptrdiff_t'. */
#define HAVE_PTRDIFF_T 1

/* Define to 1 if you have a C99 compliant `snprintf' function. */
#if defined(_MSC_VER) && _MSC_VER >= 1900
#define HAVE_SNPRINTF 1
#else
#undef HAVE_SNPRINTF
#endif

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#if defined(_MSC_VER) && _MSC_VER >= 1600
#define HAVE_STDINT_H 1
#else
#undef HAVE_STDINT_H
#endif

/* Have the <stdlib.h> header file */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if `decimal_point' is member of `struct lconv'. */
#define HAVE_STRUCT_LCONV_DECIMAL_POINT 1

/* Define to 1 if `thousands_sep' is member of `struct lconv'. */
#define HAVE_STRUCT_LCONV_THOUSANDS_SEP 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#undef HAVE_SYS_TYPES_H

/* Define to 1 if the system has the type `uintmax_t'. */
#if defined(_MSC_VER) && _MSC_VER >= 1600
#define HAVE_UINTMAX_T 1
#else
#undef HAVE_UINTMAX_T
#endif

/* Define to 1 if the system has the type `uintptr_t'. */
#if defined(__linux__)
#undef HAVE_UINTPTR_T
#else
#define HAVE_UINTPTR_T 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if the system has the type `unsigned long long int'. */
#if defined(_MSC_VER) && _MSC_VER >= 1300
#define HAVE_UNSIGNED_LONG_LONG_INT 1
#else
#undef HAVE_UNSIGNED_LONG_LONG_INT
#endif

/* Define to 1 if you have the <varargs.h> header file. */
#define HAVE_VARARGS_H 1

/* Define to 1 if you have the `vasprintf' function. */
#undef HAVE_VASPRINTF

/* Define to 1 if you have the `va_copy' function or macro. */
#if defined(_MSC_VER) && _MSC_VER >= 1800
#define HAVE_VA_COPY 1
#elif defined(__linux__)
#define HAVE_VA_COPY 1
#else 
#undef HAVE_VA_COPY
#endif

/* Define to 1 if you have a C99 compliant `vsnprintf' function. */
#if defined(_MSC_VER) && _MSC_VER >= 1900
#define HAVE_VSNPRINTF 1
#else
#undef HAVE_VSNPRINTF
#endif

/* Define to 1 if you have the `__va_copy' function or macro. */
#undef HAVE___VA_COPY

/* OS is Linux */
#undef LINUX

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#undef LT_OBJDIR

/* Platform */
#if defined(_WIN64)
#   define MATIO_PLATFORM "x86_64-pc-windows"
#elif defined(_WIN32)
#   define MATIO_PLATFORM "i686-pc-windows"
#elif defined(__linux__)
#   define MATIO_PLATFORM "posix"
#endif

/* Debug disabled */
#undef NODEBUG

/* Name of package */
#define PACKAGE "matio"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "t-beu@users.sourceforge.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "MATIO"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "MATIO 1.5.15"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "matio"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://sourceforge.net/projects/matio"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.5.15"

/* The size of `char', as computed by sizeof. */
#define SIZEOF_CHAR 1

/* The size of `double', as computed by sizeof. */
#define SIZEOF_DOUBLE 8

/* The size of `float', as computed by sizeof. */
#define SIZEOF_FLOAT 4

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

#if defined(_WIN64)
    /* The size of `void *', as computed by sizeof. */
#   define SIZEOF_VOID_P 8
    /* The size of `size_t', as computed by sizeof. */
#    define SIZEOF_SIZE_T 8
#elif defined(_WIN32)
    /* The size of `void *', as computed by sizeof. */
#   define SIZEOF_VOID_P 4
    /* The size of `size_t', as computed by sizeof. */
#    define SIZEOF_SIZE_T 4
#endif

/* Define to 1 if you have the ANSI C header files. */
#undef STDC_HEADERS

/* Version number of package */
#define VERSION "1.5.15"

/* Z prefix */
#undef Z_PREFIX
