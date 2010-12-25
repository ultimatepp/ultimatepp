#include "pa_upp.h"

#if defined(flagPOSIX) && defined(PA_USE_ALSA)
	#include "hostapi/alsa/pa_linux_alsa.c"
#endif
