#ifndef _coff_coff_h
#define _coff_coff_h

#include <Core/Core.h>
#include <time.h>


NAMESPACE_UPP

#include "defs.h"
#include "util.h"
#include "lib.h"

#ifdef PLATFORM_WIN32
	#define PATH_HAS_CASE 0
#else
	#define PATH_HAS_CASE 1
#endif

END_UPP_NAMESPACE

#endif
