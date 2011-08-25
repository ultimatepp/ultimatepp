#ifndef _Ffmpeg_Ffmpeg_h_
#define _Ffmpeg_Ffmpeg_h_

#include <SDL/SDLCtrl.h>

extern "C" {
	#include <libavutil/eval.h>		
	#include <libavutil/fifo.h>
	#include <libavutil/pixfmt.h>
	#include <libavutil/avstring.h>
	#include <libavutil/colorspace.h>				
	#include <libavutil/pixdesc.h>
	#include <libavutil/opt.h>
	#include <libavutil/error.h>
	
	#include <libavcore/imgutils.h>
	#include <libavcore/parseutils.h>
	
	#include <libavformat/avformat.h>
	
	#include <libavdevice/avdevice.h>
	
	#include <libswscale/swscale.h>
	
	#include <libavcodec/audioconvert.h>	
	#include <libavcodec/avfft.h>
} 

#if defined(_MSC_VER)
	extern AVRational myAVTIMEBASEQ;
	#undef AV_TIME_BASE_Q
	#define AV_TIME_BASE_Q	myAVTIMEBASEQ	
	#define snprintf( buf, count, format, ... )  _snprintf_s( buf, 512, count, format, __VA_ARGS__ )
#endif

#endif

