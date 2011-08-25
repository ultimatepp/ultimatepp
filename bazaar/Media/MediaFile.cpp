#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "MediaFile.h"
#include "utility.h"


MediaFile::MediaFile() {
    avcodec_register_all();		// Register all codecs, demux and protocols
    avdevice_register_all();
    //avfilter_register_all();
    av_register_all();
    
    av_log_set_callback(AvLogCallback);
    
    videoq.SetMaxSize(MAX_VIDEOQ_SIZE);
    audioq.SetMaxSize(MAX_AUDIOQ_SIZE);
    subtitleq.SetMaxSize(MAX_SUBTITLEQ_SIZE);

	threadCount = 1;
    fileData = 0;
    
    requestAbort = paused = seek_req = false;
    last_paused = seek_flags = 0;
    seek_pos = seek_rel = 0;
    showAudioFps = 25;
    rgb = false;
    forceAspect = true;
    audioFactor = 1000;
    imgRect.Clear();
}

MediaFile::~MediaFile() {
    if (fileData) {
        av_close_input_file(fileData);
        fileData = NULL; /* safety */
    } 	
    //SDL_DestroyMutex(pictq_mutex);
    //SDL_DestroyCond(pictq_cond);
    //SDL_DestroyMutex(subpq_mutex);
    //SDL_DestroyCond(subpq_cond);
}

void MediaFile::Reset() {
	numVideoStreams = numAudioStreams = 0;
	videoData.Clear();
	audioData.Clear();
	
	second = 0;
    lastSecond = -1;
    
    audio_disable = video_disable = false;
    seek_by_bytes = false;
    
    audioq.Init();
    videoq.Init();
    subtitleq.Init();
    
    memset(&flush_pkt, 0, sizeof(flush_pkt));
	av_init_packet(&flush_pkt);
    flush_pkt.data = (uint8_t *)"FLUSH";
    
  	//pictq_mutex = SDL_CreateMutex();
    //pictq_cond = SDL_CreateCond();
    memset(&pictq, 0, sizeof(pictq));
    pictq_size = pictq_rindex = pictq_windex = 0;

  	//subpq_mutex = SDL_CreateMutex();
    //subpq_cond = SDL_CreateCond();
    memset(&subpq, 0, sizeof(subpq));
    subpq_size = subpq_rindex = subpq_windex = 0;
        
    audioStream = videoStream = subtitleStream = 0;
    idAudio = idVideo = idSubtitle = -1;
    
    reformat_ctx = 0;
    
    //decoderThreadId = video_tid = refresh_tid = subtitle_tid = 0;
    
    subtitle_stream_changed = false;
    debug = debug_mv = 0;
    workaround_bugs = 1;
    fast = false;
	genpts = lowres = idct = 0;
	decoder_reorder_pts= -1;
	skip_frame = skip_idct = skip_loop_filter = AVDISCARD_DEFAULT;
	audio_callback_time = 0;
	audio_buf_size = 0;
    audio_buf_index = 0;
    audio_buf = 0;
    showAudio = true;
    sample_array_index = 0;
    
  	audio_clock = audio_diff_cum = audio_diff_avg_coef = audio_diff_threshold = 0;
    audio_diff_avg_count = 0;

    audio_hw_buf_size = 0;
    
    memset(&audio_pkt_temp, 0, sizeof(AVPacket));
    memset(&audio_pkt, 0, sizeof(AVPacket));
    
    video_current_pts = 0;
    video_current_pts_time = video_current_pos = 0;
    
    av_sync_type = 0;
    
    frame_timer = 0;
    video_clock = 0;
    step = false;
    lastAudioShow = 0;
    
    decodeThEnd = decodeThEnded = audioThEnd = videoThEnd = subtitleThEnd = true;
}

int MediaFile::GetAudioId() {
	int id = GetAudio();
	for (int i = 0; i < audioData.GetCount(); ++i) {
		if (audioData[i].id == id)
			return i;
	}
	return -1;
}

void MediaFile::SetAudioId(int id) {
	if (id == -1)
		SetAudio(-1);
	else if (id < 0 || id >= audioData.GetCount())
		ASSERT(true);
	else
		SetAudio(audioData[id].id);
}
	
int MediaFile::GetVideoId() {
	int id = GetVideo();
	for (int i = 0; i < videoData.GetCount(); ++i) {
		if (videoData[i].id == id)
			return i;
	}
	return -1;
}

