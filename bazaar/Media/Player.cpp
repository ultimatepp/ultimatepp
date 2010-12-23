#include <CtrlLib/CtrlLib.h>

using namespace Upp;

void MemoryBreakpoint(dword serial);

#include <Functions4U/Functions4U.h>
#include <Media/Ffmpeg.h>
#include "MediaPlayer.h"

void video_refresh_timer(void *opaque);

/* get the current audio output buffer size, in samples. With SDL, we
   cannot have a precise information */
int MediaPlayer::audio_write_get_buf_size()
{
    return audio_buf_size - audio_buf_index;
}

static int packet_queue_put(PacketQueue *q, AVPacket *pkt, AVPacket *flush_pkt);

/* packet queue handling */
static void packet_queue_init(PacketQueue *q, AVPacket &pkt, AVPacket *flush_pkt)
{
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
    packet_queue_put(q, &pkt, flush_pkt);
}

static void packet_queue_flush(PacketQueue *q)
{
    AVPacketList *pkt, *pkt1;

    SDL_LockMutex(q->mutex);
    for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1) {
        pkt1 = pkt->next;
        av_free_packet(&pkt->pkt);
        av_freep(&pkt);
    }
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->nb_packets = 0;
    q->size = 0;
    SDL_UnlockMutex(q->mutex);
}

static void packet_queue_end(PacketQueue *q)
{
    packet_queue_flush(q);
    SDL_DestroyMutex(q->mutex);
    SDL_DestroyCond(q->cond);
}

static int packet_queue_put(PacketQueue *q, AVPacket *pkt, AVPacket *flush_pkt)
{
    AVPacketList *pkt1;

    /* duplicate the packet */
    if (pkt != flush_pkt && av_dup_packet(pkt) < 0)
        return -1;

    pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));
    if (!pkt1)
        return -1;
    pkt1->pkt = *pkt;
    pkt1->next = NULL;


    SDL_LockMutex(q->mutex);

    if (!q->last_pkt)

        q->first_pkt = pkt1;
    else
        q->last_pkt->next = pkt1;
    q->last_pkt = pkt1;
    q->nb_packets++;
    q->size += pkt1->pkt.size + sizeof(*pkt1);
    /* XXX: should duplicate packet data in DV case */
    SDL_CondSignal(q->cond);

    SDL_UnlockMutex(q->mutex);
    return 0;
}

static void packet_queue_abort(PacketQueue *q)
{
    SDL_LockMutex(q->mutex);

    q->abort_request = 1;

    SDL_CondSignal(q->cond);

    SDL_UnlockMutex(q->mutex);
}

/* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
static int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block)
{
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for(;;) {
        if (q->abort_request) {
            ret = -1;
            break;
        }

        pkt1 = q->first_pkt;
        if (pkt1) {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt)
                q->last_pkt = NULL;
            q->nb_packets--;
            q->size -= pkt1->pkt.size + sizeof(*pkt1);
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

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

#define ALPHA_BLEND(a, oldp, newp, s)\
((((oldp << s) * (255 - (a))) + (newp * (a))) / (255 << s))

#define RGBA_IN(r, g, b, a, s)\
{\
    unsigned int v = ((const uint32_t *)(s))[0];\
    a = (v >> 24) & 0xff;\
    r = (v >> 16) & 0xff;\
    g = (v >> 8) & 0xff;\
    b = v & 0xff;\
}

#define YUVA_IN(y, u, v, a, s, pal)\
{\
    unsigned int val = ((const uint32_t *)(pal))[*(const uint8_t*)(s)];\
    a = (val >> 24) & 0xff;\
    y = (val >> 16) & 0xff;\
    u = (val >> 8) & 0xff;\
    v = val & 0xff;\
}

#define YUVA_OUT(d, y, u, v, a)\
{\
    ((uint32_t *)(d))[0] = (a << 24) | (y << 16) | (u << 8) | v;\
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
    VideoPicture *vp = &pictq[pictq_rindex];
    if (vp->bmp) {
        if (subtitle_st) {
            if (subpq_size > 0) {
                SubPicture *sp = &subpq[subpq_rindex];

                if (vp->pts >= sp->pts + (float(sp->sub.start_display_time)/1000.)) {
                    SDL_LockYUVOverlay (vp->bmp);
					
					AVPicture pict;
                    pict.data[0] = vp->bmp->pixels[0];
                    pict.data[1] = vp->bmp->pixels[2];
                    pict.data[2] = vp->bmp->pixels[1];

                    pict.linesize[0] = vp->bmp->pitches[0];
                    pict.linesize[1] = vp->bmp->pitches[2];
                    pict.linesize[2] = vp->bmp->pitches[1];

                    for (unsigned i = 0; i < sp->sub.num_rects; i++)
                        blend_subrect(&pict, sp->sub.rects[i], vp->bmp->w, vp->bmp->h);

                    SDL_UnlockYUVOverlay (vp->bmp);
                }
            }
        }
        // We suppose the screen has a 1.0 pixel ratio 
		SDL_Rect rect;
		if (forceAspect) {
			int cx = screen->w;
			int cy = screen->h;
			double frameAspect  = cx/double(cy); 
			if (frameAspect > aspect_ratio) {	
				double x = (cx - aspect_ratio*cy)/2.;
				rect.y = 0;
				rect.h = cy;
				rect.x = int(x);
				rect.w = int(aspect_ratio*cy);
			} else {
				double y = (cy - cx/aspect_ratio)/2.;
				rect.y = int(y);
				rect.h = int(cx/aspect_ratio);
				rect.x = 0;
				rect.w = cx;
			}
		} else {
	        rect.x = 0;
	        rect.y = 0;
	        rect.w = screen->w;
	        rect.h = screen->h;
    	}
        SDL_DisplayYUVOverlay(vp->bmp, &rect);
    } 
}

static inline int compute_mod(int a, int b)
{
    a = a % b;
    if (a >= 0)
        return a;
    else
        return a + b;
}

void MediaPlayer::video_audio_display()
{
    int i, i_start, x, y1, y, ys, delay, n, nb_display_channels;
    int ch, channels, h, h2, bgcolor, fgcolor;
    int16_t time_diff;
    int rdft_bits, nb_freq;

    for(rdft_bits=1; (1<<rdft_bits)<2*screen->h; rdft_bits++)
        ;
    nb_freq= 1<<(rdft_bits-1);

    /* compute display index : center on currently output samples */
    channels = audio_st->codec->channels;
    nb_display_channels = channels;
    if (!paused) {
        int data_used= show_audio==1 ? screen->w : (2*nb_freq);
        n = 2 * channels;
        delay = audio_write_get_buf_size();
        delay /= n;

        /* to be more precise, we take into account the time spent since
           the last buffer computation */
        if (audio_callback_time) {
            time_diff = int16_t(av_gettime() - audio_callback_time);
            delay -= (time_diff * audio_st->codec->sample_rate) / 1000000;
        }

        delay += 2*data_used;
        if (delay < data_used)
            delay = data_used;

        i_start= x = compute_mod(sample_array_index - delay * channels, SAMPLE_ARRAY_SIZE);
        if(show_audio==1){
            h= INT_MIN;
            for(i=0; i<1000; i+=channels){
                int idx= (SAMPLE_ARRAY_SIZE + x - i) % SAMPLE_ARRAY_SIZE;
                int a= sample_array[idx];
                int b= sample_array[(idx + 4*channels)%SAMPLE_ARRAY_SIZE];
                int c= sample_array[(idx + 5*channels)%SAMPLE_ARRAY_SIZE];
                int d= sample_array[(idx + 9*channels)%SAMPLE_ARRAY_SIZE];
                int score= a-d;
                if(h<score && (b^c)<0){
                    h= score;
                    i_start= idx;
                }
            }
        }
        last_i_start = i_start;
    } else 
        i_start = last_i_start;

    bgcolor = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    fill_rectangle(screen, xleft, ytop, screen->w, screen->h, bgcolor);

    fgcolor = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);

    /* total height for one channel */
    h = screen->h / nb_display_channels;
    /* graph height / 2 */
    h2 = (h * 9) / 20;
    for(ch = 0;ch < nb_display_channels; ch++) {
        i = i_start + ch;
        y1 = ytop + ch * h + (h / 2); /* position of center line */
        for(x = 0; x < screen->w; x++) {
            y = (sample_array[i] * h2) >> 15;
            if (y < 0) {
                y = -y;
                ys = y1 - y;
            } else {
                ys = y1;
            }
            fill_rectangle(screen, xleft + x, ys, 1, y, fgcolor);
            i += channels;
            if (i >= SAMPLE_ARRAY_SIZE)
                i -= SAMPLE_ARRAY_SIZE;
        }
    }

    fgcolor = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);

    for(ch = 1; ch < nb_display_channels; ch++) {
        y = ytop + ch * h;
        fill_rectangle(screen, xleft, y, screen->w, 1, fgcolor);
    }
    SDL_UpdateRect(screen, xleft, ytop, screen->w, screen->h);
}

