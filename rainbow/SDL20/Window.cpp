#include "Local.h"

#ifdef GUI_SDL20

NAMESPACE_UPP

bool SDLWindow::Create(const Rect& rect, const char *title)
{
	win = SDL_CreateWindow(title, rect.left, rect.top, rect.GetWidth(), rect.GetHeight(),
	                       SDL_WINDOW_SHOWN);
	if(!win)
		return false;
	MemoryIgnoreLeaksBegin();
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	MemoryIgnoreLeaksEnd();
	if(!ren) {
		Destroy();
		return false;
	}
	INTERLOCKED {
		static int64 h;
		serial = h++;
	}	
	return true;
}

void SDLWindow::Destroy()
{
	if(ren) {
		SDL_DestroyRenderer(ren);
		ren = NULL;
	}
	if(win) {
		SDL_DestroyWindow(win);
		win = NULL;
	}
}

void SDLWindow::Present()
{
	if(ren)
		SDL_RenderPresent(ren);
}

SDLWindow::SDLWindow()
{
	ren = NULL;
	win = NULL;
}

SDLWindow::~SDLWindow()
{
	Destroy();
}

END_UPP_NAMESPACE

#endif