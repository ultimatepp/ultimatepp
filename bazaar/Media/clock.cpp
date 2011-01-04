#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Media/MediaFile.h>
#include "utility.h"


// Get the current audio clock value
double MediaFile::get_audio_clock()
{
    double pts = audio_clock;
    int bytes_per_sec = 0;
    if (audioStream) 
        bytes_per_sec = audioStream->codec->sample_rate * 2 * audioStream->codec->channels;
    
    if (bytes_per_sec)
        pts -= double(GetAudioOutputBufferSize()) / bytes_per_sec;
    return pts;
}

// Get the current video clock value 
double MediaFile::get_video_clock()
{
    double delta;
    if (paused) 
        delta = 0;
    else 
        delta = (GetUSec() - video_current_pts_time) / 1000000.0;
    
    return video_current_pts + delta;
}

// Get the current external clock value 
double MediaFile::get_external_clock()
{
    return externalClock.Elapsed() / 1000.0;
}

// Get the current master clock value 
double MediaFile::get_master_clock()
{
    double val;

    if (av_sync_type == AV_SYNC_VIDEO_MASTER) {
        if (videoStream)
            val = get_video_clock();
        else
            val = get_audio_clock();
    } else if (av_sync_type == AV_SYNC_AUDIO_MASTER) {
        if (audioStream)
            val = get_audio_clock();
        else
            val = get_video_clock();
    } else 
        val = get_external_clock();
    
    return val;
}