void MediaFile::SetVideoId(int id) {
	if (id == -1)
		SetVideo(-1);
	else if (id < 0 || id >= videoData.GetCount())
		ASSERT(true);
	else
		SetVideo(videoData[id].id);
}

void MediaFile::SetSecond(double sec) {
	if (sec < 0 || sec > GetDuration())
		return;
	
	second = sec;
	lastSecond = -1;
	sec *= AV_TIME_BASE;
	if (fileData->start_time != AV_NOPTS_VALUE)
		sec += fileData->start_time;
	stream_seek(int64_t(sec), 0, false);
}

int MediaFile::GetAudioOutputBufferSize() {
    return audio_buf_size - audio_buf_index;
}

void MediaFile::stream_seek(int64_t pos, int64_t rel, bool seek_by_bytes)
{
    if (!seek_req) {
        seek_pos = pos;
        seek_rel = rel;
        if (seek_by_bytes)
            seek_flags |= AVSEEK_FLAG_BYTE;
        else
        	seek_flags &= ~AVSEEK_FLAG_BYTE;
        seek_req = true;
    }
}

// open a given stream. Return 0 if OK 
bool MediaFile::StreamOpen(int stream_index)
{
    SDL_AudioSpec wanted_spec, spec;

    if (stream_index < 0 || unsigned(stream_index) >= fileData->nb_streams)
        return false;

    AVCodecContext *avctx = fileData->streams[stream_index]->codec;

    // prepare audio output 
    if (avctx->codec_type == AVMEDIA_TYPE_AUDIO) {
        if (avctx->channels > 0) 
            avctx->request_channels = FFMIN(2, avctx->channels);
        else 
            avctx->request_channels = 2;
    }

   	AVCodec *codec = avcodec_find_decoder(avctx->codec_id);
    avctx->debug_mv = debug_mv;
    avctx->debug = debug;
    avctx->workaround_bugs = workaround_bugs;
    avctx->lowres = lowres;
    if(lowres) 
    	avctx->flags |= CODEC_FLAG_EMU_EDGE;
    avctx->idct_algo= idct;
    if(fast) 
    	avctx->flags2 |= CODEC_FLAG2_FAST;
    avctx->skip_frame= skip_frame;
    avctx->skip_idct= skip_idct;
    avctx->skip_loop_filter= skip_loop_filter;
    
    avcodec_thread_init(avctx, threadCount);
    if (!codec || avcodec_open(avctx, codec) < 0)
        return false;

    // prepare audio output 
    if (avctx->codec_type == AVMEDIA_TYPE_AUDIO) {
        wanted_spec.freq = avctx->sample_rate;
        wanted_spec.format = AUDIO_S16SYS;
        wanted_spec.channels = avctx->channels;
        wanted_spec.silence = 0;
        wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
        wanted_spec.callback = sdl_audio_callback;
        wanted_spec.userdata = this;
        
        if (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
            SetError(Format("SDL_OpenAudio: %s", SDL_GetError()));
            return false;
        }
        
        audio_hw_buf_size = spec.size;
        audio_src_fmt = SAMPLE_FMT_S16;
    }

    fileData->streams[stream_index]->discard = AVDISCARD_DEFAULT;
    switch(avctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
        idAudio = stream_index;
        audioStream = fileData->streams[stream_index];
        audio_buf_size = 0;
        audio_buf_index = 0;

        // init averaging filter 
        audio_diff_avg_coef = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
        audio_diff_avg_count = 0;
        // since we do not have a precise enough audio fifo fullness,
        // we correct audio sync only if larger than this threshold 
        audio_diff_threshold = 2.0 * SDL_AUDIO_BUFFER_SIZE / avctx->sample_rate;

        memset(&audio_pkt, 0, sizeof(audio_pkt));
        
    	audioq.Init(flush_pkt, &flush_pkt);
    	audioThEnd = false;
    	SDL_PauseAudio(0);
    
        break;
    case AVMEDIA_TYPE_VIDEO:
        idVideo = stream_index;
        videoStream = fileData->streams[stream_index];

        //frame_last_delay = 40e-3;
        frame_timer = (double)GetUSec() / 1000000.0;
        video_current_pts_time = GetUSec();

    	videoq.Init(flush_pkt, &flush_pkt);
    	video_tid.Run(THISBACK(VideoThread));
    	//video_tid = SDL_CreateThread(VideoThread, this);

        break;
    case AVMEDIA_TYPE_SUBTITLE:
        idSubtitle = stream_index;
        subtitleStream = fileData->streams[stream_index];
        
    	subtitleq.Init(flush_pkt, &flush_pkt);
    	subtitle_tid.Run(THISBACK(SubtitleThread));
    	//subtitle_tid = SDL_CreateThread(SubtitleThread, this);
    
        break;
    default:
        break;
    }
    return true;
}

