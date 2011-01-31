#ifndef _CtrlMover_CtrlMover_h
#define _CtrlMover_CtrlMover_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <CtrlFinder/CtrlFinder.h>
#include <RectCtrl/RectCtrl.h>

class CtrlMover : public CtrlFinder
{
public:
	typedef CtrlMover CLASSNAME;
	typedef CtrlFinder V;
	CtrlMover();

	virtual void Visit(Ctrl& c);
	virtual void Clear();
	virtual void Reload();

	virtual void State(int reason);	
	virtual void Updated();

	void OnCtrlLeft(Ctrl& c, Point p, dword keyflags);
	void OnMissed(Point p, dword keyflags);

	static void CalcOffset(Ctrl& c, Ctrl& par, Point& p);
	static Point GetOffset(Ctrl& c, Ctrl& par);
	
protected:
	void OnRectChange();

	RectCtrl rc;
	RectCtrl::Style rcst;
};

#endif

