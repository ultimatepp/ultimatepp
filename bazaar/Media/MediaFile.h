#ifndef _Media_mediafile_h_
#define _Media_mediafile_h_

#include <SDL/SDLCtrl.h>
#include <Media/ffmpeg_base.h>
#include "mt.h"

/* SDL audio buffer size, in samples. Should be small to have precise
   A/V sync as SDL does not have hardware buffer fullness info. */
#define SDL_AUDIO_BUFFER_SIZE 1024

#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)
#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_SUBTITLEQ_SIZE (5 * 16 * 1024)

#define VIDEO_PICTURE_QUEUE_SIZE 2
#define SUBPICTURE_QUEUE_SIZE 4

// we use about AUDIO_DIFF_AVG_NB A-V differences to make the average 
#define AUDIO_DIFF_AVG_NB   20

// NOTE: the size must be big enough to compensate the hardware audio buffersize size 
#define SAMPLE_ARRAY_SIZE (2*65536)

/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

/* no AV sync correction is done if below the AV sync threshold */
#define AV_SYNC_THRESHOLD 0.01
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

#define FF_ALLOC_EVENT   (SDL_USEREVENT)
#define FF_REFRESH_EVENT (SDL_USEREVENT + 1)
#define FF_QUIT_EVENT    (SDL_USEREVENT + 2)

enum {
    AV_SYNC_AUDIO_MASTER, /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

struct VideoPicture {
    double pts;          	// presentation time stamp for this picture
    double target_clock; 	// GetUSec() time at which this should be displayed ideally
    int64_t pos;         	// byte position in file
    SDL_Overlay *overlay;
    SDL_Surface *surface;
    int width, height; 		// source height & width 
    bool allocated;
    enum PixelFormat pix_fmt;
};

struct SubPicture {
    double pts; 			// presentation time stamp for this picture 
    AVSubtitle sub;
};

class PacketQueue {
public:
	PacketQueue();
	void SetMaxSize(int _maxSize)	{maxSize = _maxSize;};
	void Init();
	void Init(AVPacket &pkt, AVPacket *flush_pkt);
	int Put(AVPacket *pkt, AVPacket *flush_pkt);
	int Get(AVPacket *pkt,  bool decodeEnd);
	void Flush();
	void End();
	void Abort();
	inline bool IsAbort() 			{return abort_request;};
	bool IsFull()					{return size > maxSize;};
	
private:
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    int maxSize;
    bool abort_request;
    Mutex mutex;
    ConditionVariable cond;
};

void sdl_audio_callback(void *data, Uint8 *stream, int len);
	
class MediaFile {
typedef MediaFile CLASSNAME; 
public:
	friend int decoderInterrupt_cb();
	friend void sdl_audio_callback(void *data, Uint8 *stream, int len);

	MediaFile();
	~MediaFile();

	inline bool IsOpened()				{return fileData;};

	inline String GetFileName()			{return !fileData ? "" : fileData->filename;};
	inline double GetDuration()			{return !fileData ? -1 : double(fileData->duration) / AV_TIME_BASE;};
	inline int64 GetDurationTimeBase()	{return !fileData ? -1 : fileData->duration;};
	inline int GetNumStreams()			{return !fileData ? -1 : fileData->nb_streams;};	
	
	int GetNumVideo()					{return numVideoStreams;};
	int GetNumAudio()					{return numAudioStreams;};
	
	inline double GetSecond() 			{return second;};
	void SetSecond(double second);
	
	void EnableAudio(bool v = true)		{audio_disable = !v;};
	void EnableVideo(bool v = true)		{video_disable = !v;};
	
	inline void SetVideo(int id)		{if(id < GetNumStreams() && id >= -1) idVideo = id;};	
	inline void SetAudio(int id)		{if(id < GetNumStreams() && id >= -1) idAudio = id;};
	inline void SetSubtitle(int id)		{if(id < GetNumStreams() && id >= -1) idSubtitle = id;};
	inline int GetVideo()				{return idVideo;};
	inline int GetAudio()				{return idAudio;};
	inline int GetSubtitle()			{return idSubtitle;};
	
	inline MediaFile& ShowAudio(bool show = true)	{showAudio = show; return *this;};
	inline MediaFile& SetShowAudioFPS(double fps)	{showAudioFps = fps; return *this;};
	
	inline MediaFile& SetAudioFactor(double f)	{audioFactor = int(f*1000); return *this;};
	
