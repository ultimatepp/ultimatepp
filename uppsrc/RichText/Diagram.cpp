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

void DiagramItem::Reset()
{
	shape = SHAPE_RECT;
	qtf.Clear();
	width = 2;
	ink = Black();
	paper = White();
	blob_id = 0;
	flip_horz = false;
	flip_vert = false;
	aspect_ratio = false;
	rotate = 0;
	
	cap[0] = cap[1] = CAP_NONE;
	dash = 0;
}

void DiagramItem::Serialize(Stream& s)
{ // used for undo/redo mostly
	s % pos
	  % size
	  % shape
	  % qtf
	  % width
	  % ink
	  % paper
	  % blob_id
	  % flip_horz
	  % flip_vert
	  % aspect_ratio
	  % rotate
	  % cap[0]
	  % cap[1];
}

void DiagramItem::Normalize()
{
	if(IsLine())
		return;
	size.cx = abs(size.cx);
	size.cy = abs(size.cy);
}

void DiagramItem::FixPosition()
{
	Normalize();
	pos.x = clamp(pos.x, 0.0, 100000.0);
	pos.y = clamp(pos.y, 0.0, 100000.0);
	if(IsLine())
		return;
	size.cx = clamp(size.cx, 4.0, 100000.0);
	size.cy = clamp(size.cy, 4.0, 100000.0);
}

bool DiagramItem::IsClick(Point p, const Diagram& diagram, bool relaxed) const
{
	if(IsLine())
		return DistanceFromSegment(p, pos, pos + size) < width + 10;
	Rectf rect = GetRect();
	Pointf cp = rect.CenterPoint();
	if(rotate) {
		p -= cp;
		p = Rotation(-1).Transform(p);
		p += cp;
	}
	if(!rect.Contains(p))
		return false;
	if(shape == SHAPE_IMAGE || relaxed)
		return true;
	Image test = MakeValue(
		[&] {
			return String((const char *)&shape, sizeof(shape));
		},
		[&](Value& v) {
			ImagePainter p(64, 64);
			p.Clear(RGBAZero());
			DiagramItem m = *this;
			m.paper = Blue();
			m.ink = Blue();
			m.pos = Pointf(0, 0);
			m.size = Sizef(64, 64);
			m.Paint(p, diagram);
			Image img = p.GetResult();
			v = img;
			return img.GetLength() * sizeof(RGBA);
		}
	).To<Image>();
	return test[clamp(int(64 * (p.y - rect.top) / rect.GetHeight()), 0, 63)]
	           [clamp(int(64 * (p.x - rect.left) / rect.GetWidth()), 0, 63)].a;
}

#if 0
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
#endif

Rect DiagramItem::GetTextEditRect() const
{
	if(IsLine()) {
		int d = max(10, int(Length(size) + 0.5));
		return Rect(pos.x - d / 2, pos.y, pos.x + d, pos.y);
	}
	return GetRect();
}

void DiagramItem::Save(StringBuffer& r) const
{
	r << Shape[clamp(shape, 0, Shape.GetCount() - 1)] << ' ';
	r << pos.x << ' ' << pos.y << ' ' << size.cx << ' ' << size.cy;
	if(qtf.GetCount())
		r << " " << AsCString(qtf);
	auto col = [&](Color c) {
		if(IsNull(c))
			return String("null");
		return Format("%02x%02x%02x", (int)c.GetR(), (int)c.GetG(), (int)c.GetB());
	};
	if(rotate)
		r << " rotate " << rotate;
	if(blob_id.GetCount())
		r << " blob_id " << AsCString(blob_id);
	if(ink != Black())
		r << " stroke " << col(ink);
	if(paper != White())
		r << " fill " << col(paper);
	if(width != 2)
		r << " width " << width;
	if(cap[0] != CAP_NONE)
		r << " start " << LineCap[clamp(cap[0], 0, LineCap.GetCount())];
	if(cap[1] != CAP_NONE)
		r << " end " << LineCap[clamp(cap[1], 0, LineCap.GetCount())];
	if(dash)
		r << " dash " << dash;
	if(flip_vert)
		r << " flip_vert";
	if(flip_horz)
		r << " flip_horz";
	if(aspect_ratio)
		r << " aspect_ratio";
	r << ";";
}

