#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "MediaFile.h"
#include "MediaPlayer.h"
#include "utility.h"
#include "mediaplayer_in.h"


static inline void fill_rectangle(SDL_Surface *screen,
                                  int x, int y, int w, int h, int color)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_FillRect(screen, &rect, color);
}


#define BPP 1

static void blend_subrect(AVPicture *dst, const AVSubtitleRect *rect, int imgw, int imgh)
{
    int wrap, wrap3, width2, skip2;
    int y, u, v, a, u1, v1, a1, w, h;
    uint8_t *lum, *cb, *cr;
    const uint8_t *p;
    const uint32_t *pal;
    int dstx, dsty, dstw, dsth;

    dstw = av_clip(rect->w, 0, imgw);
    dsth = av_clip(rect->h, 0, imgh);
    dstx = av_clip(rect->x, 0, imgw - dstw);
    dsty = av_clip(rect->y, 0, imgh - dsth);
    lum = dst->data[0] + dsty * dst->linesize[0];
    cb = dst->data[1] + (dsty >> 1) * dst->linesize[1];
    cr = dst->data[2] + (dsty >> 1) * dst->linesize[2];

    width2 = ((dstw + 1) >> 1) + (dstx & ~dstw & 1);
    skip2 = dstx >> 1;
    wrap = dst->linesize[0];
    wrap3 = rect->pict.linesize[0];
    p = rect->pict.data[0];
    pal = (const uint32_t *)rect->pict.data[1];  /* Now in YCrCb! */

    if (dsty & 1) {
        lum += dstx;
        cb += skip2;
        cr += skip2;

        if (dstx & 1) {
            YUVA_IN(y, u, v, a, p, pal);
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
            cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
            cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
            cb++;
            cr++;
            lum++;
            p += BPP;
        }
        for(w = dstw - (dstx & 1); w >= 2; w -= 2) {
            YUVA_IN(y, u, v, a, p, pal);
            u1 = u;
            v1 = v;
            a1 = a;
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

            YUVA_IN(y, u, v, a, p + BPP, pal);
            u1 += u;
            v1 += v;
            a1 += a;
            lum[1] = ALPHA_BLEND(a, lum[1], y, 0);
            cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 1);
            cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 1);
            cb++;
            cr++;
            p += 2 * BPP;
            lum += 2;
        }
        if (w) {
            YUVA_IN(y, u, v, a, p, pal);
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
            cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
            cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
            p++;
            lum++;
        }
        p += wrap3 - dstw * BPP;
        lum += wrap - dstw - dstx;
        cb += dst->linesize[1] - width2 - skip2;
        cr += dst->linesize[2] - width2 - skip2;
    }
    for(h = dsth - (dsty & 1); h >= 2; h -= 2) {
        lum += dstx;
        cb += skip2;
        cr += skip2;

        if (dstx & 1) {
            YUVA_IN(y, u, v, a, p, pal);
            u1 = u;
            v1 = v;
            a1 = a;
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
            p += wrap3;
            lum += wrap;
            YUVA_IN(y, u, v, a, p, pal);
            u1 += u;
            v1 += v;
            a1 += a;
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
            cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 1);
            cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 1);
            cb++;
            cr++;
            p += -wrap3 + BPP;
            lum += -wrap + 1;
        }
        for(w = dstw - (dstx & 1); w >= 2; w -= 2) {
            YUVA_IN(y, u, v, a, p, pal);
            u1 = u;
            v1 = v;
            a1 = a;
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

            YUVA_IN(y, u, v, a, p + BPP, pal);
            u1 += u;
            v1 += v;
            a1 += a;
            lum[1] = ALPHA_BLEND(a, lum[1], y, 0);
            p += wrap3;
            lum += wrap;

            YUVA_IN(y, u, v, a, p, pal);
            u1 += u;
            v1 += v;
            a1 += a;
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

            YUVA_IN(y, u, v, a, p + BPP, pal);
            u1 += u;
            v1 += v;
            a1 += a;
            lum[1] = ALPHA_BLEND(a, lum[1], y, 0);

            cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 2);
            cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 2);

            cb++;
            cr++;
            p += -wrap3 + 2 * BPP;
            lum += -wrap + 2;
        }
        if (w) {
            YUVA_IN(y, u, v, a, p, pal);
            u1 = u;
            v1 = v;
            a1 = a;
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
            p += wrap3;
            lum += wrap;
            YUVA_IN(y, u, v, a, p, pal);
            u1 += u;
            v1 += v;
            a1 += a;
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
            cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u1, 1);
            cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v1, 1);
            cb++;
            cr++;
            p += -wrap3 + BPP;
            lum += -wrap + 1;
        }
        p += wrap3 + (wrap3 - dstw * BPP);
        lum += wrap + (wrap - dstw - dstx);
        cb += dst->linesize[1] - width2 - skip2;
        cr += dst->linesize[2] - width2 - skip2;
    }
    /* handle odd height */
    if (h) {
        lum += dstx;
        cb += skip2;
        cr += skip2;

        if (dstx & 1) {
            YUVA_IN(y, u, v, a, p, pal);
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
            cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
            cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
            cb++;
            cr++;
            lum++;
            p += BPP;
        }
        for(w = dstw - (dstx & 1); w >= 2; w -= 2) {
            YUVA_IN(y, u, v, a, p, pal);
            u1 = u;
            v1 = v;
            a1 = a;
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);

            YUVA_IN(y, u, v, a, p + BPP, pal);
            u1 += u;
            v1 += v;
            a1 += a;
            lum[1] = ALPHA_BLEND(a, lum[1], y, 0);
            cb[0] = ALPHA_BLEND(a1 >> 2, cb[0], u, 1);
            cr[0] = ALPHA_BLEND(a1 >> 2, cr[0], v, 1);
            cb++;
            cr++;
            p += 2 * BPP;
            lum += 2;
        }
        if (w) {
            YUVA_IN(y, u, v, a, p, pal);
            lum[0] = ALPHA_BLEND(a, lum[0], y, 0);
            cb[0] = ALPHA_BLEND(a >> 2, cb[0], u, 0);
            cr[0] = ALPHA_BLEND(a >> 2, cr[0], v, 0);
        }
    }
}

