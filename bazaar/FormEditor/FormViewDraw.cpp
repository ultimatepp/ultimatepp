#include "FormView.hpp"

#define IMAGECLASS FormViewImg
#define IMAGEFILE <FormEditor/FormView.iml>
#include <Draw/iml_source.h>

void FormView::Paint(Draw& w)
{
	if (!IsLayout())
	{
		w.DrawRect(GetRect(), White());
		return;
	}

	Rect r = Zoom(GetPageRect());

	DrawGrid(w);
	DrawRect(w, r, 1, LtBlue());

	w.DrawRect(0, 0, r.left, 3000, White());
	w.DrawRect(r.right + 1, 0, 3000, 3000, White());
	w.DrawRect(r.left, 0, 5000, r.top, White());
	w.DrawRect(r.left, r.bottom + 1, 3000, 3000, White());

//	if (_showInfo)
//	{
//		r.SetSize( Zoom(Size(804, 604)) );
//		DrawRect(w, r.Offseted( Zoom(Size(-2, -2)) ), 1, LtMagenta());
//		r = Zoom(GetPageRect());
//	}

	if (GetObjectCount() > 0 && _showInfo == 2)
	{
		Rect b = Zoom(GetObjectsRect()).Offseted(1, 1);
		b.SetSize( b.GetSize() + Size(-2, -2) );
		DrawRect(w, b, 1, Yellow());
	}

	Vector<int> sel = GetSelected();

	bool coloring = GetBool("View.Coloring");
	int ci = 0;
	if (sel.GetCount() > 0)
	{
		if (sel.GetCount() == 1)
		{
			for (int i = 0; i < GetObjectCount(); ++i)
			{
				if (ci++ == _colors.GetCount() - 1) ci = 0;

				if (coloring && i != sel[0])
					DrawObject(w, i, _colors[ci], false);
				else
					DrawObject(w, i, (!coloring && (i == sel[0])) ? _colors[ci] : LtGray(),
						i == sel[0]);
			}
		}
		else
		{
			for (int i = 0; i < GetObjectCount(); ++i)
			{
				if (ci++ == _colors.GetCount() - 1) ci = 0;

				bool found = false;
				for (int j = 0; j < sel.GetCount(); ++j)
					if ( i == sel[j])
					{
						found = true;
						break;
					}
				if (coloring && !found)
					DrawObject(w, i, _colors[ci], false);
				else
					DrawObject(w, i, (!coloring && found) ? _colors[ci] : LtGray(), false);
			}

			Size g = GetGridSize();
			Rect s = GetSelectionRect().Offseted(-g.cx / 2, -g.cy / 2);
			s.SetSize(s.GetSize() + Size(g.cx, g.cy));

			Vector<int> sel = GetSelected();

			bool a1 = true;	// allow horz align
			bool a2 = true;	// allow vert align
			dword f1;		// first horz align
			dword f2;		// first vert align
	
			for (int i = 0; i < sel.GetCount(); ++i)
			{
				FormObject *pI = GetObject(sel[i]);
				if (!pI) continue;

				if (i == 0) { f1 = pI->GetHAlign(); f2 = pI->GetVAlign(); }

				if (f1 != pI->GetHAlign()) { a1 = false; }
				if (f2 != pI->GetVAlign()) { a2 = false; }
			}

			DrawSprings(w, Zoom(s), f1, f2, a1, a2, a1, a2, false);
			DrawRect(w, Zoom(s), 1, LtRed());

			s = GetSelectionRect().Offseted(-g.cx, -g.cy);
			s.SetSize(s.GetSize() + Size(g.cx * 2, g.cy * 2));

			DrawGroupTools(w, Zoom(s));
		}
		return;
	}

	for (int i = 0; i < GetObjectCount(); ++i)
	{
		if (ci++ == _colors.GetCount() - 1) ci = 0;
		DrawObject(w, i, coloring ? _colors[ci] : LtGray());
	}

	if (sel.GetCount() == 0)
		w.DrawImage(r.right, r.bottom, FormViewImg::SizerBR());
}

void FormView::DrawGrid(Draw& w)
{
	if (!IsLayout())
		return;

	if (GetBool("Grid.Visible", true))
	{
		Point p = GetPageRect().TopLeft();
		Size sz = GetPageRect().GetSize();

		Size g = GetGridSize();

		for (int i = p.x; i <= sz.cx + g.cx; i += g.cx)
			for (int j = p.y; j <= sz.cy + g.cy; j += g.cy)
				w.DrawRect(ZoomX(i), ZoomY(j), 1, 1, Gray);
	}
}

