/*
 * Copyright (c) 2010-2019, Christopher C. Hulbert
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

#ifndef MATIO_PUBCONF_H
#define MATIO_PUBCONF_H 1

/* Matio major version number */
#define MATIO_MAJOR_VERSION 1

/* Matio minor version number */
#define MATIO_MINOR_VERSION 5

/* Matio release level number */
#define MATIO_RELEASE_LEVEL 15

/* Matio version number */
#define MATIO_VERSION 1515

/* Matio version string */
#define MATIO_VERSION_STR "1.5.15"

/* Default file format */
#define MAT_FT_DEFAULT MAT_FT_MAT5

/* Define to 1 if you have the <stdint.h> header file. */
#if defined(_MSC_VER) && _MSC_VER >= 1600
#define MATIO_HAVE_STDINT_H 1
#else
#undef MATIO_HAVE_STDINT_H
#endif

/* Define to 1 if you have the <inttypes.h> header file. */
#if defined(_MSC_VER) && _MSC_VER >= 1800
#define MATIO_HAVE_INTTYPES_H 1
#else
#undef MATIO_HAVE_INTTYPES_H
#endif

#if MATIO_HAVE_STDINT_H
/* int16 type */
#define _mat_int16_t int16_t

/* int32 type */
#define _mat_int32_t int32_t

/* int64 type */
#define _mat_int64_t int64_t

/* int8 type */
#define _mat_int8_t int8_t

/* uint16 type */
#define _mat_uint16_t uint16_t

/* uint32 type */
#define _mat_uint32_t uint32_t

/* uint64 type */
#define _mat_uint64_t uint64_t

/* uint8 type */
#define _mat_uint8_t uint8_t
#else
/* int16 type */
#define _mat_int16_t short

/* int32 type */
#define _mat_int32_t int

/* int64 type */
#if defined(_MSC_VER) && _MSC_VER < 1300
#define _mat_int64_t __int64
#else
#define _mat_int64_t long long
#endif

/* int8 type */
#define _mat_int8_t signed char

/* uint16 type */
#define _mat_uint16_t unsigned short

/* uint32 type */
#define _mat_uint32_t unsigned

/* uint64 type */
#if defined(_MSC_VER) && _MSC_VER < 1300
#define _mat_uint64_t unsigned __int64
#else
#define _mat_uint64_t unsigned long long
#endif

/* uint8 type */
#define _mat_uint8_t unsigned char
#endif

#if MATIO_HAVE_INTTYPES_H
#   include <inttypes.h>
#endif

#if MATIO_HAVE_STDINT_H
#   include <stdint.h>
#endif

#ifdef _mat_int64_t
    typedef _mat_int64_t mat_int64_t;
#endif
#ifdef _mat_uint64_t
    typedef _mat_uint64_t mat_uint64_t;
#endif
#ifdef _mat_int32_t
    typedef _mat_int32_t mat_int32_t;
#endif
#ifdef _mat_uint32_t
    typedef _mat_uint32_t mat_uint32_t;
#endif
#ifdef _mat_int16_t
    typedef _mat_int16_t mat_int16_t;
#endif
#ifdef _mat_uint16_t
    typedef _mat_uint16_t mat_uint16_t;
#endif
#ifdef _mat_int8_t
    typedef _mat_int8_t mat_int8_t;
#endif
#ifdef _mat_uint8_t
    typedef _mat_uint8_t mat_uint8_t;
#endif

/* 
  The following macros handle noreturn attributes according to the latest
  C11/C++11 standard with fallback to the MSVC extension if using an older
  compiler.
*/
#define MATIO_NORETURNATTR
#if __STDC_VERSION__ >= 201112L
#define MATIO_NORETURN _Noreturn
#elif __cplusplus >= 201103L
#define MATIO_NORETURN [[noreturn]]
#elif defined(_MSC_VER) && _MSC_VER >= 1200
#define MATIO_NORETURN __declspec(noreturn)
#else
#define MATIO_NORETURN
#endif

/*
  The following macros handle format attributes for type-checks against a
  format string.
*/

#define MATIO_FORMATATTR_PRINTF1
#define MATIO_FORMATATTR_PRINTF2
#define MATIO_FORMATATTR_VPRINTF

#endif /* MATIO_PUBCONF_H */