static void free_subpicture(SubPicture *sp) 
{
    avsubtitle_free(&sp->sub);
}

void MediaPlayer::video_image_display()
{
    VideoPicture &vp = pictq[pictq_rindex];
    if (!vp.overlay && !vp.surface) 
        return;
    if (subtitleStream) {
        if (subpq_size > 0) {
            SubPicture *sp = &subpq[subpq_rindex];

            if (vp.pts >= sp->pts + (float(sp->sub.start_display_time)/1000.)) {
                SDL_LockYUVOverlay(vp.overlay);
				
				AVPicture pict;
                pict.data[0] = vp.overlay->pixels[0];
                pict.data[1] = vp.overlay->pixels[2];
                pict.data[2] = vp.overlay->pixels[1];

                pict.linesize[0] = vp.overlay->pitches[0];
                pict.linesize[1] = vp.overlay->pitches[2];
                pict.linesize[2] = vp.overlay->pitches[1];

                for (unsigned i = 0; i < sp->sub.num_rects; i++)
                    blend_subrect(&pict, sp->sub.rects[i], vp.overlay->w, vp.overlay->h);

                SDL_UnlockYUVOverlay(vp.overlay);
            }
        }
    }
    // We suppose the screen has a 1.0 pixel ratio 
	SDL_Rect rect;
	INTERLOCKED_(mtx) {
		rect.x = imgRect.left;
		rect.y = imgRect.top;
		rect.w = imgRect.GetWidth();
		rect.h = imgRect.GetHeight();
	}	
	if (vp.overlay)
    	SDL_DisplayYUVOverlay(vp.overlay, &rect);
	else if (vp.surface) {
		if (WhenFrame) {
			SDLSurface srf(vp.surface);
			WhenFrame(srf);
		}
		SDL_BlitSurface(vp.surface, 0, screen, &rect);
		SDL_Flip(screen);
	}
}

void MediaPlayer::Layout() {
	SDLCtrl::Layout();
	
	if (!screen)
		return;
	INTERLOCKED_(mtx) {
		if (forceAspect) {			// Fit image to screen considering aspect ratio
			int cx = screen->w;
			int cy = screen->h;
			double frameAspect  = cx/double(cy); 
			if (frameAspect > aspect_ratio) {	
				double x = (cx - aspect_ratio*cy)/2.;
				imgRect.Set(int(x), 0, int(x + aspect_ratio*cy), cy);
			} else {
				double y = (cy - cx/aspect_ratio)/2.;
				imgRect.Set(0, int(y), cx, int(y + cx/aspect_ratio));
			}
		} else 
			imgRect.Set(0, 0, screen->w, screen->h);
	}
}

