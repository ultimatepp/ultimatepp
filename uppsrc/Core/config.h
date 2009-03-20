#if __GNUC__

	#define COMPILER_GCC 1

	#if __unix
		#define PLATFORM_POSIX 1
		
		#ifdef flagGUI
			#define PLATFORM_X11 1
		#endif
	
		#if __linux
			#define PLATFORM_LINUX 1
		#else // ToDo
			#define PLATFORM_BSD 1
			#define PLATFORM_FREEBSD 1
		#endif
	#endif
	
	#if  __x86_64
		#define CPU_LE 1
		#define CPU_LITTLE_ENDIAN 1
		#define CPU_UNALIGNED 1
		#define CPU_X86 1
		#define CPU_64 1
		#define CPU_AMD64 1
		#define CPU_SSE2 1
	#elif __i386
		#define CPU_LE 1
		#define CPU_LITTLE_ENDIAN 1
		#define CPU_UNALIGNED 1
		#define CPU_X86 1
		#define CPU_32 1
		#ifdef flagSSE2
			#define CPU_SSE2 1
		#endif
	#elif __sparc  // ToDo!
		#define CPU_32 1
		#define CPU_SPARC 1
		#define CPU_BE 1
		#define CPU_BIG_ENDIAN 1
		#define CPU_ALIGNED 1
	#elif __arm // ToDo!
		#define CPU_32 1
		#define CPU_ARM 1
		#define CPU_LE 1
		#define CPU_LITTLE_ENDIAN 1 // is it really?
		#define CPU_ALIGNED 1
	#else
		#error Unknown CPU architecture
	#endif
#endif

#ifdef _MSC_VER
	#define COMPILER_MSC 1
	#ifndef _CPPRTTI
		#error  RTTI must be enabled !!!
	#endif  //_CPPRTTI
	#if _MSC_VER <= 1300
		#error  MSC 6.0 not supported anymore
	#endif
	#pragma warning(disable: 4786)
	#define _CRT_SECURE_NO_DEPRECATE 1 // we really need strcpy etc. to work with MSC 8.0
	
	#define PLATFORM_WIN32 1

	#define CPU_LE 1
	#define CPU_LITTLE_ENDIAN 1
	#define CPU_UNALIGNED 1
	#define CPU_X86 1

	#ifdef _WIN64
		#define PLATFORM_WIN64 1
		#define CPU_64 1
		#define CPU_AMD64 1
		#define CPU_SSE2 1
	#else
		#define CPU_32 1
		#ifdef flagSSE2
			#define CPU_SSE2 1
		#endif
	#endif			
#endif
