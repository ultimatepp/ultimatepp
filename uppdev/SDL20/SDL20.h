#ifndef _SDL20_SDL20_h_
#define _SDL20_SDL20_h_

#include <Painter/Painter.h>

#include <SDL2/SDL.h>

#include <plugin/FT_fontsys/FT_fontsys.h>

using namespace Upp;

#ifdef PLATFORM_POSIX
#include <CtrlCore/stdids.h>
#endif

NAMESPACE_UPP

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

class SystemDraw : public SDraw {
public:
	SDLWindow *win;

	virtual void  PutImage(Point p, const Image& m, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);

	void Set(SDLWindow& win_);

	SystemDraw();	
	~SystemDraw();
};

END_UPP_NAMESPACE

/*
class BackDraw : public SDLDraw {
public:
	virtual bool  IsPaintingOp(const Rect& r) const;

protected:
	HBITMAP hbmpold;
	HBITMAP hbmp;

	Size    size;
	Draw   *painting;
	Point   painting_offset;

public:
	void  Put(SystemDraw& w, int x, int y);
	void  Put(SystemDraw& w, Point p)                  { Put(w, p.x, p.y); }

	void Create(SystemDraw& w, int cx, int cy);
	void Create(SystemDraw& w, Size sz)                { Create(w, sz.cx, sz.cy); }
	void Destroy();

	void SetPaintingDraw(Draw& w, Point off)           { painting = &w; painting_offset = off; }

	BackDraw();
	~BackDraw();
};
*/

#endif
