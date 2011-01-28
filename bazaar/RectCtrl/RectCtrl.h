#ifndef _RectCtrl_RectCtrl_h
#define _RectCtrl_RectCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS Img
#define IMAGEFILE <RectCtrl/icons.iml>
#include <Draw/iml_header.h>

class RectCtrl : public Ctrl
{
public:
	typedef RectCtrl CLASSNAME;
	RectCtrl();	
	
	struct Style : public ChStyle<Style> {
		Color rectcol;
		Color handcol;
		int handsize;
		Color framecol;
		int framesize;
		Color textcol;
	};

	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);

	virtual void Updated();

	virtual void SetData(const Value& v) { r = v; UpdateRefresh(); }
	virtual Value GetData() const { return r; }

	static const Style& StyleDefault();
	void SetStyle(const Style& s) { style = &s; Refresh(); }

	Callback2<Point, dword> WhenMissed;

	//helpers
	inline static Rect HandleAt(const Point& p, int size);
	static void DrawHandle(Draw& w, const Rect& r, const Color& col, int size);
	static void DrawHandleFrame(Draw& w, const Rect& r, const Color& col, int size);
	static void DrawRectInfo(Draw& w, const Point& p, const Rect&r, const Color& framecol, const Color& textcol);
	static int GetMode(const Rect& r, const Point& p, dword keyflags, int handsize);
	static void CalcRect(Rect& r, const Point& dp, dword keyflags, int mode, const Point& g);
	static Image SetCursor(unsigned m, dword keyflags, const Image& old);

protected:
	const Style* style;
	Rect r;
	Point g; //grid

public:
	enum 
	{
		NONE = 0x0,

		LEFT = 0x1,
		RIGHT = 0x2,
		TOP = 0x4,
		BOTTOM = 0x8,

		_LEFT = 0x10,
		_RIGHT = 0x20,
		_TOP = 0x40,
		_BOTTOM = 0x80,

		LEFTTOP = LEFT | TOP,
		LEFTBOTTOM = LEFT | BOTTOM,
		RIGHTTOP = RIGHT | TOP,
		RIGHTBOTTOM = RIGHT | BOTTOM,

		MOVE = LEFTTOP | RIGHTBOTTOM,
		ALL = MOVE,

		_LEFTTOP = _LEFT | _TOP,
		_LEFTBOTTOM = _LEFT | _BOTTOM,
		_RIGHTTOP = _RIGHT | _TOP,
		_RIGHTBOTTOM = _RIGHT | _BOTTOM,

		STRETCH = _LEFTTOP | RIGHTBOTTOM,
		CENTER = STRETCH,
		_STRETCH = LEFTTOP | _RIGHTBOTTOM,
		_CENTER = _STRETCH,

		_MOVE = _LEFTTOP | _RIGHTBOTTOM,
		_ALL = _MOVE,

		GRIDX = 0x100,
		GRIDY = 0x200,
		GRID = GRIDX | GRIDY,
	};

protected:
	//cache
	Point xp;
	Rect xr;
	int mode;
	bool pressed;
	bool moving;
	Image c; //cursoroverride
};

#endif

