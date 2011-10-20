#include "XMLDragFrame.h"

NAMESPACE_UPP

// frame layout functions
void XMLDragFrame::FrameLayout(Rect &r)
{
	if(align == ToolBar::BAR_LEFT || align == ToolBar::BAR_RIGHT)
	{
		r.left += 3;
		r.right -=3;
		r.top += 10;
		r.bottom -= 3;
	}
	else
	{
		r.left += 10;
		r.right -=3;
		r.top += 3;
		r.bottom -=3;
	}
}

void XMLDragFrame::FrameAddSize(Size &s)
{
	if(align == ToolBar::BAR_LEFT || align == ToolBar::BAR_RIGHT)
	{
		s.cx += 6;
		s.cy += 13;
	}
	else
	{
		s.cx += 13;
		s.cy += 6;
	}
}

// frame painting
void XMLDragFrame::FramePaint(Draw& w, const Rect& r)
{
	w.DrawRect(r, SColorFace());
	int x1 = r.left + 1;
	int x2 = r.right - 1;
	int y1 = r.top + 1;
	int y2 = r.bottom -1;
	w.DrawLine(x1, y1, x2, y1, 1, SColorLight());
	w.DrawLine(x2, y1, x2, y2, 1, SColorShadow());
	w.DrawLine(x2, y2, x1, y2, 1, SColorShadow());
	w.DrawLine(x1, y2, x1, y1, 1, SColorLight());
	if(align == ToolBar::BAR_LEFT || align == ToolBar::BAR_RIGHT)
	{
		x1 = r.left + 5;
		x2 = r.right - 5;
		y1 = r.top + 4;
		y2 = r.top + 9;
		w.DrawLine(x1, y1, x2, y1, 2, SColorShadow());
		w.DrawLine(x1, y2, x2, y2, 2, SColorShadow());
	}
	else
	{
		x1 = r.left + 4;
		x2 = r.left + 9;
		y1 = r.top + 5;
		y2 = r.bottom - 5;
		w.DrawLine(x1, y1, x1, y2, 2, SColorShadow());
		w.DrawLine(x2, y1, x2, y2, 2, SColorShadow());
	}
}

// frame insertion/removing handlers
void XMLDragFrame::FrameAdd(Ctrl &_parent)
{
	parent = &_parent;
}

void XMLDragFrame::FrameRemove(void)
{
	parent = NULL;
}

XMLDragFrame::XMLDragFrame()
{
	align = ToolBar::BAR_TOP;
}

XMLDragFrame::~XMLDragFrame()
{
}

XMLDragFrame &XMLDragFrame::Align(int _align)
{
	align = _align;
	return *this;
}

END_UPP_NAMESPACE