void MediaFile::StreamClose(int stream_index)
{
    if (stream_index < 0 || unsigned(stream_index) >= fileData->nb_streams)
        return;

    AVCodecContext *avctx = fileData->streams[stream_index]->codec;

    switch(avctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
    	audioq.Abort();

        SDL_CloseAudio();

        audioq.End();
        if (reformat_ctx)
            av_audio_convert_free(reformat_ctx);
        reformat_ctx = NULL;
        break;
    case AVMEDIA_TYPE_VIDEO:
        videoq.Abort();

        // note: we also signal this mutex to make sure we deblock the video thread in all cases 
        INTERLOCKED_(pictq_mutex) {
        	pictq_cond.Signal();
        }

        video_tid.Wait();

        videoq.End();
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        subtitleq.Abort();

        // note: we also signal this mutex to make sure we deblock the video thread in all cases 
        INTERLOCKED_(subpq_mutex) {
	        subtitle_stream_changed = true;
	        subpq_cond.Signal();
        }

        subtitle_tid.Wait();

        subtitleq.End();
        break;
    default:
        break;
    }

    fileData->streams[stream_index]->discard = AVDISCARD_ALL;
    avcodec_close(avctx);
    switch(avctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
        audioStream = NULL;
        //idAudio = -1;
        break;
    case AVMEDIA_TYPE_VIDEO:
        videoStream = NULL;
        //idVideo = -1;
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        subtitleStream = NULL;
        //idSubtitle = -1;
        break;
    default:
        break;
    }
}

// THREAD	
/* decode one audio frame and returns its uncompressed size */
int MediaFile::audio_decode_frame(double *pts_ptr)
{
    AVPacket *pkt_temp = &audio_pkt_temp;
    AVPacket *pkt = &audio_pkt;
    AVCodecContext *dec= audioStream->codec;
    int n, len1, data_size;
    double pts;

    for(;;) {
        // The audio packet can contain several frames 
        while (pkt_temp->size > 0) {
            data_size = sizeof(audio_buf1);
#if (((LIBAVCODEC_VERSION_MAJOR <= 52) && (LIBAVCODEC_VERSION_MINOR <= 20)) || UBUNTU_TRICK)
            len1 = avcodec_decode_audio2(dec, (int16_t *)audio_buf1, &data_size, pkt_temp->data, 
            																	 pkt_temp->size);
#else
            len1 = avcodec_decode_audio3(dec, (int16_t *)audio_buf1, &data_size, pkt_temp);                                   
#endif            
            
            if (len1 < 0) {
                pkt_temp->size = 0;	// if error, we skip the frame 
                break;
            }
            pkt_temp->data += len1;
            pkt_temp->size -= len1;
            if (data_size <= 0)
                continue;

            if (dec->sample_fmt != audio_src_fmt) {
                if (reformat_ctx)
                    av_audio_convert_free(reformat_ctx);
                reformat_ctx= av_audio_convert_alloc(SAMPLE_FMT_S16, 1,
                                                         dec->sample_fmt, 1, NULL, 0);
                if (!reformat_ctx) {
                    SetError(Format("Cannot convert %s sample format to %s sample format\n",
                        avcodec_get_sample_fmt_name(dec->sample_fmt),
                        avcodec_get_sample_fmt_name(SAMPLE_FMT_S16)));
                	break;
                }
                audio_src_fmt= dec->sample_fmt;
            }

            if (reformat_ctx) {
                const void *ibuf[6]= {audio_buf1};
                void *obuf[6]= {audio_buf2};
                int istride[6]= {av_get_bits_per_sample_format(dec->sample_fmt)/8};
                int ostride[6]= {2};
                int len= data_size/istride[0];
                if (av_audio_convert(reformat_ctx, obuf, ostride, ibuf, istride, len)<0) {
                    printf("av_audio_convert() failed\n");
                    break;
                }
                audio_buf= audio_buf2;
                /* FIXME: existing code assume that data_size equals framesize*channels*2
                          remove this legacy cruft */
                data_size= len*2;
            } else
                audio_buf= audio_buf1;

            // if no pts, then compute it 
            pts = audio_clock;
            *pts_ptr = pts;
            n = 2 * dec->channels;
            audio_clock += double(data_size) / double(n * dec->sample_rate);

            return data_size;
        }

        if (pkt->data)								// free the current packet 
            av_free_packet(pkt);

        if (paused || audioq.IsAbort()) 
            return -1;

		int ret = audioq.Get(pkt, decodeThEnd);		// read next packet 
        if (ret <= 0)		
            return ret;
        if(pkt->data == flush_pkt.data){
            avcodec_flush_buffers(dec);
            continue;
        }

        pkt_temp->data = pkt->data;
        pkt_temp->size = pkt->size;

        if (pkt->pts != AV_NOPTS_VALUE) 	// if update the audio clock with the pts 
            audio_clock = av_q2d(audioStream->time_base)*pkt->pts;
    }
}

