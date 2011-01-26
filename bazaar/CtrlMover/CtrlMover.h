#ifndef _CtrlMover_CtrlMover_h
#define _CtrlMover_CtrlMover_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <CtrlFinder/CtrlFinder.h>
#include <RectCtrl/RectCtrl.h>
#include <Gen/Gen.h>

//the control mover is a layer on top of other ctrl surfaces, and can grab their Ctrls to move them.

class CtrlMover : public CtrlFinder, public Visiting<Ctrl>
{
public:
	typedef CtrlMover CLASSNAME;
	typedef Visiting<Ctrl> V;
	CtrlMover();

	virtual void Visit(Ctrl& c);
	virtual void Clear();
	virtual void Reload();

	virtual void State(int reason);	
	virtual void Updated();

	void OnCtrlLeft(Ctrl& c, Point p, dword keyflags);
	void OnMissed(Point p, dword keyflags);
	
protected:
	void OnRectChange();

	RectCtrl rc;
	RectCtrl::Style rcst;
};

#endif

