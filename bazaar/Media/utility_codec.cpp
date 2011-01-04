#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Media/ffmpeg_base.h>
#include "utility.h"


int GetCodecBitRate(AVCodecContext *codec) {
    int bit_rate, bits_per_sample;

    switch(codec->codec_type) {
    case CODEC_TYPE_VIDEO:
    case CODEC_TYPE_DATA:
    case CODEC_TYPE_SUBTITLE:
    case CODEC_TYPE_ATTACHMENT:
        bit_rate = max(codec->bit_rate, av_get_bits_per_sample(codec->codec_id));
        break;
    case CODEC_TYPE_AUDIO:
        bits_per_sample = av_get_bits_per_sample(codec->codec_id);
        bit_rate = bits_per_sample ? codec->sample_rate * codec->channels * bits_per_sample : 
        							 codec->bit_rate;
        break;
    default:
        bit_rate = 0;
        break;
    }
    return bit_rate;
}	

size_t av_get_codec_tag_string(char *buf, size_t buf_size, unsigned int codec_tag)
{
    size_t len, ret = 0;

    for (int i = 0; i < 4; i++) {
        len = snprintf(buf, buf_size,
                       isprint(codec_tag&0xFF) ? "%c" : "[%d]", codec_tag&0xFF);
        buf      += len;
        buf_size  = buf_size > len ? buf_size - len : 0;
        ret      += len;
        codec_tag>>=8;
    }
    return ret;
}

String GetCodecName(AVCodecContext *codec, bool encode) {
  	String ret;
    AVCodec *p;
    
	if (encode)
        p = avcodec_find_encoder(codec->codec_id);
    else
        p = avcodec_find_decoder(codec->codec_id);

    if (p) 
        ret = String(p->long_name) + " (" + p->name + ")";
    else if (codec->codec_id == CODEC_ID_MPEG2TS) 
        ret = "mpeg2ts"; // fake mpeg2 transport stream codec (currently not registered) 
    else if (codec->codec_name[0] != '\0')
        ret = String(codec->codec->long_name) + " (" + codec->codec_name + ")";
    else {		// output avi tags 
        char tag_buf[32];
        av_get_codec_tag_string(tag_buf, sizeof(tag_buf), codec->codec_tag);
        ret = Format("%s / 0x%04X", tag_buf, int(codec->codec_tag));
    } 
    if (codec->codec_type == CODEC_TYPE_VIDEO) {
    	if (codec->mb_decision)
        	ret << " (hq)";
    }
    return ret;
}