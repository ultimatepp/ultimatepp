#ifndef _SDL20_SDL20_h_
#define _SDL20_SDL20_h_

#include <Painter/Painter.h>

#include <SDL2/SDL.h>

using namespace Upp;

struct SDLWindow {
	SDL_Window   *win;
	SDL_Renderer *ren;
	int64         serial;

	bool Create(const Rect& rect, const char *title);
	void Destroy();
	
	void Present();
	
	operator bool() const { return win; }
	
	SDLWindow();
	~SDLWindow();
};

struct SDLDraw : SDraw {
	SDLWindow *win;

	virtual void  PutImage(Point p, const Image& m, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
	virtual Image RenderGlyph(Point at, int angle, int chr, Font fnt, Color color, Size sz);

	void Set(SDLWindow& win_);

	SDLDraw();	
	~SDLDraw();
};

#endif
