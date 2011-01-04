#ifndef _Media_mediaplayer_h
#define _Media_mediaplayer_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <SDL/SDLCtrl.h>
#include "ffmpeg_base.h"
#include "MediaFile.h"

class MediaPlayer : public MediaFile, public SDLCtrl {
typedef MediaPlayer CLASSNAME;	
public:
	MediaPlayer();
	~MediaPlayer();
 	
	bool Load(const char *filename);
	bool Play();
	void Stop();
	void Pause();
	MediaPlayer &ForceAspect(bool f = true) {forceAspect = f; 	Layout(); return *this;};
	
	bool IsPaused()							{return paused;};
	
	Callback1<SDLSurface &> WhenFrame;
	Callback WhenSecond;
	Callback WhenPause;	

	String GetTags()	{return tags;};
	
	void Reset(SDL_Surface *screen, int videoFlags);
	
	void ResetError() 	{MediaFile::ResetError(); SDLCtrl::ResetError();};
	String GetError()	{return MediaFile::GetError()+SDLCtrl::GetError();};

private:
	bool Load0Failed();
	void close();
	void video_refresh();
	bool event_loop();
	void video_image_display();
	void video_audioWaveform();
	void video_display();
	double compute_frame_delay(double frame_current_pts);
	bool output_picture2(AVFrame *src_frame, double pts1);
	void step_to_next_frame();
	void toggle_audio_display();
	bool alloc_picture();
	void Layout();

    AVInputFormat *iformat;
    int last_i_start;
	enum PixelFormat frame_pix_fmt;
	SDL_Surface *screen;
	double aspect_ratio;
	int videoFlags;
	//bool forceAspect;
	uint64 msec;	
};

#endif

