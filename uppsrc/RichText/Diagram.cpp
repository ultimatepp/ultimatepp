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

void DiagramItem::Reset()
{
	shape = SHAPE_RECT;
	qtf.Clear();
	width = 2;
	ink = Black();
	paper = White();
	blob_id = 0;
	
	cap[0] = cap[1] = CAP_NONE;
	dash = 0;
}

void Point2::Normalize()
{
	if(pt[0].x > pt[1].x)
		Swap(pt[0].x, pt[1].x);
	if(pt[0].y > pt[1].y)
		Swap(pt[0].y, pt[1].y);
}

void DiagramItem::FixPosition()
{
	double x = min(pt[0].x, pt[1].x);
	if(x < 0) {
		pt[0].x -= x;
		pt[1].x -= x;
	}
	double y = min(pt[0].y, pt[1].y);
	if(y < 0) {
		pt[0].y -= y;
		pt[1].y -= y;
	}
	if(IsLine())
		return;
	if(pt[1].x - pt[0].x < 8)
		pt[1].x = pt[0].x + 8;
	if(pt[1].y - pt[0].y < 8)
		pt[1].y = pt[0].y + 8;
}

bool DiagramItem::IsClick(Point p) const
{
	if(IsLine())
		return DistanceFromSegment(p, pt[0], pt[1]) < width + 10;
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
	int cx = (int)Distance(pt[0], pt[1]);
	if(cx < 16)
		return false;
	if(IsLine()) {
		int txt_cy = txt.GetHeight(zoom, cx);
		Pointf sp;
		double dist = DistanceFromSegment(p0, pt[0], pt[1], &sp);
		p.y = txt_cy + 10 - dist;
		if(p0.y >= sp.y) // only consider text above
			return false;
		p.x = Distance(pt[0].x < pt[1].x ? pt[0] : pt[1], sp);
		if(dist < 25 && abs(p.x - cx / 2) < min(10, cx / 8))
			return true;
		p = p / z;
		page.top = 0;
		page.left = 0;
		page.right = int(cx / z);
	}
	else {
		int txt_cy = txt.GetHeight(zoom, r.GetWidth());
		r = RectC(r.left, r.top + (r.GetHeight() - txt_cy) / 2, r.GetWidth(), txt_cy);
		Rect r2 = r;
		int x = r.CenterPoint().x;
		int dx = min(20, int(abs(pt[0].x - pt[1].x) / 6));
		r2.left = x - dx;
		r2.right = x + dx;
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
		int d = max(10, int(Distance(pt[0], pt[1]) + 0.5));
		Point c = (pt[0] + pt[1]) / 2;
		return Rect(c.x - d / 2, c.y, c.x + d, c.y);
	}
	return GetRect();
}

void DiagramItem::Save(StringBuffer& r) const
{
	r << Shape[clamp(shape, 0, Shape.GetCount() - 1)] << ' ';
	r << pt[0].x << ' ' << pt[0].y << ' ' << pt[1].x << ' ' << pt[1].y;
	if(qtf.GetCount())
		r << " " << AsCString(qtf);
	auto col = [&](Color c) {
		if(IsNull(c))
			return String("null");
		return Format("%02x%02x%02x", (int)c.GetR(), (int)c.GetG(), (int)c.GetB());
	};
	if(blob_id.GetCount())
		r << " blob_id " << AsCString(blob_id);
	if(ink != Black())
		r << " ink " << col(ink);
	if(paper != White())
		r << " paper " << col(paper);
	if(width != 2)
		r << " width " << width;
	if(cap[0] != CAP_NONE)
		r << " start " << LineCap[clamp(cap[0], 0, LineCap.GetCount())];
	if(cap[1] != CAP_NONE)
		r << " end " << LineCap[clamp(cap[1], 0, LineCap.GetCount())];
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
	this->pt[0].x = p.ReadDouble();
	this->pt[0].y = p.ReadDouble();
	this->pt[1].x = p.ReadDouble();
	this->pt[1].y = p.ReadDouble();
	auto col = [&] {
		if(p.Id("null"))
			return Color(Null);
		dword x = p.ReadNumber(16);
		return Color(byte(x >> 16), byte(x >> 8), byte(x));
	};
	for(;;) {
		auto Cap = [&] { return max(LineCap.Find(p.ReadId()), 0); };
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
			cap[0] = Cap();
		else
		if(p.Id("end"))
			cap[1] = Cap();
		else
		if(p.Id("dash"))
			dash = clamp(p.ReadInt(), 0, (int)DASH_COUNT);
		else
		if(p.Id("blob_id"))
			blob_id = p.ReadString();
		else
			p.Skip();
	}
	FixPosition();
}

