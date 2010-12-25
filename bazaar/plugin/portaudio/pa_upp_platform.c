#include "pa_upp.h"

#ifdef __clang__
	#warning "Clang does not compile PortAudio correctly. You might experience hang ups when using setStreamFinishedCallback."
#endif

#ifdef flagPOSIX
	#include "os/unix/pa_unix_hostapis.c"
	#include "os/unix/pa_unix_util.c"
#else
	#include "os/win/pa_win_hostapis.c"
	#include "os/win/pa_win_util.c"
	#include "os/win/pa_win_waveformat.c"
//	#include "os/win/pa_x86_plain_converters.c"
#endif

const char* PortAudioCompileFlags(){
	return
	#ifdef flagPOSIX
		#if defined(PA_USE_ALSA)
			"ALSA "
		#endif
		#if defined(PA_USE_ASIHPI)
			"ASIHPI "
		#endif
		#if defined(PA_USE_JACK)
			"JACK "
		#endif
		#if defined(PA_USE_OSS)
			"OSS "
		#endif
	#else
		#if !defined(PA_NO_DS)
			"DSOUND "
		#endif
		#if !defined(PA_NO_WASAPI)
			"WASAPI "
		#endif
		#if !defined(PA_NO_WDMKS)
			"WDMKS "
		#endif
		#if !defined(PA_NO_WMME)
			"WMME "
		#endif
	#endif
	;
}
