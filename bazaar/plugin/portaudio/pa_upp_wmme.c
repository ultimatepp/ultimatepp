#include "pa_upp.h"

#if !defined(flagPOSIX) && defined(PA_USE_WMME)
	#include "hostapi/wmme/pa_win_wmme.c"
#endif
