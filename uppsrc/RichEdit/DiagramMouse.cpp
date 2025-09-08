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
			if(Distance(m.pt[0], p) < 6)
				return Point(-1, -1);
			if(Distance(m.pt[1], p) < 6)
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

void DiagramEditor::Grid(int shape, Point& p)
{
	if(grid && !GetShift())
		p = shape == DiagramItem::SHAPE_LINE ? (p + Point(3, 3)) / 8 * 8 : (p + Point(7, 7)) / 16 * 16;
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
		Grid(m, p);
		m.pt[0] = m.pt[1] = p;
		m.FixPosition();
		draghandle = Point(1, 1);
		return;
	}

	if(IsCursor()) {
		drag_cp = CursorItem().GetRect().CenterPoint();
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
		Grid(DiagramItem::SHAPE_RECT, p);
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
		Pointf p0 = p;
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
			if(!m.IsLine())
				m.Normalize();
			Rectf r = m.GetRect();
			if(m.IsLine()) {
				Do(draghandle.x, m.pt[0].x, m.pt[1].x, p.x);
				Do(draghandle.y, m.pt[0].y, m.pt[1].y, p.y);
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
				if(rotated)
					p = m.Rotation(-1).Transform(Pointf(p) - drag_cp) + drag_cp;
				Do(draghandle.x, r.left, r.right, p.x);
				Do(draghandle.y, r.top, r.bottom, p.y);
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
				m.pt[0] = r.TopLeft();
				m.pt[1] = r.BottomRight();
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

void DiagramEditor::RightDown(Point p, dword keyflags)
{
	Map(p);

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
				Caps(menu, i == 0);
				int cap = menu.Execute();
				if(cap < 0)
					return;
				m.cap[i] = cap;
				GetAttrs();
				Sync();
				return;
			}
			if(m.IsClick(p, data)) {
				ColumnPopUp menu;
				Dashes(menu);
				menu.count = DiagramItem::DASH_COUNT + 15;
				menu.columns = 5;
				menu.WhenPaintItem = [=](Draw& w, Size isz, int ii, bool sel) {
					PopPaint(w, ii < DiagramItem::DASH_COUNT ? DashIcon(ii) : WidthIcon(ii - DiagramItem::DASH_COUNT), sel);
				};

				int n = menu.Execute();
				if(n < 0)
					return;
				if(n < DiagramItem::DASH_COUNT)
					m.dash = n;
				else
					m.width = n - DiagramItem::DASH_COUNT;
				GetAttrs();
				Sync();
				return;
			}
		}
	}

	ColumnPopUp shape;
	Shapes(shape);
	
	tool = -1;

	int si = shape.Execute();
	
	if(si < 0)
		return;

	Sizef size;
	String mdata;
	if(si == DiagramItem::SHAPE_SVGPATH) {
		mdata = SelectFontSymbolSvg(size);
		if(IsNull(mdata))
			return;
	}

	if(si == DiagramItem::SHAPE_IMAGE) {
		String path = SelectFileOpen("Images (*.png *.gif *.jpg *.bmp *.svg)\t*.png *.gif *.jpg *.bmp *.svg");

		if(GetFileLength(path) > 17000000) {
			Exclamation("Image is too large!");
			return;
		}
		mdata = LoadFile(path);
		if(IsNull(mdata))
			return;
		size = Null;
		if(IsSVG(mdata)) {
			Rectf f = GetSVGBoundingBox(mdata);
			size = f.GetSize();
		}
		else {
			StringStream ss(mdata);
			One<StreamRaster> r = StreamRaster::OpenAny(ss);
			if(r)
				size = r->GetSize();
		}
		if(IsNull(size)) {
			Exclamation(Format(t_("Unsupported image format in file [* \1%s\1]."), path));
			return;
		}
	}
	
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
	
	Size sz;

	DiagramItem& m = AddItem(si);
	if(mdata.GetCount())
		m.blob_id = data.AddBlob(mdata);
	m.shape = si; // shape must be set before SetAttrs to avoid Normalise
	Sizef szf = m.GetStdSize(data);
	if(IsNull(cp)) {
		m.pt[0] = p;
		m.pt[1] = p + szf;
	}
	else {
		m.pt[0] = cp;
		m.pt[1] = p;
	}
	if(si == DiagramItem::SHAPE_IMAGE) {
		m.ink = Null;
		m.paper = Black();
		m.width = 0;
		SetAttrs(ATTR_ALL & ~(ATTR_SHAPE|ATTR_PAPER|ATTR_INK|ATTR_WIDTH));
	}
	else
	if(si == DiagramItem::SHAPE_SVGPATH) {
		m.ink = Null;
		m.paper = Black();
		m.width = 0;
		SetAttrs(ATTR_ALL & ~(ATTR_SHAPE|ATTR_PAPER|ATTR_INK));
	}
	else
		SetAttrs(ATTR_ALL & ~ATTR_SHAPE);
	Sync();
}

void DiagramEditor::RightUp(Point, dword keyflags)
{
	Commit();
}

}