Size Diagram::GetSize() const
{
	if(!IsNull(size))
		return size;
	if(item.GetCount() == 0)
		return Size(0, 0);
	Pointf tl, br;
	tl = br = item[0].pt[0];
	for(const DiagramItem& m : item) {
		tl.x = min(tl.x, m.pt[0].x, m.pt[1].x);
		tl.y = min(tl.y, m.pt[0].y, m.pt[1].y);
		br.x = max(br.x, m.pt[0].x, m.pt[1].x);
		br.y = max(br.y, m.pt[0].y, m.pt[1].y);
	}
	
	Sizef fsz = br + tl;
	Sizef isz = img.GetSize();
	if(img_hd)
		isz /= 2;
	
	return Size((int)ceil(max(isz.cx, fsz.cx)), (int)ceil(max(isz.cy, fsz.cy)));
}

String Diagram::AddBlob(const String& data)
{
	String id = MD5String(data);
	blob.GetAdd(id) = data;
	return id;
}

String Diagram::GetBlob(const String& id) const
{
	return blob.Get(id, Null);
}

Image Diagram::GetBlobImage(const String& id) const
{
	Value v = MakeValue(
		[&] {
			return id;
		},
		[&](Value& v) {
			Image m = StreamRaster::LoadStringAny(GetBlob(id));
			v = m;
			return int(m.GetLength() * sizeof(RGBA));
		}
	);
	return v.Is<Image>() ? (Image)v : Image();
}

Rectf Diagram::GetBlobSvgPathBoundingBox(const String id) const
{
	Value v = MakeValue(
		[&] {
			return id;
		},
		[&](Value& v) {
			v = GetSVGPathBoundingBox(GetBlob(id));
			return 32;
		}
	);
	return v.Is<Rectf>() ? (Rectf)v : (Rectf)Null;
}

void Diagram::Paint(Painter& w, const Diagram::PaintInfo& p) const
{
	w.Begin();
	if(img_hd)
		w.Scale(0.5);
	w.DrawImage(0, 0, img);
	w.End();
	Index<Pointf> conn;
	if(p.display_grid)
		for(const DiagramItem& m : item)
			if(m.IsLine())
				conn << m.pt[0] << m.pt[1];
	for(int i = 0; i < item.GetCount(); i++) {
		dword style = 0;
		if(i == p.cursor)
			style = Display::CURSOR;
		else
		if(p.sel.Find(i) >= 0)
			style = Display::SELECT;
		if(p.editor)
			style |= DiagramItem::EDITOR;
		if(p.display_grid)
			style |= DiagramItem::GRID;
		if(p.dark)
			style |= DiagramItem::DARK;
		if(p.fast)
			style |= DiagramItem::FAST;
		item[i].Paint(w, *this, style, &conn);
	}
}

void Diagram::Serialize(Stream& s)
{
	s % img % item % size;
}

void Diagram::Save(StringBuffer& r) const
{
	if(!IsNull(size))
		r << "size " << size.cx << " " << size.cy << ";\n";
	if(!IsNull(img)) {
		r << "bk_image ";
		if(img_hd)
			r << "HD ";
		r << AsCString(Base64Encode(PNGEncoder().SaveString(img))) << ";\n";
	}
	VectorMap<String, String> sd;
	for(const DiagramItem& m : item) {
		int id = 0;
		if(m.blob_id.GetCount()) {
			int q = blob.Find(m.blob_id);
			if(q >= 0) {
				id = sd.Find(m.blob_id);
				if(id < 0) {
					sd.Add(m.blob_id, blob[q]);
					id = sd.GetCount();
				}
				else
					id++;
			}
		}
		m.Save(r);
		r << '\n';
	}

	for(int i = 0; i < sd.GetCount(); i++)
		r << "blob " << AsCString(Base64Encode(sd[i]), 20000, "     ") << ";\n";
}

void Diagram::Load(CParser& p)
{
	item.Clear();
	blob.Clear();
	while(!p.IsEof())
		if(p.Id("size")) {
			size.cx = clamp(p.ReadInt(), 1, 10000);
			size.cy = clamp(p.ReadInt(), 1, 10000);
			p.Char(';');
		}
		else
		if(p.Id("bk_image")) {
			img_hd = p.Id("HD");
			img = StreamRaster::LoadStringAny(Base64Decode(p.ReadString()));
			p.Char(';');
		}
		else
		if(p.Id("blob")) {
			String data = Base64Decode(p.ReadString());
			blob.GetAdd(MD5String(data)) = data;
			p.Char(';');
		}
		else
			item.Add().Load(p);
}

}
