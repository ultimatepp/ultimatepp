#include "pa_upp.h"

#if defined(flagPOSIX) && defined(PA_USE_ASIHPI)
	#include "hostapi/asihpi/pa_linux_asihpi.c"
#endif