void DiagramItem::Load(CParser& p, const Diagram& diagram)
{
	Reset();
	int q = Shape.Find(p.ReadId());
	if(q < 0)
		p.ThrowError("Unknown element");
	shape = q;
	this->pos.x = p.ReadDouble();
	this->pos.y = p.ReadDouble();
	this->size.cx = p.ReadDouble();
	this->size.cy = p.ReadDouble();
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
		if(p.Id("rotate"))
			rotate = clamp(p.ReadDouble(), -360.0, 360.0);
		else
		if(p.Id("stroke"))
			ink = col();
		else
		if(p.Id("fill"))
			paper = col();
		else
		if(p.Id("width"))
			width = clamp(p.ReadDouble(), 0.0, 50.0);
		else
		if(p.Id("flip_vert"))
			flip_vert = true;
		else
		if(p.Id("flip_horz"))
			flip_horz = true;
		else
		if(p.Id("aspect_ratio"))
			aspect_ratio = true;
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

void Diagram::GetSize0(Pointf& tl, Pointf& br) const
{
	if(item.GetCount() == 0) {
		tl = br = Pointf(0, 0);
		return;
	}
	tl = br = item[0].GetRect().TopLeft();
	for(const DiagramItem& m : item) {
		Rectf r = m.GetRect();
		Pointf cp = r.CenterPoint();
		Xform2D rot;
		if(m.rotate)
			rot = m.Rotation();
		auto Do = [&](Pointf p) {
			if(m.rotate) {
				p -= cp;
				p = rot.Transform(p);
				p += cp;
			}
			p.x = max(p.x, 0.0);
			p.y = max(p.y, 0.0);
			tl.x = min(tl.x, p.x);
			tl.y = min(tl.y, p.y);
			br.x = max(br.x, p.x);
			br.y = max(br.y, p.y);
		};
		Do(r.TopLeft());
		Do(r.TopRight());
		Do(r.BottomLeft());
		Do(r.BottomRight());
	}
	
}

Size Diagram::GetEditSize() const
{
	Pointf tl, br;
	GetSize0(tl, br);
	Size sz(int(ceil(br.x)), int(ceil(br.y)));
	return IsNull(size) ? sz : Size(max(sz.cx, size.cx), max(sz.cy, size.cy));
}

Size Diagram::GetSize() const
{
	if(!IsNull(size))
		return size;
	Pointf tl, br;
	GetSize0(tl, br);
	Sizef fsz = br + tl;

	Sizef isz = img.GetSize();
	if(img_hd)
		isz /= 2;

	const double lim = 0.5 * INT_MAX;
	
	return Size((int)clamp(ceil(max(isz.cx, fsz.cx)), 0.0, lim),
	            (int)clamp(ceil(max(isz.cy, fsz.cy)), 0.0, lim));
}

String Diagram::AddBlob(const String& data)
{
	if(IsNull(data))
		return Null;
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

Rectf Diagram::GetBlobSvgPathBoundingBox(const String& id) const
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

void Diagram::SweepBlobs(const Index<String>& keep_ids)
{
	blob.RemoveIf([&](int i) { return keep_ids.Find(blob.GetKey(i)) < 0; });
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
				conn << m.pos << m.pos + m.size;
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
	r << "QDF 1.0;\n";
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
	img.Clear();
	img_hd = false;
	size = Null;
	while(!p.IsEof())
		if(p.Id("QDF")) {
			p.ReadDouble();
			p.PassChar(';');
		}
		else
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
			item.Add().Load(p, *this);
}

}
