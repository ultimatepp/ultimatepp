#include <CtrlLib/CtrlLib.h>

class ScrollArea : public StaticRect {
	virtual void Layout();
	virtual void ChildGotFocus();

private:
	StaticRect std_pane;
	Ctrl      *pane;
	ScrollBars sb;

	void Init();
	void SetSb();
	void Scroll();

public:
	void Reset();
	void SetPaneSize(Size sz);
	void SetPane(Ctrl& pane);

	void Add(Ctrl& ctrl);

	Ctrl& GetArea()            { return *pane; }
	Size  GetPaneSize()        { return pane->GetSize(); }

	typedef ScrollArea CLASSNAME;

	ScrollArea();
};

void ScrollArea::SetPaneSize(Size sz)
{
	pane->LeftPos(0, sz.cx).TopPos(0, sz.cy);
}

Size ScrollArea::GetPaneSize()
{
	return pane->GetRect().GetSize();
}

void ScrollArea::Scroll()
{
	Size sz = GetPaneSize();
	pane.LeftPos(-sb.x, sz.cx).TopPos(-sb.x, sz.cx);
}

void ScrollArea::SetPane(Ctrl& _pane)
{
	pane = &_pane;
	sb.x = 0;
	sb.y = 0;
	Layout();
}

void ScrollArea::Layout()
{
	sb.SetTotal(GetPaneSize());
	sb.SetPage(GetSize());
}

void ScrollArea::Reset()
{
	area = &std_pane;
	SetPaneSize(GetSize());
	sb.Set(0, 0, 0);
	while(pane.GetFirstChild())
		pane.GetFirstChild()->Remove();
}

void ScrollArea::Add(Ctrl& c)
{
	pane.Add(c);
}

void ScrollArea::ChildGotFocus()
{
	Ctrl *f = GetFocusCtrl();
	for(Ctrl *q = pane.GetFirstChild(); q; q = q->GetNext())
		if(q->HasChildDeep(f)) {
			Rect r = q->GetRect();
			sb.ScrollInto(r.top, r.Height());
			break;
		}
}

void ScrollArea::AfterCreate()
{
	Ctrl::Add(pane.HSizePos());
}

ScrollArea::ScrollArea()
{
	Clear();
	SetFrame(sb);
	sb.AutoHide();
	sb.WhenScroll = THISBACK(Scroll);
	Color(SLtGray);
}
