#ifndef _Media_utility_h_
#define _Media_utility_h_


String GetTags(AVFormatContext *fileData);
String GetAvError(int err);

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

#endif
