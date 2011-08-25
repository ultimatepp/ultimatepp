#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "MediaPlayer.h"
#include "utility.h"
#include "mediaplayer_in.h"

thread__ MediaFile *globalMediaFile;

int decoderInterrupt_cb() {
    return (globalMediaFile && globalMediaFile->requestAbort);
}

// MediaFile streams retrieval and storage in packets
void MediaFile::DecoderThread()//void *data)
{
    //MediaFile &mf = *this;// *(MediaFile*)data;
    requestAbort = false;
    decodeThEnd = decodeThEnded = false;
	globalMediaFile = this;
    url_set_interrupt_cb(decoderInterrupt_cb);
    
	int ret;	
    for(;;) {
        if (requestAbort)				// Abort request
            break;
        if (paused != last_paused) {	// Pausing
            last_paused = paused;
            if (paused)
                av_read_pause(fileData);// Pauses a network based stream
            else
                av_read_play(fileData);
        }
        if (seek_req) {					// Somebody asked a seek
            decodeThEnd = false;
            int64_t seek_target= seek_pos;
            int64_t seek_min = seek_rel > 0 ? seek_target - seek_rel + 2: INT64_MIN;
            int64_t seek_max = seek_rel < 0 ? seek_target - seek_rel - 2: INT64_MAX;

            if (0 > avformat_seek_file(fileData, -1, seek_min, seek_target, seek_max, seek_flags))
                SetError(Format("%s: error while seeking", GetFileName()));
            else {
                if (idAudio >= 0) {
                    audioq.Flush();
                    audioq.Put(&flush_pkt, &flush_pkt);
                }
                if (idSubtitle >= 0) {
                    subtitleq.Flush();
                    subtitleq.Put(&flush_pkt, &flush_pkt);
                }
                if (idVideo >= 0) {
                    videoq.Flush();
                    videoq.Put(&flush_pkt, &flush_pkt);
                }
            }
            seek_req = false;
        }

        // If any of the queues is full, it waits
        if (audioq.IsFull() || videoq.IsFull() || subtitleq.IsFull()) {
            Sleep(10);
            continue;
        }
        AVPacket pkt;
        if(url_feof(fileData->pb)) {
            /*av_init_packet(&pkt);
            pkt.data = NULL;
            pkt.size = 0;
            pkt.stream_index = idVideo;
            videoq.Put(&pkt, &flush_pkt);
            continue;*/
            break;
        }
        ret = av_read_frame(fileData, &pkt);
        if (ret < 0) {
            if (ret != AVERROR_EOF && url_ferror(fileData->pb) == 0) {
                Sleep(100); 	// Wait for user event 
                continue;
            } else
                break;
        }
        if (pkt.stream_index == idAudio) 
            audioq.Put(&pkt, &flush_pkt);
        else if (pkt.stream_index == idVideo) 
            videoq.Put(&pkt, &flush_pkt);
        else if (pkt.stream_index == idSubtitle) 
            subtitleq.Put(&pkt, &flush_pkt);
        else 
            av_free_packet(&pkt);
    }
    decodeThEnd = true;		// Ask PaquetQeue to stop, so all threads end
    
    while (!requestAbort && !(audioThEnd && videoThEnd && subtitleThEnd))// Wait until end 
        Sleep(100);

    // Close each stream 
    if (idAudio >= 0)
        StreamClose(idAudio);
    if (idVideo >= 0)
        StreamClose(idVideo);
    if (idSubtitle >= 0)
        StreamClose(idSubtitle);

    url_set_interrupt_cb(NULL);

	decodeThEnded = true;
	
    //return 0;
}

void MediaFile::VideoThread() //void *data)
{
    //MediaFile &mf = *this;	// *(MediaFile *)data;
    AVFrame *frame= avcodec_alloc_frame();
    videoThEnd = false;

    for(;;) {
        while (paused && !videoq.IsAbort()) 
            Sleep(10);
        
        AVPacket pkt;
        if (videoq.Get(&pkt, decodeThEnd) <= 0)				// Gets a video packet
            break;

        if(pkt.data == flush_pkt.data) 
            avcodec_flush_buffers(videoStream->codec);
      	else {
	        videoStream->codec->reordered_opaque = pkt.pts;
	        int got_picture;
			// It does not check for errors
#if (((LIBAVCODEC_VERSION_MAJOR <= 52) && (LIBAVCODEC_VERSION_MINOR <= 20)) || UBUNTU_TRICK)
        	int len1 = avcodec_decode_video(videoStream->codec, frame, &got_picture, pkt.data, pkt.size);
#else
        	int len1 = avcodec_decode_video2(videoStream->codec, frame, &got_picture, &pkt);
#endif

			double pts;
	        if((decoder_reorder_pts || pkt.dts == AV_NOPTS_VALUE) &&
	           (frame->reordered_opaque != AV_NOPTS_VALUE))
	            pts = double(frame->reordered_opaque);
	        else if(pkt.dts != AV_NOPTS_VALUE)
	            pts = double(pkt.dts);
	        else
	            pts = 0;
	        pts *= av_q2d(videoStream->time_base);

	        if (got_picture) {		// Compute video clock
    			double frame_delay;

    			if (pts != 0) 
        			video_clock = pts;	// Update video clock with pts, if present 
    
			    // Update video clock for next frame 
			    frame_delay = av_q2d(videoStream->codec->time_base);
			    
			    // For MPEG2, the frame can be repeated, so we update the clock accordingly 
			    frame_delay += frame->repeat_pict * (frame_delay * 0.5);
			    video_clock += frame_delay;

				if (!queue_picture(frame, pts))
	                break;
	        }
	        av_free_packet(&pkt);
	        if (step)
	         	stream_pause();
      	}
    }
    av_free(frame);
    videoThEnd = true;
    //return 0;
}

