#include "FormView.hpp"
#include "ScrollContainer.hpp"

void FormView::SetFormSize(const Size& sz)
{
	FormLayout* l = GetCurrentLayout();
	if (!l) return;
	l->SetNumber("Form.Width", sz.cx);
	l->SetNumber("Form.Height", sz.cy);
	Layout();
	if (_container)
		_container->Layout();
}

void FormView::RectToGrid(Rect& r)
{
	Size g = GetGridSize();
	
	if (r.left -  r.left / g.cx * g.cx < g.cx / 2)
		r.left -= r.left % g.cx;
	else
		r.left += g.cx - r.left % g.cx;
			
	if (r.right -  r.right / g.cx * g.cx < g.cx / 2)
		r.right -= r.right % g.cx;
	else
		r.right += g.cx - r.right % g.cx;
					
	if (r.top -  r.top / g.cy * g.cy < g.cy / 2)
		r.top -= r.top % g.cy;
	else
		r.top += g.cy - r.top % g.cy;
					
	if (r.bottom -  r.bottom / g.cy * g.cy < g.cy / 2)
		r.bottom -= r.bottom % g.cy;
	else
		r.bottom += g.cy - r.bottom % g.cy;
}

void FormView::PointToGrid(Point& r)
{
	Size g = GetGridSize();

	if (r.x -  r.x / g.cx * g.cx < g.cx / 2) r.x -= r.x % g.cx;
		else r.x += g.cx - r.x % g.cx;
	if (r.y -  r.y / g.cy * g.cy < g.cy / 2) r.y -= r.y % g.cy;
		else r.y += g.cy - r.y % g.cy;
}

int FormView::XToGrid(int x, bool use)
{
	Size g = GetGridSize();

	if (!use) return x;
	return (x - x / g.cx * g.cx < g.cx / 2)
		? x - x % g.cx
		: x + g.cx - x % g.cx;
}

int FormView::YToGrid(int y, bool use)
{
	Size g = GetGridSize();

	if (!use) return y;
	return (y - y / g.cy * g.cy < g.cy / 2)
		? y - y % g.cy
		: y + g.cy - y % g.cy;
}

bool FormView::IsObjectButton(Point p)
{
	if (!IsLayout())
		return false;

	_frameResize = false;

	Vector<int> sel = GetSelected();
	if (sel.GetCount() == 1)
	{
		FormObject *pI = GetObject(sel[0]);
		Rect r = Offseted(pI->GetRect());

		if (Zoom(Rect( Point(r.right - 6, r.bottom - 6), Size(24, 24) ))
			.Contains(p))
				OverrideCursor(Image::SizeBottomRight);
		else
		{
			OverrideCursor(_cursor);
			return false;
		}

		return true;
	}

	if (sel.GetCount() != 0)
		return false;

	if (Zoom(Rect( Point(GetPageRect().right - 6, GetPageRect().bottom - 6), Size(24, 24) ))
		.Contains(p))
	{
		OverrideCursor(Image::SizeBottomRight);
		_frameResize = true;
		return true;
	}
	else
	{
		OverrideCursor(_cursor);
		return false;
	}

	OverrideCursor(_cursor);
	return false;
}

dword FormView::IsGroupButton(Point p)
{
	if (!IsLayout())
		return TOOL_NONE;

	_leftCur   = 0;
	_topCur    = 0;
	_rightCur  = 0;
	_bottomCur = 0;

	Rect r = _groupRect;
	Point p1;
	Rect t;
	int v;

	p1 = r.TopLeft();
	v = (r.BottomLeft().y >= GetRect().Height())
		? GetRect().Height() - (GetRect().Height() - r.TopLeft().y) / 2 - 16
		: ((r.TopLeft().y < 0)
			? r.BottomLeft().y / 2 - 16
			: r.CenterLeft().y - 16);
	if (p1.x >= 20)  // left tool
	{
		if ( Rect(Point(r.CenterLeft().x - 16, v), Size(24, 24)).Contains(p) )
		{
			_leftCur = 1; Refresh();
			return TOOL_LEFT;
		}
		else
		{
			_leftCur = 0; Refresh();
		}
	}
	else
	{
		if ( Rect(Point(0, v), Size(24, 24) ).Contains(p) )
		{
			_leftCur = 1; Refresh();
			return TOOL_LEFT;
		}
		else
		{
			_leftCur = 0; Refresh();
		}
	}

	v = (r.TopLeft().x < 0)
		?  r.TopRight().x / 2 - 11
		: (r.TopRight().x > GetRect().Width()
			? GetRect().Width() - (GetRect().Width() - r.TopLeft().x) / 2 - 11
			: r.TopCenter().x - 11);
	if (p1.y >= 20)
	{
		if ( Rect( Point(v, r.TopCenter().y - 16), Size(24, 24) ).Contains(p) )
		{
			_topCur = 1; Refresh();
			return TOOL_TOP;
		}
		else
		{
			_topCur = 0; Refresh();
		}
	}
	else
	{
		if ( Rect( Point(v, 2), Size(24, 24) ).Contains(p) )
		{
			_topCur = 1; Refresh();
			return TOOL_TOP;
		}
		else
		{
			_topCur = 0; Refresh();
		}
	}

	p1 = r.BottomRight();
	v = (p1.y >= GetRect().Height())
		? GetRect().Height() - (GetRect().Height() - r.TopLeft().y) / 2 - 16
		: ((r.TopLeft().y < 0)
			? r.BottomLeft().y / 2 - 16
			: r.CenterLeft().y - 16);
	if (p1.x <= GetRect().Width() - 20)
	{
		if ( Rect( Point(r.CenterRight().x - 6, v), Size(24, 24) ).Contains(p) )
		{
			_rightCur = 1; Refresh();
			return TOOL_RIGHT;
		}
		else
		{
			_rightCur = 0; Refresh();
		}
	}
	else
	{
		if ( Rect( Point(GetRect().Width() - 16, v), Size(24, 24) ).Contains(p) )
		{
			_rightCur = 1; Refresh();
			return TOOL_RIGHT;
		}
		else
		{
			_rightCur = 0; Refresh();
		}
	}


	v = (r.TopLeft().x < 0)
		?  r.TopRight().x / 2 - 11
		: (r.TopRight().x > GetRect().Width()
			? GetRect().Width() - (GetRect().Width() - r.TopLeft().x) / 2 - 11
			: r.TopCenter().x - 11);
	if (p1.y <= GetRect().Height() - 20)
	{
		if ( Rect( Point(v, r.BottomCenter().y - 8), Size(24, 24) ).Contains(p) )
		{
			_bottomCur = 1; Refresh();
			return TOOL_BOTTOM;
		}
		else
		{
			_bottomCur = 0; Refresh();
	}
	}
	else
	{
		if ( Rect( Point(v, GetRect().Height() - 16), Size(24, 24) ).Contains(p) )
		{
			_bottomCur = 1; Refresh();
			return TOOL_BOTTOM;
		}
		else
		{
			_bottomCur = 0; Refresh();
		}
	}

	return TOOL_NONE;
}
