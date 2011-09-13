#ifndef _AutoScroller_AutoScroller_hpp_
#define _AutoScroller_AutoScroller_hpp_

#include "AutoScroller.h"

template<class C>
AutoScroller<C>::AutoScroller()
{
	C::AddFrame(scroll);
	scroll.AutoHide();
	scroll.WhenScroll = THISBACK(OnScroll);
}

template<class C>
void AutoScroller<C>::Scroll(const Point& p)
{
	if(!HasPane()) return;
	Rect _r = pane->GetRect();
	Rect r(-p, _r.GetSize());
	pane->SetRect(r);
	WhenScrolled();
}

template<class C>
void AutoScroller<C>::OnScroll()
{
	Scroll(scroll.Get());
}

template<class C>
void AutoScroller<C>::Layout()
{
	Size psz = C::GetSize();
	scroll.SetPage(psz);
	if(!HasPane()) return;
	Size tsz = pane->GetSize();
	scroll.SetTotal(tsz);
}

template<class C>
void AutoScroller<C>::EnableScroll(bool b)
{
	scroll.x.Enable(b);
	scroll.y.Enable(b);
}

#endif
