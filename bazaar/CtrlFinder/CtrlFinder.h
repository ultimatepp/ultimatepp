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

	typedef Callback3<Ctrl*&, Point&, int&> CtrlFilterType;

	enum
	{
		VISIBLE = 0x1,
		INVISIBLE = 0x2,
		
		ENABLED = 0x4,
		DISABLED = 0x8,

		VIEW = 0x10,
		FRAME = 0x20,
		
		DEEP = 0x80,
		
		DEF = VISIBLE | INVISIBLE | ENABLED | DISABLED | VIEW | FRAME | DEEP,
	};

	static void StdCtrlFilter(Ctrl*& q, Point& pt, int& f);	
	static Ctrl* ChildAtPoint(Ctrl& par, Point& pt, int& f, const CtrlFilterType& fil);
	static Ctrl* GetCtrl(Ctrl& c, Point& p, int& f, const CtrlFilterType& fil);

	CtrlFinder() : flags(DEF), filter(STDBACK(StdCtrlFilter)) {}

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
	void SetCtrl(Ctrl& c) { /*ASSERT(c.GetParent());*/ ctrl = &c; }
	
	CtrlFilterType filter; //set to NULL if should not be handled, change flags if desired
	int flags;

protected:
	Ptr<Ctrl> ctrl;
};

#endif

