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
int MediaFile::DecoderThread(void *data)
{
    MediaFile &mf = *(MediaFile*)data;
    mf.requestAbort = false;
    mf.decodeThEnd = mf.decodeThEnded = false;
	globalMediaFile = &mf;
    url_set_interrupt_cb(decoderInterrupt_cb);
    
	int ret;	
    for(;;) {
        if (mf.requestAbort)				// Abort request
            break;
        if (mf.paused != mf.last_paused) {	// Pausing
            mf.last_paused = mf.paused;
            if (mf.paused)
                av_read_pause(mf.fileData);	// Pauses a network based stream
            else
                av_read_play(mf.fileData);
        }
        if (mf.seek_req) {					// Somebody asked a seek
            mf.decodeThEnd = false;
            int64_t seek_target= mf.seek_pos;
            int64_t seek_min = mf.seek_rel > 0 ? seek_target - mf.seek_rel + 2: INT64_MIN;
            int64_t seek_max = mf.seek_rel < 0 ? seek_target - mf.seek_rel - 2: INT64_MAX;

            if (0 > avformat_seek_file(mf.fileData, -1, seek_min, seek_target, seek_max, mf.seek_flags))
                mf.SetError(Format("%s: error while seeking", mf.GetFileName()));
            else {
                if (mf.idAudio >= 0) {
                    mf.audioq.Flush();
                    mf.audioq.Put(&mf.flush_pkt, &mf.flush_pkt);
                }
                if (mf.idSubtitle >= 0) {
                    mf.subtitleq.Flush();
                    mf.subtitleq.Put(&mf.flush_pkt, &mf.flush_pkt);
                }
                if (mf.idVideo >= 0) {
                    mf.videoq.Flush();
                    mf.videoq.Put(&mf.flush_pkt, &mf.flush_pkt);
                }
            }
            mf.seek_req = false;
        }

        // If any of the queues is full, it waits
        if (mf.audioq.IsFull() || mf.videoq.IsFull() || mf.subtitleq.IsFull()) {
            Sleep(10);
            continue;
        }
        AVPacket pkt;
        if(url_feof(mf.fileData->pb)) {
            /*av_init_packet(&pkt);
            pkt.data = NULL;
            pkt.size = 0;
            pkt.stream_index = mf.idVideo;
            mf.videoq.Put(&pkt, &mf.flush_pkt);
            continue;*/
            break;
        }
        ret = av_read_frame(mf.fileData, &pkt);
        if (ret < 0) {
            if (ret != AVERROR_EOF && url_ferror(mf.fileData->pb) == 0) {
                Sleep(100); 	// Wait for user event 
                continue;
            } else
                break;
        }
        if (pkt.stream_index == mf.idAudio) 
            mf.audioq.Put(&pkt, &mf.flush_pkt);
        else if (pkt.stream_index == mf.idVideo) 
            mf.videoq.Put(&pkt, &mf.flush_pkt);
        else if (pkt.stream_index == mf.idSubtitle) 
            mf.subtitleq.Put(&pkt, &mf.flush_pkt);
        else 
            av_free_packet(&pkt);
    }
    mf.decodeThEnd = true;		// Ask PaquetQeue to stop, so all threads end
    
    while (!mf.requestAbort && !(mf.audioThEnd && mf.videoThEnd && mf.subtitleThEnd))// Wait until end 
        Sleep(100);

    // Close each stream 
    if (mf.idAudio >= 0)
        mf.StreamClose(mf.idAudio);
    if (mf.idVideo >= 0)
        mf.StreamClose(mf.idVideo);
    if (mf.idSubtitle >= 0)
        mf.StreamClose(mf.idSubtitle);

    url_set_interrupt_cb(NULL);

	mf.decodeThEnded = true;
	
    return 0;
}

