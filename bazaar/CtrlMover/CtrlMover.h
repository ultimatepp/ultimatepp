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

	CtrlMover();

	virtual void State(int reason);	
	virtual void Updated();

	void OnCtrlLeft(Ctrl& c, Point p, dword keyflags);
	void OnMissed(Point p, dword keyflags);

	static void CalcOffset(const Ctrl& c, const Ctrl& q, Point& p);
	static Point GetOffset(const Ctrl& c, const Ctrl& q);
	
protected:
	void OnRectChange();

	RectCtrl rc;
	RectCtrl::Style rcst;
};

#endif

