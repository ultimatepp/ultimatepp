#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "SDLCtrl.h"

SDL_Cursor *CursorFromImage(const Image &image) {
	int width = image.GetWidth();
	int height = image.GetHeight();
	int dim = width*height/8;
	Buffer<byte> data(dim);
	Buffer<byte> mask(dim);
	
  	int i = -1;
  	for (int row = 0; row < height; ++row) {
    	for (int col = 0; col < width; ++col) {
      		if (col % 8) {
        		data[i] <<= 1;
        		mask[i] <<= 1;
      		} else {
        		++i;
        		data[i] = mask[i] = 0;
      		}
      		const RGBA *rgba = &image[row][col];
      		if (*rgba == Black()) {
          		data[i] |= 0x01;
          		mask[i] |= 0x01;
        	} else if (*rgba == White())
          		mask[i] |= 0x01;
      	}
    }
  	return SDL_CreateCursor(data, mask, width, height, image.GetHotSpot().x, image.GetHotSpot().y);
}

SDLCtrl::SDLCtrlIn::SDLCtrlIn() {
	surface = 0;
	delIn = false;
	cursor = 0;
	
	int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
#if !defined(PLATFORM_WIN32) && !defined(__APPLE__)
    flags |= SDL_INIT_EVENTTHREAD; // Not supported on Windows or Mac OS X 
#endif
    if (SDL_Init (flags) < 0) {
		SetError("Init failure");
		init = false;
		return;
	} 
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

#ifdef PLATFORM_WIN32			// I cannot get cursor colors now
	if (cursor = CursorFromImage(Image::Arrow()))		// Restores normal arrow cursor
		SDL_SetCursor(cursor);
#endif
	init = true;
	
	videoflags = SDL_NOFRAME | SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
	bpp = 24;
	
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);

	if(1 > SDL_GetWMInfo(&wmInfo)) {
		SetError("WMInfo failure");
		return;
	}
#ifdef PLATFORM_WIN32
	hwndSDL = wmInfo.window;
#else
	wmInfo.info.x11.lock_func();
	hwnd = long(wmInfo.info.x11.wmwindow);
	display = (XDisplay*)wmInfo.info.x11.display;
	XSync(display, false);
	wmInfo.info.x11.unlock_func();
#endif
}

SDLCtrl::SDLCtrlIn::~SDLCtrlIn() {
	if (cursor)
		SDL_FreeCursor(cursor);
	if (surface)
		SDL_FreeSurface(surface);
	if (init)
		SDL_Quit();
}

bool SDLCtrl::SDLCtrlIn::CreateScreen() {
#ifdef PLATFORM_POSIX	
	if (!isInitialized)
		return false;
#endif
	if (surface)
		SDL_FreeSurface(surface);
	Rect r = GetView();
	surface = SDL_SetVideoMode(r.GetWidth(), r.GetHeight(), bpp, videoflags);
	if(!surface) {
		SetError(SDL_GetError());
		return false;
	}
	return true;
}

void SDLCtrl::SDLCtrlIn::Layout() {
#ifdef PLATFORM_WIN32	
	if (!hwndSDL || !hwnd) 
		return;
	GuiLock __;
	HWND phwnd = GetTopCtrl()->GetHWND();
	if(phwnd) {	
		Rect r = GetScreenView();
		SetWindowPos(hwnd, NULL, r.left, r.top, 
					 r.GetWidth(), r.GetHeight(), SWP_NOACTIVATE|SWP_NOZORDER);
	}
	ShowWindow(hwndSDL, IsVisible() ? SW_SHOW : SW_HIDE);
#endif
	if(!CreateScreen())
		return;
}

#ifdef PLATFORM_WIN32	
void SDLCtrl::SDLCtrlIn::State(int reason) {
	if (!hwndSDL)
		return;
	switch(reason) {
	case OPEN:
		{			
			GuiLock __;		
			CloseHWND();
			HWND phwnd = GetTopCtrl()->GetHWND();
			if(phwnd) {
				hwnd = hwndSDL;
				SetParent(hwndSDL, phwnd);
			}
		}
		break;
	default:
		DHCtrl::State(reason);
	}
}
#endif

void SDLCtrl::SDLCtrlIn::SetError(String str) {
	if (!strError.IsEmpty())
		strError << "\n"; 
	strError << InitCaps(str); 
}