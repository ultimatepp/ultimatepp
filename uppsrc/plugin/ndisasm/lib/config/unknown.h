/* SPDX-License-Identifier: BSD-2-Clause */
/* Copyright 2016 The NASM Authors - All Rights Reserved */

/*
 * config/unknown.h - U++ vendored-build variant.
 *
 * Compiled without autoconf as C (+ one C++ wrapper) with gcc, clang or
 * llvm-mingw on POSIX, macOS and MinGW targets. Provide what configure
 * would have; do NOT strip the `inline` keyword (that turns every
 * extern_inline helper and CRT header inline into a strong definition
 * in each object file -> duplicate symbols at link time).
 */

#ifndef NASM_CONFIG_UNKNOWN_H
#define NASM_CONFIG_UNKNOWN_H

/* Standard C99 inline semantics; out-of-line copies of the extern_inline
   helpers are provided by the vendored nasmlib/ilog2.c. */
#define HAVE_STDC_INLINE 1

/* POSIX-like targets always have <inttypes.h> (uintptr_t, PRIx64, ...) */
#define HAVE_INTTYPES_H 1

/* Declared by both glibc (string.h) and mingw-w64 (corecrt_memory.h);
   compiler.h's static inline fallback collides with both. */
#define HAVE_MEMPCPY 1

/* Provided by the C library on all targets reaching this file */
#define HAVE_STRNLEN 1
#define HAVE_DECL_STRNLEN 1

/*
 * This header is processed by compiler.h BEFORE any system header, so
 * __GLIBC__ is not yet visible here. Probe for it explicitly: features.h
 * is glibc's canonical config header and defines __GLIBC__. (musl also
 * ships features.h but leaves __GLIBC__ unset - it keeps the NASM
 * fallbacks below.)
 */
#if defined(__linux__) && !defined(__GLIBC__)
# include <features.h>
#endif
#if defined(__GLIBC__)
/* glibc's <endian.h> defines htole16/32/64 and le16toh etc. as macros;
   bytesex.h's own static inline fallbacks collide with them. */
# define HAVE_HTOLE16 1
# define HAVE_HTOLE32 1
# define HAVE_HTOLE64 1
#endif

#ifndef restrict
# define restrict
#endif

#endif /* NASM_CONFIG_UNKNOWN_H */


// -DHAVE_MEMPCPY -DHAVE_HTOLE16 -DHAVE_HTOLE32 -DHAVE_HTOLE64 -DHAVE_INTTYPES_H -DHAVE_DECL_STRNLEN
// -DHAVE_STDC_INLINE -DHAVE_STRNLEN -DHAVE_DECL_STRNLEN