	struct StreamData {
		int id;
		String codec;
		String lang;
		uint32 bitrate; 
		String tags;		// Tags in metadata
	};
	struct VideoData : public StreamData {
		String pixFmt;
		Size size;
		Size par;			// Pixel aspect ratio
		Size dar;			// Display aspect ratio
		Size frameRate, tbr, tbn, tbc;		
	};
	struct AudioData : public StreamData {
		int channels;
		String bits;
		uint32 sampleRate;	
	};
	Array<AudioData> audioData;
	int GetAudioId();
	void SetAudioId(int id);
		
	Array<VideoData> videoData;
	int GetVideoId();
	void SetVideoId(int id);
	
	inline MediaFile& SetRGB(bool v)	{rgb = v; return *this;};	
		
protected:	
	void Reset();
	void stream_seek(int64_t pos, int64_t rel, bool seek_by_bytes);
	bool StreamOpen(int streamId);
	void StreamClose(int streamId);
	int audio_decode_frame(double *pts_ptr);
	void update_sample_display(short *samples, int samples_size);
	int synchronize_audio(short *samples, int samples_size1, double pts);
	int GetAudioOutputBufferSize();
	double get_audio_clock();
	double get_video_clock();
	double get_master_clock();
	double get_external_clock();
	bool queue_picture(AVFrame *src_frame, double pts);
	void stream_pause();
	void DecoderThread();//void *arg);
	void VideoThread();//(void *arg);
	void SubtitleThread();//(void *arg);
	bool BeginDecoder();
	
	AVFormatContext *fileData;
	PacketQueue videoq, audioq, subtitleq;
	AtomicVar requestAbort,
			  decodeThEnd, decodeThEnded, 
			  audioThEnd, videoThEnd, subtitleThEnd;
	int threadCount;
	int numVideoStreams, numAudioStreams;
	double second, lastSecond;
	int idVideo, idAudio, idSubtitle;
	String tags;
	bool audio_disable, video_disable;
	bool seek_by_bytes;
	int64_t start_time;
	bool seek_req;
    int seek_flags;
    int64_t seek_pos;
    int64_t seek_rel;
   	int paused, last_paused;
	AVPacket flush_pkt;
	
	VideoPicture pictq[VIDEO_PICTURE_QUEUE_SIZE];
    int pictq_size, pictq_rindex, pictq_windex;
    Mutex pictq_mutex;
    ConditionVariable pictq_cond;

    SubPicture subpq[SUBPICTURE_QUEUE_SIZE];
    int subpq_size, subpq_rindex, subpq_windex;
    Mutex subpq_mutex;
    ConditionVariable subpq_cond;
    
	AVStream *audioStream, *videoStream, *subtitleStream;
	
	AVAudioConvert *reformat_ctx;
	
	Thread video_tid, decoderThreadId, /*refresh_tid,*/ subtitle_tid;
		
	int subtitle_stream_changed;
	int debug, debug_mv, workaround_bugs;
	int lowres;
	bool fast;
	int genpts;
	int idct;	
	enum AVDiscard skip_frame, skip_idct, skip_loop_filter;
	int decoder_reorder_pts;
	
	int64_t audio_callback_time;
	
	// Samples output by the codec. we reserve more space for avsync compensation 
    DECLARE_ALIGNED(16,uint8_t,audio_buf1)[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
    DECLARE_ALIGNED(16,uint8_t,audio_buf2)[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
    uint8_t *audio_buf;
    unsigned int audio_buf_size; 	// In bytes 
    int audio_buf_index; 			// In bytes 
    
    bool showAudio; // if true, display audio samples in graphs 
    double showAudioFps; 
    double lastAudioShow;
    
    int16_t sample_array[SAMPLE_ARRAY_SIZE];
    int sample_array_index;
    
  	double audio_clock;
    double audio_diff_cum; // used for AV difference average computation 
    double audio_diff_avg_coef;
    double audio_diff_threshold;
    int audio_diff_avg_count;
    int audio_hw_buf_size;
    
    enum SampleFormat audio_src_fmt;
    
   	AVPacket audio_pkt, audio_pkt_temp;
   	
  	double video_current_pts;    	//Current displayed pts (different from video_clock if frame fifos are used)
    int64_t video_current_pts_time;	//Time (GetUSec) at which we updated video_current_pts - used to have running video pts
	int64_t video_current_pos;    	//Current displayed file pos
	
	int av_sync_type;
	TimeStop externalClock;
    double video_clock;          	// pts of last decoded frame / predicted pts of next decoded frame
    
    double frame_timer;
    bool step;						// Jump to next frame
    
	String strError;
	String GetError() 				{return strError;};
	void SetError(String str);
	void ResetError()				{strError = "";};
	
	AtomicVar rgb;						// Is the image YUV or RGB
	bool forceAspect;					// Force aspect ratio
	Rect imgRect;						// Image size
	Mutex mtx;
	
	AtomicVar audioFactor;
};


#endif
