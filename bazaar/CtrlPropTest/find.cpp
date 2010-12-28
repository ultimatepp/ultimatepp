#include "CtrlPropTest.h"

void CtrlPropTest::OnCtrlLeft(Ctrl& c, Point p, dword keyflags)
{
	if(&c == &rc) return;
	if(&c == &hk) return;
	if(!CanEdit()) return;
	rc.SetData(c.GetRect());
	Add(rc.SizePos());
	ctrl = &c;
}

void CtrlPropTest::OnCtrlRight(Ctrl& c, Point p, dword keyflags)
{
	if(&c == &rc) return;
	if(&c == &hk) return;
	if(!CanEdit()) return;
	if(keyflags == K_SHIFT)
		DoList(c);
	else
		DoEdit(c);
}

void CtrlPropTest::OnRectChange()
{
	if(!ctrl) return;
	ctrl->SetRect(rc.GetData());
}

void CtrlPropTest::OnMissed(Point p, dword keyflags)
{
	rc.Remove(); //leave move
	hk.LeftDown(p, keyflags);
}
