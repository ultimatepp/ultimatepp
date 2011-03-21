#ifndef _CtrlFinder_CtrlFinder_h
#define _CtrlFinder_CtrlFinder_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Gen/Gen.h>

class MouseHookCtrl : public ParentCtrl
{
public:
	typedef MouseHookCtrl CLASSNAME;

	virtual void LeftDown(Point p, dword keyflags) { WhenLeftDown(p,keyflags); }
	virtual void RightDown(Point p, dword keyflags) { WhenRightDown(p,keyflags); }
	virtual void LeftUp(Point p, dword keyflags) { WhenLeftUp(p,keyflags); }
	virtual void RightUp(Point p, dword keyflags) { WhenRightUp(p,keyflags); }
	virtual void MouseMove(Point p, dword keyflags) { WhenMouseMove(p,keyflags); }
	virtual void LeftRepeat(Point p, dword keyflags) { WhenLeftRepeat(p,keyflags); }
	virtual void RightRepeat(Point p, dword keyflags) { WhenRightRepeat(p,keyflags); }

	Callback2<Point, dword> WhenLeftDown;
	Callback2<Point, dword> WhenRightDown;
	Callback2<Point, dword> WhenLeftUp;
	Callback2<Point, dword> WhenRightUp;
	Callback2<Point, dword> WhenMouseMove;
	Callback2<Point, dword> WhenLeftRepeat;
	Callback2<Point, dword> WhenRightRepeat;
};


class CtrlFinder : public ParentCtrl, public Visiting<Ctrl>
{
public:
	typedef CtrlFinder CLASSNAME;
	typedef MouseHookCtrl R;
	typedef Visiting<Ctrl> V;

	enum
	{
		VISIBLE = 0x1,
		ENABLED = 0x2,
		VIEW = 0x4,
		FRAME = 0x8,
		
		DEEP = 0x10,
		
		DEF = VISIBLE | ENABLED | VIEW | FRAME | DEEP,
	};
	
	Ctrl* ChildAtPoint(Ctrl& par, Point& pt, int f);
	Ctrl* GetCtrl(Ctrl& c, Point& p, int f = DEF);

	CtrlFinder() : flags(DEF) {}

	virtual void Visit(Ctrl& c);
	virtual void Reload();
	virtual void Clear();

	virtual void LeftDown(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);

	Callback3<Ctrl&, Point, dword> WhenLeftDown;
	Callback3<Ctrl&, Point, dword> WhenRightDown;

	virtual Value GetData() const { return RawToValue(~ctrl); }
	Ctrl* GetCtrl() const { return ctrl; }
	void ClearCtrl() { ctrl = NULL; }
	
	int flags;

protected:
	Ptr<Ctrl> ctrl;
};

#endif

