#include "pa_upp.h"

#if defined(flagPOSIX) && defined(PA_USE_OSS)
	#include "hostapi/oss/pa_unix_oss.c"
#endif
