#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Media/ffmpeg_base.h>
#include "utility.h"


int av_strerror(int errnum, char *errbuf, size_t errbuf_size)
{
    int ret = 0;
    const char *errstr = NULL;

    switch (errnum) {
    case AVERROR_EOF:               errstr = "End of file"; break;
    case AVERROR_INVALIDDATA:       errstr = "Invalid data found when processing input"; break;
    case AVERROR_NUMEXPECTED:       errstr = "Number syntax expected in filename"; break;
    case AVERROR_PATCHWELCOME:      errstr = "Not yet implemented in FFmpeg, patches welcome"; break;
    case AVERROR_DEMUXER_NOT_FOUND: errstr = "Demuxer not found"; break;
    case AVERROR_MUXER_NOT_FOUND:   errstr = "Muxer not found"; break;
    case AVERROR_DECODER_NOT_FOUND: errstr = "Decoder not found"; break;
    case AVERROR_ENCODER_NOT_FOUND: errstr = "Encoder not found"; break;
    case AVERROR_PROTOCOL_NOT_FOUND:errstr = "Protocol not found"; break;
    case AVERROR_FILTER_NOT_FOUND:  errstr = "Filter not found"; break;
    case AVERROR_BSF_NOT_FOUND:     errstr = "Bitstream filter not found"; break;
    case AVERROR_STREAM_NOT_FOUND:  errstr = "Stream not found"; break;
    }

    if (errstr) {
        av_strlcpy(errbuf, errstr, errbuf_size);
    } else {
#if HAVE_STRERROR_R
        ret = strerror_r(AVUNERROR(errnum), errbuf, errbuf_size);
#else
        ret = -1;
#endif
        if (ret < 0)
            snprintf(errbuf, errbuf_size, "Error number %d occurred", errnum);
    }

    return ret;
}

void av_free_packet(AVPacket *pkt)
{
    if (pkt) {
        if (pkt->destruct) pkt->destruct(pkt);
        pkt->data = NULL; pkt->size = 0;
    }
}

// XXX: suppress the packet queue 
static void flush_packet_queue(AVFormatContext *s)
{
    AVPacketList *pktl;

    for(;;) {
        pktl = s->packet_buffer;
        if (!pktl)
            break;
        s->packet_buffer = pktl->next;
        av_free_packet(&pktl->pkt);
        av_free(pktl);
    }
    while(s->raw_packet_buffer){
        pktl = s->raw_packet_buffer;
        s->raw_packet_buffer = pktl->next;
        av_free_packet(&pktl->pkt);
        av_free(pktl);
    }
    s->packet_buffer_end=
    s->raw_packet_buffer_end= NULL;
    s->raw_packet_buffer_remaining_size = RAW_PACKET_BUFFER_SIZE;
}

// Flush the frame reader.
void ff_read_frame_flush(AVFormatContext *s)
{
    AVStream *st;
    int j;

    flush_packet_queue(s);

    s->cur_st = NULL;

    // for each stream, reset read state 
    for(unsigned i = 0; i < s->nb_streams; i++) {
        st = s->streams[i];

        if (st->parser) {
            av_parser_close(st->parser);
            st->parser = NULL;
            av_free_packet(&st->cur_pkt);
        }
        st->last_IP_pts = AV_NOPTS_VALUE;
        st->cur_dts = AV_NOPTS_VALUE; // we set the current DTS to an unspecified origin 
        st->reference_dts = AV_NOPTS_VALUE;
        // fail safe 
        st->cur_ptr = NULL;
        st->cur_len = 0;

        st->probe_packets = MAX_PROBE_PACKETS;

        for(j=0; j<MAX_REORDER_DELAY+1; j++)
            st->pts_buffer[j]= AV_NOPTS_VALUE;
    }
}

int avformat_seek_file(AVFormatContext *s, int stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, int flags)
{
    if(min_ts > ts || max_ts < ts)
        return -1;

    ff_read_frame_flush(s);

    if (s->iformat->read_seek2)
        return s->iformat->read_seek2(s, stream_index, min_ts, ts, max_ts, flags);

    if(s->iformat->read_timestamp){
        //try to seek via read_timestamp()
    }

    //Fallback to old API if new is not implemented but old is
    //Note the old has somewat different sematics
    if(s->iformat->read_seek || 1)
        return av_seek_frame(s, stream_index, ts, flags | (ts - min_ts > max_ts - ts ? AVSEEK_FLAG_BACKWARD : 0));

    // try some generic seek like av_seek_frame_generic() but with new ts semantics
}

void avsubtitle_free(AVSubtitle *sub)
{
    for (unsigned i = 0; i < sub->num_rects; i++)
    {
        av_freep(&sub->rects[i]->pict.data[0]);
        av_freep(&sub->rects[i]->pict.data[1]);
        av_freep(&sub->rects[i]->pict.data[2]);
        av_freep(&sub->rects[i]->pict.data[3]);
        av_freep(&sub->rects[i]->text);
        av_freep(&sub->rects[i]->ass);
        av_freep(&sub->rects[i]);
    }

    av_freep(&sub->rects);

    memset(sub, 0, sizeof(AVSubtitle));
}

