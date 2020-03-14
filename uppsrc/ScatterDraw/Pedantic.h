#ifndef _ScatterDraw_Pedantic_h_
#define _ScatterDraw_Pedantic_h_

#if defined(flagDEBUG) && defined(flagPEDANTIC)

	#if defined(COMPILER_GCC) 
	#pragma GCC diagnostic push
	#pragma GCC diagnostic warning "-Wall"
	#pragma GCC diagnostic warning "-Wextra"
	#pragma GCC diagnostic warning "-Wpedantic"
	#pragma GCC diagnostic warning "-Wcast-align"
	#pragma GCC diagnostic warning "-Wcast-qual"
	#pragma GCC diagnostic warning "-Wctor-dtor-privacy"
	#pragma GCC diagnostic warning "-Wdisabled-optimization"
	#pragma GCC diagnostic warning "-Wformat=2"
	#pragma GCC diagnostic warning "-Winit-self"
	//#pragma GCC diagnostic warning "-Wlogical-op"
	#pragma GCC diagnostic warning "-Wmissing-include-dirs"
	//#pragma GCC diagnostic warning "-Wnoexcept"
	#pragma GCC diagnostic warning "-Wold-style-cast"
	//#pragma GCC diagnostic warning "-Woverloaded-virtual"
	#pragma GCC diagnostic warning "-Wredundant-decls"
	#pragma GCC diagnostic warning "-Wshadow"
	#pragma GCC diagnostic warning "-Wsign-promo"
	//#pragma GCC diagnostic warning "-Wstrict-null-sentinel"
	#pragma GCC diagnostic warning "-Wstrict-overflow=4"
	#pragma GCC diagnostic warning "-Wundef"
	#pragma GCC diagnostic warning "-Wunused-variable"
	#endif

#endif

#endif
