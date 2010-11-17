#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Functions4U/Functions4U_Gui.h>
#include <SDL/SDLCtrl.h>

SDL_Cursor *CursorFromImage(const Image &image)
{
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
      		const RGBA *rgba = GetPixel(image, col, row);
      		if (*rgba == Black()) {
          		data[i] |= 0x01;
          		mask[i] |= 0x01;
        	} else if (*rgba == White())
          		mask[i] |= 0x01;
      	}
    }
  	return SDL_CreateCursor(data, mask, width, height, image.GetHotSpot().x, image.GetHotSpot().y);
}

SDLCtrl::SDLCtrl() {
	screen = 0;
	
	int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
#if !defined(PLATFORM_WIN32) && !defined(__APPLE__)
    flags |= SDL_INIT_EVENTTHREAD; /* Not supported on Windows or Mac OS X */
#endif
    if (SDL_Init (flags) < 0) {
		SetError("Init failure");
		init = false;
		return;
	} 
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	if (cursor = CursorFromImage(Image::Arrow()))
		SDL_SetCursor(cursor);
	init = true;
	
	videoflags = SDL_NOFRAME | SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
	bpp = 8;
	
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);

	if(1 > SDL_GetWMInfo(&wmInfo)) {
		SetError("SDLCtrl: WMInfo failure");
		return;
	}
#ifdef PLATFORM_WIN32
	hwndSDL = long(wmInfo.window);
#else
	wmInfo.info.x11.lock_func();
	hwnd = long(wmInfo.info.x11.wmwindow);
	display = (XDisplay*)wmInfo.info.x11.display;
	XSync(display, false);
	wmInfo.info.x11.unlock_func();
#endif
}

SDLCtrl::~SDLCtrl() {
	if (cursor)
		SDL_FreeCursor(cursor);
	if (screen)
		SDL_FreeSurface(screen);
	if (init)
		SDL_Quit();
}

bool SDLCtrl::CreateScreen() {
#ifdef PLATFORM_POSIX	
	if (!isInitialized)
		return false;
#endif
	if (screen)
		SDL_FreeSurface(screen);
	Rect r = GetRect();
#ifndef __APPLE__
	screen = SDL_SetVideoMode(r.GetWidth(), r.GetHeight(), 0, videoflags);
#else
	screen = SDL_SetVideoMode(r.GetWidth(), r.GetHeight(), bpp, videoflags);
#endif
	if(!screen) {
		SetError(SDL_GetError());
		return false;
	}
	return true;
}

void SDLCtrl::Layout() {
#ifdef PLATFORM_WIN32	
	if (!hwndSDL || !hwnd) 
		return;
	GuiLock __;
	HWND phwnd = GetTopCtrl()->GetHWND();
	if(phwnd) {	
		Rect r = GetScreenRect();
		SetWindowPos(hwnd, NULL, r.left, r.top, 
					 r.GetWidth(), r.GetHeight(), SWP_NOACTIVATE|SWP_NOZORDER);
	}
	ShowWindow(HWND(hwndSDL), IsVisible() ? SW_SHOW : SW_HIDE);
#endif
	if(!CreateScreen())
		return;
}

#ifdef PLATFORM_WIN32	
void SDLCtrl::State(int reason)
{
	if (!hwndSDL)
		return;
	switch(reason) {
	case OPEN:
		{			
			GuiLock __;		
			CloseHWND();
			HWND phwnd = GetTopCtrl()->GetHWND();
			if(phwnd) {
				hwnd = HWND(hwndSDL);
				SetParent(HWND(hwndSDL), phwnd);
			}
		}
		break;
	default:
		DHCtrl::State(reason);
	}
}
#endif

void SDLCtrl::SetError(String str) {
	if (!strError.IsEmpty())
		strError << "\n"; 
	strError << ToUpper(str[0]) + DeQtfLf(str.Mid(1)); 
}