// Copy samples for viewing in editor window 
void MediaFile::update_sample_display(short *samples, int samples_size) {
    int channels = audioStream->codec->channels;
	int size = samples_size / sizeof(short);
    while (size > 0) {
        int len = SAMPLE_ARRAY_SIZE - sample_array_index;
        if (len > size)
            len = size;
        memcpy(sample_array + sample_array_index, samples, len * sizeof(short));
        samples += len;
        sample_array_index += len;
        if (sample_array_index >= SAMPLE_ARRAY_SIZE)
            sample_array_index = 0;
        size -= len;
    }
}

// Return the new audio buffer size (samples can be added or deleted
// to get better sync if video or external master clock)
int MediaFile::synchronize_audio(short *samples, int samples_size1, double pts)
{
    int n, samples_size;
    double ref_clock;

    n = 2 * audioStream->codec->channels;
    samples_size = samples_size1;

    /* if not master, then we try to remove or add samples to correct the clock */
    if (((av_sync_type == AV_SYNC_VIDEO_MASTER && videoStream) ||
         av_sync_type == AV_SYNC_EXTERNAL_CLOCK)) {
        double diff, avg_diff;
        int wanted_size, min_size, max_size, nb_samples;

        ref_clock = get_master_clock();
        diff = get_audio_clock() - ref_clock;

        if (diff < AV_NOSYNC_THRESHOLD) {
            audio_diff_cum = diff + audio_diff_avg_coef * audio_diff_cum;
            if (audio_diff_avg_count < AUDIO_DIFF_AVG_NB) 
                audio_diff_avg_count++; // not enough measures to have a correct estimate 
            else {	
                /* estimate the A-V difference */
                avg_diff = audio_diff_cum * (1.0 - audio_diff_avg_coef);

                if (fabs(avg_diff) >= audio_diff_threshold) {
                    wanted_size = samples_size + ((int)(diff * audioStream->codec->sample_rate) * n);
                    nb_samples = samples_size / n;

                    min_size = ((nb_samples * (100 - SAMPLE_CORRECTION_PERCENT_MAX)) / 100) * n;
                    max_size = ((nb_samples * (100 + SAMPLE_CORRECTION_PERCENT_MAX)) / 100) * n;
                    if (wanted_size < min_size)
                        wanted_size = min_size;
                    else if (wanted_size > max_size)
                        wanted_size = max_size;

                    /* add or remove samples to correction the synchro */
                    if (wanted_size < samples_size) {
                        /* remove samples */
                        samples_size = wanted_size;
                    } else if (wanted_size > samples_size) {
                        uint8_t *samples_end, *q;
                        int nb;

                        /* add samples */
                        nb = (samples_size - wanted_size);
                        samples_end = (uint8_t *)samples + samples_size - n;
                        q = samples_end + n;
                        while (nb > 0) {
                            memcpy(q, samples_end, n);
                            q += n;
                            nb -= n;
                        }
                        samples_size = wanted_size;
                    }
                }
            }
        } else {
            // Too big difference : may be initial PTS errors, so reset A-V filter 
            audio_diff_avg_count = 0;
            audio_diff_cum = 0;
        }
    }
    return samples_size;
}

