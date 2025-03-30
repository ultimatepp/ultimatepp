/* Copyright (c) 2014 Alexander Lamaison <alexander.lamaison@gmail.com>
 * Copyright (c) 1999-2011 Douglas Gilbert. All rights reserved.
 *
 * Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided
 * that the following conditions are met:
 *
 *   Redistributions of source code must retain the above
 *   copyright notice, this list of conditions and the
 *   following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials
 *   provided with the distribution.
 *
 *   Neither the name of the copyright holder nor the names
 *   of any other contributors may be used to endorse or
 *   promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF TH☕IS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */

// Note: This file is modified to work with SSH package for U++.
// If you get errors during compilation, try modifying the definitons below:

/* Package information */
#define PACKAGE           "Upp::SSH"
#define PACKAGE_BUGREPORT "https://github.com/ultimatepp/ultimatepp/issues"
#define PACKAGE_NAME      "U++ SSH package"
#define PACKAGE_STRING    "U++ SSH package, a libssh2 wrapper, (ver. 2024.1)"
#define PACKAGE_URL       "https://github.com/ultimatepp/ultimatepp"
#define PACKAGE_VERSION   "2024.1b (libssh2 v.1.11.1)"

/* Headers */
#define HAVE_INTTYPES_H
#define HAVE_STDLIB_H
#if defined(flagGCC) || defined(flagCLANG)
#define HAVE_ERRNO_H
#define HAVE_UNISTD_H
#define HAVE_SYS_TIME_H
#endif
#ifdef flagPOSIX
#define HAVE_ARPA_INET_H
#define HAVE_MEMORY_H
#define HAVE_SYS_SELECT_H
#define HAVE_SYS_UIO_H
#define HAVE_SYS_SOCKET_H
#define HAVE_FCNTL_H
#define HAVE_SYS_IOCTL_H
#define HAVE_SYS_UN_H
#elif flagWIN32
#define HAVE_WINDOWS_H 
#define HAVE_WS2TCPIP_H 
#define HAVE_WINSOCK2_H 
#define HAVE_NTDEF_H 
#define HAVE_NTSTATUS_H
#endif

/* DEBUG */
#ifdef flagLIBSSH2TRACE
#define LIBSSH2DEBUG
#endif

/*
* Z compression requires plugin/z on Windows and for static builds.
* Below patch, which applies to libssh2/comp.c, allows using the
* Z compression on Upp::SSH Win32 and for static builds.
*/
#if defined(flagWIN32) || defined(flagNOSO)
#define UPP_ZLIB_INCLUDE <plugin/z/lib/zlib.h>
#else
#define UPP_ZLIB_INCLUDE <zlib.h>
#endif

/* Let us enable Z compression. */
#define LIBSSH2_HAVE_ZLIB

// enable DSS (this is actually deprecated, but we want to keep bw-compat, until the next release.
#define LIBSSH2_DSA_ENABLE

/* Upp-SSH package uses OpenSSL by default. */
#define LIBSSH2_OPENSSL

// #undef LIBSSH2_WINCNG
// #undef LIBSSH2_MBEDTLS

/* Types */
#define HAVE_LONGLONG

/* Functions */
#ifdef flagPOSIX
#define HAVE_POLL
#endif

#if defined(flagGCC) || defined(flagCLANG)
#define HAVE_GETTIMEOFDAY
#endif

/*#define HAVE_INET_ADDR*/

#define HAVE_SELECT
#define HAVE_SOCKET
#define HAVE_STRTOLL
//#define HAVE_STRTOI64 
#define HAVE_SNPRINTF

/* OpenSSL functions */
#define HAVE_EVP_AES_128_CTR
#define LIBSSH2_DH_GEX_NEW

/* Socket non-blocking support */
#ifdef flagPOSIX
#define HAVE_O_NONBLOCK
#elif flagWIN32
#define HAVE_IOCTLSOCKET 
#endif
/* #undef HAVE_FIONBIO */
/* #undef HAVE_IOCTLSOCKET_CASE */
/* #undef HAVE_SO_NONBLOCK */
/* #undef HAVE_DISABLED_NONBLOCKING */

#ifdef flagWIN32
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifdef _MSC_VER
#if _MSC_VER < 1900
#define snprintf _snprintf
#if _MSC_VER < 1500
#define vsnprintf _vsnprintf
#endif
#define strdup _strdup
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif
#else
#ifndef __MINGW32__
#define strncasecmp strnicmp
#define strcasecmp stricmp
#endif
#endif
#endif

/* snprintf not in Visual Studio CRT and _snprintf dangerously incompatible.
   We provide a safe wrapper if snprintf not found */
//#ifdef flagMSC
//#undef HAVE_SNPRINTF
//#endif

#ifndef HAVE_SNPRINTF
#include <stdio.h>
#include <stdarg.h>
/* Want safe, 'n += snprintf(b + n ...)' like function. If cp_max_len is 1
* then assume cp is pointing to a null char and do nothing. Returns number
* number of chars placed in cp excluding the trailing null char. So for
* cp_max_len > 0 the return value is always < cp_max_len; for cp_max_len
* <= 0 the return value is 0 (and no chars are written to cp). */
static int snprintf(char * cp, int cp_max_len, const char * fmt, ...)
{
    va_list args;
    int n;

    if (cp_max_len < 2)
        return 0;
    va_start(args, fmt);
    n = vsnprintf(cp, cp_max_len, fmt, args);
    va_end(args);
    return (n < cp_max_len) ? n : (cp_max_len - 1);
}

#define HAVE_SNPRINTF
#endif