// Allocate a picture (needs to do that in main thread to avoid potential locking problems)
bool MediaPlayer::alloc_picture()
{
    VideoPicture &vp = pictq[pictq_windex];

    if (vp.overlay)
        SDL_FreeYUVOverlay(vp.overlay);
    else if (vp.surface)
		SDL_FreeSurface(vp.surface);
    
	if (videoStream) {
	    vp.width   = videoStream->codec->width;
	    vp.height  = videoStream->codec->height;
	    vp.pix_fmt = videoStream->codec->pix_fmt;
	} else {
		vp.width   = screen->w;
	    vp.height  = screen->h;
	    vp.pix_fmt = PIX_FMT_YUV420P;
	}
	if (rgb) {
		vp.surface = SDL_CreateRGBSurface(0, vp.width, vp.height, 24, 0x0000FF, 0x00FF00, 0xFF0000, 0);
		vp.overlay = 0;
	} else {
    	vp.overlay = SDL_CreateYUVOverlay(vp.width, vp.height, SDL_YV12_OVERLAY, screen);
    	vp.surface = 0;
	}
 
    if ((rgb && !vp.surface) ||
    	(!rgb && (!vp.overlay || vp.overlay->pitches[0] < vp.width))) {	
        SDLCtrl::SetError(Format("Error: the video system does not support an image\n"
                        "size of %dx%d pixels.", vp.width, vp.height));
        return false;
    }

    INTERLOCKED_(pictq_mutex) {
    	vp.allocated = true;
    	pictq_cond.Signal();
    }
    return true;
}

void MediaPlayer::video_audioWaveform()
{
    int i_start, x, y1, y, ys, delay, n, nb_display_channels;
    int ch, channels, h, h2, bgcolor, fgcolor;
    int16_t time_diff;
    int rdft_bits, nb_freq;

    for(rdft_bits=1; (1<<rdft_bits)<2*screen->h; rdft_bits++)
        ;
    nb_freq= 1<<(rdft_bits-1);

    // compute display index : center on currently output samples 
    channels = audioStream->codec->channels;
    nb_display_channels = channels;
    if (!paused) {
        int data_used = screen->w;
        n = 2 * channels;
        delay = GetAudioOutputBufferSize();
        delay /= n;

        // to be more precise, we take into account the time spent since the last buffer computation 
        if (audio_callback_time) {
            time_diff = int16_t(GetUSec() - audio_callback_time);
            delay -= (time_diff * audioStream->codec->sample_rate) / 1000000;
        }

        delay += 2*data_used;
        if (delay < data_used)
            delay = data_used;

        i_start = x = compute_mod(sample_array_index - delay * channels, SAMPLE_ARRAY_SIZE);
        
        h= INT_MIN;
        for(int i = 0; i < 1000; i += channels) {
            int idx= (SAMPLE_ARRAY_SIZE + x - i) % SAMPLE_ARRAY_SIZE;
            int a= sample_array[idx];
            int b= sample_array[(idx + 4*channels)%SAMPLE_ARRAY_SIZE];
            int c= sample_array[(idx + 5*channels)%SAMPLE_ARRAY_SIZE];
            int d= sample_array[(idx + 9*channels)%SAMPLE_ARRAY_SIZE];
            int score= a-d;
            if(h < score && (b^c)<0){
                h = score;
                i_start= idx;
            }            
        }
        last_i_start = i_start;
    } else 
        i_start = last_i_start;

    bgcolor = SDL_MapRGB(screen->format, 0x80, 0x80, 0x80);
    
    fill_rectangle(screen, 0, 0, screen->w, screen->h, bgcolor);

    fgcolor = SDL_MapRGB(screen->format, 0x00, 0xff, 0x00);

    // total height for one channel 
    h = screen->h / nb_display_channels;
    // graph height / 2 
    h2 = (h * 9) / 20;
    for(ch = 0; ch < nb_display_channels; ch++) {
        int i = i_start + ch;
        y1 = ch * h + (h / 2); /* position of center line */
        for(x = 0; x < screen->w; x++) {
            y = (sample_array[i] * h2) >> 15;
            ys = y1 - y;
            /*if (y < 0) {
                y = -y;
                ys = y1 - y;
            } else {
                ys = y1;
            }*/
            fill_rectangle(screen, x, ys, 1, 1/*y*/, fgcolor);
            i += channels;
            if (i >= SAMPLE_ARRAY_SIZE)
                i -= SAMPLE_ARRAY_SIZE;
        }
    }

    fgcolor = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);

    for(ch = 1; ch < nb_display_channels; ch++) {
        y = ch * h;
        fill_rectangle(screen, 0, y, screen->w, 1, fgcolor);
    }
    SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
}