// Called by VideoThread to store the just decoded frame
bool MediaFile::queue_picture(AVFrame *frame, double pts) {
    // wait until we have space to put a new picture 
    INTERLOCKED_(pictq_mutex) {
    	while (pictq_size >= VIDEO_PICTURE_QUEUE_SIZE && !videoq.IsAbort()) 
        	pictq_cond.Wait(pictq_mutex);
    }

    if (videoq.IsAbort())
        return false;

    VideoPicture &vp = pictq[pictq_windex];

    // alloc or resize hardware picture buffer 
    if ((!vp.overlay && !rgb) || (!vp.surface && rgb) || vp.width != videoStream->codec->width || 
    			   vp.height != videoStream->codec->height) {
        SDL_Event event;

        vp.allocated = false;

        // the allocation must be done in the main thread to avoid locking problems 
        event.type = FF_ALLOC_EVENT;
        event.user.data1 = this;
        SDL_PushEvent(&event);

        // wait until the picture is allocated 
        INTERLOCKED_(pictq_mutex) {
        	while (!vp.allocated && !videoq.IsAbort()) 
        		pictq_cond.Wait(pictq_mutex);
        }

        if (videoq.IsAbort())
            return false;
    }
    int dwidth, dheight;
	INTERLOCKED_(mtx) {
		dwidth = imgRect.GetWidth();
		dheight = imgRect.GetHeight();
	}    
	
	SwsContext *context = 0;
	
    // if the frame is not skipped, then display it 
    if (vp.overlay) {			// Conversion to YUV420P
        context = SWSGetContext(videoStream->codec->width, videoStream->codec->height,
    							videoStream->codec->pix_fmt, 
    							videoStream->codec->width, videoStream->codec->height, 
    							PIX_FMT_YUV420P);	// Image transformed but not scaled
        if (!context) {
            SetError("Cannot initialize the conversion context");
            return false;
        }

    	SDL_LockYUVOverlay(vp.overlay);
		
		AVPicture pict;                
        memset(&pict,0,sizeof(AVPicture));
        pict.data[0] = vp.overlay->pixels[0];
        pict.data[1] = vp.overlay->pixels[2];
        pict.data[2] = vp.overlay->pixels[1];

        pict.linesize[0] = vp.overlay->pitches[0];
        pict.linesize[1] = vp.overlay->pitches[2];
        pict.linesize[2] = vp.overlay->pitches[1];
        
        sws_scale(context, frame->data, frame->linesize,
                  0, videoStream->codec->height, pict.data, pict.linesize);
         
        SDL_UnlockYUVOverlay(vp.overlay);
    } else if (vp.surface) {		// Convertion to RGB
        int pitch = vp.surface->pitch;
        PixelFormat pix_fmt_dest;
        switch (vp.surface->format->BitsPerPixel) {
     	case 32:	pix_fmt_dest = PIX_FMT_RGB32;
     				break;
     	case 24:	pix_fmt_dest = PIX_FMT_RGB24;
     				break;
     	default:	return false;
        }
		context = SWSGetContext(videoStream->codec->width, videoStream->codec->height,
    							videoStream->codec->pix_fmt, 
    							dwidth, dheight, pix_fmt_dest);	// Image transformed and scaled           			
        if (!context) {
            SetError("Cannot initialize the conversion context");
            return false;
        }
		SDL_LockSurface(vp.surface);
		
    	sws_scale(context, frame->data, frame->linesize,
                  0, videoStream->codec->height, (uint8_t* const*)&(vp.surface->pixels), &pitch);
         
        SDL_UnlockSurface(vp.surface);        
    }
    if (context)
        sws_freeContext(context);         
	if (vp.overlay || vp.surface) {
        vp.pts = pts;

        // now we can update the picture count 
        if (++pictq_windex == VIDEO_PICTURE_QUEUE_SIZE)
            pictq_windex = 0;
        INTERLOCKED_(pictq_mutex) {
        	pictq_size++;
        }
    }
    return true;
}

/* pause or resume the video */
void MediaFile::stream_pause() {
    paused = !paused;
    if (!paused) {
        video_current_pts = get_video_clock();
        frame_timer += (GetUSec() - video_current_pts_time) / 1000000.0;
    }
}

void MediaFile::SetError(String str) {
	if (!strError.IsEmpty())
		strError << "\n"; 
	strError << ToUpper(str[0]) + DeQtfLf(str.Mid(1)); 
}

bool MediaFile::BeginDecoder() {    
	return decoderThreadId.Run(THISBACK(DecoderThread));
	//decoderThreadId = SDL_CreateThread(DecoderThread, this);
    //return !!decoderThreadId;
}

