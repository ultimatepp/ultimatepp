#include <Draw/Draw.h>

#ifdef PLATFORM_WIN32
#include "DrawWin32.h"
#endif

#ifdef PLATFORM_X11
#include "DrawX11.h"
#endif

NAMESPACE_UPP

typedef DrawLock GuiLock;

inline void EnterGuiMutex()          { EnterGMutex(); }
inline void EnterGuiMutex(int n)     { EnterGMutex(n); }
inline void LeaveGuiMutex()          { LeaveGMutex(); }
inline int  LeaveGuiMutexAll()       { return LeaveGMutexAll(); }

bool ScreenInPaletteMode();
Size GetScreenSize();

class BackDraw : public SystemDraw {
public:
	virtual bool  IsPaintingOp(const Rect& r) const;

protected:
#ifdef PLATFORM_WIN32
	HBITMAP hbmpold;
	HBITMAP hbmp;
#endif
#ifdef PLATFORM_X11
	Pixmap  pixmap;
#endif
	Size    size;
	Draw   *painting;
	Point   painting_offset;


public:
	void  Put(SystemDraw& w, int x, int y);
	void  Put(SystemDraw& w, Point p)                  { Put(w, p.x, p.y); }

	void Create(SystemDraw& w, int cx, int cy);
	void Create(SystemDraw& w, Size sz)                { Create(w, sz.cx, sz.cy); }
//	void Create(int cx, int cy);
//	void Create(Size sz)                               { Create(sz.cx, sz.cy); }
	void Destroy();

	void SetPaintingDraw(Draw& w, Point off)           { painting = &w; painting_offset = off; }

	BackDraw();
	~BackDraw();
/*	
// Deprecated:
	void  Put(Draw& w, int x, int y)             { Put(*(SystemDraw*)&w, x, y); }
	void  Put(Draw& w, Point p)                  { Put(*(SystemDraw*)&w, p.x, p.y); }

	void Create(Draw& w, int cx, int cy)         { Create(*(SystemDraw*)&w, cx, cy); }
	void Create(Draw& w, Size sz)                { Create(*(SystemDraw*)&w, sz.cx, sz.cy); }*/
};

class ImageDraw : public SystemDraw {
	Size    size;

#ifdef PLATFORM_WIN32
	struct  Section {
		HDC     dc;
		HBITMAP hbmp, hbmpOld;
		RGBA   *pixels;

		void Init(int cx, int cy);
		~Section();
	};

	Section     rgb;
	Section     a;
	SystemDraw  alpha;
#endif

#ifdef PLATFORM_X11
	SystemDraw   alpha;
#endif

	bool    has_alpha;

	void Init();
	Image Get(bool pm) const;

public:
	Draw& Alpha();                       

	operator Image() const;
	
	Image GetStraight() const;
	
	ImageDraw(Size sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();
};

typedef ImageDraw SystemImageDraw;

void DrawDragRect(SystemDraw& w, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, uint64 pattern);

void SetSurface(SystemDraw& w, int x, int y, int cx, int cy, const RGBA *pixels);

END_UPP_NAMESPACE
