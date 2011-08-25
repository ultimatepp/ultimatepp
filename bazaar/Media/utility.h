#ifndef _Media_utility_h_
#define _Media_utility_h_

#include "audioSystem.h"

String GetTags(AVFormatContext *fileData);
String GetAvError(int err);
SwsContext *SWSGetContext(int width, int height, PixelFormat pix_fmt, 
						  int d_width, int d_height, PixelFormat d_pix_fmt);
void AvLogCallback(void* ptr, int level, const char* fmt, va_list vl);
	
const char *GetStreamAudioBitFormat(AVStream *stream);
String GetStreamTags(AVStream *stream);
String GetStreamLanguage(AVStream *stream);

int GetCodecBitRate(AVCodecContext *codec);
String GetCodecName(AVCodecContext *codec, bool encode);

inline int compute_mod(int a, int b)
{
    a = a % b;
    if (a >= 0)
        return a;
    else
        return a + b;
}

int64 GetUSec();


#endif
