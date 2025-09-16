#include "RichEdit.h"

namespace Upp {

void DiagramEditor::Map(Point& p)
{
	p = (Pointf)p / GetZoom() + (Pointf)(Point)sb;
}

Point DiagramEditor::GetSizeHandle(Point p) const
{
	Point h = { 0, 0 };
	if(IsNull(data.size)) // automatic size - no resize
		return h;
	Size sz = data.GetSize();
	if(abs(sz.cx - p.x) < 8)
		h.x = 1;
	if(abs(sz.cy - p.y) < 8)
		h.y = 1;
	return h;
}

Point DiagramEditor::GetHandle(int i, Point p_) const
{ // -1 top/left, 1 right/botom
	Point h(0, 0);
	Pointf p = p_;
	if(i >= 0) {
		const DiagramItem& m = data.item[i];
		if(m.IsLine()) {
			double r = (m.width + 12) / 2 - 1;
			if(Distance(m.pos, p) <= r)
				return Point(-1, -1);
			if(Distance(m.pos + m.size, p) <= r)
				return Point(1, 1);

		}
		else {
			Rect r = m.GetRect();

			p -= r.CenterPoint();
			r -= r.CenterPoint();
			
			p = m.Rotation(-1).Transform(p);

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
	for(int pass = 0; pass < 2; pass++)
		for(int i = data.item.GetCount() - 1; i >= 0; i--)
			if(data.item[i].IsClick(p, data, pass))
				return i;
	return -1;
}

Image DiagramEditor::CursorImage(Point p, dword keyflags)
{
	Map(p);
	
	if(edit_text)
		return Image::Arrow();

/*
	for(int i = 0; i < data.item.GetCount(); i++)
		if(data.item[i].IsTextClick(p))
			return Image::IBeam();
*/
	Point h = HasCapture() ? sizehandle : GetSizeHandle(p);
	if(h.x && h.y)
		return Image::SizeBottomRight();
	if(h.x)
		return Image::SizeHorz();
	if(h.y)
		return Image::SizeVert();

	int i = FindItem(p);
	
	if(HasCapture() && doselection)
		return Image::Arrow();

	h = HasCapture() ? draghandle : IsCursor() ? GetHandle(cursor, p) : Null;

	if(IsNull(h))
		return Image::Arrow();

	if(HasCapture() && IsCursor() && CursorItem().IsLine())
		return Image::Arrow();

	int m = h.x * h.y;
	if((h.x || h.y)  && IsCursor() && CursorItem().IsLine())
		return DiagramImg::LineCursor();
	
	if(h.x == -1 && h.y == 1)
		return DiagramImg::RotateCursor();
	
	double rot;
	if(m > 0)
		rot = -M_PI / 4;
	else
	if(m < 0)
		rot = M_PI / 4;
	else
	if(h.x)
		rot = M_PI / 2;
	else
	if(h.y)
		rot = 0;
	else
		return Image::Arrow();
	
	rot += M_2PI * CursorItem().rotate / 360;
	
	return MakeValue(
		[&] { return String((const char *)&rot, sizeof(rot)); },
		[&] (Value& v) {
			ImagePainter w(DPI(32, 32));
			w.Clear();
			const double x1 = 10;
			const double x2 = 14;
			const double x3 = 18;
			const double x4 = 22;
			const double y1 = 2;
			const double y2 = 11;
			const double y3 = 21;
			const double y4 = 30;
			const double m = 16;
			w.Scale(DPI(1));
			w.Translate(m, m);
			w.Rotate(rot);
			w.Translate(-m, -m);
			w.Move(m, y1).Line(x4, y2).Line(x3, y2).Line(x3, y3).Line(x4, y3)
			 .Line(m, y4).Line(x1, y3).Line(x2, y3).Line(x2, y2).Line(x1, y2)
			 .Close();
			w.Stroke(2, White());
			w.Fill(White());
			w.Stroke(1, Black());
			Image img = w.GetResult();
			SetHotSpots(img, DPI(16, 16));
			v = img;
			return img.GetLength() * sizeof(RGBA);
		}
	).To<Image>();
}

void DiagramEditor::MouseWheel(Point, int zdelta, dword keyflags) {
	if(keyflags & K_ALT) {
		if(IsCursor()) {
			DiagramItem& m = CursorItem();
			if(m.IsLine()) {
				int angle = int(Bearing(m.size) * 180 / M_PI);
				angle = ((angle + 360 + sgn(zdelta) * 15) / 15 * 15) % 360;
				m.size = Length(m.size) * Polar(angle * M_PI / 180);
			}
			else
				m.rotate = ((int(m.rotate) + sgn(zdelta) * 15) / 15 * 15) % 360;
			Commit();
			Sync();
		}
		return;
	}
	if(keyflags & K_CTRL) {
		zoom_percent = clamp((zoom_percent / 25 + sgn(zdelta)) * 25, 25, 400);
		Sync();
		return;
	}
	if(keyflags & K_SHIFT)
		sb.WheelX(zdelta);
	else
		sb.WheelY(zdelta);
}

void DiagramEditor::LeftDouble(Point p, dword keyflags)
{
	if(IsCursor() && !(keyflags & K_CTRL))
		StartText();
}

void DiagramEditor::Grid(Point& p)
{
	if(grid && !GetShift())
		p = (p + Point(3, 3)) / 8 * 8;
}

void DiagramEditor::Grid(Pointf& p)
{
	Point pp = p;
	Grid(pp);
	p = pp;
}

void DiagramEditor::LeftDown(Point p, dword keyflags)
{
	moved = moving = false;
	
	conns.Clear();

	Map(p);

	FinishText();
	dragstart = dragcurrent = p;
	base_rotate = CursorItem().rotate;

	SetCapture();

	sizehandle = GetSizeHandle(p);
	if(sizehandle.x || sizehandle.y)
		return;
	
	if(IsCursor()) {
		drag_cp = CursorItem().pos;
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
			dragfrom = CursorItem().pos;
			sdragfrom.SetCount(sel.GetCount());
			for(int i = 0; i < sel.GetCount(); i++)
				sdragfrom[i] = data.item[sel[i]].pos;
			if(sel.GetCount() > 1 || !CursorItem().IsLine())
				PrepareConns();
			else
				conns.Clear();
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

	moved = moved || p != dragstart;

	if(HasCapture() && IsCursor() && draghandle == Point(999,999) && tool >= 0) { // adding tool based shape
		DiagramItem& m = CursorItem();
		Pointf p0 = dragstart;
		Grid(p0);
		Pointf p1 = p;
		Grid(p1);
		m.size = p1 - p0;
		if(m.IsLine())
			m.pos = p0;
		else {
			m.size.cx = max(0.5 * m.size.cx, 4.0);
			m.size.cy = max(0.5 * m.size.cy, 4.0);
			m.pos = p0 + m.size;
			ASSERT(m.pos - m.size == p0);
			m.FixPosition();
			ASSERT(m.pos - m.size == p0);
		}
		m.FixPosition();
		Sync();
		return;
	}
	if(HasCapture() && doselection) { // do rectangular selection
		if(tool >= 0) { // start tool
			if(Distance(dragstart, p) >= 8) {
				KillCursor();
				DiagramItem& m = AddItem(tl[tool].shape);
				m = tl[tool];
				Grid(p);
				m.pos = p;
				m.size = Sizef(8, 8);
				draghandle = Point(999,999);
			}
			return;
		}
		dragcurrent = p;
		Rectf r(dragstart, dragcurrent);
		r.Normalize();
		sel.Clear();
		KillCursor();
		for(int i = 0; i < data.item.GetCount(); i++) {
			Rectf m = data.item[i].GetRect();
			if(r.Contains(m.TopLeft()) && r.Contains(m.BottomRight())) {
				sel.FindAdd(i);
				SetCursor(i);
			}
		}
		Sync();
		return;
	}
	if(HasCapture() && (sizehandle.x || sizehandle.y)) { // resize canvas
		Grid(p);
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
		if(IsNull(draghandle)) { // move selection
			Pointf offset = Point(p - dragstart);
			Pointf p = dragfrom + offset;
			Grid(p);
			offset = p - dragfrom;
			for(int i = 0; i < sel.GetCount(); i++) {
				int ii = sel[i];
				if(ii >= 0 && ii < data.item.GetCount() && i < sdragfrom.GetCount()) {
					data.item[ii].pos = sdragfrom[i] + offset;
					Grid(data.item[ii].pos);
					data.item[ii].FixPosition();
				}
			}
		}
		else {
			Pointf p0 = p;
			Grid(p);
			if(!m.IsLine())
				m.Normalize();
			Rectf r = m.GetRect();
			if(m.IsLine()) {
				Pointf pf = p;
				if(!GetShift()) {
					double d0 = SquaredDistance(p0, pf);
					for(const DiagramItem& m : data.item)
						for(Pointf cs : m.GetConnections()) {
							double d1 = SquaredDistance(p0, cs);
							if(d1 < d0) {
								d0 = d1;
								pf = cs;
							}
						}
				}
				Pointf p2 = m.pos + m.size;
				if(draghandle.x < 0)
					m.pos = pf;
				else
					p2 = pf;
				m.size = p2 - m.pos;
			}
			else
			if(draghandle.x == -1 && draghandle.y == 1) {
				Pointf bl = Xform2D::Rotation(M_PI * base_rotate / 180.0).Transform(r.BottomLeft() - drag_cp);
				m.rotate = base_rotate + 180.0 * (Bearing((Pointf)p0 - drag_cp) - Bearing(bl)) / M_PI;
				if(grid && !GetShift())
					m.rotate = (int(m.rotate + 360 + 7) / 15 * 15) % 360;
			}
			else {
				bool rotated = m.rotate;
				r -= drag_cp;
				if(rotated)
					p = m.Rotation(-1).Transform(Pointf(p) - drag_cp) + drag_cp;
				Sizef hsz = r.GetSize() / 2;
				auto Do = [](int h, double& hsz, double a, double cp) {
					if(h)
						hsz = abs(a - cp);
				};
				Do(draghandle.x, hsz.cx, p.x, drag_cp.x);
				Do(draghandle.y, hsz.cy, p.y, drag_cp.y);
				hsz.cx = max(hsz.cx, 8.0);
				hsz.cy = max(hsz.cy, 8.0);
				m.size = hsz;
				if(m.aspect_ratio) {
					Sizef sz1, sz2;
					ComputeAspectSize(m, sz1, sz2);
					m.size = (draghandle.x && draghandle.y && sz1.cx < sz2.cx || draghandle.x ? sz1 : sz2) / 2;
				}
			}
		}
		UseConns();
		Sync();
		return;
	}
}

void DiagramEditor::LeftUp(Point p, dword flags)
{
	Map(p);
	if(!moving && !(flags & K_CTRL) && !doselection) {
		sel.Clear();
		SetCursor(FindItem(p));
	}
	moving = doselection = false;
	conns.Clear();
	Sync();
	Commit();
}

}