#include "RichText.h"

namespace Upp {

double DistanceFromSegment(Pointf P, Pointf P0, Pointf P1, Pointf *p = NULL)
{
	Pointf v = P1 - P0;
	Pointf w = P - P0;
	auto Dot = [](Pointf a, Pointf b) { return a.x * b.x + a.y * b.y; };
	double c1 = Dot(w, v);
	double c2 = Dot(v, v);
	if(p)
		*p = Null;
	if(c1 <= 0)
	    return Distance(P, P0);
	if(c1 >= c2)
	    return Distance(P, P1);
	Pointf sp = P0 + c1 / c2 * v;
	if(p)
		*p = sp;
	return Distance(P, sp);
}

Index<String> DiagramItem::LineCap = { "none", "arrow", "round" };
Index<String> DiagramItem::Shape = { "line", "rect", "round_rect", "ellipse", "diamond", "oval", "parallelogram" };

void DiagramItem::Reset()
{
	shape = SHAPE_RECT;
	qtf.Clear();
	width = 2;
	ink = Black();
	paper = White();
	
	line_start = CAP_NONE;
	line_end = CAP_NONE;
	dash = 0;
}

void Point2::Normalize()
{
	if(p1.x > p2.x)
		Swap(p1.x, p2.x);
	if(p1.y > p2.y)
		Swap(p1.y, p2.y);
}

bool DiagramItem::IsClick(Point p) const
{
	if(IsLine())
		return DistanceFromSegment(p, p1, p2) < width + 20;
	else
		return GetRect().Inflated(5).Contains(p);
}

bool DiagramItem::IsTextClick(Point p0) const
{
	Zoom zoom = Diagram::TextZoom();
	double z = zoom.AsDouble();
	RichText txt = ParseQTF(qtf);
	Rect r = GetRect();
	Rect page;
	Pointf p;
	if(IsLine()) {
		int cx = Distance(p1, p2);
		int txt_cy = txt.GetHeight(zoom, cx);
		Pointf sp;
		double dist = DistanceFromSegment(p0, p1, p2, &sp);
		p.y = txt_cy + 10 - dist;
		if(p0.y >= sp.y) // only consider text above
			return false;
		p.x = Distance(p1.x < p2.x ? p1 : p2, sp);
		if(dist < 25 && abs(p.x - cx / 2) < 10)
			return true;
		p = p / z;
		page.top = 0;
		page.left = 0;
		page.right = cx / z;
	}
	else {
		int txt_cy = txt.GetHeight(zoom, r.GetWidth());
		r = RectC(r.left, r.top + (r.GetHeight() - txt_cy) / 2, r.GetWidth(), txt_cy);
		Rect r2 = r;
		int x = r.CenterPoint().x;
		r2.left = x - 20;
		r2.right = x + 20;
		if(r2.Contains(p0))
			return true;
		if(!r.Contains(p0))
			return false;
		p = (Pointf)p0 / z;
		page = (Rectf)r / z;
	}
	page.bottom = INT_MAX;
	int pos = txt.GetPos((int)p.x, PageY(0, (int)p.y), page);
	return pos < txt.GetLength() && txt.GetRichPos(pos).chr != '\n' && txt.GetCaret(pos, page).Contains(p);
}

Rect DiagramItem::GetTextEditRect() const
{
	if(IsLine()) {
		int   d = max(10, int(Distance(p1, p2) + 0.5));
		Point c = (p1 + p2) / 2;
		return Rect(c.x - d / 2, c.y, c.x + d, c.y);
	}
	return GetRect();
}

void DiagramItem::Paint(Painter& w, dword style) const
{
	Zoom zoom = Diagram::TextZoom();
	
	RichText txt = ParseQTF(qtf);

	static Vector<double> dashes[4] = { { 0 }, { 1 }, { 2 }, { 1, 2 } };
	
	auto DoDash = [&] {
		if(dash) {
			Vector<double> d = clone(dashes[clamp(dash, 0, __countof(dashes))]);
			for(double& h : d)
				h *= width;
			w.Dash(d, 0);
		}
	};
	
	Color sel1 = SColorHighlight();
	Color sel2 = Blend(SColorHighlight(), SWhite());
	
	if(IsLine()) {
		Pointf v = p2 - p1;
		if(style) {
			w.Move(p1).Line(p2).EndPath();
			w.Begin();
			if((style & EDITOR) && width == 0)
				w.Dash("5 1").Stroke(1, 100 * sel2);
			if(style & (Display::CURSOR | Display::SELECT)) {
				w.LineCap(LINECAP_ROUND).Stroke(width + 12, (style & Display::SELECT ? 30 : 200) * sel2);
				double r = (width + 12) / 2 - 1;
				w.Circle(p1, r).Fill(sel1);
				w.Circle(p2, r).Fill(sel1);
			}
			w.End();
		}
		double d = Length(v);
		v = Upp::Normalize(v);

		Pointf a1 = p1;
		Pointf a2 = p2;
		if(d > 20) { // enough length to have caps
			if(line_start == CAP_ARROW)
				a1 += v * 8;
			if(line_end == CAP_ARROW)
				a2 -= v * 8;
		}
		
		w.Move(a1).Line(a2);
		DoDash();
		w.Stroke(width, ink);
		
		Pointf o = Orthogonal(v);
		if(d > 20) {
			auto PaintCap = [&](int k, Pointf p, Pointf a) {
				switch(k) {
				case CAP_ARROW:
					w.Move(p).Line(a + 4 * o).Line(a - 4 * o).Fill(ink);
					break;
				case CAP_CIRCLE:
					w.Circle(p, 5).Fill(ink);
					break;
				}
			};
			PaintCap(line_start, p1, a1 + v);
			PaintCap(line_end, p2, a2 - v);
		}
		 
		int cx = Distance(p1, p2);
		int txt_cy = txt.GetHeight(zoom, cx);
		
		w.Begin();
		double angle = Bearing(p2 - p1);
		if(angle >= -M_PI / 2 && angle <= M_PI / 2) {
			w.Translate(p1 - o * (txt_cy + 10));
			w.Rotate(angle);
		}
		else {
			w.Translate(p2 + o * (txt_cy + 10));
			w.Rotate(angle + M_PI);
		}
		txt.Paint(zoom, w, 0, 0, cx);
		w.End();
	}
	else {
		if(style & (Display::CURSOR | Display::SELECT)) {
			w.RoundedRectangle(GetRect(), 5)
			 .Fill((style & Display::SELECT ? 30 : 200) * sel2);
			w.RoundedRectangle(GetRect().Inflated(2), 5)
			 .Stroke(6, (style & Display::SELECT ? 30 : 200) * sel1);
		}

		int txt_cy = txt.GetHeight(zoom, GetRect().GetWidth());
		Rectf r(p1, p2);
		r.Normalize();
		Pointf c = r.CenterPoint();
		int sz = min(r.Width(), r.Height());
		switch(shape) {
		case SHAPE_ROUNDRECT:
			w.RoundedRectangle(r.left, r.top, r.GetWidth(), r.GetHeight(), sz > 30 ? 8 : sz > 15 ? 4 : 2);
			break;
		case SHAPE_OVAL:
			if(r.GetWidth() > r.GetHeight()) {
				double ra = r.GetHeight() / 2;
				w.Move(r.left + ra, r.top)
				 .Line(r.right - ra, r.top)
				 .Arc(r.right - ra, r.top + ra, ra, -M_PI / 2, M_PI)
				 .Line(r.left + ra, r.bottom)
				 .Arc(r.left + ra, r.top + ra, ra, M_PI / 2, M_PI);
				break;
			}
		case SHAPE_ELLIPSE:
			w.Ellipse(r);
			break;
		case SHAPE_DIAMOND:
			w.Move(c.x, r.top).Line(r.right, c.y).Line(c.x, r.bottom).Line(r.left, c.y).Close();
			break;
		case SHAPE_PARALLELOGRAM:
			w.Move(r.left + r.Width() / 6, r.top).Line(r.right, r.top)
			 .Line(r.right - r.Width() / 6, r.bottom).Line(r.left, r.bottom).Close();
			break;
		default:
			w.Rectangle(r);
			break;
		}
		DoDash();
		w.Stroke(width, ink).Fill(paper);
		txt.Paint(zoom, w, r.left, r.top + (r.GetHeight() - txt_cy) / 2, r.GetWidth());
	}
}

void DiagramItem::Save(StringBuffer& r) const
{
	r << Shape[clamp(shape, 0, Shape.GetCount())] << ' ';
	r << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y;
	if(qtf.GetCount())
		r << " " << AsCString(qtf);
	auto col = [&](Color c) {
		if(IsNull(c))
			return String("null");
		return Format("%02x%02x%02x", (int)c.GetR(), (int)c.GetG(), (int)c.GetB());
	};
	if(ink != Black())
		r << " ink " << col(ink);
	if(paper != White())
		r << " paper " << col(paper);
	if(width != 2)
		r << " width " << width;
	if(line_start != CAP_NONE)
		r << " start " << LineCap[clamp(line_start, 0, LineCap.GetCount())];
	if(line_end != CAP_NONE)
		r << " end " << LineCap[clamp(line_end, 0, LineCap.GetCount())];
	if(dash)
		r << " dash " << dash;
	r << ";";
}

void DiagramItem::Load(CParser& p)
{
	Reset();
	int q = Shape.Find(p.ReadId());
	if(q < 0)
		p.ThrowError("Unknown element");
	shape = q;
	p1.x = p.ReadDouble();
	p1.y = p.ReadDouble();
	p2.x = p.ReadDouble();
	p2.y = p.ReadDouble();
	auto col = [&] {
		if(p.Id("null"))
			return Color(Null);
		dword x = p.ReadNumber(16);
		return Color(byte(x >> 16), byte(x >> 8), byte(x));
	};
	for(;;) {
		auto cap = [&] { return max(LineCap.Find(p.ReadId()), 0); };
		if(p.Char(';'))
			break;
		else
		if(p.IsString())
			qtf = p.ReadString();
		else
		if(p.Id("ink"))
			ink = col();
		else
		if(p.Id("paper"))
			paper = col();
		else
		if(p.Id("width"))
			width = clamp(p.ReadDouble(), 0.0, 50.0);
		else
		if(p.Id("start"))
			line_start = cap();
		else
		if(p.Id("end"))
			line_end = cap();
		else
		if(p.Id("dash"))
			dash = clamp(p.ReadInt(), 0, (int)DASH_COUNT);
		else
			p.Skip();
	}
}

Size Diagram::GetSize() const
{ // TODO: Negatives?
	if(item.GetCount() == 0)
		return Size(0, 0);
	Pointf tl, br;
	tl = br = item[0].p1;
	for(const DiagramItem& m : item) {
		tl.x = min(tl.x, m.p1.x, m.p2.x);
		tl.y = min(tl.y, m.p1.y, m.p2.y);
		br.x = max(br.x, m.p1.x, m.p2.x);
		br.y = max(br.y, m.p1.y, m.p2.y);
	}
	
	Sizef fsz = br + tl;
	Sizef isz = img.GetSize();
	if(img_hd)
		isz /= 2;
	
	return Size(ceil(max(isz.cx, fsz.cx)), ceil(max(isz.cy, fsz.cy)));
}

void Diagram::Paint(Painter& w, const Diagram::PaintInfo& p) const
{
	w.Begin();
	if(img_hd)
		w.Scale(0.5);
	w.DrawImage(0, 0, img);
	w.End();
	for(int i = 0; i < item.GetCount(); i++) {
		dword style = 0;
		if(i == p.cursor)
			style = Display::CURSOR;
		else
		if(p.sel.Find(i) >= 0)
			style = Display::SELECT;
		if(p.editor)
			style |= DiagramItem::EDITOR;
		item[i].Paint(w, style);
	}
}

void Diagram::Serialize(Stream& s)
{
	s % img % item;
}

void Diagram::Save(StringBuffer& r) const
{
	if(!IsNull(img)) {
		r << "bk_image ";
		if(img_hd)
			r << "HD ";
		r << AsCString(Base64Encode(PNGEncoder().SaveString(img))) << ";\n";
	}
	for(const DiagramItem& m : item) {
		m.Save(r);
		r << '\n';
	}
}

void Diagram::Load(CParser& p)
{
	item.Clear();
	while(!p.IsEof())
		if(p.Id("bk_image")) {
			img_hd = p.Id("HD");
			img = StreamRaster::LoadStringAny(Base64Decode(p.ReadString()));
			p.Char(';');
		}
		else
			item.Add().Load(p);
}

}
