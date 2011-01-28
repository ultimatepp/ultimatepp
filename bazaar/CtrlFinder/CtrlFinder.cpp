#include "CtrlFinder.h"

Ctrl* GetCtrl(Ctrl& c, Point p, dword keyflags, bool ignoreframe)
{
	Ctrl* q = c.GetFirstChild();
	while(q)
	{
		if(!q->InFrame() || !ignoreframe)
		{
			Rect r = q->GetRect();
			if(r.Contains(p)) return q;
		}
		q = q->GetNext();	
	}
	return NULL;
}

void CtrlFinder::Reload()
{
	if(IsEmpty()) return;
	V::Reload();
	
	Remove();
	Get().Add(SizePos());
}

void CtrlFinder::Visit(Ctrl& c)
{
	V::Visit(c);
	Enable();	
}

void CtrlFinder::Clear()
{
	c = NULL;	
	Remove();
	V::Clear();
}

void CtrlFinder::OnCtrlLeft(Point p, dword keyflags)
{
	c = NULL;
	if(IsEmpty()) return;
	c = ::GetCtrl(Get(), p, keyflags, ignoreframe);
	if(c == this) c = NULL;
	if(c) WhenLeftDown(*c, p, keyflags);
	else WhenMissed(p, keyflags);
	Action();
}
void CtrlFinder::OnCtrlRight(Point p, dword keyflags)
{
	c = NULL;
	if(IsEmpty()) return;
	c = ::GetCtrl(Get(), p, keyflags, ignoreframe);
	if(c == this) c = NULL;
	if(c) WhenRightDown(*c, p, keyflags);
	else WhenMissed(p, keyflags);
	Action();
}
