#ifndef _Media_Player_h_
#define _Media_Player_h_


#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)
#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_SUBTITLEQ_SIZE (5 * 16 * 1024)

/* SDL audio buffer size, in samples. Should be small to have precise
   A/V sync as SDL does not have hardware buffer fullness info. */
#define SDL_AUDIO_BUFFER_SIZE 1024

/* no AV sync correction is done if below the AV sync threshold */
#define AV_SYNC_THRESHOLD 0.01
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

#define FRAME_SKIP_FACTOR 0.05

/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
#define SAMPLE_ARRAY_SIZE (2*65536)

struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    int abort_request;
    SDL_mutex *mutex;
    SDL_cond *cond;
};

#define VIDEO_PICTURE_QUEUE_SIZE 2
#define SUBPICTURE_QUEUE_SIZE 4

struct VideoPicture {
    double pts;                                  ///<presentation time stamp for this picture
    double target_clock;                         ///<av_gettime() time at which this should be displayed ideally
    int64_t pos;                                 ///<byte position in file
    SDL_Overlay *bmp;
    int width, height; /* source height & width */
    int allocated;
    enum PixelFormat pix_fmt;
};

struct SubPicture {
    double pts; /* presentation time stamp for this picture */
    AVSubtitle sub;
};

enum {
    AV_SYNC_AUDIO_MASTER, /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

bool play_stream(const char * input_filename);

class VideoState {
public:
	VideoState();
	void Reset();
	void close();
	
	bool event_loop();
	void schedule_refresh(int delay);
	int audio_write_get_buf_size();
	void video_image_display();
	void video_audio_display();
	int video_open();
	void video_display();
	double get_audio_clock();
	double get_video_clock();
	double get_external_clock();
	double get_master_clock();
	void stream_seek(int64_t pos, int64_t rel, int seek_by_bytes);
	void stream_pause();
	double compute_frame_delay(double frame_current_pts);
	int queue_picture(AVFrame *src_frame, double pts);
	int output_picture2(AVFrame *src_frame, double pts1);
	void update_sample_display(short *samples, int samples_size);
	int synchronize_audio(short *samples, int samples_size1, double pts);
	int audio_decode_frame(double *pts_ptr);
	int stream_component_open(int stream_index);
	void stream_component_close(int stream_index);
	void stream_cycle_channel(int codec_type);
	void toggle_pause();
	void step_to_next_frame();
	void toggle_audio_display();
	bool alloc_picture();

    SDL_Thread *parse_tid;
    SDL_Thread *video_tid;
    SDL_Thread *refresh_tid;
    AVInputFormat *iformat;
    int no_background;
    int abort_request;
    int paused;
    int last_paused;
    int seek_req;
    int seek_flags;
    int64_t seek_pos;
    int64_t seek_rel;
    int read_pause_return;
    AVFormatContext *icc;		

    int audio_stream;

    int av_sync_type;
    double external_clock; /* external clock base */
    int64_t external_clock_time;

    double audio_clock;
    double audio_diff_cum; /* used for AV difference average computation */
    double audio_diff_avg_coef;
    double audio_diff_threshold;
    int audio_diff_avg_count;
    AVStream *audio_st;
    PacketQueue audioq;
    int audio_hw_buf_size;
    /* samples output by the codec. we reserve more space for avsync compensation */
    DECLARE_ALIGNED(16,uint8_t,audio_buf1)[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
    DECLARE_ALIGNED(16,uint8_t,audio_buf2)[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
    uint8_t *audio_buf;
    unsigned int audio_buf_size; /* in bytes */
    int audio_buf_index; /* in bytes */
    AVPacket audio_pkt_temp;
    AVPacket audio_pkt;
    enum SampleFormat audio_src_fmt;
    AVAudioConvert *reformat_ctx;

    int show_audio; /* if true, display audio samples */
    int16_t sample_array[SAMPLE_ARRAY_SIZE];
    int sample_array_index;
    int last_i_start;

    SDL_Thread *subtitle_tid;
    int subtitle_stream;
    int subtitle_stream_changed;
    AVStream *subtitle_st;
    PacketQueue subtitleq;
    SubPicture subpq[SUBPICTURE_QUEUE_SIZE];
    int subpq_size, subpq_rindex, subpq_windex;
    SDL_mutex *subpq_mutex;
    SDL_cond *subpq_cond;

    double frame_timer;
    double frame_last_pts;
    double frame_last_delay;
    double video_clock;                          ///<pts of last decoded frame / predicted pts of next decoded frame
    int video_stream;
    AVStream *video_st;
    PacketQueue videoq;
    double video_current_pts;                    ///<current displayed pts (different from video_clock if frame fifos are used)
    int64_t video_current_pts_time;              ///<time (av_gettime) at which we updated video_current_pts - used to have running video pts
	int64_t video_current_pos;                   ///<current displayed file pos
    VideoPicture pictq[VIDEO_PICTURE_QUEUE_SIZE];
    int pictq_size, pictq_rindex, pictq_windex;
    SDL_mutex *pictq_mutex;
    SDL_cond *pictq_cond;

    String filename;
    int width, height, xleft, ytop;

    float skip_frames;
    int refresh;
    
	const char *input_filename;
	const char *window_title;
	int fs_screen_width;
	int fs_screen_height;
	int screen_width;
	int screen_height;
	int frame_width;
	int frame_height;
	enum PixelFormat frame_pix_fmt;
	int audio_disable;
	int video_disable;
	int wanted_audio_stream;
	int wanted_video_stream;
	int wanted_subtitle_stream;
	int seek_by_bytes;
	int show_status ;
	int64_t start_time;
	int64_t duration;
	int debug;
	int debug_mv;
	int step;
	int thread_count;
	int workaround_bugs;
	int fast;
	int genpts;
	int lowres;
	int idct;
	enum AVDiscard skip_frame;
	enum AVDiscard skip_idct;
	enum AVDiscard skip_loop_filter;
	int decoder_reorder_pts;
	int loop;
	int framedrop;
	
	int rdftspeed;
	int64_t audio_callback_time;
	
	AVPacket flush_pkt;
	
	SDL_Surface *screen;
	
	String strError;
};


#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT    (SDL_USEREVENT + 2)

#endif
