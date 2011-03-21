#ifndef _CtrlPos_CtrlPos_h_
#define _CtrlPos_CtrlPos_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <RectCtrl/RectCtrl.h>
#include <LogPosCtrl/LogPosCtrl.h>
#include <CtrlFinder/CtrlFinder.h>
#include <Gen/Gen.h>

//for some helpers
#include <CtrlMover/CtrlMover.h>

class CtrlPos : public CtrlFinder
{
public:
	typedef CtrlPos CLASSNAME;
	typedef CtrlFinder V;
	CtrlPos();

	virtual void Clear();

	virtual void Paint(Draw& w);	
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void State(int reason);	
	virtual void Updated();
	virtual Rect GetVoidRect() const { return Ctrl::GetVoidRect(); }

	void SetStyle(const RectCtrl::Style& s) { style = &s; Refresh(); }

	static void DrawAlignHandle(Draw& w, const Rect& _r, const Rect& r, const Ctrl::LogPos& pos, const Color& col);
	static bool GetAlignMode(const Rect& _r, const Rect& r, const Point& pp, Ctrl::LogPos& pos, int handsize);

protected:	
	const RectCtrl::Style* style;
	Point g;
	Point xp;
	Ctrl::LogPos xpos;
	int mode;
	bool pressed;
	bool moving;
	Image ci;
};

#endif