int MediaFile::VideoThread(void *data)
{
    MediaFile &mf = *(MediaFile *)data;
    AVFrame *frame= avcodec_alloc_frame();
    mf.videoThEnd = false;

    for(;;) {
        while (mf.paused && !mf.videoq.IsAbort()) 
            Sleep(10);
        
        AVPacket pkt;
        if (mf.videoq.Get(&pkt, mf.decodeThEnd) <= 0)				// Gets a video packet
            break;

        if(pkt.data == mf.flush_pkt.data) 
            avcodec_flush_buffers(mf.videoStream->codec);
      	else {
	        mf.videoStream->codec->reordered_opaque = pkt.pts;
	        int got_picture;
			// It does not check for errors
	        int len1 = avcodec_decode_video2(mf.videoStream->codec, frame, &got_picture, &pkt);
			double pts;
	        if((mf.decoder_reorder_pts || pkt.dts == AV_NOPTS_VALUE) &&
	           (frame->reordered_opaque != AV_NOPTS_VALUE))
	            pts = double(frame->reordered_opaque);
	        else if(pkt.dts != AV_NOPTS_VALUE)
	            pts = double(pkt.dts);
	        else
	            pts = 0;
	        pts *= av_q2d(mf.videoStream->time_base);

	        if (got_picture) {		// Compute video clock
    			double frame_delay;

    			if (pts != 0) 
        			mf.video_clock = pts;	// Update video clock with pts, if present 
    
			    // Update video clock for next frame 
			    frame_delay = av_q2d(mf.videoStream->codec->time_base);
			    
			    // For MPEG2, the frame can be repeated, so we update the clock accordingly 
			    frame_delay += frame->repeat_pict * (frame_delay * 0.5);
			    mf.video_clock += frame_delay;

				if (!mf.queue_picture(frame, pts))
	                break;
	        }
	        av_free_packet(&pkt);
	        if (mf.step)
	         	mf.stream_pause();
      	}
    }
    av_free(frame);
    mf.videoThEnd = true;
    return 0;
}

int MediaFile::SubtitleThread(void *data)
{
    MediaFile &mf = *(MediaFile *)data;
    mf.subtitleThEnd = false;
    
    for(;;) {
        while (mf.paused && !mf.subtitleq.IsAbort()) 
            Sleep(10);
        
        AVPacket pkt;
        if (mf.subtitleq.Get(&pkt, 1) < 0)
            break;

        if(pkt.data == mf.flush_pkt.data){
            avcodec_flush_buffers(mf.subtitleStream->codec);
            continue;
        }
        SDL_LockMutex(mf.subpq_mutex);
        while (mf.subpq_size >= SUBPICTURE_QUEUE_SIZE && !mf.subtitleq.IsAbort()) 
            SDL_CondWait(mf.subpq_cond, mf.subpq_mutex);
        
        SDL_UnlockMutex(mf.subpq_mutex);

        if (mf.subtitleq.IsAbort())
    		return 0;

        SubPicture *sp = &mf.subpq[mf.subpq_windex];

        double pts = 0;
        if (pkt.pts != AV_NOPTS_VALUE)
            pts = av_q2d(mf.subtitleStream->time_base)*pkt.pts;

		int got_subtitle;
		// It does not check for errors
        int len1 = avcodec_decode_subtitle2(mf.subtitleStream->codec, &sp->sub, &got_subtitle, 
        									&pkt);
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
            if (++mf.subpq_windex == SUBPICTURE_QUEUE_SIZE)	// New picture is added
                mf.subpq_windex = 0;
            SDL_LockMutex(mf.subpq_mutex);
            mf.subpq_size++;
            SDL_UnlockMutex(mf.subpq_mutex);
        }
        av_free_packet(&pkt);
    }
    mf.subtitleThEnd = true;
    return 0;
}

// Prepare a new audio buffer 
void sdl_audio_callback(void *data, Uint8 *stream, int len)
{
    MediaFile &mf = *(MediaFile *)data;

    mf.audio_callback_time = av_gettime();

    while (len > 0) {
        if (unsigned(mf.audio_buf_index) >= mf.audio_buf_size) {
      		double pts;
           	int audio_size = mf.audio_decode_frame(&pts);
           	if (audio_size < 0) {
             	mf.audio_buf = mf.audio_buf1;
               	mf.audio_buf_size = 1024;
               	memset(mf.audio_buf, 0, mf.audio_buf_size);	// If error, it outputs silence 
           	} else if (audio_size == 0)
           		mf.audioThEnd = true;
           	else {
               	if (mf.showAudio)
                   	mf.update_sample_display((int16_t *)mf.audio_buf, audio_size);
               	audio_size = mf.synchronize_audio((int16_t *)mf.audio_buf, audio_size, pts);
               	mf.audio_buf_size = audio_size;
           	}
           	mf.audio_buf_index = 0;
        }
        int len1 = mf.GetAudioOutputBufferSize();
        if (len1 > len)
            len1 = len;
        memcpy(stream, (uint8_t *)mf.audio_buf + mf.audio_buf_index, len1);
        len -= len1;
        stream += len1;
        mf.audio_buf_index += len1;
    }
}
