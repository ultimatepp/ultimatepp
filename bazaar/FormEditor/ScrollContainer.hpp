#ifndef SCROLL_CONTAINER_HPP
#define SCROLL_CONTAINER_HPP

#include "FormView.hpp"

class LayoutCtrl : public ParentCtrl
{
public:
	virtual void SetMinSize(Size sz) { _min = sz; }
	virtual Size GetMinSize() const  { return IsNull(_min) ? Ctrl::GetMinSize() : _min; }

	virtual void MouseWheel(Point p, int zdelta, dword keyflags);

	LayoutCtrl() { _min = Null; Offset = 10; pScrollBars = NULL; }

	int Offset;
	ScrollBars* pScrollBars;

private:
	Size _min;
};

inline void LayoutCtrl::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if (!pScrollBars) return;
	pScrollBars->SetLine(40, 40);
	zdelta < 0 ? pScrollBars->LineDown() : pScrollBars->LineUp();
}

class ScrollContainer : public ParentCtrl
{
public:
	typedef	ScrollContainer CLASSNAME;

	ScrollContainer()
	{
		_transparent = false;
		Offset = 20;
		MouseReceiver = NULL;
		Clear();
		scrollbars.WhenScroll = THISBACK(Scroll);
		AddFrame(scrollbars.AutoHide(true));
		SetFrame(ThinInsetFrame());
	}
	
	~ScrollContainer() { Clear(); }

	virtual ScrollContainer& Set(FormView &p, Size sz = Null);
	virtual FormView* Get() const { return pane; }
	virtual void  Clear(bool remove = true);

	void Paint(Draw& w)
	{
		if (!_transparent)
			w.DrawRect(GetRect(), _color);
	}

	inline void AddToScroll(LayoutCtrl& ctrl)
	{
		ctrl.pScrollBars = &scrollbars;
	}

	virtual void LeftDown(Point p, dword keyflags)
	{
		if (MouseReceiver)
			MouseReceiver->LeftDown(Point(-1, -1), keyflags);
	}

	virtual void RightDown(Point p, dword keyflags)
	{
		if (MouseReceiver)
			MouseReceiver->RightDown(Point(-1, -1), keyflags);
	}

	virtual void LeftDouble(Point p, dword keyflags)  { LeftDown(p, keyflags); }
	virtual void RightDouble(Point p, dword keyflags) { RightDown(p, keyflags); }

	virtual void Layout();

	Ctrl* MouseReceiver;
	Callback1<Point> WhenScroll;

	int Offset;
	void Transparent(bool flag = false, Color c = White())
	{
		_color = c;
		_transparent = flag;
		flag ? TransparentBackPaint() : NoTransparent();
	}

protected:
	ScrollBars scrollbars;
	Point      psb;
	Ptr<FormView>  pane;
	bool _transparent;
	Color _color;

	virtual void ChildRemoved(Ctrl *child) { if (child == pane) Clear(false); }
	virtual void Scroll();
};

inline void ScrollContainer::Layout()
{
	if (pane)
	{
		scrollbars.SetPage(pane->DeZoom(scrollbars.GetReducedViewSize()));
		scrollbars.SetTotal(pane->GetFormSize() + Size(45, 45));
	}
}

inline void ScrollContainer::Scroll()
{
	Point newpos = scrollbars;
	Point p;
	if (pane)
	{
		p = psb-newpos;
		pane->SetPageRect(pane->GetPageRect().Offseted(p));
		pane->Refresh();
		pane->WhenChildAllPos.Execute();
	}
	psb = newpos;
	WhenScroll.Execute(p);
}

inline ScrollContainer& ScrollContainer::Set(FormView &p, Size sz)
{
	Clear(true);
	if (!p.IsLayout())
		return *this;
	if (IsNull(sz))
		sz = p.GetMinSize();
	p.SetPageRect(Rect(Point(Offset, Offset), sz));
	p.SizePos();
	pane = &p;
	Add(p);
	Layout();
	Scroll();
	return *this;
}

inline void ScrollContainer::Clear(bool remove)
{
	if (remove && pane && pane->GetParent() == this)
		((Ctrl*)pane)->Remove();
	pane = NULL;
	psb = Point(0, 0);
	scrollbars.SetX(0, 0, 0);
	scrollbars.SetY(0, 0, 0);
}

#endif // .. SCROLL_CONTAINER_HPP