// called to display each frame 
void MediaPlayer::video_refresh() {
  	if (videoStream) {
        if (pictq_size > 0) {						// There is picture
            double masterClock = get_master_clock();

			double diff = video_current_pts - masterClock;
            if(diff > 0 && diff < 2) 
                return;

            // Update current video pts 
            VideoPicture &vp = pictq[pictq_rindex];	// Dequeue the picture 
            video_current_pts = vp.pts;
            video_current_pts_time = GetUSec();
			video_current_pos = vp.pos;
            
			diff = video_current_pts - masterClock;		
			if (diff > 2) {
	            // update queue size and signal for next picture 
	            if (++pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE)
	                pictq_rindex = 0;
	
	            INTERLOCKED_(pictq_mutex) {
	            	pictq_size--;
	            	pictq_cond.Signal();
	            }
                return;
            } 
	
            if(subtitleStream) {
                if (subtitle_stream_changed) {
                    INTERLOCKED_(subpq_mutex) {
	                    while (subpq_size) {
	                        free_subpicture(&subpq[subpq_rindex]);
	
	                        // update queue size for next picture 
	                        if (++subpq_rindex == SUBPICTURE_QUEUE_SIZE)
	                            subpq_rindex = 0;
	
	                        subpq_size--;
	                    }
	                    subtitle_stream_changed = false;
	
	                    subpq_cond.Signal();
                    }
                } else {
                    if (subpq_size > 0) {
                        SubPicture *sp, *sp2;
                        sp = &subpq[subpq_rindex];

                        if (subpq_size > 1)
                            sp2 = &subpq[(subpq_rindex + 1) % SUBPICTURE_QUEUE_SIZE];
                        else
                            sp2 = NULL;

                        if ((video_current_pts > (sp->pts + (float(sp->sub.end_display_time) / 1000)))
                                || (sp2 && video_current_pts > (sp2->pts + (float(sp2->sub.start_display_time) / 1000)))) {
                            free_subpicture(sp);

                            // update queue size and signal for next picture 
                            if (++subpq_rindex == SUBPICTURE_QUEUE_SIZE)
                                subpq_rindex = 0;

                            INTERLOCKED_(subpq_mutex) {
	                            subpq_size--;
	                            subpq_cond.Signal();
                            }
                        }
                    }
                }
            }
            video_image_display();		// display image 

            // update queue size and signal for next picture 
            if (++pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE)
                pictq_rindex = 0;

            INTERLOCKED_(pictq_mutex) {
            	pictq_size--;
            	pictq_cond.Signal();
            }
        }
    } else if (audioStream && showAudio) {
        if (get_external_clock() > lastAudioShow + 1./showAudioFps) {
        	video_audioWaveform();			// Display audio waveform    
        	lastAudioShow = get_external_clock();
        }
    } 
}

void MediaPlayer::close() {
    // Free all pictures 
    for(int i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++) {
        VideoPicture &vp = pictq[i];
        if (vp.overlay) {
            SDL_FreeYUVOverlay(vp.overlay);
            vp.overlay = 0;
        } else if (vp.surface) {
            SDL_FreeSurface(vp.surface);
            vp.surface = 0;
        }
    }
}

bool MediaPlayer::Load0Failed() {
    requestAbort = true;

    // Close each stream
    if (idAudio >= 0)
        StreamClose(idAudio);
    if (idVideo >= 0)
        StreamClose(idVideo);
    if (idSubtitle >= 0)
        StreamClose(idSubtitle);
    if (fileData) {
        av_close_input_file(fileData);
        fileData = NULL;
    }
    url_set_interrupt_cb(NULL);
	return false;
}

