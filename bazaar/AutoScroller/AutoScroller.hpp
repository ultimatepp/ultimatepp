#ifndef _AutoScroller_AutoScroller_hpp_
#define _AutoScroller_AutoScroller_hpp_

#include "AutoScroller.h"

template<class C>
AutoScroller<C>::AutoScroller()
{
	EnableScroll();
	scroll.WhenScroll = THISBACK(OnScroll);
	Add(pane.TopPos(0).LeftPos(0));
	SetPaneSize(GetSize());
}

template<class C>
void AutoScroller<C>::OnScroll()
{
	Rect r(-scroll.Get(), pane.GetRect().GetSize());
	pane.SetRect(r);
}

template<class C>
void AutoScroller<C>::Layout()
{
	Size sz = GetSize();
	if(sz != scroll.GetPage())
		scroll.SetPage(sz);
}

template<class C>
void AutoScroller<C>::SetPaneSize(const Size & sz)
{
	Ctrl::LogPos logp;
	logp.x = ParentCtrl::PosLeft(0, sz.cx);
	logp.y = ParentCtrl::PosTop(0, sz.cy);
	pane.SetPos(logp);
	scroll.SetTotal(sz);
}

template<class C>
void AutoScroller<C>::EnableScroll(bool b)
{
	if(b) AddFrame(scroll.AutoHide());
	else RemoveFrame(scroll);
	scroll.x.Enable(b);
	scroll.y.Enable(b);
	return;
	scroll.Show(b);
	Layout();
}

#endif