void FormView::DrawRect(Draw& w, const Point& p, const Size& sz, int width, Color clr)
{
	if (!IsLayout())
		return;

	w.DrawPolyline( Vector<Point>() << Point(p.x, p.y) << Point(p.x + sz.cx, p.y) 
		<< Point(p.x + sz.cx, p.y + sz.cy) << Point(p.x, p.y + sz.cy)
			<< Point(p.x, p.y), width, clr);
}

void FormView::DrawRect(Draw& w, const Rect& r, int width, Color clr)
{
	if (!IsLayout())
		return;

	DrawRect(w, r.TopLeft(), r.Size(), width, clr);
}

void FormView::DrawSprings(Draw& w, const Rect& r, dword align, dword valign,
	bool drawLeft, bool drawTop, bool drawRight, bool drawBottom, bool centered)
{
	if (!IsLayout())
		return;

	int x = r.CenterPoint().x;
	int y = r.CenterPoint().y;

	if (drawLeft)
	{
		if (!centered)
			y = YToGrid(r.top + r.Height() / 3);

		if (align == Ctrl::LEFT || align == Ctrl::SIZE)
			w.DrawLine(2, y, r.left, y, 1, LtRed());
		else
			for (int i = 5; i <= r.left; i += 5)
				w.DrawLine(i, YToGrid(y - 5, !centered), i, YToGrid(y + 5, !centered), 1,
					LtRed());
	}

	if (drawRight)
	{
		if (!centered)
			y = YToGrid(r.top + r.Height() * 2 / 3);

		if (align == Ctrl::RIGHT || align == Ctrl::SIZE)
			w.DrawLine(r.right, y, GetRect().Width() - 2, y, 1, LtRed());
		else
			for (int i = r.right; i < GetRect().Width() + 5; i += 5)
				w.DrawLine(i, YToGrid(y - 5, !centered), i, YToGrid(y + 5, !centered), 1,
					LtRed());
	}

	if (drawTop)
	{
		if (!centered)
			x = XToGrid(r.left + r.Width() / 3);

		if (valign == Ctrl::TOP || valign == Ctrl::SIZE)
			w.DrawLine(x, 2, x, r.top, 1, LtRed());
		else
			for (int i = 5; i <= r.top; i += 5)
				w.DrawLine(XToGrid(x - 5, !centered), i, XToGrid(x + 5, !centered), i, 1,
					LtRed());
	}

	if (drawBottom)
	{
		if (!centered)
			x = XToGrid(r.left + r.Width() * 2 / 3);

		if (valign == Ctrl::BOTTOM || valign == Ctrl::SIZE)
			w.DrawLine(x, r.bottom, x, GetRect().Height() - 2, 1, LtRed());
		else
			for (int i = r.bottom; i < GetRect().Height() + 5; i += 5)
				w.DrawLine(XToGrid(x - 5, !centered), i, XToGrid(x + 5, !centered), i, 1,
					LtRed());
	}
}

