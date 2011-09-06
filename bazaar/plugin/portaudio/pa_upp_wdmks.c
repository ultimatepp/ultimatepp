#include "pa_upp.h"

#if !defined(flagPOSIX) & defined(PA_USE_WDMKS)
	#include "hostapi/wdmks/pa_win_wdmks.c"
	#include "os/win/pa_win_wdmks_utils.c"
#endif
