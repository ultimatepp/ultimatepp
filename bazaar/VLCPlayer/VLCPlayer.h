#ifndef _Media_mediaplayer_h
#define _Media_mediaplayer_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <SDL/SDLCtrl.h>
#include <SDL/SDL_mutex.h>


#include <vlc/vlc.h>

struct Ctx
{
    SDL_Surface *surf;
    SDL_mutex *mutex;
};

class VLCPlayer :  public SDLCtrl {
typedef VLCPlayer CLASSNAME;	
public:
	volatile Atomic initialized;
	volatile Atomic  playeractive;
	libvlc_instance_t *libvlc;
	libvlc_media_t *m,*m1;
	libvlc_media_player_t *mp;
	libvlc_media_list_t *ml;
	libvlc_media_list_player_t *mlp;
	libvlc_playback_mode_t emode;
    char  *vlc_argv[];
    int vlc_argc;
	int done, action, pause, n;
	int options;
	virtual void Close();
	Rect rr;
	void Layout();

	void Init();
	VLCPlayer();
	~VLCPlayer();

	void Play();
	void PlayLoop();
	void Stop();
	void ClearPlayList();
	void AddPlayList(String s);
//	void Pause();
	Size ws;
	void SetVSize(Size ws1) { ws = ws1; }
	void StartPlay();
	
private:
	SDL_Surface  *screen;
	SDL_Event event;
	SDL_Rect rect;
	Ctx ctx;
};

#endif

