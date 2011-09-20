#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "VLCPlayer.h"


static void *lock(void *data, void **p_pixels)
{
	Ctx *ctx = (Ctx*)data;
	
	SDL_LockMutex(ctx->mutex);
	SDL_LockSurface(ctx->surf);
	*p_pixels = ctx->surf->pixels;
	return NULL; /* picture identifier, not needed here */
}

static void unlock(void *data, void *id, void *const *p_pixels)
{
	Ctx *ctx = (Ctx*)data;
	
	SDL_UnlockSurface(ctx->surf);
	SDL_UnlockMutex(ctx->mutex);
	
	
}

static void display(void *data, void *id)
{
	/* VLC wants to display the video */
	(void) data;
}



void VLCPlayer::Layout(){
}

void VLCPlayer::Init() {
	if (initialized) return;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO |SDL_INIT_EVENTTHREAD) == -1)
	{
		LOG("cannot initialize SDL\n");
	}
	
	rr = GetRect();
	rect.x = 0;
	rect.y = 0;
	rect.w = ws.cx;
	rect.h = ws.cy;
	
	
	
//	libvlc_media_player_set_xdrawable (mp, GetParent().GetWindow());
		
	options = SDL_ANYFORMAT | SDL_HWSURFACE |  SDL_DOUBLEBUF | SDL_HWPALETTE;

	ctx.mutex = SDL_CreateMutex();
	ctx.surf = SDL_CreateRGBSurface(SDL_SWSURFACE, rect.w, rect.h, 32, 0xff0000, 0x00FF00, 0x0000ff, 0);
	screen = SDL_SetVideoMode(rect.w, rect.h, 16, options);
	done = 0, action = 0, pause = 0, n = 0;

	/*
	 *  Initialise libVLC
	 */
    char const *vlc_argv[] = 
	{
		"-I", "dummy", /* Don't use any interface */
		"--no-media-library",
		"--ignore-config", /* Don't use VLC's config */
//		"--extraintf=logger", //log anything
//		"--verbose=2", //be much more verbose then normal for debugging purpose
		"--no-video-title-show",
		"--no-xlib", /* tell VLC to not use Xlib */
    };
	vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);


	libvlc = libvlc_new(vlc_argc, vlc_argv);
	
	ml = libvlc_media_list_new (libvlc);
	
	mp = libvlc_media_player_new(libvlc);
	
	mlp = libvlc_media_list_player_new(libvlc);
	
	libvlc_media_list_player_set_media_player(mlp,mp);
	emode = libvlc_playback_mode_loop;
	libvlc_media_list_player_set_playback_mode(mlp,emode);
	
	libvlc_media_list_player_set_media_list(mlp,ml);
	
	libvlc_video_set_callbacks(mp, lock, unlock, display, &ctx);
	libvlc_video_set_format(mp, "RV32", rect.w, rect.h, rect.w*4);
	initialized = true;
}


void VLCPlayer::Play() {
	
	if (AtomicRead(playeractive)==1) return;
	if (AtomicRead(initialized)!=1) Init();
	libvlc_media_list_player_play(mlp);
	
	AtomicWrite(playeractive,1);
	
	Ctrl::ProcessEvents();
	Thread().Run(THISBACK(PlayLoop));
}
void VLCPlayer::PlayLoop() {
	
	while (!done && AtomicRead(playeractive)==1)
	{
		action = 0;
		//Ctrl::ProcessEvents();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					done = 1;
					break;
				case SDL_KEYDOWN:
					action = event.key.keysym.sym;
					break;
			}
		}
	
		SDL_LockMutex(ctx.mutex);

		rr = GetRect();
		
		
		rect.x = 0;
		rect.y = 0;
		rect.w = ws.cx;
		rect.h = ws.cy;
		//if (rr.Width()< 8 || rr.Height() < 8 ) rect.w = rect.h = 16;
		
		SDL_BlitSurface(ctx.surf, NULL, screen, &rect);
		SDL_UnlockMutex(ctx.mutex);
		
		SDL_Flip(screen);
		SDL_Delay(20);
	}
	return;
}

void VLCPlayer::Stop() {
	libvlc_media_list_player_stop(mlp);
	AtomicWrite(playeractive,0);
};
VLCPlayer::VLCPlayer() {
	SetBpp(16);
	
	this->SetTimeCallback(1000,THISBACK(StartPlay));
}
void VLCPlayer::StartPlay(){
	Play();
}

void VLCPlayer::ClearPlayList() {
	while (libvlc_media_list_count(ml)>0){
		libvlc_media_list_remove_index (ml, 0);
	}
	
}
void VLCPlayer::AddPlayList(String s){
	Init();
	if (FileExists(s)) {
		m = libvlc_media_new_path(libvlc, s);
		libvlc_media_list_add_media (ml, m);
		libvlc_media_release(m);
	}	
}

VLCPlayer::~VLCPlayer() {
	/*
	 * Clean up libVLC
	 */
//	if (mlp!=NULL) libvlc_media_list_player_release(mlp);
//	if (mp!=NULL) libvlc_media_player_release(mp);
//	if (libvlc!=NULL) libvlc_release(libvlc);
}
void VLCPlayer::Close() {
	libvlc_media_list_player_stop(mlp);
	AtomicWrite(playeractive,0);
	Sleep(15);

	SDL_DestroyMutex(ctx.mutex);
	SDL_FreeSurface(ctx.surf);
	SDL_FreeSurface(screen);
	SDL_Quit();
	
	TopWindow::CloseTopCtrls();
	
}
