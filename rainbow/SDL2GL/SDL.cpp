#include "Local.h"

#ifdef GUI_SDL20GL

NAMESPACE_UPP

#define LLOG(x)       //LOG(x)

dword SDLsettings;

void USDLSetup(dword flags)
{
	SDLsettings = flags;
}

Size      screen_size;
SDLWindow screen;

SDL_TimerID waketimer_id = 0;
Uint32 WakeCb(Uint32 interval, void *param)
{
	//wake up message que, FIXME maybe it can be done better?
	SDL_Event event;
	event.type=SDL_USEREVENT;
	SDL_PushEvent(&event);
	return 0;
}

void WakeUpGuiThread()
{
	waketimer_id = SDL_AddTimer(20, WakeCb, NULL);
}

void FBInit()
{
	GuiLock __;
	
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);

	Ctrl::InitFB();

#if 0
	if(SDL_Init(SDL_INIT_VIDEO/* | SDL_INIT_TIMER*/) < 0) //timer not needed, we post to queue directly
	{
		Cout() << Format("Couldn't initialize SDL: %s\n", SDL_GetError());
		return;
	}

// TODO
//	SDL_EnableUNICODE(1); //for unicode keycode availability
//	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL/2);
//	SDL_ShowCursor(0);

	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	//ASSERT(vi->hw_available);

	width = vi->current_w;
	height = vi->current_h;
	bpp = vi->vfmt->BitsPerPixel;
	ASSERT(bpp == 32);
	
	//FIXME adjustable
	videoflags = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE;// | SDL_NOFRAME | SDL_FULLSCREEN;
#endif
	screen_size = Size(1500, 900);
	screen.Create(Rect(Point(20, 20), screen_size), "First test");

	Ctrl::SetDesktopSize(screen_size);
}

void FBDeInit()
{
	SDL_RemoveTimer(waketimer_id);
	Ctrl::ExitFB();
	screen.Destroy();
	SDL_Quit();
}

END_UPP_NAMESPACE

#endif