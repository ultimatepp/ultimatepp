#ifndef _ZMQ_ZMQ_DEF_h_
#define _ZMQ_ZMQ_DEF_h_

#include <Core/Core.h>

#undef ZMQ_HAVE_WINDOWS
#undef ZMQ_HAVE_LINUX
#undef ZMQ_HAVE_SOLARIS
#undef ZMQ_HAVE_FREEBSD
#undef ZMQ_HAVE_OSX
#undef ZMQ_HAVE_NETBSD
#undef ZMQ_HAVE_OPENBSD
#undef ZMQ_HAVE_AIX
#undef ZMQ_HAVE_HPUX
#undef ZMQ_HAVE_MINGW32
#undef ZMQ_HAVE_CYGWIN

#if defined(PLATFORM_WIN32) && PLATFORM_WIN32
	#define ZMQ_HAVE_WINDOWS 1
#elif defined(PLATFORM_LINUX)
	#define ZMQ_HAVE_LINUX 1
#elif defined(PLATFORM_SOLARIS)
	#define ZMQ_HAVE_SOLARIS 1
#elif defined(PLATFORM_FREEBSD)
	#define ZMQ_HAVE_FREEBSD 1
#elif defined(__APPLE__)
	#define ZMQ_HAVE_OSX 1
#elif defined(PLATFORM_NETBSD)
	#define ZMQ_HAVE_NETBSD 1
#elif defined(PLATFORM_OPENBSD)
	#define ZMQ_HAVE_OPENBSD 1
#elif defined(PLATFORM_AIX)
	#define ZMQ_HAVE_AIX 1
#elif defined(PLATFORM_HPUX)
	#define ZMQ_HAVE_HPUX 1
#elif defined(COMPILER_MINGW)
	#define ZMQ_HAVE_MINGW32 1
#elif defined(PLATFORM_CYGWIN)
	#define ZMQ_HAVE_CYGWIN 1  
#endif

#define ZMQ_NOT_LIBRARY

#endif
