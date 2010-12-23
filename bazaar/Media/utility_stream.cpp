#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Media/ffmpeg_base.h>
#include "utility.h"


// Returns a string with the bit format of an audio stream
const char *GetStreamAudioBitFormat(AVStream *st) {
	switch(st->codec->sample_fmt) {
	case SAMPLE_FMT_U8:		return "Unsigned 8";	
	case SAMPLE_FMT_S16:	return "Signed 16";	
	case SAMPLE_FMT_S32:	return "Signed 32";	
	case SAMPLE_FMT_FLT:	return "Float";	
	}
	return "Unknown";
}

String GetStreamLanguage(AVStream *stream) {
	AVMetadataTag *lang = av_metadata_get(stream->metadata, "language", NULL, 0);
	if (lang)
		return lang->value;
	else 
		return "";
}

// Return a String with all stream tags but language
String GetStreamTags(AVStream *stream) {
	char tagStrings[1024];		
	tagStrings[0] = 0;
	AVMetadataTag *tag = NULL;
	
	while ((tag = av_metadata_get(stream->metadata, "", tag, AV_METADATA_IGNORE_SUFFIX))) {
		if (tag->key[0] != 0 && strcmp(tag->key, "language") != 0 && tag->value[0] != 0) {
	   		int ln = strlen(tagStrings);
	     	snprintf(tagStrings + ln, 1024-ln, "%s%s: %s", 
	        		tagStrings[0] != 0 ? "\n" : "", tag->key, tag->value);
		}
	}
	return tagStrings;
}