/* display the current picture, if any */
void MediaPlayer::video_display()
{
    if (audio_st && show_audio)
        video_audio_display();
    else if (video_st)
        video_image_display();
}

static Uint32 sdl_refresh_timer_cb(Uint32 interval, void *opaque)
{
    SDL_Event event;
    event.type = FF_REFRESH_EVENT;
    event.user.data1 = opaque;
    SDL_PushEvent(&event);
    return 0; /* 0 means stop timer */
}

/* schedule a video refresh in 'delay' ms */
void MediaPlayer::schedule_refresh(int delay)
{
    if(!delay) 
    	delay=1; //SDL seems to be buggy when the delay is 0
    SDL_AddTimer(delay, sdl_refresh_timer_cb, this);
}

/* get the current audio clock value */
double MediaPlayer::get_audio_clock()
{
    double pts;
    int hw_buf_size, bytes_per_sec;
    pts = audio_clock;
    hw_buf_size = audio_write_get_buf_size();
    bytes_per_sec = 0;
    if (audio_st) {
        bytes_per_sec = audio_st->codec->sample_rate * 2 * audio_st->codec->channels;
    }
    if (bytes_per_sec)
        pts -= (double)hw_buf_size / bytes_per_sec;
    return pts;
}

/* get the current video clock value */
double MediaPlayer::get_video_clock()
{
    double delta;
    if (paused) {
        delta = 0;
    } else {
        delta = (av_gettime() - video_current_pts_time) / 1000000.0;
    }
    return video_current_pts + delta;
}

/* get the current external clock value */
double MediaPlayer::get_external_clock()
{
    int64_t ti = av_gettime();
    return external_clock + ((ti - external_clock_time) * 1e-6);
}

/* get the current master clock value */
double MediaPlayer::get_master_clock()
{
    double val;

    if (av_sync_type == AV_SYNC_VIDEO_MASTER) {
        if (video_st)
            val = get_video_clock();
        else
            val = get_audio_clock();
    } else if (av_sync_type == AV_SYNC_AUDIO_MASTER) {
        if (audio_st)
            val = get_audio_clock();
        else
            val = get_video_clock();
    } else {
        val = get_external_clock();
    }
    return val;
}

/* seek in the stream */
void MediaPlayer::stream_seek(int64_t pos, int64_t rel, int seek_by_bytes)
{
    if (!seek_req) {
        seek_pos = pos;
        seek_rel = rel;
        seek_flags &= ~AVSEEK_FLAG_BYTE;
        if (seek_by_bytes)
            seek_flags |= AVSEEK_FLAG_BYTE;
        seek_req = 1;
    }
}

/* pause or resume the video */
void MediaPlayer::stream_pause()
{
    paused = !paused;
    if (!paused) {
        video_current_pts = get_video_clock();
        frame_timer += (av_gettime() - video_current_pts_time) / 1000000.0;
    }
}

double MediaPlayer::compute_frame_delay(double frame_current_pts)
{
    double actual_delay, delay, sync_threshold, ref_clock, diff;

    /* compute nominal delay */
    delay = frame_current_pts - frame_last_pts;
    if (delay <= 0 || delay >= 10.0) {
        /* if incorrect delay, use previous one */
        delay = frame_last_delay;
    } else {
        frame_last_delay = delay;
    }
    frame_last_pts = frame_current_pts;

    /* update delay to follow master synchronisation source */
    if (((av_sync_type == AV_SYNC_AUDIO_MASTER && audio_st) ||
         av_sync_type == AV_SYNC_EXTERNAL_CLOCK)) {
        /* if video is slave, we try to correct big delays by
           duplicating or deleting a frame */
        ref_clock = get_master_clock();
        diff = frame_current_pts - ref_clock;

        /* skip or repeat frame. We take into account the
           delay to compute the threshold. I still don't know
           if it is the best guess */
        sync_threshold = FFMAX(AV_SYNC_THRESHOLD, delay);
        if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
            if (diff <= -sync_threshold)
                delay = 0;
            else if (diff >= sync_threshold)
                delay = 2 * delay;
        }
    }

    frame_timer += delay;
    /* compute the REAL delay (we need to do that to avoid
       long term errors */
    actual_delay = frame_timer - (av_gettime() / 1000000.0);
    if (actual_delay < 0.010) {
        /* XXX: should skip picture */
        actual_delay = 0.010;
    }

#if defined(DEBUG_SYNC)
    printf("video: delay=%0.3f actual_delay=%0.3f pts=%0.3f A-V=%f\n",
            delay, actual_delay, frame_current_pts, -diff);
#endif

    return actual_delay;
}

