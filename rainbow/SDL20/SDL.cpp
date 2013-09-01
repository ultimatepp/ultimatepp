#include "Local.h"

#ifdef GUI_SDL20

NAMESPACE_UPP

#define LLOG(x)       //LOG(x)

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
void ScheduleWakup()
{
	waketimer_id = SDL_AddTimer(20, WakeCb, NULL);
}

void FBQuitSession()
{
	SDL_Event event;
	event.type=SDL_QUIT;
	SDL_PushEvent(&event);
}

bool FBIsWaitingEvent()
{
	SDL_PumpEvents();
	SDL_Event events;
	int tc = SDL_PeepEvents(&events, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
	return (tc>0);
}

bool FBProcessEvent(bool *quit)
{
	DLOG("FBProcessEvent");
	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT && quit)
			*quit = true;
		HandleSDLEvent(&event);
		return true;
	}
	return false;
}

void FBSleep(int ms)
{
	DLOG("FBSleep " << ms);
	SDL_Delay(ms); //sleep should be wakeable with input
	//ProcessEvents needs to process sth from queue each now and then.
	//if no input is generated, no TimerProc call is performed, because queue ws empty.
	//win32 backend has WM_TIMER message, that gets posted automatically into queue each 10ms.
	//we dont have means to define a 'background' timer in an efficient way
	//which could wakeup queue with smaller intervall (10ms) than sleep (20ms).
	//we could go long way with a rescheduled timer (expensive)
	//or the short way, keeping queue busy after each sleep
	WakeCb(0,0);
}

void FBInitUpdate()
{
	DLOG("FBInitUpdate");
// TODO
//	if(SDL_MUSTLOCK(screen))
//		SDL_LockSurface(screen);
}

void FBUpdate(const Rect& inv)
{
	DLOG("FBUpdate");
// TODO
#if 0
	//The invalidated areas accumulate in the update region until the region is processed when the next WM_PAINT message occurs
	const ImageBuffer& framebuffer = Ctrl::GetFrameBuffer();

#if 1
	memcpy(screen->pixels, ~framebuffer, framebuffer.GetLength() * sizeof(RGBA));
#endif

#if 0
	ASSERT(Size(screen->w,screen->h) == framebuffer.GetSize());

	Size ssz = inv.GetSize();
	Size dsz = framebuffer.GetSize();

	ASSERT(Rect(dsz).Contains(inv));

	for(int i = inv.top; i < inv.bottom; i++)
	{
		uint32 o = i * dsz.cx + inv.left;
		memcpy(((RGBA*)screen->pixels) + o, (~framebuffer) + o, ssz.cx * sizeof(RGBA));
	}
#endif
#endif
}

void FBFlush()
{
}

void FBInit()
{
	GuiLock __;
	
	SDL_Init(SDL_INIT_EVERYTHING);

	Ctrl::InitFB();

#if 0
	if(SDL_Init(SDL_INIT_VIDEO/* | SDL_INIT_TIMER*/) < 0) //timer not needed, we post to queue directly
	{
		Cout() << Format("Couldn't initialize SDL: %s\n", SDL_GetError());
		return;
	}
#endif

// TODO
//	SDL_EnableUNICODE(1); //for unicode keycode availability
//	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL/2);
//	SDL_ShowCursor(0);

#if 0
	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	//ASSERT(vi->hw_available);

	width = vi->current_w;
	height = vi->current_h;
	bpp = vi->vfmt->BitsPerPixel;
	ASSERT(bpp == 32);
	
	//FIXME adjustable
	videoflags = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE;// | SDL_NOFRAME | SDL_FULLSCREEN;
#endif

	screen_size = Size(1024, 768);
	screen.Create(screen_size, "First test");

	Ctrl::SetFramebufferSize(screen_size);
	
	DLOG("INIT!");
}

void FBDeInit()
{
	SDL_RemoveTimer(waketimer_id);
	Ctrl::ExitFB();
	screen.Destroy();
	SDL_Quit();
	DLOG("EXIT!");
}

END_UPP_NAMESPACE

#endif