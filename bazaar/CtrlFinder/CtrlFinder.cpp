#include "CtrlFinder.h"

Ctrl* GetCtrl(Ctrl* parent, Point p, dword keyflags)
{
	Ctrl* c = parent->GetFirstChild();
	while(c)
	{
		Rect r = c->GetRect();
		if(r.Contains(p)) return c;
		c = c->GetNext();	
	}
	return NULL;
}

void CtrlFinder::OnCtrlLeft(Point p, dword keyflags)
{
	Ctrl* c = GetCtrlFromParent(p, keyflags);
	if(c) WhenLeftDown(*c, p, keyflags);
}
void CtrlFinder::OnCtrlRight(Point p, dword keyflags)
{
	Ctrl* c = GetCtrlFromParent(p, keyflags);
	if(c) WhenRightDown(*c, p, keyflags);
}
