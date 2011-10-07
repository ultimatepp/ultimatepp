#include "XMLToolBarContainer.h"

NAMESPACE_UPP

// frame layout functions
void XMLToolBarFrameBorder::FrameLayout(Rect &r)
{
	switch(state)
	{
		case XMLToolBar::TOOLBAR_LEFT:
		case XMLToolBar::TOOLBAR_RIGHT:
			r.left += 3;
			r.right -= 3;
			break;
			
		case XMLToolBar::TOOLBAR_TOP:
		case XMLToolBar::TOOLBAR_BOTTOM:
			r.top +=3;
			r.bottom -= 3;
			break;
		
		default:
			NEVER();
	}
}

void XMLToolBarFrameBorder::FrameAddSize(Size &s)
{
	switch(state)
	{
		case XMLToolBar::TOOLBAR_LEFT:
		case XMLToolBar::TOOLBAR_RIGHT:
			s.cx += 6;
			break;
			
		case XMLToolBar::TOOLBAR_TOP:
		case XMLToolBar::TOOLBAR_BOTTOM:
			s.cy += 6;
			break;
		
		default:
			NEVER();
	}
}

// frame painting
void XMLToolBarFrameBorder::FramePaint(Draw& w, const Rect& r)
{
	int x1 = r.left;
	int y1 = r.top;
	int x2 = r.right;
	int y2 = r.bottom;
	switch(state)
	{
		case XMLToolBar::TOOLBAR_LEFT:
		case XMLToolBar::TOOLBAR_RIGHT:
			x1 += 1;
			x2 -= 1;
			w.DrawLine(x1, y1, x1, y2, 0, SColorShadow());
			w.DrawLine(x2, y1, x2, y2, 0, SColorShadow());
			break;
			
		case XMLToolBar::TOOLBAR_TOP:
		case XMLToolBar::TOOLBAR_BOTTOM:
			y1 += 1;
			y2 -= 1;
			w.DrawLine(x1, y1, x2, y1, 0, SColorShadow());
			w.DrawLine(x1, y2, x2, y2, 0, SColorShadow());
			break;
		
		default:
			NEVER();
	}
}

XMLToolBarFrameBorder::XMLToolBarFrameBorder()
{
	state = XMLToolBar::TOOLBAR_TOP;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void XMLToolBarContainer::SetState(XMLToolBar::XMLToolBarState _state)
{
	border.SetState(_state);
}

void XMLToolBarContainer::FrameAddSize(Size &s)
{
	return border.FrameAddSize(s);
}

void XMLToolBarContainer::FrameLayout(Rect &r)
{
	return border.FrameLayout(r);
}

XMLToolBarContainer::XMLToolBarContainer()
{
	AddFrame(border);
	SetState(XMLToolBar::TOOLBAR_TOP);
}

END_UPP_NAMESPACE
