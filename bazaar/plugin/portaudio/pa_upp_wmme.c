#include "pa_upp.h"

#if !defined(flagPOSIX) && !defined(PA_NO_WMME)
	#include "hostapi/wmme/pa_win_wmme.c"
#endif
