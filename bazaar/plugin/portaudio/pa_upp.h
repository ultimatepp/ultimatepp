#ifndef PA_UPP_H
#define PA_UPP_H

#ifdef flagPOSIX

	#define HAVE_SYS_SOUNDCARD_H 1
	#ifdef flagLINUX
		#define HAVE_LINUX_SOUNDCARD_H 1
	#endif
	#if 0 //TODO
		#define HAVE_MACHINE_SOUNDCARD_H 1
	#endif

#else

	#include "os/win/pa_win_wdmks_utils.h"
	#include "os/win/pa_x86_plain_converters.h"
	//#include "pa_win_waveformat.h" //?

#endif

#endif
