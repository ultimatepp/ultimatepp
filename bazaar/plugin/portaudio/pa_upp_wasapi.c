#include "pa_upp.h"

#if !defined(flagPOSIX) && defined(PA_USE_WASAPI)
	#include "hostapi/wasapi/pa_win_wasapi.c"
#endif