/* called to display each frame */
void video_refresh_timer(void *opaque)
{
    MediaPlayer *is = (MediaPlayer *)opaque;
    VideoPicture *vp;

    SubPicture *sp, *sp2;

    if (is->video_st) {
        if (is->pictq_size == 0) {
            /* if no picture, need to wait */
            is->schedule_refresh(1);
        } else {
            /* dequeue the picture */
            vp = &is->pictq[is->pictq_rindex];

            /* update current video pts */
            is->video_current_pts = vp->pts;
            is->video_current_pts_time = av_gettime();
			is->video_current_pos = vp->pos;
            /* launch timer for next picture */
            is->schedule_refresh((int)(is->compute_frame_delay(vp->pts) * 1000 + 0.5));

            if(is->subtitle_st) {
                if (is->subtitle_stream_changed) {
                    SDL_LockMutex(is->subpq_mutex);

                    while (is->subpq_size) {
                        free_subpicture(&is->subpq[is->subpq_rindex]);

                        /* update queue size and signal for next picture */
                        if (++is->subpq_rindex == SUBPICTURE_QUEUE_SIZE)
                            is->subpq_rindex = 0;

                        is->subpq_size--;
                    }
                    is->subtitle_stream_changed = 0;

                    SDL_CondSignal(is->subpq_cond);
                    SDL_UnlockMutex(is->subpq_mutex);
                } else {
                    if (is->subpq_size > 0) {
                        sp = &is->subpq[is->subpq_rindex];

                        if (is->subpq_size > 1)
                            sp2 = &is->subpq[(is->subpq_rindex + 1) % SUBPICTURE_QUEUE_SIZE];
                        else
                            sp2 = NULL;

                        if ((is->video_current_pts > (sp->pts + ((float) sp->sub.end_display_time / 1000)))
                                || (sp2 && is->video_current_pts > (sp2->pts + ((float) sp2->sub.start_display_time / 1000))))
                        {
                            free_subpicture(sp);

                            /* update queue size and signal for next picture */
                            if (++is->subpq_rindex == SUBPICTURE_QUEUE_SIZE)
                                is->subpq_rindex = 0;

                            SDL_LockMutex(is->subpq_mutex);
                            is->subpq_size--;
                            SDL_CondSignal(is->subpq_cond);
                            SDL_UnlockMutex(is->subpq_mutex);
                        }
                    }
                }
            }

            /* display picture */
            is->video_display();

            /* update queue size and signal for next picture */
            if (++is->pictq_rindex == VIDEO_PICTURE_QUEUE_SIZE)
                is->pictq_rindex = 0;

            SDL_LockMutex(is->pictq_mutex);
            is->pictq_size--;
            SDL_CondSignal(is->pictq_cond);
            SDL_UnlockMutex(is->pictq_mutex);
        }
    } else if (is->audio_st) {
        /* draw the next audio frame */

        is->schedule_refresh(40);

        /* if only audio stream, then display the audio bars (better
           than nothing, just to test the implementation */

        /* display picture */
        is->video_display();
    } else {
        is->schedule_refresh(100);
    }
    if (is->show_status) {
        static int64_t last_time;
        int64_t cur_time;
        int aqsize, vqsize, sqsize;
        double av_diff;

        cur_time = av_gettime();
        if (!last_time || (cur_time - last_time) >= 30000) {
            aqsize = 0;
            vqsize = 0;
            sqsize = 0;
            if (is->audio_st)
                aqsize = is->audioq.size;
            if (is->video_st)
                vqsize = is->videoq.size;
            if (is->subtitle_st)
                sqsize = is->subtitleq.size;
            av_diff = 0;
            if (is->audio_st && is->video_st)
                av_diff = is->get_audio_clock() - is->get_video_clock();
            printf("%7.2f A-V:%7.3f s:%3.1f aq=%5dKB vq=%5dKB sq=%5dB"/* f=%ld/%ld*/"   \r",
                   is->get_master_clock(), av_diff, FFMAX(is->skip_frames-1, 0), aqsize / 1024, 
                   vqsize / 1024, sqsize/*, is->pts_ctx.num_faulty_dts, is->pts_ctx.num_faulty_pts*/);
            fflush(stdout);
            last_time = cur_time;
        }
    }
}

void MediaPlayer::close()
{
    VideoPicture *vp;
    
    /* XXX: use a special url_shutdown call to abort parse cleanly */
    abort_request = 1;
    SDL_WaitThread(parse_tid, NULL);
    SDL_WaitThread(refresh_tid, NULL);

    /* free all pictures */
    for(int i=0; i<VIDEO_PICTURE_QUEUE_SIZE; i++) {
        vp = &pictq[i];
        if (vp->bmp) {
            SDL_FreeYUVOverlay(vp->bmp);
            vp->bmp = NULL;
        }
    }
    SDL_DestroyMutex(pictq_mutex);
    SDL_DestroyCond(pictq_cond);
    SDL_DestroyMutex(subpq_mutex);
    SDL_DestroyCond(subpq_cond);
}

/* allocate a picture (needs to do that in main thread to avoid
   potential locking problems */
bool MediaPlayer::alloc_picture()
{
    VideoPicture *vp;

    vp = &pictq[pictq_windex];

    if (vp->bmp)
        SDL_FreeYUVOverlay(vp->bmp);

	if (video_st) {
	    vp->width   = video_st->codec->width;
	    vp->height  = video_st->codec->height;
	    vp->pix_fmt = video_st->codec->pix_fmt;
	} else {
		vp->width   = screen->w;
	    vp->height  = screen->h;
	    vp->pix_fmt = PIX_FMT_YUV420P;
	}

    vp->bmp = SDL_CreateYUVOverlay(vp->width, vp->height,
                                   SDL_YV12_OVERLAY,
                                   screen);
    if (!vp->bmp || vp->bmp->pitches[0] < vp->width) {
        /* SDL allocates a buffer smaller than requested if the video
         * overlay hardware is unable to support the requested size. */
        fprintf(stderr, "Error: the video system does not support an image\n"
                        "size of %dx%d pixels. Try using -lowres or -vf \"scale=w:h\"\n"
                        "to reduce the image size.\n", vp->width, vp->height );
        return false;
    }

    SDL_LockMutex(pictq_mutex);
    vp->allocated = 1;
    SDL_CondSignal(pictq_cond);
    SDL_UnlockMutex(pictq_mutex);
    return true;
}

/**
 *
 * @param pts the dts of the pkt / pts of the frame and guessed if not known
 */
