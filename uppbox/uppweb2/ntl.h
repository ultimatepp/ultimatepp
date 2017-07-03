#ifndef NTL_H
#define NTL_H

#include <assert.h>
#include <string.h>

#include <new>

#ifndef ASSERT
#define ASSERT assert
#endif

#define pick_  const   // Do not beat us for this. Imagine it is #define pick_ &&
#define _cm_   ,

typedef unsigned char byte;

#ifdef _MSC_VER // Solve some MSC problems...

	#ifdef TYPENAME
    	//use predefined value
	#elif _MSC_VER < 1300
		#define TYPENAME
	#else
		#define TYPENAME typename
	#endif

	// Later we might consider #pragma warning(push/pop)
	#pragma warning(disable : 4800) //'type' : forcing value to bool 'true' or 'false'
	#pragma warning(disable : 4786) // identifier was truncated
	#pragma warning(disable : 4345) // POD type constructed will be default-initialized
	#pragma warning(disable : 4675) //'function' : resolved overload was found by Koenig lookup

	#define COMPILER_MSC

#else

	#define TYPENAME typename

#endif


#ifdef __GNUC__
#define COMPILER_GCC
#endif

#if defined(_M_IX86) || defined(__i386__)
	#define CPU_IA32
#endif

#include <detail/Topt.h>
#include <detail/Vcont.h>
#include <detail/BiCont.h>
#include <detail/Index.h>
#include <detail/Map.h>
#include <detail/Algo.h>
#include <detail/Other.h>
#include <detail/Vcont.hpp>
#include <detail/Index.hpp>

#endif
