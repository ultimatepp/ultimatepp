#include "pa_upp.h"

#if defined(flagPOSIX) && defined(PA_USE_JACK)
	#include "hostapi/jack/pa_jack.c"
#endif