bool MediaPlayer::Load(const char *_fileName) {
	ResetError();
	Reset(GetSurface(), GetVideoFlags());
	
    int err;
    
    AVFormatParameters params, *ap = &params;
    int eof=0;
    int pkt_in_play_range = 0;

    fileData = avformat_alloc_context();

    idVideo = idAudio = idSubtitle = -1;

    memset(ap, 0, sizeof(*ap));

    ap->prealloced_context = 1;
    ap->time_base.num = 1;
    ap->time_base.den = 25;
    ap->pix_fmt = frame_pix_fmt;

    err = av_open_input_file(&fileData, ToSystemCharset(_fileName), iformat, 0, ap);
    if (err < 0) {
        MediaFile::SetError(GetAvError(err));
        return Load0Failed();
    }

    if(genpts)
        fileData->flags |= AVFMT_FLAG_GENPTS;

    err = av_find_stream_info(fileData);
    if (err < 0) {
        MediaFile::SetError(GetAvError(err));
        return Load0Failed();
    }
    if(fileData->pb)
        fileData->pb->eof_reached= 0; //FIXME hack, ffplay maybe should not use url_feof() to test for the end

    // if seeking requested, we execute it 
    if (start_time != AV_NOPTS_VALUE) {
        int64_t timestamp;

        timestamp = start_time;
        // add the stream start time 
        if (fileData->start_time != AV_NOPTS_VALUE)
            timestamp += fileData->start_time;
        int ret = av_seek_frame(fileData, -1, timestamp, AVSEEK_FLAG_BACKWARD);
        if (ret < 0) {
            MediaFile::SetError(Format("Could not seek to position %0.3f", (double)timestamp / AV_TIME_BASE));
            return Load0Failed();
        }
    }

    for(unsigned i = 0; i < fileData->nb_streams; i++) {
        AVStream *st = fileData->streams[i];
        st->discard = AVDISCARD_ALL;
        switch(st->codec->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
        	numAudioStreams++;
        	{
	        	AudioData &audioD = audioData.Add();
	        	audioD.id = i;
				audioD.codec = GetCodecName(st->codec, 0);
				audioD.lang = GetStreamLanguage(st);
				audioD.sampleRate = st->codec->sample_rate;
				audioD.channels = st->codec->channels;
				audioD.bitrate 	= GetCodecBitRate(st->codec);
				audioD.bits 	= GetStreamAudioBitFormat(st);
				audioD.tags 	= GetStreamTags(st);
        	}
            if (!audio_disable)
            	idAudio = i;
            break;
        case AVMEDIA_TYPE_VIDEO:
        	numVideoStreams++;
        	{
	        	VideoData &videoD = videoData.Add();
	        	videoD.id = i;
				videoD.codec = GetCodecName(st->codec, 0);
				if (st->codec->pix_fmt != PIX_FMT_NONE) 
    				videoD.pixFmt = avcodec_get_pix_fmt_name(st->codec->pix_fmt);
				videoD.lang = GetStreamLanguage(st);
				videoD.size.cx = st->codec->width;
				videoD.size.cy = st->codec->height;
				if (st->codec->sample_aspect_ratio.num) {
					AVRational display_aspect_ratio;
		         	av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den,
		                          st->codec->width*st->codec->sample_aspect_ratio.num,
		                          st->codec->height*st->codec->sample_aspect_ratio.den,
		                          1024*1024);
					videoD.par.cx = st->codec->sample_aspect_ratio.num;
					videoD.par.cy = st->codec->sample_aspect_ratio.den;
					videoD.dar.cx = display_aspect_ratio.num;
					videoD.dar.cy = display_aspect_ratio.den;
		       	} else
		       		videoD.par.cx = videoD.par.cy = videoD.dar.cx = videoD.dar.cy = 1;
				
				videoD.tags = GetStreamTags(st);				
				videoD.frameRate.cx = st->avg_frame_rate.num;
				videoD.frameRate.cy = st->avg_frame_rate.den;
				videoD.tbr.cx 	  	= st->r_frame_rate.num;
				videoD.tbr.cy 	  	= st->r_frame_rate.den;
				videoD.tbn.cx  	  	= st->time_base.num;
				videoD.tbn.cy 	  	= st->time_base.den;
				videoD.tbc.cx  	  	= st->codec->time_base.num;
				videoD.tbc.cy 	  	= st->codec->time_base.den;
				
				videoD.bitrate = GetCodecBitRate(st->codec);
	            if (!video_disable)
	                idVideo = i;
        	}
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            if (!video_disable)
            	idSubtitle = i;
            break;
        default:
            break;
        }
    }
    tags = ::GetTags(fileData);
    
    if (idAudio >= 0) 
		audioStream = fileData->streams[idAudio];
    if (idVideo >= 0) 
		videoStream = fileData->streams[idVideo];
    if (idSubtitle >= 0) 
		audioStream = fileData->streams[idSubtitle];

	return true;
}

