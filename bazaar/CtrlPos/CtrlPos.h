#ifndef _CtrlPos_CtrlPos_h_
#define _CtrlPos_CtrlPos_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <RectCtrl/RectCtrl.h>
#include <LogPosCtrl/LogPosCtrl.h>
#include <CtrlFinder/CtrlFinder.h>

class CtrlPos : public CtrlFinder
{
public:
	typedef CtrlPos CLASSNAME;
	typedef CtrlFinder V;

	enum
	{
		NEST = 0x100,
	};

	CtrlPos();

	virtual void Paint(Draw& w);	
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void LeftDouble(Point p, dword flags);
	virtual bool Key(dword key, int count);
	virtual void Updated();
	virtual Rect GetVoidRect() const { return Ctrl::GetVoidRect(); }

	void SetStyle(const RectCtrl::Style& s) { style = &s; Refresh(); }

	static void DrawAlignHandle(Draw& w, const Rect& _r, const Rect& r, const Ctrl::LogPos& pos, const Color& col);
	static bool GetAlignMode(const Rect& _r, const Rect& r, const Point& pp, Ctrl::LogPos& pos, int handsize);
	static void GetAlignRects(const Ctrl& ctxuser, const Ctrl& finalctx, Rect& r, Rect& _r);
	static void DrawHintFrame(Draw& w, const Ctrl& g, const Ctrl& q, const Color& hintcol, const CtrlFilterType& filter, int flags);

	void CombineAdd(Vector<Ctrl*>& c, pick_ Vector<Ctrl*> _c);
	void CombineSubtract(Vector<Ctrl*>& c, pick_ Vector<Ctrl*> _c);

	Callback WhenLeftDouble;

protected:	
	void DrawSelected(Draw& w, const Vector<Ctrl*>& ctrls);

	const RectCtrl::Style* style;
	Point g;

	//storing info where the drag began (xp), the old logpos values (xpos) and the old parents (xpars)
	//for calculation speed: precalculated offset to pctrl context and the original rect in pctrl context
	Point xp;
	Array<Ctrl::LogPos> xpos;
	Vector<Ctrl*> xpars;
	Vector<Point> xop;
	Vector<Rect> xr;
	
	//Rect rd; //debug
	//Rect rd2; //debug
	
	int mode;
	bool pressed;
	bool moving;
	Image ci;
};

#endif
