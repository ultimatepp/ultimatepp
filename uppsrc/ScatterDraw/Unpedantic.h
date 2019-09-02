#ifndef _ScatterDraw_Unpedantic_h_
#define _ScatterDraw_Unpedantic_h_

#if defined(flagDEBUG) && defined(flagPEDANTIC)

	#if defined(COMPILER_GCC) 
	#pragma GCC diagnostic ignored "-Wall"
	#pragma GCC diagnostic ignored "-Wextra"
	#pragma GCC diagnostic ignored "-Wpedantic"
	#pragma GCC diagnostic ignored "-Wcast-align"
	#pragma GCC diagnostic ignored "-Wcast-qual"
	#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
	#pragma GCC diagnostic ignored "-Wdisabled-optimization"
	#pragma GCC diagnostic ignored "-Wformat=2"
	#pragma GCC diagnostic ignored "-Winit-self"
	#pragma GCC diagnostic ignored "-Wlogical-op"
	#pragma GCC diagnostic ignored "-Wmissing-include-dirs"
	#pragma GCC diagnostic ignored "-Wnoexcept"
	#pragma GCC diagnostic ignored "-Wold-style-cast"
	//#pragma GCC diagnostic ignored "-Woverloaded-virtual"
	#pragma GCC diagnostic ignored "-Wredundant-decls"
	#pragma GCC diagnostic ignored "-Wshadow"
	#pragma GCC diagnostic ignored "-Wsign-promo"
	#pragma GCC diagnostic ignored "-Wstrict-null-sentinel"
	#pragma GCC diagnostic ignored "-Wstrict-overflow=4"
	#pragma GCC diagnostic ignored "-Wundef"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#endif
	
#endif

#endif