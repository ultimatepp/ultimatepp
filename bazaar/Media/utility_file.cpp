#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Media/ffmpeg_base.h>
#include "utility.h"


// Return a String with all file tags
String GetTags(AVFormatContext *fileData) {
    char tagStrings[1024];		
	tagStrings[0] = 0;
    AVMetadataTag *tag = NULL;
    
  	//av_metadata_conv(fileData, NULL, fileData->iformat->metadata_conv);
    while ((tag = av_metadata_get(fileData->metadata, "", tag, AV_METADATA_IGNORE_SUFFIX))) {
        if (tag->key[0] != 0 && tag->value[0] != 0) {
	        int ln = strlen(tagStrings);
	        snprintf(tagStrings + ln, 1024-ln, "%s%s: %s", 
	        		 tagStrings[0] != 0 ? "\n" : "", tag->key, tag->value);
        }
    }
    return tagStrings;
}

String GetAvError(int err) {
    char errbuf[128];
    const char *errbuf_ptr = errbuf;

    if (av_strerror(err, errbuf, sizeof(errbuf)) < 0)
        errbuf_ptr = strerror(AVUNERROR(err));
    return errbuf_ptr;
}

int handle_jpeg(enum PixelFormat *format)
{
    switch (*format) {
    case PIX_FMT_YUVJ420P: *format = PIX_FMT_YUV420P; return 1;
    case PIX_FMT_YUVJ422P: *format = PIX_FMT_YUV422P; return 1;
    case PIX_FMT_YUVJ444P: *format = PIX_FMT_YUV444P; return 1;
    case PIX_FMT_YUVJ440P: *format = PIX_FMT_YUV440P; return 1;
    default:                                          return 0;
    }
}

SwsContext *SWSGetContext(int width, int height, PixelFormat pix_fmt, 
						  int d_width, int d_height, PixelFormat d_pix_fmt) {
//#if (LIBSWSCALE_VERSION_MAJOR <= 0 && LIBSWSCALE_VERSION_MINOR < 12)
    return sws_getContext(width, height, pix_fmt, d_width, d_height, d_pix_fmt, 
    					  SWS_BILINEAR/*SWS_BICUBIC*/, NULL, NULL, NULL);
/*#else
	SwsContext *context = sws_alloc_context();
	if (context) {
        av_set_int(context, "srcw", width);
        av_set_int(context, "srch", height);
        av_set_int(context, "src_format", pix_fmt);
        av_set_int(context, "src_range", handle_jpeg(&pix_fmt));
        av_set_int(context, "dstw", width);
        av_set_int(context, "dsth", height);
        av_set_int(context, "dst_format", dst_pix_fmt);
        av_set_int(context, "dst_range", handle_jpeg((PixelFormat*)&dst_pix_fmt));
        av_set_int(context, "sws_flags", SWS_BICUBIC);

        if (sws_init_context(context, NULL, NULL) < 0) {
            //SetError("Cannot initialize resampling context");
            sws_freeContext(context);
            return 0;
        }
    }
    return context;
#endif    */
}

void AvLogCallback(void* ptr, int level, const char* fmt, va_list vl)
{
	if (level > AV_LOG_ERROR)
		return;
	char str[1024];
	snprintf(str, 4, "%3d ", level);
    vsnprintf(str+4, 1024-4, fmt, vl);
    int kk = 1;
}