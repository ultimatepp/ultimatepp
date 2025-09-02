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
			if(Distance(m.pos, p) < 6)
				return Point(-1, -1);
			if(Distance(m.pos + m.size, p) < 6)
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

	if(HasCapture() && i >= 0 && data.item[i].IsLine())
		return Image::Arrow();

	int m = h.x * h.y;
	if((h.x || h.y) && i >= 0 && data.item[i].IsLine())
		return Image::SizeAll();
	
	if(h.x == -1 && h.y == 1)
		return DiagramImg::RotateCursor();
	
	double rot;
	if(m > 0)
		rot = - M_PI / 4;
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
	
	if(tool >= 0) {
		KillCursor();
		DiagramItem& m = AddItem(tl[tool].shape);
		m = tl[tool];
		Grid(p);
		m.pos = p;
		m.size = Sizef(0, 0);
		m.FixPosition();
		draghandle = Point(1, 1);
		return;
	}

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

	if(HasCapture() && doselection) { // do rectangular selection
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
	if(HasCapture() && (sizehandle.x || sizehandle.y)) {
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
				auto Do = [](int h, double& a1, double& a2, double a) {
					if(h)
						(h < 0 ? a1 : a2) = a;
				};
				Pointf p2 = m.pos + m.size;
				Do(draghandle.x, m.pos.x, p2.x, p.x);
				Do(draghandle.y, m.pos.y, p2.y, p.y);
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
			#if 0
				if(m.aspect_ratio && 0) {
					m.Normalize();
					Sizef sz1, sz2;
					ComputeAspectSize(m, sz1, sz2);
					Sizef sz;
					if(draghandle.y == 0)
						sz = sz1;
					else
					if(draghandle.x == 0)
						sz = sz2;
					else
						sz = sz1.cx < sz2.cx ? sz1 : sz2;
					if(draghandle.x < 0)
						m.pt[0].x = m.pt[1].x - sz.cx;
					else
						m.pt[1].x = m.pt[0].x + sz.cx;
					if(draghandle.y < 0)
						m.pt[0].y = m.pt[1].y - sz.cy;
					else
						m.pt[1].y = m.pt[0].y + sz.cy;
				}
			#endif
				m.size = hsz;
			}
		}
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

}