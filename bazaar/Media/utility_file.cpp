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