void MediaPlayer::Reset(SDL_Surface *_screen, int _videoFlags)
{
	MediaFile::Reset();
	
    iformat = 0;
    last_i_start = 0;
	
	aspect_ratio = 1;
    screen = 0;
    
    memset(&frame_pix_fmt, 0, sizeof(frame_pix_fmt));

	frame_pix_fmt = PIX_FMT_NONE;
	av_sync_type = AV_SYNC_AUDIO_MASTER;
	start_time = AV_NOPTS_VALUE;
	
	fast = false;
	genpts = 0;
	lowres = 0;
	idct = FF_IDCT_AUTO;

	screen = _screen;
	videoFlags = _videoFlags;
}

void MediaPlayer::Pause()
{
 	stream_pause();
    step = false;
}

void MediaPlayer::step_to_next_frame()
{
  	if (paused)
       	stream_pause();
    step = true;
}

/* handle an event sent by the GUI */
bool MediaPlayer::event_loop()
{
    for(;;) {
        Ctrl::ProcessEvents();
        
 		SDL_Event event;      
        if (SDL_PollEvent(&event)) {
	        switch(event.type) {
	        case SDL_MOUSEBUTTONDOWN:
	        	Pause();
	            break;
	        case FF_ALLOC_EVENT:
	            if (!alloc_picture())
	                return false;
	            // Repaints background when change of size
	        	fill_rectangle(screen, 0, 0, screen->w, screen->h, 
	        									SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
	    		SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
	            break;
	        default:
	            break;
	        }
        }
        if (decodeThEnded)
            return true;

        video_refresh();
	
        second = get_master_clock();
        if (fabs(second - lastSecond) > 1.) {
            lastSecond = second;
            if (WhenSecond)
                WhenSecond();
        }
    }
    return true;
}

MediaPlayer::MediaPlayer() {
	SetBpp(24);
		    
    Reset(0, 0);
}

bool MediaPlayer::Play() {
	ResetError();
	
	// open the streams and launch threads
	decodeThEnd = decodeThEnded = false;
	requestAbort = false;
  	StreamOpen(idAudio);
  	StreamOpen(idVideo);
	StreamOpen(idSubtitle);
    
  	if (idVideo < 0 && idAudio < 0) {
        MediaFile::SetError("Could not open codecs");
        return Load0Failed();
    }
    
	if (!alloc_picture()) {
    	SDLCtrl::SetError("Could not alloc picture");
		return Load0Failed();
	}
    
    VideoPicture &vp = pictq[pictq_rindex];
    
    if (vp.overlay || vp.surface) {
        if (videoStream) {
		   	if (videoStream->sample_aspect_ratio.num)
		        aspect_ratio = float(av_q2d(videoStream->sample_aspect_ratio));
		    else if (videoStream->codec->sample_aspect_ratio.num)
		        aspect_ratio = float(av_q2d(videoStream->codec->sample_aspect_ratio));
		    else
		        aspect_ratio = 0;
		
		    if (aspect_ratio <= 0.0)
		        aspect_ratio = 1.0;
		    aspect_ratio *= float(vp.width) / float(vp.height);
        } else
            aspect_ratio = 1;
    }
    Layout();	// After aspect_ratio setting
    
    SetSecond(0);  
	externalClock.Reset();
	
	if (!fileData)
		throw Exc("MediaFile has been deleted");
	
    if (!BeginDecoder()) 
        return false;
 
    event_loop();
	close();
	
	return true;
}

void MediaPlayer::Stop() {
	requestAbort = true;
};

MediaPlayer::~MediaPlayer() {
}