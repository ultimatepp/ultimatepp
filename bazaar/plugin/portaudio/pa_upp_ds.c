#include "pa_upp.h"

#if !defined(flagPOSIX) && defined(PA_USE_DS)
	#include "hostapi/dsound/pa_win_ds.c"
	//#include "hostapi/dsound/pa_win_ds_dynlink.c"
#endif