bool MediaPlayer::queue_picture(AVFrame *src_frame, double pts)
{
    VideoPicture *vp;

    /* wait until we have space to put a new picture */
    SDL_LockMutex(pictq_mutex);
    while (pictq_size >= VIDEO_PICTURE_QUEUE_SIZE &&
           !videoq.abort_request) {
        SDL_CondWait(pictq_cond, pictq_mutex);
    }
    SDL_UnlockMutex(pictq_mutex);

    if (videoq.abort_request)
        return false;

    vp = &pictq[pictq_windex];

    /* alloc or resize hardware picture buffer */
    if (!vp->bmp ||
        vp->width != video_st->codec->width ||
        vp->height != video_st->codec->height) {
        SDL_Event event;

        vp->allocated = 0;

        /* the allocation must be done in the main thread to avoid
           locking problems */
        event.type = FF_ALLOC_EVENT;
        event.user.data1 = this;
        SDL_PushEvent(&event);

        /* wait until the picture is allocated */
        SDL_LockMutex(pictq_mutex);
        while (!vp->allocated && !videoq.abort_request) {
            SDL_CondWait(pictq_cond, pictq_mutex);
        }
        SDL_UnlockMutex(pictq_mutex);

        if (videoq.abort_request)
            return false;
    }

    /* if the frame is not skipped, then display it */
    if (vp->bmp) {
        AVPicture pict;
        SwsContext *context;
        
        /* get a pointer on the bitmap */
        SDL_LockYUVOverlay (vp->bmp);

        int dst_pix_fmt = PIX_FMT_YUV420P;
        memset(&pict,0,sizeof(AVPicture));
        pict.data[0] = vp->bmp->pixels[0];
        pict.data[1] = vp->bmp->pixels[2];
        pict.data[2] = vp->bmp->pixels[1];

        pict.linesize[0] = vp->bmp->pitches[0];
        pict.linesize[1] = vp->bmp->pitches[2];
        pict.linesize[2] = vp->bmp->pitches[1];
        context = sws_getContext(video_st->codec->width, video_st->codec->height,
            							 video_st->codec->pix_fmt,
            							 video_st->codec->width, video_st->codec->height,
            						(PixelFormat)dst_pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
        if (context == NULL) {
            strError = "Cannot initialize the conversion context";
            return false;
        }
        sws_scale(context, src_frame->data, src_frame->linesize,
                  0, video_st->codec->height, pict.data, pict.linesize);
     	sws_freeContext(context);         
         
        /* update the bitmap content */
        SDL_UnlockYUVOverlay(vp->bmp);

        vp->pts = pts;

        /* now we can update the picture count */
        if (++pictq_windex == VIDEO_PICTURE_QUEUE_SIZE)
            pictq_windex = 0;
        SDL_LockMutex(pictq_mutex);
        pictq_size++;
        SDL_UnlockMutex(pictq_mutex);
    }
    return true;
}

/**
 * compute the exact PTS for the picture if it is omitted in the stream
 * @param pts1 the dts of the pkt / pts of the frame
 */
bool MediaPlayer::output_picture2(AVFrame *src_frame, double pts1)
{
    double frame_delay, pts;

    pts = pts1;

    if (pts != 0) {
        /* update video clock with pts, if present */
        video_clock = pts;
    } else {
        pts = video_clock;
    }
    /* update video clock for next frame */
    frame_delay = av_q2d(video_st->codec->time_base);
    /* for MPEG2, the frame can be repeated, so we update the
       clock accordingly */
    frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
    video_clock += frame_delay;

#if defined(DEBUG_SYNC) && 0
    {
        int ftype;
        if (src_frame->pict_type == FF_B_TYPE)
            ftype = 'B';
        else if (src_frame->pict_type == FF_I_TYPE)
            ftype = 'I';
        else
            ftype = 'P';
        printf("frame_type=%c clock=%0.3f pts=%0.3f\n",
               ftype, pts, pts1);
    }
#endif
    return queue_picture(src_frame, pts);
}

int video_thread(void *arg)
{
    MediaPlayer *is = (MediaPlayer *)arg;
    AVPacket pkt1, *pkt = &pkt1;
    int len1, got_picture;
    AVFrame *frame= avcodec_alloc_frame();
    double pts;

    for(;;) {
        while (is->paused && !is->videoq.abort_request) 
            SDL_Delay(10);
        
        if (packet_queue_get(&is->videoq, pkt, 1) < 0)
            break;

        if(pkt->data == is->flush_pkt.data) 
            avcodec_flush_buffers(is->video_st->codec);
      	else {
	        // NOTE: ipts is the PTS of the _first_ picture beginning in this packet, if any 
	        is->video_st->codec->reordered_opaque= pkt->pts;
	        len1 = avcodec_decode_video(is->video_st->codec,
	                                    frame, &got_picture,
	                                    pkt->data, pkt->size);
	
	        if((is->decoder_reorder_pts || pkt->dts == AV_NOPTS_VALUE)
	           && frame->reordered_opaque != AV_NOPTS_VALUE)
	            pts= double(frame->reordered_opaque);
	        else if(pkt->dts != AV_NOPTS_VALUE)
	            pts= double(pkt->dts);
	        else
	            pts= 0;
	        pts *= av_q2d(is->video_st->time_base);
	//            if (len1 < 0)
	//                break;
	        if (got_picture) {
	            if (!is->output_picture2(frame, pts))
	                break;
	        }
	        av_free_packet(pkt);
	        if (is->step)
	         	is->stream_pause();
      	}
    }
    av_free(frame);
    return 0;
}

int subtitle_thread(void *arg)
{
    MediaPlayer *is = (MediaPlayer *)arg;
    SubPicture *sp;
    AVPacket pkt1, *pkt = &pkt1;
    int len1, got_subtitle;
    double pts;
    int j;
    int r, g, b, y, u, v, a;

    for(;;) {
        while (is->paused && !is->subtitleq.abort_request) {
            SDL_Delay(10);
        }
        if (packet_queue_get(&is->subtitleq, pkt, 1) < 0)
            break;

        if(pkt->data == is->flush_pkt.data){
            avcodec_flush_buffers(is->subtitle_st->codec);
            continue;
        }
        SDL_LockMutex(is->subpq_mutex);
        while (is->subpq_size >= SUBPICTURE_QUEUE_SIZE &&
               !is->subtitleq.abort_request) {
            SDL_CondWait(is->subpq_cond, is->subpq_mutex);
        }
        SDL_UnlockMutex(is->subpq_mutex);

        if (is->subtitleq.abort_request)
            goto the_end;

        sp = &is->subpq[is->subpq_windex];

       /* NOTE: ipts is the PTS of the _first_ picture beginning in
           this packet, if any */
        pts = 0;
        if (pkt->pts != AV_NOPTS_VALUE)
            pts = av_q2d(is->subtitle_st->time_base)*pkt->pts;

        len1 = avcodec_decode_subtitle2(is->subtitle_st->codec,
                                    &sp->sub, &got_subtitle,
                                    pkt);
//            if (len1 < 0)
//                break;
        if (got_subtitle && sp->sub.format == 0) {
            sp->pts = pts;

            for (unsigned i = 0; i < sp->sub.num_rects; i++)
            {
                for (j = 0; j < sp->sub.rects[i]->nb_colors; j++)
                {
                    RGBA_IN(r, g, b, a, (uint32_t*)sp->sub.rects[i]->pict.data[1] + j);
                    y = RGB_TO_Y_CCIR(r, g, b);
                    u = RGB_TO_U_CCIR(r, g, b, 0);
                    v = RGB_TO_V_CCIR(r, g, b, 0);
                    YUVA_OUT((uint32_t*)sp->sub.rects[i]->pict.data[1] + j, y, u, v, a);
                }
            }

            /* now we can update the picture count */
            if (++is->subpq_windex == SUBPICTURE_QUEUE_SIZE)
                is->subpq_windex = 0;
            SDL_LockMutex(is->subpq_mutex);
            is->subpq_size++;
            SDL_UnlockMutex(is->subpq_mutex);
        }
        av_free_packet(pkt);
//        if (step)
//            if (cur_stream)
//                stream_pause(cur_stream);
    }
 the_end:
    return 0;
}

/* copy samples for viewing in editor window */
void MediaPlayer::update_sample_display(short *samples, int samples_size)
{
    int size, len, channels;

    channels = audio_st->codec->channels;

    size = samples_size / sizeof(short);
    while (size > 0) {
        len = SAMPLE_ARRAY_SIZE - sample_array_index;
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

/* return the new audio buffer size (samples can be added or deleted
   to get better sync if video or external master clock) */
int MediaPlayer::synchronize_audio(short *samples, int samples_size1, double pts)
{
    int n, samples_size;
    double ref_clock;

    n = 2 * audio_st->codec->channels;
    samples_size = samples_size1;

    /* if not master, then we try to remove or add samples to correct the clock */
    if (((av_sync_type == AV_SYNC_VIDEO_MASTER && video_st) ||
         av_sync_type == AV_SYNC_EXTERNAL_CLOCK)) {
        double diff, avg_diff;
        int wanted_size, min_size, max_size, nb_samples;

        ref_clock = get_master_clock();
        diff = get_audio_clock() - ref_clock;

        if (diff < AV_NOSYNC_THRESHOLD) {
            audio_diff_cum = diff + audio_diff_avg_coef * audio_diff_cum;
            if (audio_diff_avg_count < AUDIO_DIFF_AVG_NB) {
                /* not enough measures to have a correct estimate */
                audio_diff_avg_count++;
            } else {
                /* estimate the A-V difference */
                avg_diff = audio_diff_cum * (1.0 - audio_diff_avg_coef);

                if (fabs(avg_diff) >= audio_diff_threshold) {
                    wanted_size = samples_size + ((int)(diff * audio_st->codec->sample_rate) * n);
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
            /* too big difference : may be initial PTS errors, so
               reset A-V filter */
            audio_diff_avg_count = 0;
            audio_diff_cum = 0;
        }
    }

    return samples_size;
}

/* decode one audio frame and returns its uncompressed size */
int MediaPlayer::audio_decode_frame(double *pts_ptr)
{
    AVPacket *pkt_temp = &audio_pkt_temp;
    AVPacket *pkt = &audio_pkt;
    AVCodecContext *dec= audio_st->codec;
    int n, len1, data_size;
    double pts;

    for(;;) {
        // The audio packet can contain several frames 
        while (pkt_temp->size > 0) {
            data_size = sizeof(audio_buf1);
            len1 = avcodec_decode_audio3(dec, (int16_t *)audio_buf1, &data_size, pkt_temp);                                   
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
                    fprintf(stderr, "Cannot convert %s sample format to %s sample format\n",
                        avcodec_get_sample_fmt_name(dec->sample_fmt),
                        avcodec_get_sample_fmt_name(SAMPLE_FMT_S16));
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
#if defined(DEBUG_SYNC)
            {
                static double last_clock;
                printf("audio: delay=%0.3f clock=%0.3f pts=%0.3f\n",
                       audio_clock - last_clock,
                       audio_clock, pts);
                last_clock = is->audio_clock;
            }
#endif
            return data_size;
        }

        if (pkt->data)								// free the current packet 
            av_free_packet(pkt);

        if (paused || audioq.abort_request) 
            return -1;

        if (packet_queue_get(&audioq, pkt, 1) < 0)	// read next packet 
            return -1;
        if(pkt->data == flush_pkt.data){
            avcodec_flush_buffers(dec);
            continue;
        }

        pkt_temp->data = pkt->data;
        pkt_temp->size = pkt->size;

        if (pkt->pts != AV_NOPTS_VALUE) 	// if update the audio clock with the pts 
            audio_clock = av_q2d(audio_st->time_base)*pkt->pts;
    }
}

/* prepare a new audio buffer */
void sdl_audio_callback(void *opaque, Uint8 *stream, int len)
{
    MediaPlayer *is = (MediaPlayer *)opaque;
    int audio_size, len1;
    double pts;

    is->audio_callback_time = av_gettime();

    while (len > 0) {
        if (unsigned(is->audio_buf_index) >= is->audio_buf_size) {
           audio_size = is->audio_decode_frame(&pts);
           if (audio_size < 0) {
                // if error, just output silence 
               is->audio_buf = is->audio_buf1;
               is->audio_buf_size = 1024;
               memset(is->audio_buf, 0, is->audio_buf_size);
           } else {
               if (is->show_audio)
                   is->update_sample_display((int16_t *)is->audio_buf, audio_size);
               audio_size = is->synchronize_audio((int16_t *)is->audio_buf, audio_size,
                                              pts);
               is->audio_buf_size = audio_size;
           }
           is->audio_buf_index = 0;
        }
        len1 = is->audio_buf_size - is->audio_buf_index;
        if (len1 > len)
            len1 = len;
        memcpy(stream, (uint8_t *)is->audio_buf + is->audio_buf_index, len1);
        len -= len1;
        stream += len1;
        is->audio_buf_index += len1;
    }
}

// open a given stream. Return 0 if OK 
bool MediaPlayer::stream_component_open(int stream_index)
{
    SDL_AudioSpec wanted_spec, spec;

    if (stream_index < 0 || unsigned(stream_index) >= icc->nb_streams)
        return false;

    AVCodecContext *avctx = icc->streams[stream_index]->codec;

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
    avcodec_thread_init(avctx, thread_count);

    //set_context_opts(avctx, avcodec_opts[avctx->codec_type], 0, codec);

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
            fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
            return false;
        }
        audio_hw_buf_size = spec.size;
        audio_src_fmt = SAMPLE_FMT_S16;
    }

    icc->streams[stream_index]->discard = AVDISCARD_DEFAULT;
    switch(avctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
        audio_stream = stream_index;
        audio_st = icc->streams[stream_index];
        audio_buf_size = 0;
        audio_buf_index = 0;

        // init averaging filter 
        audio_diff_avg_coef = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
        audio_diff_avg_count = 0;
        // since we do not have a precise enough audio fifo fullness,
        // we correct audio sync only if larger than this threshold 
        audio_diff_threshold = 2.0 * SDL_AUDIO_BUFFER_SIZE / avctx->sample_rate;

        memset(&audio_pkt, 0, sizeof(audio_pkt));
        packet_queue_init(&audioq, flush_pkt, &flush_pkt);
        SDL_PauseAudio(0);
        break;
    case AVMEDIA_TYPE_VIDEO:
        video_stream = stream_index;
        video_st = icc->streams[stream_index];

        frame_last_delay = 40e-3;
        frame_timer = (double)av_gettime() / 1000000.0;
        video_current_pts_time = av_gettime();

        packet_queue_init(&videoq, flush_pkt, &flush_pkt);
        video_tid = SDL_CreateThread(video_thread, this);
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        subtitle_stream = stream_index;
        subtitle_st = icc->streams[stream_index];
        packet_queue_init(&subtitleq, flush_pkt, &flush_pkt);

        subtitle_tid = SDL_CreateThread(subtitle_thread, this);
        break;
    default:
        break;
    }
    return true;
}

void MediaPlayer::stream_component_close(int stream_index)
{
    if (stream_index < 0 || unsigned(stream_index) >= icc->nb_streams)
        return;

    AVCodecContext *avctx = icc->streams[stream_index]->codec;

    switch(avctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
        packet_queue_abort(&audioq);

        SDL_CloseAudio();

        packet_queue_end(&audioq);
        if (reformat_ctx)
            av_audio_convert_free(reformat_ctx);
        reformat_ctx = NULL;
        break;
    case AVMEDIA_TYPE_VIDEO:
        packet_queue_abort(&videoq);

        // note: we also signal this mutex to make sure we deblock the video thread in all cases 
        SDL_LockMutex(pictq_mutex);
        SDL_CondSignal(pictq_cond);
        SDL_UnlockMutex(pictq_mutex);

        SDL_WaitThread(video_tid, NULL);

        packet_queue_end(&videoq);
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        packet_queue_abort(&subtitleq);

        // note: we also signal this mutex to make sure we deblock the video thread in all cases 
        SDL_LockMutex(subpq_mutex);
        subtitle_stream_changed = 1;

        SDL_CondSignal(subpq_cond);
        SDL_UnlockMutex(subpq_mutex);

        SDL_WaitThread(subtitle_tid, NULL);

        packet_queue_end(&subtitleq);
        break;
    default:
        break;
    }

    icc->streams[stream_index]->discard = AVDISCARD_ALL;
    avcodec_close(avctx);
    switch(avctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
        audio_st = NULL;
        audio_stream = -1;
        break;
    case AVMEDIA_TYPE_VIDEO:
        video_st = NULL;
        video_stream = -1;
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        subtitle_st = NULL;
        subtitle_stream = -1;
        break;
    default:
        break;
    }
}

/* since we have only one decoding thread, we can use a global
   variable instead of a thread local variable */
static MediaPlayer *global_video_state;

int decode_interrupt_cb(void)
{
    return (global_video_state && global_video_state->abort_request);
}

String GetAvError(int err)
{
    char errbuf[128];
    const char *errbuf_ptr = errbuf;

    if (av_strerror(err, errbuf, sizeof(errbuf)) < 0)
        errbuf_ptr = strerror(AVUNERROR(err));
    return errbuf_ptr;
}

bool MediaPlayer::Load0Failed() {
	 /* disable interrupting */
    global_video_state = NULL;

    /* close each stream */
    if (audio_stream >= 0)
        stream_component_close(audio_stream);
    if (video_stream >= 0)
        stream_component_close(video_stream);
    if (subtitle_stream >= 0)
        stream_component_close(subtitle_stream);
    if (icc) {
        av_close_input_file(icc);
        icc = NULL; /* safety */
    }
    url_set_interrupt_cb(NULL);
	return false;
}

String GetCodecName(AVCodecContext *enc, int encode) {
  	String ret;
    AVCodec *p;
    
	if (encode)
        p = avcodec_find_encoder(enc->codec_id);
    else
        p = avcodec_find_decoder(enc->codec_id);

    if (p) 
        ret = String(p->long_name) + " (" + p->name + ")";
    else if (enc->codec_id == CODEC_ID_MPEG2TS) 
        ret = "mpeg2ts"; // fake mpeg2 transport stream codec (currently not registered) 
    else if (enc->codec_name[0] != '\0')
        ret = String(enc->codec->long_name) + " (" + enc->codec_name + ")";
    else {		// output avi tags 
        char tag_buf[32];
        av_get_codec_tag_string(tag_buf, sizeof(tag_buf), enc->codec_tag);
        ret = Format("%s / 0x%04X", tag_buf, int(enc->codec_tag));
    } 
    if (enc->codec_type == CODEC_TYPE_VIDEO) {
    	if (enc->mb_decision)
        	ret << " (hq)";
    }
    return ret;
}

String GetStreamLanguage(AVStream *st) {
	AVMetadataTag *lang = av_metadata_get(st->metadata, "language", NULL, 0);
	if (lang)
		return lang->value;
	else 
		return "";
}

/*
size_t av_get_codec_tag_string(char *buf, size_t buf_size, unsigned int codec_tag) {
    size_t ret = 0;
    for (int i = 0; i < 4; i++) {
        size_t len = snprintf(buf, buf_size, isprint(codec_tag&0xFF) ? "%c" : "[%d]", codec_tag&0xFF);
        buf += len;
        buf_size = buf_size > len ? buf_size - len : 0;
        ret += len;
        codec_tag >>= 8;
    }
    return ret;
}	
*/

int get_bit_rate(AVCodecContext *ctx) {
    int bit_rate, bits_per_sample;

    switch(ctx->codec_type) {
    case CODEC_TYPE_VIDEO:
    case CODEC_TYPE_DATA:
    case CODEC_TYPE_SUBTITLE:
    case CODEC_TYPE_ATTACHMENT:
        bit_rate = max(ctx->bit_rate, av_get_bits_per_sample(ctx->codec_id));
        break;
    case CODEC_TYPE_AUDIO:
        bits_per_sample = av_get_bits_per_sample(ctx->codec_id);
        bit_rate = bits_per_sample ? ctx->sample_rate * ctx->channels * bits_per_sample : ctx->bit_rate;
        break;
    default:
        bit_rate = 0;
        break;
    }
    return bit_rate;
}	

String GetTags(AVStream *st) {
	char tagStrings[1024];		
	tagStrings[0] = 0;
	AVMetadataTag *tag = NULL;
	
	while ((tag = av_metadata_get(st->metadata, "", tag, AV_METADATA_IGNORE_SUFFIX))) {
		if (tag->key[0] != 0 && tag->value[0] != 0) {
	   		int ln = strlen(tagStrings);
	     	snprintf(tagStrings + ln, 1024-ln, "%s%s: %s", 
	        		tagStrings[0] != 0 ? "\n" : "", tag->key, tag->value);
		}
	}
	return tagStrings;
}

const char *GetAudioBits(AVStream *st) {
	switch(st->codec->sample_fmt) {
	case SAMPLE_FMT_U8:		return "Unsigned 8";	
	case SAMPLE_FMT_S16:	return "Signed 16";	
	case SAMPLE_FMT_S32:	return "Signed 32";	
	case SAMPLE_FMT_FLT:	return "Float";	
	}
	return "Unknown";
}

bool MediaPlayer::Load0(const char *_filename) {
    int err;
    
    AVFormatParameters params, *ap = &params;
    int eof=0;
    int pkt_in_play_range = 0;

    icc = avformat_alloc_context();

	/////////////////////////////////////////////// UNIFICAR //////////////////////////////////////////////
	audio_stream = video_stream = subtitle_stream = -1;
    idVideo = idAudio = idSubtitle = -1;

    global_video_state = this;
    url_set_interrupt_cb(decode_interrupt_cb);

    memset(ap, 0, sizeof(*ap));

    ap->prealloced_context = 1;
    ap->time_base.num = 1;
    ap->time_base.den = 25;
    ap->pix_fmt = frame_pix_fmt;

    err = av_open_input_file(&icc, _filename, iformat, 0, ap);
    if (err < 0) {
        strError = GetAvError(err);
        return Load0Failed();
    }

    if(genpts)
        icc->flags |= AVFMT_FLAG_GENPTS;

    err = av_find_stream_info(icc);
    if (err < 0) {
        strError = GetAvError(err);
        return Load0Failed();
    }
    if(icc->pb)
        icc->pb->eof_reached= 0; //FIXME hack, ffplay maybe should not use url_feof() to test for the end

    if(seek_by_bytes < 0)
        seek_by_bytes= !!(icc->iformat->flags & AVFMT_TS_DISCONT);

    // if seeking requested, we execute it 
    if (start_time != AV_NOPTS_VALUE) {
        int64_t timestamp;

        timestamp = start_time;
        // add the stream start time 
        if (icc->start_time != AV_NOPTS_VALUE)
            timestamp += icc->start_time;
        int ret = av_seek_frame(icc, -1, timestamp, AVSEEK_FLAG_BACKWARD);
        if (ret < 0) {
            strError = Format("Could not seek to position %0.3f", (double)timestamp / AV_TIME_BASE);
            return Load0Failed();
        }
    }

    for(unsigned i = 0; i < icc->nb_streams; i++) {
        AVStream *st = icc->streams[i];
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
				audioD.bitrate = get_bit_rate(st->codec);
				audioD.bits   = GetAudioBits(st);
				audioD.tags   = ::GetTags(st);
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
				
				videoD.tags = ::GetTags(st);				
				videoD.frameRate.cx = st->avg_frame_rate.num;
				videoD.frameRate.cy = st->avg_frame_rate.den;
				videoD.tbr.cx 	  	= st->r_frame_rate.num;
				videoD.tbr.cy 	  	= st->r_frame_rate.den;
				videoD.tbn.cx  	  	= st->time_base.num;
				videoD.tbn.cy 	  	= st->time_base.den;
				videoD.tbc.cx  	  	= st->codec->time_base.num;
				videoD.tbc.cy 	  	= st->codec->time_base.den;
				
				videoD.bitrate = get_bit_rate(st->codec);
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
    
    // Fill data tag in tagStrings
    char tagStrings[1024];		
	tagStrings[0] = 0;
    AVMetadataTag *tag = NULL;
  	av_metadata_conv(icc, NULL, icc->iformat->metadata_conv);
    while ((tag = av_metadata_get(icc->metadata, "", tag, AV_METADATA_IGNORE_SUFFIX))) {
        if (tag->key[0] != 0 && tag->value[0] != 0) {
	        int ln = strlen(tagStrings);
	        snprintf(tagStrings + ln, 1024-ln, "%s%s: %s", 
	        		 tagStrings[0] != 0 ? "\n" : "", tag->key, tag->value);
        }
    }
    tags = tagStrings;
    
	//////////////////////////////////////////////////////// Mover a PLAY /////////////////
    if (show_status) 
        dump_format(icc, 0, _filename, 0);

    // open the streams 
    if (idAudio >= 0) 
        stream_component_open(idAudio);

//    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) 
        stream_component_open(idVideo);

    if (idSubtitle >= 0) 
        stream_component_open(idSubtitle);
    
  	if (idVideo < 0 && idAudio < 0) {
        strError =  "Could not open codecs";
        return Load0Failed();
    }
    
	if (!alloc_picture()) {
    	strError =  "Could not alloc cpicture";
		return Load0Failed();
	}
    
    VideoPicture *vp = &pictq[pictq_rindex];
    
    if (vp->bmp) {
        if (video_st) {
		   	if (video_st->sample_aspect_ratio.num)
		        aspect_ratio = float(av_q2d(video_st->sample_aspect_ratio));
		    else if (video_st->codec->sample_aspect_ratio.num)
		        aspect_ratio = float(av_q2d(video_st->codec->sample_aspect_ratio));
		    else
		        aspect_ratio = 0;
		
		    if (aspect_ratio <= 0.0)
		        aspect_ratio = 1.0;
		    aspect_ratio *= (float)vp->width / (float)vp->height;
        } else
            aspect_ratio = 1;
    }
    filename = _filename;
    
  	return true;
}

/* this thread gets the stream from the disk or the network */
int decode_thread(void *arg)
{
    MediaPlayer *is = (MediaPlayer*)arg;
    int ret;
    
    AVPacket pkt1, *pkt = &pkt1;
    is->abort_request = false;
    
    for(;;) {
        if (is->abort_request)
            break;
        if (is->paused != is->last_paused) {
            is->last_paused = is->paused;
            if (is->paused)
                is->read_pause_return= av_read_pause(is->icc);
            else
                av_read_play(is->icc);
        }
        if (is->seek_req) {
            int64_t seek_target= is->seek_pos;
            int64_t seek_min= is->seek_rel > 0 ? seek_target - is->seek_rel + 2: INT64_MIN;
            int64_t seek_max= is->seek_rel < 0 ? seek_target - is->seek_rel - 2: INT64_MAX;
//FIXME the +-2 is due to rounding being not done in the correct direction in generation
//      of the seek_pos/seek_rel variables

            ret = avformat_seek_file(is->icc, -1, seek_min, seek_target, seek_max, is->seek_flags);
            if (ret < 0) 
                fprintf(stderr, "%s: error while seeking\n", is->icc->filename);
            else {
                if (is->audio_stream >= 0) {
                    packet_queue_flush(&is->audioq);
                    packet_queue_put(&is->audioq, &is->flush_pkt, &is->flush_pkt);
                }
                if (is->subtitle_stream >= 0) {
                    packet_queue_flush(&is->subtitleq);
                    packet_queue_put(&is->subtitleq, &is->flush_pkt, &is->flush_pkt);
                }
                if (is->video_stream >= 0) {
                    packet_queue_flush(&is->videoq);
                    packet_queue_put(&is->videoq, &is->flush_pkt, &is->flush_pkt);
                }
            }
            is->seek_req = 0;
        }

        /* if the queue are full, no need to read more */
        if (is->audioq.size > MAX_AUDIOQ_SIZE || is->videoq.size > MAX_VIDEOQ_SIZE ||
            is->subtitleq.size > MAX_SUBTITLEQ_SIZE) {
            SDL_Delay(10);		/* wait 10 ms */
            continue;
        }
        if(url_feof(is->icc->pb)) {
            av_init_packet(pkt);
            pkt->data=NULL;
            pkt->size=0;
            pkt->stream_index= is->video_stream;
            packet_queue_put(&is->videoq, pkt, &is->flush_pkt);
            continue;
        }
        ret = av_read_frame(is->icc, pkt);
        if (ret < 0) {
            if (ret != AVERROR_EOF && url_ferror(is->icc->pb) == 0) {
                SDL_Delay(100); /* wait for user event */
                continue;
            } else
                break;
        }
        if (pkt->stream_index == is->audio_stream) 
            packet_queue_put(&is->audioq, pkt, &is->flush_pkt);
        else if (pkt->stream_index == is->video_stream) 
            packet_queue_put(&is->videoq, pkt, &is->flush_pkt);
        else if (pkt->stream_index == is->subtitle_stream) 
            packet_queue_put(&is->subtitleq, pkt, &is->flush_pkt);
        else 
            av_free_packet(pkt);
    }
    /* wait until the end */
    while (!is->abort_request) 
        SDL_Delay(100);

    ret = 0;

// fail:

    /* disable interrupting */
    global_video_state = NULL;

    /* close each stream */
    if (is->audio_stream >= 0)
        is->stream_component_close(is->audio_stream);
    if (is->video_stream >= 0)
        is->stream_component_close(is->video_stream);
    if (is->subtitle_stream >= 0)
        is->stream_component_close(is->subtitle_stream);
    if (is->icc) {
        av_close_input_file(is->icc);
        is->icc = NULL; /* safety */
    }
    url_set_interrupt_cb(NULL);

    if (ret != 0) {
        SDL_Event event;
        event.type = FF_QUIT_EVENT;
        SDL_PushEvent(&event);
    }
    return 0;
}


Array <String> Split(String sentence, char separator) {
	Array <String> words;
	
	int pos = 0;
	
	for (;;) {
		int npos = sentence.Find(separator, pos);
		if (npos < 0)
			break;
		words.Add(sentence.Mid(pos, npos-pos));
		pos = npos;	
	}
	return words;
}

void MediaPlayer::Reset(SDL_Surface *_screen, int _videoFlags)
{
	numVideoStreams = numAudioStreams = 0;
	videoData.Clear();
	audioData.Clear();
	
    parse_tid = video_tid = refresh_tid = 0;
    iformat = 0;

    abort_request = paused = last_paused = seek_req = seek_flags = 0;
    seek_pos = seek_rel = 0;
    read_pause_return = 0;
    icc = 0;		

    audio_stream = 0;

    av_sync_type = 0;
    external_clock = 0;
    external_clock_time = 0;

    audio_clock = audio_diff_cum = audio_diff_avg_coef = audio_diff_threshold = 0;
    audio_diff_avg_count = 0;
    audio_st = 0;
    memset(&audioq, 0, sizeof(PacketQueue));
    audio_hw_buf_size = 0;

    audio_buf = 0;
    audio_buf_size = 0;
    audio_buf_index = 0;
    memset(&audio_pkt_temp, 0, sizeof(AVPacket));
    memset(&audio_pkt, 0, sizeof(AVPacket));
    reformat_ctx = 0;

    show_audio = 0;
    sample_array_index = 0;
    last_i_start = 0;

    subtitle_tid = 0;
    subtitle_stream = subtitle_stream_changed = 0;
    subtitle_st = 0;
    memset(&subtitleq, 0, sizeof(PacketQueue));
    memset(&subpq, 0, sizeof(subpq));
    subpq_size = subpq_rindex = subpq_windex = 0;
    subpq_mutex = 0;
    subpq_cond = 0;

    frame_timer = frame_last_pts = frame_last_delay = video_clock = 0;
    video_stream = 0;
    video_st = 0;
    memset(&videoq, 0, sizeof(PacketQueue));
    video_current_pts = 0;
    video_current_pts_time = video_current_pos = 0;
    memset(&pictq, 0, sizeof(pictq));
    pictq_size = pictq_rindex = pictq_windex = 0;
    pictq_mutex = 0;
    pictq_cond = 0;

    xleft = ytop = 0;

    skip_frames = 0;
    refresh = 0;
    
	fs_screen_width = fs_screen_height = 0;
	
	seek_by_bytes = 0;
	
	debug = debug_mv = 0;
	step = false;
	thread_count = workaround_bugs = 0;
	fast = false;
	genpts = lowres = idct = decoder_reorder_pts = loop = framedrop = 0;
	
	strError = "";
	aspect_ratio = 1;

    screen = 0;
    memset(&flush_pkt, 0, sizeof(flush_pkt));
    
    decoder_reorder_pts = 0;
    loop = 0;
    framedrop = 0;
    workaround_bugs = 0;
    thread_count = 0;
    
    wanted_subtitle_stream = 0;
    memset(&frame_pix_fmt, 0, sizeof(frame_pix_fmt));
	pictq_rindex = pictq_windex = 0;
	pictq_size = 0;
	memset(pictq, 0, sizeof(pictq));
	video_current_pts = 0;
	video_current_pts_time = 0;
	frame_timer = frame_last_pts = frame_last_delay = video_clock = 0;
    video_stream = 0;
    
    filename = "";
    iformat = NULL;
    ytop = 0;
    xleft = 0;

	frame_pix_fmt = PIX_FMT_NONE;
	wanted_audio_stream= 0;
	wanted_video_stream= 0;
	wanted_subtitle_stream= -1;
	show_status = 1;
	av_sync_type = AV_SYNC_AUDIO_MASTER;
	start_time = AV_NOPTS_VALUE;
	debug = 0;
	debug_mv = 0;
	step = false;
	thread_count = 1;
	workaround_bugs = 1;
	fast = false;
	genpts = 0;
	lowres = 0;
	idct = FF_IDCT_AUTO;
	skip_frame= AVDISCARD_DEFAULT;
	skip_idct= AVDISCARD_DEFAULT;
	skip_loop_filter= AVDISCARD_DEFAULT;
	decoder_reorder_pts= -1;
	loop=1;
	framedrop=1;
	rdftspeed=20;
	audio_disable = false;
	video_disable = false;
	screen = _screen;
	videoFlags = _videoFlags;
	audio_callback_time = 0;
	
	av_init_packet(&flush_pkt);
    flush_pkt.data= (uint8_t *)"FLUSH";
    
    /* start video display */
    pictq_mutex = SDL_CreateMutex();
    pictq_cond = SDL_CreateCond();

    subpq_mutex = SDL_CreateMutex();
    subpq_cond = SDL_CreateCond();
    
    keyboardControl = true;
    show_audio = 0;
    
    idAudio = idVideo = idSubtitle = -1;
    
    /* add the refresh timer to draw the picture */
    schedule_refresh(40);    
}

void MediaPlayer::stream_cycle_channel(int codec_type)
{
    AVFormatContext *_icc = icc;
    int start_index, stream_index;
    AVStream *st;

    if (codec_type == AVMEDIA_TYPE_VIDEO)
        start_index = video_stream;
    else if (codec_type == AVMEDIA_TYPE_AUDIO)
        start_index = audio_stream;
    else
        start_index = subtitle_stream;
    if (start_index < (codec_type == AVMEDIA_TYPE_SUBTITLE ? -1 : 0))
        return;
    stream_index = start_index;
    for(;;) {
        if (unsigned(++stream_index) >= icc->nb_streams) {
            if (codec_type == AVMEDIA_TYPE_SUBTITLE) {
                stream_index = -1;
                goto the_end;
            } else
                stream_index = 0;
        }
        if (stream_index == start_index)
            return;
        st = _icc->streams[stream_index];
        if (st->codec->codec_type == codec_type) {
            /* check that parameters are OK */
            switch(codec_type) {
            case AVMEDIA_TYPE_AUDIO:
                if (st->codec->sample_rate != 0 &&
                    st->codec->channels != 0)
                    goto the_end;
                break;
            case AVMEDIA_TYPE_VIDEO:
            case AVMEDIA_TYPE_SUBTITLE:
                goto the_end;
            default:
                break;
            }
        }
    }
 the_end:
    stream_component_close(start_index);
    stream_component_open(stream_index);
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

void MediaPlayer::toggle_audio_display()
{
    int bgcolor = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    show_audio = (show_audio + 1) % 3;
    fill_rectangle(screen,
                xleft, ytop, screen->w, screen->h,
                bgcolor);
	SDL_UpdateRect(screen, xleft, ytop, screen->w, screen->h);
}

/* handle an event sent by the GUI */
bool MediaPlayer::event_loop()
{
    SDL_Event event;
    double incr, pos, frac;

    for(;;) {
        Ctrl::ProcessEvents();
       
        SDL_WaitEvent(&event);
        if (keyboardControl) {
	        switch(event.type) {
	        case SDL_KEYDOWN:
	            switch(event.key.keysym.sym) {
	            case SDLK_ESCAPE:
	            case SDLK_q:
	                return true;
	                break;
	            case SDLK_p:
	            case SDLK_SPACE:
	                Pause();
	                break;
	            case SDLK_s: //S: Step to next frame
	                step_to_next_frame();
	                break;
	            case SDLK_a:
	               	stream_cycle_channel(AVMEDIA_TYPE_AUDIO);
	                break;
	            case SDLK_v:
					stream_cycle_channel(AVMEDIA_TYPE_VIDEO);
	                break;
	            case SDLK_t:
	             	stream_cycle_channel(AVMEDIA_TYPE_SUBTITLE);
	                break;
	            case SDLK_w:
	                toggle_audio_display();
	                break;
	            case SDLK_LEFT:
	                incr = -10.0;
	                goto do_seek;
	            case SDLK_RIGHT:
	                incr = 10.0;
	                goto do_seek;
	            case SDLK_UP:
	                incr = 60.0;
	                goto do_seek;
	            case SDLK_DOWN:
	                incr = -60.0;
	            do_seek:
	                if (seek_by_bytes) {
	                    if (video_stream >= 0 && video_current_pos>=0)
	                        pos = double(video_current_pos);
	                    else if(audio_stream >= 0 && audio_pkt.pos>=0)
	                        pos = double(audio_pkt.pos);
	                    else
	                        pos = double(url_ftell(icc->pb));
	                    if (icc->bit_rate)
	                        incr *= icc->bit_rate / 8.0;
	                    else
	                        incr *= 180000.0;
	                    pos += incr;
	                    stream_seek(int64_t(pos), int64_t(incr), 1);
	                } else {
	                    pos = get_master_clock();
	                    pos += incr;
	                    stream_seek((int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
	                }
	                break;
	            default:
	                break;
	            }
	            break;
	        case SDL_MOUSEBUTTONDOWN:
	        case SDL_MOUSEMOTION:
	        	double x;
	            if(event.type ==SDL_MOUSEBUTTONDOWN)
	                x= event.button.x;
	            else {
	                if(event.motion.state != SDL_PRESSED)
	                    break;
	                x = event.motion.x;
	            }
	            if(seek_by_bytes || icc->duration <= 0) {
	                uint64_t size=  url_fsize(icc->pb);
	                stream_seek(int64_t(size*x/screen->w), 0, 1);
	            } else {
	                int64_t ts;
	                int ns, hh, mm, ss;
	                int tns, thh, tmm, tss;
	                tns = int(icc->duration/1000000LL);
	                thh = tns/3600;
	                tmm = (tns%3600)/60;
	                tss = (tns%60);
	                frac = x/screen->w;
	                ns = int(frac*tns);
	                hh = ns/3600;
	                mm = (ns%3600)/60;
	                ss = (ns%60);
	                fprintf(stderr, "Seek to %2.0f%% (%2d:%02d:%02d) of total duration (%2d:%02d:%02d)       \n", frac*100,
	                        hh, mm, ss, thh, tmm, tss);
	                ts = int64_t(frac*icc->duration);
	                if (icc->start_time != AV_NOPTS_VALUE)
	                    ts += icc->start_time;
	                stream_seek(ts, 0, 0);
	            }
	            break;
	        }
        }
        switch(event.type) {
        case SDL_QUIT:
        case FF_QUIT_EVENT:
            return true;
            break;
        case FF_ALLOC_EVENT:
            if (!alloc_picture())
                return false;
            break;
        case FF_REFRESH_EVENT:
            video_refresh_timer(this);
            refresh=0;
            break;
        default:
            break;
        }
    }
    return true;
}


bool MediaPlayer::BeginDecodeThread() {
	if (icc == 0) {
		Exclamation("WEEEEEA");
		return false;
	}
    parse_tid = SDL_CreateThread(decode_thread, this);
    if (!parse_tid) {
//        av_free(is);
        return false;
    }
    return true;
}

MediaPlayer::MediaPlayer() {
	SetBpp(24);

	//videoFrame = 0;	
	//reload = false;
	
	av_log_set_flags(AV_LOG_SKIP_REPEATED);

    /* register all codecs, demux and protocols */
    avcodec_register_all();
    avdevice_register_all();
    av_register_all();
    
    Reset(0, 0);
    
    forceAspect = true;
	background = Black();
}

void MediaPlayer::SetMSec(uint64 _msec) {

}

bool MediaPlayer::Load(const char* _fileName) {
	ResetError();
	Reset(GetSurface(), GetVideoFlags());
	if (!Load0(ToSystemCharset(_fileName))) 
        return false;
 
    Layout();
    return true;
}

bool MediaPlayer::Play() {
	ResetError();
		
	if (!BeginDecodeThread())
        return false;
    event_loop();
	close();
	
	return true;
}

void MediaPlayer::Stop() {
	SDL_Event event;

	event.type = FF_QUIT_EVENT;
	SDL_PushEvent(&event);
	playing = false;
};