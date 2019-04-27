#include "LayDes.h"

#include <limits>

PropertyPane::PropertyPane()
	: line_height(std::numeric_limits<int>::max())
{
	Clear();
	SetFrame(sb);
	sb.AutoHide();
	sb.WhenScroll = THISBACK(Scroll);
	Color(SColorFace);
}

void PropertyPane::Layout()
{
	sb.SetTotal(y);
	sb.SetPage(GetSize().cy);
	if(line_height < std::numeric_limits<int>::max())
		sb.SetLine(line_height);
}

void PropertyPane::Clear()
{
	y = 0;
	sb.Set(0, 0, 0);
	while(pane.GetFirstChild())
		pane.GetFirstChild()->Remove();
}

void PropertyPane::Add(ItemProperty& c)
{
	pane.Add(c);
	int cy = c.GetHeight();
	if(cy < line_height)
		line_height = cy;
	c.HSizePos().TopPos(y, cy);
	y += cy;
	pane.TopPos(0, y);
	Layout();
}

void PropertyPane::Scroll()
{
	pane.TopPos(-sb, y);
}

void PropertyPane::ChildGotFocus()
{
	Ctrl *f = GetFocusCtrl();
	for(Ctrl *q = pane.GetFirstChild(); q; q = q->GetNext())
		if(q->HasChildDeep(f)) {
			Rect r = q->GetRect();
			sb.ScrollInto(r.top, r.Height());
			break;
		}
}

void PropertyPane::MouseWheel(Point, int zdelta, dword)
{
	sb.Wheel(zdelta);
}

void PropertyPane::AfterCreate()
{
	Ctrl::Add(pane.HSizePos());
}
