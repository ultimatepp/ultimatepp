#ifndef _CtrlFinder_CtrlFinder_h
#define _CtrlFinder_CtrlFinder_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class MouseHookCtrl : public ParentCtrl
{
public:
	typedef MouseHookCtrl CLASSNAME;
	MouseHookCtrl() : minsize(Null) { Update(); }

	void    SetMinSize(Size sz) { minsize = sz; }
	virtual Rect GetVoidRect()const { return GetSize(); }
	virtual Size GetStdSize() const { return GetMinSize(); }
	virtual Size GetMinSize() const { return IsNull(minsize) ? Ctrl::GetMinSize() : minsize; }

	virtual void LeftDown(Point p, dword keyflags) { WhenLeftDown(p,keyflags); }
	virtual void RightDown(Point p, dword keyflags) { WhenRightDown(p,keyflags); }
	virtual void LeftUp(Point p, dword keyflags) { WhenLeftUp(p,keyflags); }
	virtual void RightUp(Point p, dword keyflags) { WhenRightUp(p,keyflags); }
	virtual void MouseMove(Point p, dword keyflags) { WhenMouseMove(p,keyflags); }
	virtual void LeftRepeat(Point p, dword keyflags) { WhenLeftRepeat(p,keyflags); }
	virtual void RightRepeat(Point p, dword keyflags) { WhenRightRepeat(p,keyflags); }

	virtual void Updated() { IgnoreMouse(!IsEnabled()); }

	Callback2<Point, dword> WhenLeftDown;
	Callback2<Point, dword> WhenRightDown;
	Callback2<Point, dword> WhenLeftUp;
	Callback2<Point, dword> WhenRightUp;
	Callback2<Point, dword> WhenMouseMove;
	Callback2<Point, dword> WhenLeftRepeat;
	Callback2<Point, dword> WhenRightRepeat;

protected:
	Size minsize;
};

Ctrl* GetCtrl(Ctrl* parent, Point p, dword keyflags);

class CtrlFinder : public MouseHookCtrl
{
public:
	typedef CtrlFinder CLASSNAME;
	typedef MouseHookCtrl R;
	CtrlFinder() { R::WhenLeftDown = THISBACK(OnCtrlLeft); R::WhenRightDown = THISBACK(OnCtrlRight);}

	Ctrl* GetCtrl(Point p, dword keyflags) { return ::GetCtrl(this, p, keyflags); }
	Ctrl* GetCtrlFromParent(Point p, dword keyflags) { return ::GetCtrl(GetParent(), p, keyflags); }
	
	Callback3<Ctrl&, Point, dword> WhenLeftDown;
	Callback3<Ctrl&, Point, dword> WhenRightDown;

protected:
	void OnCtrlLeft(Point p, dword keyflags);
	void OnCtrlRight(Point p, dword keyflags);
};

#endif

