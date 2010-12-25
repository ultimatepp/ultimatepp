#include "pa_upp.h"

#if !defined(flagPOSIX) && !defined(PA_NO_WASAPI)
	#include "hostapi/wasapi/pa_win_wasapi.c"
#endif
