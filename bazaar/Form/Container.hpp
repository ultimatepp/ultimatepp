#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class Container : public Ctrl
{
private:
	ScrollBars scrollbars;
	Point      psb;
	Ptr<Ctrl>  pane;

	virtual void Paint(Draw& w) { w.DrawRect(GetRect(), SColorFace()); Ctrl::Paint(w); }
	virtual void Layout();
	virtual void ChildRemoved(Ctrl *child) { if (child == pane) Clear(); }

	void Scroll();

public:
	typedef	Container CLASSNAME;

	Container& Set(Ctrl &p, Size sz = Null);
	Ctrl*      Get() const { return pane; }
	void       Clear();
	
	Container()
	{
		Clear();
		scrollbars.WhenScroll = THISBACK(Scroll);
		AddFrame(scrollbars.AutoHide(true));
		scrollbars.HideX();
	}
};

inline void Container::Layout()
{
	if (pane)
	{
		scrollbars.SetPage(scrollbars.GetReducedViewSize());
		scrollbars.SetTotal(Size(pane->GetSize().cx + 5, pane->GetSize().cy + 5));
	}
}

inline void Container::Scroll()
{
	Point newpos = scrollbars;
	pane->SetRect(pane->GetRect().Offseted(psb-newpos));
	psb = newpos;	
}

inline Container & Container::Set(Ctrl &p, Size sz)
{
	if (IsNull(sz))
		sz = p.GetMinSize();
	p.SetRect(0, 0, sz.cx, sz.cy);
	pane = &p;
	Add(p);
	Layout();
	return *this;
}

inline void Container::Clear()
{
	if (pane && pane->GetParent() == this)
		pane->Remove();
	pane = NULL;
	psb = Point(0, 0);
	scrollbars.SetX(0, 0, 0);
	scrollbars.SetY(0, 0, 0);
}

#endif // .. CONTAINER_HPP
