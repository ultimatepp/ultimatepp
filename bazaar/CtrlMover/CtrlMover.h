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
	void OnCtrlLeftMulti(const Vector<Ctrl*>* pctrls, Rect r, dword keyflags);

	void OnMissed(Point p, dword keyflags);

protected:
	void OnRectChange();

	RectCtrl rc;
	RectCtrl::Style rcst;
};

#endif

