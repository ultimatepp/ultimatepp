#include "RichEdit.h"

namespace Upp {

void DiagramEditor::Map(Point& p)
{
	p = (Pointf)p / GetZoom() + (Pointf)(Point)sb;
}

Point DiagramEditor::GetSizeHandle(Point p) const
{
	Point h = { 0, 0 };
	Size sz = data.GetSize();
	if(abs(sz.cx - p.x) < 8)
		h.x = 1;
	if(abs(sz.cy - p.y) < 8)
		h.y = 1;
	return h;
}

Point DiagramEditor::GetHandle(int i, Point p) const
{ // -1 top/left, 1 right/botom
	Point h(0, 0);
	if(i >= 0) {
		const DiagramItem& m = data.item[i];
		if(m.IsLine()) {
			if(Distance(m.pt[0], p) < 6)
				return Point(-1, -1);
			if(Distance(m.pt[1], p) < 6)
				return Point(1, 1);

		}
		else {
			Rect r = m.GetRect();
			Rect rr = r.Inflated(5);
			r.Deflate(min(10, r.GetWidth() / 2), min(10, r.GetHeight() / 2));
			if(rr.Contains(p)) {
				if(p.y < r.top)
					h.y = -1;
				if(p.y > r.bottom)
					h.y = 1;
				if(p.x < r.left)
					h.x = -1;
				if(p.x > r.right)
					h.x = 1;
			}
		}
	}
	return h;
}

int   DiagramEditor::FindItem(Point p) const
{
	int mini = -1;
	double mina = INT_MAX;
	for(int i = data.item.GetCount() - 1; i >= 0; i--) {
		const DiagramItem& m = data.item[i];
		Rectf r = m.GetRect();
		if(m.IsClick(p) || m.IsTextClick(p)) {
			double a = m.IsLine() ? 0 : r.Width() * r.Height();
			if(a < mina) {
				mina = a;
				mini = i;
			}
		}
	}
	return mini;
}

Image DiagramEditor::CursorImage(Point p, dword keyflags)
{
	Map(p);

/*
	for(int i = 0; i < data.item.GetCount(); i++)
		if(data.item[i].IsTextClick(p))
			return Image::IBeam();
*/
	Point h = GetSizeHandle(p);
	if(h.x && h.y)
		return Image::SizeBottomRight();
	if(h.x)
		return Image::SizeHorz();
	if(h.y)
		return Image::SizeVert();

	int i = FindItem(p);

	h = HasCapture() ? draghandle : IsCursor() ? GetHandle(cursor, p) : Null;

	if(IsNull(h))
		return Image::Arrow();

	if(HasCapture() && i >= 0 && data.item[i].IsLine())
		return Image::Arrow();

	int m = h.x * h.y;
	if((h.x || h.y) && i >= 0 && data.item[i].IsLine())
		return Image::SizeAll();
	if(m > 0)
		return Image::SizeBottomRight();
	if(m < 0)
		return Image::SizeBottomLeft();
	if(h.x)
		return Image::SizeHorz();
	if(h.y)
		return Image::SizeVert();

	return Image::Arrow();
}

void DiagramEditor::LeftDouble(Point p, dword keyflags)
{
	if(IsCursor() && !(keyflags & K_CTRL))
		StartText();
}

void DiagramEditor::Grid(int shape, Point& p)
{
	p = shape == DiagramItem::SHAPE_LINE ? p / 8 * 8 : p / 16 * 16;
}

void DiagramEditor::LeftDown(Point p, dword keyflags)
{
	moving = false;
	
	conns.Clear();

	Map(p);

	FinishText();
	dragstart = dragcurrent = p;

	SetCapture();

	sizehandle = GetSizeHandle(p);
	if(sizehandle.x || sizehandle.y)
		return;
	
	if(tool >= 0) {
		KillCursor();
		DiagramItem& m = AddItem(tl[tool].shape);
		m = tl[tool];
		Grid(m, p);
		m.pt[0] = m.pt[1] = p;
		m.FixPosition();
		draghandle = Point(1, 1);
		return;
	}

	if(IsCursor()) {
		Point h = GetHandle(cursor, p);
		if(h.x || h.y) {
			draghandle = h;
			Sync();
			PrepareConns();
			return;
		}
	}
	
	if(sel.GetCount() == 1 && !(keyflags & K_CTRL)) {
		sel.Clear();
		cursor = -1;
	}

	int i = FindItem(p);
	if(i >= 0) {
		if((keyflags & K_CTRL) && sel.Find(i) >= 0) {
			sel.RemoveKey(i);
			if(sel.GetCount())
				SetCursor(sel.Top());
			else
				KillCursor();
		}
		else
			SetCursor(i);
		if(IsCursor()) {
			dragfrom = GetCursorRect();
			sdragfrom.SetCount(sel.GetCount());
			for(int i = 0; i < sel.GetCount(); i++)
				sdragfrom[i] = data.item[sel[i]];
			PrepareConns();
			draghandle = Null;
			Point h = GetHandle(cursor, p);
			if(h.x || h.y)
				draghandle = h;
		}
	}
	else {
		if(!(keyflags & K_CTRL)) {
			sel.Clear();
			SetCursor(-1);
		}
		doselection = true;
	}

	Sync();
}

void DiagramEditor::MouseMove(Point p, dword keyflags)
{
	Map(p);

	if(HasCapture() && doselection) { // do rectangular selection
		dragcurrent = p;
		Rect r(dragstart, dragcurrent);
		r.Normalize();
		sel.Clear();
		KillCursor();
		for(int i = 0; i < data.item.GetCount(); i++)
			if(r.Contains(data.item[i].pt[0]) && r.Contains(data.item[i].pt[1])) {
				sel.FindAdd(i);
				SetCursor(i);
			}
		Sync();
		return;
	}
	if(HasCapture() && (sizehandle.x || sizehandle.y)) {
		if(IsNull(data.size))
			data.size = data.GetSize();
		if(sizehandle.x)
			data.size.cx = p.x;
		if(sizehandle.y)
			data.size.cy = p.y;
		Sync();
		return;
	}
	if(HasCapture() && IsCursor() && (moving || Distance(dragstart, p) >= 8)) {
		moving = true;
		DiagramItem& m = CursorItem();
		Grid(m, p);
		if(IsNull(draghandle)) { // move selection
			Rectf to = dragfrom.Offseted(p - dragstart);
			Pointf tp = to.TopLeft();
			if(grid)
				tp = (Point)tp / 16 * 16;
			Sizef sz = to.GetSize();
			m.pt[0] = tp;
			m.pt[1] = tp + sz;
			Pointf offset = tp - dragfrom.TopLeft();
			for(int i = 0; i < sel.GetCount(); i++) {
				int ii = sel[i];
				if(ii >= 0 && ii < data.item.GetCount() && i < sdragfrom.GetCount()) {
					(Point2 &)data.item[ii] = sdragfrom[i].Offseted(offset);
					data.item[ii].FixPosition();
				}
			}
		}
		else {
			auto Do = [](int h, double& a1, double& a2, double a) {
				if(h)
					(h < 0 ? a1 : a2) = a;
			};
			Do(draghandle.x, m.pt[0].x, m.pt[1].x, p.x);
			Do(draghandle.y, m.pt[0].y, m.pt[1].y, p.y);
		}
		m.FixPosition();
		UseConns();
		Sync();
		return;
	}
}

void DiagramEditor::LeftUp(Point, dword)
{
	moving = doselection = false;
	tool = -1;
	conns.Clear();
	Sync();
	Commit();
}

void DiagramEditor::RightDown(Point p, dword keyflags)
{
	Map(p);

	auto PopPaint = [=](Draw& w, const Image& m, bool sel) {
		int x = DPI(2);
		int y = DPI(1);
		if(sel)
			w.DrawImage(x, y, m, SColorHighlightText());
		else
			w.DrawImage(x, y, m);
	};

	FinishText();

	int ii = FindItem(p);
	if(ii >= 0) {
		DiagramItem& m = data.item[ii];
		if(m.IsLine()) {
			SetCursor(ii);
			Point h = GetHandle(cursor, p);
			if(h.x) {
				int i = h.x > 0;
				ColumnPopUp menu;
				menu.count = DiagramItem::CAP_COUNT;
				menu.columns = 3;
				menu.isz = IconSz() + Size(DPI(4), DPI(4));
				menu.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
					PopPaint(w, i == 0 ? CapIcon(ii, 0) : CapIcon(0, ii), sel);
				};
				int cap = menu.Execute();
				if(cap < 0)
					return;
				m.cap[i] = cap;
				GetAttrs();
				Sync();
				return;
			}
			if(m.IsClick(p)) {
				ColumnPopUp menu;
				menu.count = DiagramItem::DASH_COUNT;
				menu.columns = 4;
				menu.isz = IconSz() + Size(DPI(4), DPI(4));
				menu.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
					PopPaint(w, DashIcon(ii), sel);
				};
				int dash = menu.Execute();
				if(dash < 0)
					return;
				m.dash = dash;
				GetAttrs();
				Sync();
				return;
			}
		}
	}

	ColumnPopUp shape;
	shape.count = DiagramItem::SHAPE_COUNT;
	shape.columns = 3;
	shape.isz = IconSz() + Size(DPI(4), DPI(4));
	shape.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
		PopPaint(w, ShapeIcon(ii), sel);
	};
	
	tool = -1;

	int si = shape.Execute();
	
	if(si < 0)
		return;
	
	CancelSelection();

	Point p0 = p;
	Grid(si, p);
	Pointf cp = Null; // connect line with nearest connection point
	if(si == DiagramItem::SHAPE_LINE) {
		double mind = DBL_MAX;
		for(const DiagramItem& m : data.item)
			for(Pointf c : m.GetConnections()) {
				double d = Squared(c - (Pointf)p0);
				if(d < mind) {
					cp = c;
					mind = d;
				}
			}
	}

	DiagramItem& m = AddItem(si);
	if(IsNull(cp)) {
		m.pt[0] = p;
		m.pt[1] = p + Point(128, 64);
	}
	else {
		m.pt[0] = cp;
		m.pt[1] = p;
	}
	m.shape = si; // shape must be set to avoid Normalise
	SetAttrs(ATTR_ALL & ~ATTR_SHAPE);
	Sync();
}

void DiagramEditor::RightUp(Point, dword keyflags)
{
	Commit();
}

}