void MediaFile::SubtitleThread() //void *data)
{
    //MediaFile &mf = *this;	// *(MediaFile *)data;
    subtitleThEnd = false;
    
    for(;;) {
        while (paused && !subtitleq.IsAbort()) 
            Sleep(10);
        
        AVPacket pkt;
        if (subtitleq.Get(&pkt, 1) < 0)
            break;

        if(pkt.data == flush_pkt.data){
            avcodec_flush_buffers(subtitleStream->codec);
            continue;
        }
        INTERLOCKED_(subpq_mutex) {
	        while (subpq_size >= SUBPICTURE_QUEUE_SIZE && !subtitleq.IsAbort()) 
	            subpq_cond.Wait(subpq_mutex);
    	}

        if (subtitleq.IsAbort())
    		return ;//0;

        SubPicture *sp = &subpq[subpq_windex];

        double pts = 0;
        if (pkt.pts != AV_NOPTS_VALUE)
            pts = av_q2d(subtitleStream->time_base)*pkt.pts;

		int got_subtitle;
		// It does not check for errors
#if (((LIBAVCODEC_VERSION_MAJOR <= 52) && (LIBAVCODEC_VERSION_MINOR <= 20)) || UBUNTU_TRICK)
        int len1 = avcodec_decode_subtitle(subtitleStream->codec, &sp->sub, &got_subtitle, 
        									pkt.data, pkt.size);
#else
        int len1 = avcodec_decode_subtitle2(subtitleStream->codec, &sp->sub, &got_subtitle, 
        									&pkt);
#endif        									
        if (got_subtitle && sp->sub.format == 0) {
            sp->pts = pts;

            for (unsigned i = 0; i < sp->sub.num_rects; i++) {
                for (int j = 0; j < sp->sub.rects[i]->nb_colors; j++) {
                    int r, g, b, y, u, v, a;
                    RGBA_IN(r, g, b, a, (uint32_t*)sp->sub.rects[i]->pict.data[1] + j);
                    y = RGB_TO_Y_CCIR(r, g, b);
                    u = RGB_TO_U_CCIR(r, g, b, 0);
                    v = RGB_TO_V_CCIR(r, g, b, 0);
                    YUVA_OUT((uint32_t*)sp->sub.rects[i]->pict.data[1] + j, y, u, v, a);
                }
            }
            if (++subpq_windex == SUBPICTURE_QUEUE_SIZE)	// New picture is added
                subpq_windex = 0;
            INTERLOCKED_(subpq_mutex) {
            	subpq_size++;
            }
        }
        av_free_packet(&pkt);
    }
    subtitleThEnd = true;
    //return 0;
}

// Prepare a new audio buffer 
void sdl_audio_callback(void *data, Uint8 *stream, int len)
{
    MediaFile &mf = *(MediaFile *)data;

    mf.audio_callback_time = GetUSec();

    while (len > 0) {
        if (unsigned(mf.audio_buf_index) >= mf.audio_buf_size) {
      		double pts;
           	int audio_size = mf.audio_decode_frame(&pts);
           	if (audio_size < 0) {		// If error, it outputs silence 
             	mf.audio_buf = mf.audio_buf1;
               	mf.audio_buf_size = 1024;
               	memset(mf.audio_buf, 0, mf.audio_buf_size);
           	} else if (audio_size == 0)	// Audio is ended
           		mf.audioThEnd = true;
           	else {
               	if (mf.showAudio)
                   	mf.update_sample_display((int16_t *)mf.audio_buf, audio_size);
               	audio_size = mf.synchronize_audio((int16_t *)mf.audio_buf, audio_size, pts);
               	mf.audio_buf_size = audio_size;
           	}
           	mf.audio_buf_index = 0;
        }
        int len1 = min(len, mf.GetAudioOutputBufferSize());
        //memcpy(stream, (uint8_t *)mf.audio_buf + mf.audio_buf_index, len1);
        int16 *audioStr = (int16 *)(mf.audio_buf + mf.audio_buf_index);
        int16 *destStream = (int16 *)stream;
        for (int i = 0; i < len1/2; ++i) {
         	double val = audioStr[i]*mf.audioFactor/1000.;	// Meter tambien un "WhenAudioFrame"
          	if (val < -32767+2)		// To avoid audio saturation. A bit less than the limit
            	val = -32767+2;
           	else if (val > 32768-2)
              	val = 32768-2;
        	destStream[i] = int16(val); 
        }
        len -= len1; 
        stream += len1;
        mf.audio_buf_index += len1;
    }
}
