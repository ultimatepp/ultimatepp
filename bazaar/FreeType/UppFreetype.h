#ifndef _freetype_UppFreetype_h_
#define _freetype_UppFreetype_h_

// this file is included per compiler option in each source file
// used to adapt some parts of freetype to Upp build system

#ifndef FT2_BUILD_LIBRARY
	#define FT2_BUILD_LIBRARY
#endif

// define exports in SO mode for MSC
#if defined(_MSC_VER) && defined(flagSO)
	#ifndef FT_EXPORT
		#define FT_EXPORT(x) __declspec(dllexport) x
	#endif

	#ifndef FT_EXPORT_DEF
		#define FT_EXPORT_DEF(x) __declspec(dllexport) x
	#endif
#endif

#include "ft2build.h"

#endif
