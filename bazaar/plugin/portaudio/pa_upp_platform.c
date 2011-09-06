#include "pa_upp.h"

#ifdef __clang__
	#warning "Clang has sometimes problems to compile PortAudio correctly. Even after succesfull compilation, you might experience hang ups when using setStreamFinishedCallback."
#endif

#ifdef flagPOSIX
	#include "os/unix/pa_unix_hostapis.c"
	#include "os/unix/pa_unix_util.c"
#else
	#include "os/win/pa_win_hostapis.c"
	#include "os/win/pa_win_util.c"
	#include "os/win/pa_win_waveformat.c"
	#include "os/win/pa_win_coinitialize.c"
//	#include "os/win/pa_x86_plain_converters.c"
#endif

const char* PortAudioCompileFlags(){
	static const char* flags=
	#ifdef flagPOSIX
		#if PA_USE_ALSA
			" ALSA"
		#endif
		#if PA_USE_ASIHPI
			" ASIHPI"
		#endif
		#if PA_USE_JACK
			" JACK"
		#endif
		#if PA_USE_OSS
			" OSS"
		#endif
	#else
		#if PA_USE_DS
			" DSOUND"
		#endif
		#if PA_USE_WASAPI
			" WASAPI"
		#endif
		#if PA_USE_WDMKS
			" WDMKS"
		#endif
		#if PA_USE_WMME
			" WMME"
		#endif
	#endif
	;
	return flags+1;
}