void FormView::DrawObject(Draw& w, int id, const Color& c, bool springs)
{
	if (!IsLayout())
		return;

	FormObject *pI = GetObject(id);
	if (!pI) return;

	Rect offseted = Offseted(pI->GetRect());
	offseted = Zoom(offseted);

	if (_showInfo == 2)	
		w.DrawRect(offseted, c);

	String type = pI->Get("Type");
	if (type == "Label")
		DrawRect(w, offseted.Offseted(-1, 0), 1, LtGray());

	if (pI->IsSelected() && springs)
	{
		DrawSprings(w, offseted, pI->GetHAlign(), pI->GetVAlign());
		w.DrawImage(offseted.right, offseted.bottom, FormViewImg::SizerBR());
		DrawRect(w, offseted, 1, LtRed());
	}
	else if (pI->IsSelected() && !springs)
		DrawRect(w, Point(offseted.left, offseted.top),
			Size(offseted.Width(), offseted.Height()), 1, LtRed());
	else if (_showInfo == 2 || pI->GetBool("OutlineDraw"))
		DrawRect(w, offseted, 1, Black());

	if (pI->GetBool("OutlineDraw"))
	{
		Rect l = offseted.Offseted( ZoomX(3), ZoomY(3) );
		l = Rect( l.TopLeft(), l.GetSize() - Size(ZoomX(6), ZoomY(6)) );
		DrawRect(w, l, 1, LtCyan());
	}

	String temp = pI->Get("Variable");
	Size t = GetTextSize( temp, _Font );
	int cx = t.cx;

	int y = offseted.BottomCenter().y - t.cy - ZoomY(3);

	if (_showInfo > 0)
	{
		if (offseted.Width() > t.cx + 15)
		{
			int x1 = ZoomX(pI->GetRect().right) - DeZoomX(2) - 4 - t.cx + ZoomX(X(0));
			w.DrawRect( x1 - 2, y, t.cx + 3, t.cy, White());
//			DrawRect(w, Point(x1 - 2, y), Size(t.cx + 3, t.cy), 1, LtGray());
			w.DrawText( x1, y, temp, _Font, LtBlue);
		}

		temp = AsString(id) + " " + type;
		t = GetTextSize(temp, _Font);
		Size s = GetTextSize(AsString(id), _Font);

		if (offseted.Width() - cx > t.cx + 10)
		{
			w.DrawRect( offseted.left + ZoomX(5) - 2, y, t.cx + 3, t.cy, White());
//			DrawRect(w, Point(offseted.left + ZoomX(5) - 2, y), Size(t.cx + 3, t.cy),
//				1, LtGray());
			w.DrawText( offseted.left + ZoomX(5), y, temp, _Font, Gray());
		}
		else if (offseted.Width() > s.cx + 5)
		{
			w.DrawRect( offseted.left + ZoomX(5) - 2, y, s.cx + 3, s.cy, White());
//			DrawRect(w, Point(offseted.left + ZoomX(5) - 2, y), Size(s.cx + 3, s.cy),
//				1, LtGray());
			w.DrawText( offseted.left + ZoomX(5), y, AsString(id), _Font, Gray());
		}
	}
}

void FormView::DrawGroupTools(Draw& w, const Rect& r)
{
	if (!IsLayout())
		return;

	_groupRect = r;
	Point p;
	Rect t;
	int v;

	p = r.TopLeft();
	v = (r.BottomLeft().y >= GetRect().Height())
		? GetRect().Height() - (GetRect().Height() - r.TopLeft().y) / 2 - 10
		: ((r.TopLeft().y < 0)
			? r.BottomLeft().y / 2 - 10
			: r.CenterLeft().y - 10);
	if (p.x >= 20)  // left tool
	{
		t = Rect(Point(r.CenterLeft().x - 10, v), Size(11, 11) );
		w.DrawImage(t, _toolLeft[_leftCur]);
	}
	else
	{
		t = Rect(Point(2, v), Size(11, 11) );
		w.DrawImage(t, _toolLeft[_leftCur]);
	}

	v = (r.TopLeft().x < 0)
		?  r.TopRight().x / 2 - 5
		: (r.TopRight().x > GetRect().Width()
			? GetRect().Width() - (GetRect().Width() - r.TopLeft().x) / 2 - 5
			: r.TopCenter().x - 5);
	if (p.y >= 20)
	{
		t = Rect( Point(v, r.TopCenter().y - 10), Size(11, 11) ); // top tool
		w.DrawImage(t, _toolTop[_topCur]);
	}
	else
	{
		t = Rect( Point(v, 2), Size(11, 11) ); // top tool
		w.DrawImage(t, _toolTop[_topCur]);
	}

	p = r.BottomRight();
	v = (p.y >= GetRect().Height())
		? GetRect().Height() - (GetRect().Height() - r.TopLeft().y) / 2 - 10
		: ((r.TopLeft().y < 0)
			? r.BottomLeft().y / 2 - 110
			: r.CenterLeft().y - 10);
	if (p.x <= GetRect().Width() - 20)
	{
		t = Rect( Point(r.CenterRight().x, v), Size(11, 11) ); // right tool
		w.DrawImage(t, _toolRight[_rightCur]);
	}
	else
	{
		t = Rect( Point(GetRect().Width() - 11, v), Size(11, 11) ); // right tool
		w.DrawImage(t, _toolRight[_rightCur]);
	}

	v = (r.TopLeft().x < 0)
		?  r.TopRight().x / 2 - 5
		: (r.TopRight().x > GetRect().Width()
			? GetRect().Width() - (GetRect().Width() - r.TopLeft().x) / 2 - 5
			: r.TopCenter().x - 5);
	if (p.y <= GetRect().Height() - 20)
	{
		t = Rect( Point(v, r.BottomCenter().y), Size(11, 11) ); // bottom tool
		w.DrawImage(t, _toolBottom[_bottomCur]);
	}
	else
	{
		t = Rect( Point(v, GetRect().Height() - 11), Size(11, 11) ); // bottom tool
		w.DrawImage(t, _toolBottom[_bottomCur]);
	}
}
