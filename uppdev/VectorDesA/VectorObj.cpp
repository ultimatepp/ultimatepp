#include "VectorDes.h"
#pragma hdrstop

#define IMAGEFILE <VectorDes/VectorDes.iml>
#define IMAGESPACE VecImg
#include <Draw/iml_header.h>

void DrawHotRect(Draw& draw, int x, int y, int cx, int cy, bool hot, Color color)
{
	if(hot)
		draw.DrawRect(x, y, cx, cy, color);
	else
		DrawFrame(draw, x, y, cx, cy, color);
}

Size ScaleSize(Size sz, int size)
{
	if(sz.cx == 0 && sz.cy == 0)
		sz.cx = 1;
	return iscale(sz, size, max((int)Length(sz), 1));
}

static void VectorSerializePacked(Stream& stream, int& i)
{
	if(stream.IsStoring()) {
		if(IsNull(i))
			stream.Put(0xFF);
		else if(i >= -0x40 && i < 0x40)
			stream.Put(i + 0x40);
		else if(i >= -0x3EFF && i <= 0x3EFF) {
			stream.Put((byte)(i >> 8) + 0xBE);
			stream.Put((byte)i);
		}
		else {
			stream.Put(0xFE);
			stream.Put32le(i);
		}
	}
	else {
		int x = stream.Get();
		if(x < 0)
			return;
		if(x == 0xFF)
			i = Null;
		else if(x == 0xFE) {
			if(stream.IsEof()) {
				stream.SetError();
				return;
			}
			i = stream.Get32le();
		}
		else if(x >= 0x80) {
			int y = stream.Get();
			if(y < 0) {
				stream.SetError();
				return;
			}
			i = (x - 0xBE) * 256 + y;
		}
		else
			i = x - 0x40;
	}
}

static void VectorSerializeFixed(Stream& stream, double& d, double maxval)
{
	if(stream.IsStoring()) {
		int i = Null;
		if(!IsNull(d))
			i = fround(minmax(d, -maxval, maxval) * 0x7FFFFFFF / maxval);
		stream.Put32le(i);
	}
	else if(!stream.IsEof()) {
		int i = stream.Get32le();
		if(IsNull(i))
			d = Null;
		else
			d = i * maxval / 0x7FFFFFFF;
	}
}

static void VectorSerialize(Stream& stream, String& str)
{
	if(stream.IsStoring()) {
		int len = str.GetLength();
		if(str.IsVoid())
			len = -1;
		VectorSerializePacked(stream, len);
		stream.Put(str.Begin(), str.GetLength());
	}
	else {
		int len;
		enum { MAX_STRING_LENGTH = 100000000 };
		VectorSerializePacked(stream, len);
		if(len < 0)
			str = String::GetVoid();
		else if(len == 0)
			str = Null;
		else if(len < MAX_STRING_LENGTH) {
			Buffer<byte> data(len);
			if(!stream.GetAll(data, len)) {
				stream.SetError();
				return;
			}
			str = String(data, len);
		}
		else {
			stream.SetError();
			return;
		}
	}
}

static void VectorSerialize(Stream& stream, Point& pt)
{
	VectorSerializePacked(stream, pt.x);
	VectorSerializePacked(stream, pt.y);
}

static void VectorSerialize(Stream& stream, Size& sz)
{
	VectorSerializePacked(stream, sz.cx);
	VectorSerializePacked(stream, sz.cy);
}

static void VectorSerialize(Stream& stream, Rect& rc)
{
	VectorSerializePacked(stream, rc.left);
	VectorSerializePacked(stream, rc.top);
	VectorSerializePacked(stream, rc.right);
	VectorSerializePacked(stream, rc.bottom);
}

static void VectorSerialize(Stream& stream, Time& t)
{
	if(stream.IsStoring()) {
		if(IsNull(t))
			stream.Put(0xFF);
		else {
			bool dt = (t.hour == 0 && t.minute == 0 && t.second == 0);
			stream.Put(dt ? t.day : t.day + 0x80);
			stream.Put(t.month);
			stream.Put16le(t.year);
			if(!dt) {
				stream.Put(t.hour);
				stream.Put(t.minute);
				stream.Put(t.second);
			}
		}
	}
	else {
		int i = stream.Get();
		if(i < 0)
			return;
		if(i == 0xFF)
			t = Null;
		else {
			int day = i;
			int month = stream.Get();
			int year = stream.Get16le();
			if(year < 0) {
				stream.SetError();
				return;
			}
			year = (short)year;
			int hour = 0, minute = 0, second = 0;
			if(day >= 0x80) {
				day -= 0x80;
				hour = stream.Get();
				minute = stream.Get();
				second = stream.Get();
				if(second < 0) {
					stream.SetError();
					return;
				}
			}
			t = Time(year, month, day, hour, minute, second);
		}
	}
}

static void VectorSerialize(Stream& stream, Color& c)
{
	if(stream.IsStoring()) {
		if(IsNull(c))
			stream.Put(0xFF);
		else {
			int rgb[3] = { c.GetR(), c.GetG(), c.GetB() };
			int rgb4[3];
			for(int i = 0; i < 3; i++)
				rgb4[i] = (rgb[i] == 0x00 ? 0 : rgb[i] == 0x80 ? 1 : rgb[i] == 0xC0 ? 2 : rgb[i] == 0xFF ? 3 : -1);
			if(rgb4[0] >= 0 && rgb4[1] >= 0 && rgb4[2] >= 0)
				stream.Put(rgb4[0] + 4 * rgb4[1] + 16 * rgb4[2]);
			else {
				stream.Put(0xFE);
				stream.Put(rgb[0]);
				stream.Put(rgb[1]);
				stream.Put(rgb[1]);
			}
		}
	}
	else {
		int v = stream.Get();
		if(v == 0xFF)
			c = Null;
		else if(v == 0xFE) {
			int r = stream.Get(), g = stream.Get(), b = stream.Get();
			if(b < 0) {
				stream.SetError();
				return;
			}
			c = Color(r, g, b);
		}
		else if(v < 0x40) {
			static const byte table[] = { 0x00, 0x80, 0xC0, 0xFF };
			c = Color(table[v & 3], table[(v >> 2) & 3], table[(v >> 4) & 3]);
		}
		else
			stream.SetError();
	}
}

static void VectorSerialize(Stream& stream, Font& font)
{
	if(stream.IsStoring()) {
		if(IsNull(font)) {
			stream.Put(0xFF);
			return;
		}
		if(font.GetFace() < Font::OTHER && font.GetFace() < 0x40)
			stream.Put(font.GetFace());
		else {
			stream.Put(0xFE);
			String s = font.GetFaceName();
			VectorSerialize(stream, s);
		}
		int ht = font.GetHeight();
		VectorSerializePacked(stream, ht);
		int wd = font.GetWidth();
		VectorSerializePacked(stream, wd);
		byte bitmask = 0;
		if(font.IsBold()) bitmask |= 1;
		if(font.IsItalic()) bitmask |= 2;
		if(font.IsUnderline()) bitmask |= 4;
		if(font.IsStrikeout()) bitmask |= 8;
		if(font.IsNonAntiAliased()) bitmask |= 0x10;
		if(font.IsTrueTypeOnly()) bitmask |= 0x20;
		stream.Put(bitmask);
	}
	else {
		int x = stream.Get();
		if(x < 0)
			return;
		if(x == 0xFF) {
			font = Null;
			return;
		}
		font = StdFont();
		if(x == 0xFE) {
			String name;
			VectorSerialize(stream, name);
			if(stream.IsError())
				return;
			font.FaceName(name);
		}
		else if(x < 0x40)
			font.Face(x);
		else {
			stream.SetError();
			return;
		}
		int ht, wd;
		VectorSerializePacked(stream, ht);
		VectorSerializePacked(stream, wd);
		if(stream.IsError())
			return;
		font.Height(ht).Width(wd);
		int bits = stream.Get();
		if(bits < 0) {
			stream.SetError();
			return;
		}
		if(bits & 0x01) font.Bold();
		if(bits & 0x02) font.Italic();
		if(bits & 0x04) font.Underline();
		if(bits & 0x08) font.Strikeout();
		if(bits & 0x10) font.NonAntiAliased();
		if(bits & 0x20) font.TrueTypeOnly();
	}
}

static void VectorSerialize(Stream& stream, Value& value, int vtype)
{
	switch(vtype) {
		case INT_V: {
			int t;
			if(stream.IsStoring()) t = value;
			VectorSerializePacked(stream, t);
			if(stream.IsLoading()) value = t;
			break;
		}
		case DOUBLE_V: {
			double t;
			if(stream.IsStoring()) t = value;
			VectorSerializeFixed(stream, t, 1e6);
			if(stream.IsLoading()) value = t;
			break;
		}
		case STRING_V: {
			String t;
			if(stream.IsStoring()) t = value;
			VectorSerialize(stream, t);
			if(stream.IsLoading()) value = t;
			break;
		}
		case WSTRING_V: {
			String t;
			if(stream.IsStoring()) t = ToUtf8(WString(value));
			VectorSerialize(stream, t);
			if(stream.IsLoading()) value = FromUtf8(t);
			break;
		}
		case DATE_V: case TIME_V: {
			Time t;
			if(stream.IsStoring()) t = value;
			VectorSerialize(stream, t);
			if(stream.IsLoading()) value = t;
			break;
		}
		case COLOR_V: {
			Color t;
			if(stream.IsStoring()) t = value;
			VectorSerialize(stream, t);
			if(stream.IsLoading()) value = t;
			break;
		}
		case POINT_V: {
			Point t;
			if(stream.IsStoring()) t = value;
			VectorSerialize(stream, t);
			if(stream.IsLoading()) value = t;
			break;
		}
		case SIZE_V: {
			Size t;
			if(stream.IsStoring()) t = value;
			VectorSerialize(stream, t);
			if(stream.IsLoading()) value = t;
			break;
		}
		case RECT_V: {
			Rect t;
			if(stream.IsStoring()) t = value;
			VectorSerialize(stream, t);
			if(stream.IsLoading()) value = t;
			break;
		}
		case FONT_V: {
			Font t;
			if(stream.IsStoring()) t = value;
			VectorSerialize(stream, t);
			if(stream.IsLoading()) value = t;
			break;
		}
		default: NEVER();
	}
}


static int GetEllipseDistanceSquared(const Rect& rc, Point pt)
{
	pt -= rc.CenterPoint();
	double a = max(1, rc.Width()) / 2.0;
	double b = max(1, rc.Height()) / 2.0;
	double nx = pt.x / a;
	double ny = pt.y / b;
	if(!nx && !ny)
		nx = 1;
	double hyp = hypot(nx, ny);
	if(hyp < 1)
		return -fround(sqr((1 - hyp) * a * b));
	nx *= a / hyp;
	ny *= b / hyp;
	return fround(sqr(pt.x - nx) + sqr(pt.y - ny));
}

static int GetLineDistanceSquared(Point from, Point to, Point pt)
{
	Size ft = to - from, fp = pt - from;
	if(int tp = Squared(ft)) {
		int scalar = ScalarProduct(fp, ft);
		from += iscale(ft, minmax(scalar, 0, tp), tp);
	}
	Size diff = pt - from;
	return Squared(diff);
}

String FormatVectorFont(Font f)
{
	String out;
	switch(f.GetFace()) {
		case Font::STDFONT:      out << NFormat("StdFont(%d)", f.GetHeight()); break;
		case Font::SCREEN_SANS:  out << NFormat("ScreenSans(%d)", f.GetHeight()); break;
		case Font::SCREEN_SERIF: out << NFormat("ScreenSerif(%d)", f.GetHeight()); break;
		case Font::SCREEN_FIXED: out << NFormat("ScreenFixed(%d)", f.GetHeight()); break;
		case Font::ROMAN:        out << NFormat("Roman(%d)", f.GetHeight()); break;
		case Font::ARIAL:        out << NFormat("Arial(%d)", f.GetHeight()); break;
		case Font::COURIER:      out << NFormat("Courier(%d)", f.GetHeight()); break;
		case Font::SYMBOL:       out << NFormat("Symbol(%d)", f.GetHeight()); break;
	#ifdef PLATFORM_WIN32
		case Font::WINGDINGS:    out << NFormat("Wingdings(%d)", f.GetHeight()); break;
		case Font::TAHOMA:       out << NFormat("Tahoma(%d)", f.GetHeight()); break;
	#endif
		default:                 out << NFormat("Font(0, %d).FaceName(%s)", f.GetHeight(), AsCString(f.GetFaceName())); break;
	}
//	out << ".Height(" << f.GetHeight() << ")";
	if(f.IsBold()) out << ".Bold()";
	if(f.IsItalic()) out << ".Italic()";
	if(f.IsUnderline()) out << ".Underline()";
	if(f.IsStrikeout()) out << ".Strikeout()";
	return out;
}

Font ParseVectorFont(CParser& parser)
{
	Font f = StdFont();
	bool font = false;
	if(parser.Id("StdFont")) ;
	else if(parser.Id("ScreenSans")) f = ScreenSans();
	else if(parser.Id("ScreenSerif")) f = ScreenSerif();
	else if(parser.Id("ScreenFixed")) f = ScreenFixed();
	else if(parser.Id("Roman")) f = Roman(0);
	else if(parser.Id("Arial")) f = Arial(0);
	else if(parser.Id("Courier")) f = Courier(0);
	else if(parser.Id("Symbol")) f = Symbol(0);
#ifdef PLATFORM_WIN32
	else if(parser.Id("Wingdings")) f = WingDings(0);
	else if(parser.Id("Tahoma"))    f = Tahoma(0);
#endif
	else if(parser.Id("Font")) { f = Font(); font = true; }
	else parser.ThrowError("font spec expected");
	parser.PassChar('(');
	if(!parser.Char(')')) {
		if(font) {
			f.Face(parser.ReadInt());
			parser.PassChar(',');
		}
		f.Height(parser.ReadInt());
		parser.PassChar(')');
	}
	while(parser.Char('.')) {
		/**/ if(parser.Id("Height"))      { parser.PassChar('('); f.Height(parser.ReadInt()); parser.PassChar(')'); }
		else if(parser.Id("Bold"))        { f.Bold(); parser.PassChar('('); parser.PassChar(')'); }
		else if(parser.Id("Italic"))      { f.Italic(); parser.PassChar('('); parser.PassChar(')'); }
		else if(parser.Id("Underline"))   { f.Underline(); parser.PassChar('('); parser.PassChar(')'); }
		else if(parser.Id("Strikeout"))   { f.Strikeout(); parser.PassChar('('); parser.PassChar(')'); }
		else if(parser.Id("SetFaceName")) { f.FaceName(parser.ReadOneString()); }
		else parser.ThrowError("unknown font attribute");
	}
	return f;
}

void DrawLineRect(Draw& draw, const Rect& rc, int width, Color color)
{
	Point vertices[] = { rc.TopLeft(), rc.TopRight(), rc.BottomRight(), rc.BottomLeft(), rc.TopLeft() };
	draw.DrawPolyline(vertices, 5, width, color);
}

Rect operator * (Rect rc, const Scaling& sc)
{
	return Rect(rc.TopLeft() * sc, rc.BottomRight() * sc);
}

Rect operator / (Rect rc, const Scaling& sc)
{
	return Rect(rc.TopLeft() / sc, rc.BottomRight() / sc);
}

One<Ctrl> VectorArg::CreateEditor(int vtype, int editor)
{
	switch(editor) {
		case EDIT_VALUE: {
			switch(vtype) {
				case INT_V:     return &(*new EditIntSpin).LeftPos(0, 80).TopPosZ(0, 19);
				case DOUBLE_V:  return &(*new EditDouble).LeftPos(0, 100).TopPosZ(0, 19);
				case STRING_V:  return &(*new EditString).LeftPos(0, 100).TopPosZ(0, 19);
				case WSTRING_V: return &(*new EditString).LeftPos(0, 100).TopPosZ(0, 19);
				case DATE_V:    return &(*new EditDate).LeftPos(0, 80).TopPos(0, 19);
				case TIME_V:    return &(*new EditTime).LeftPos(0, 150).TopPos(0, 19);
				case COLOR_V:   return &(*new ColorPusher)/*.NoNotNull()*/.LeftPos(0, 100).TopPosZ(0, 19);
				case FONT_V:    return &(*-CreateVectorFontCtrl()).LeftPos(0, 150).TopPosZ(0, 40);
				default:        return NULL;
			}
		}

		case EDIT_DOC: {
			return &(*new DocEdit).LeftPosZ(0, 100).TopPosZ(0, 80);
		}

		case EDIT_ALIGN_HORZ: {
			One<DropList> droplist = new DropList;
			droplist->Add(ALIGN_LEFT, "left");
			droplist->Add(ALIGN_CENTER, "center");
			droplist->Add(ALIGN_RIGHT, "right");
			return &(*-droplist).LeftPosZ(0, 100).TopPosZ(0, 19);
		}

		case EDIT_ALIGN_VERT: {
			One<DropList> droplist = new DropList;
			droplist->Add(ALIGN_TOP, "top");
			droplist->Add(ALIGN_CENTER, "center");
			droplist->Add(ALIGN_BOTTOM, "bottom");
			return &(*-droplist).LeftPosZ(0, 100).TopPosZ(0, 19);
		}

		case EDIT_RECT_STYLE: {
			One<DropList> droplist = new DropList;
			droplist->Add(VectorRectData::STYLE_RECT,         "rectangle");
			droplist->Add(VectorRectData::STYLE_ROUNDED_RECT, "rounded rectangle");
			droplist->Add(VectorRectData::STYLE_ELLIPSE,      "ellipse");
			return &(*-droplist).LeftPosZ(0, 150).TopPosZ(0, 19);
		}

		case EDIT_LINE_CAP: {
			One<DropList> droplist = new DropList;
			droplist->SetDisplay(VectorLineCapDisplay());
			for(int i = 0; i < VectorLineData::CAP_COUNT; i++)
				droplist->Add(i);
			return &(*-droplist).LeftPosZ(150).TopPosZ(0, 19);
		}

		default: NEVER(); return NULL;
	}
}

VectorInfo::VectorInfo(String type, String vm_type, One<VectorObject> (*newfunc)(),
	One< VectorGeomArg<Rect> > rect_args_, One< VectorGeomArg<Point> > point_args_, Array<VectorArg> args_)
: type(type), vm_type(vm_type), newfunc(newfunc), rects(rect_args_), points(point_args_), args(args_)
{
	vm_index.Reserve(args.GetCount());
	GetFieldContainer(vm_index, args, &VectorArg::vm_name);
}

int VectorInfo::FindArg(String name) const
{
	return FindFieldIndex(args, &VectorArg::name, name);
}

String VectorInfo::GetName(int lang) const
{
	int f = name.Find(lang);
	return (f >= 0 ? name[f] : Nvl(default_name, type));
}

String VectorObject::FormatCoords() const
{
	String out;
	const VectorInfo& info = Info();
	if(const VectorGeomArg<Rect> *ra = info.RectArgs()) {
		for(int i = 0, n = ra->GetCount(this); i < n; i++)
			out << (out.IsEmpty() ? "" : "; ") << ra->Get(this, i);
	}
	if(const VectorGeomArg<Point> *pa = info.PointArgs()) {
		for(int i = 0, n = pa->GetCount(this); i < n; i++)
			out << (out.IsEmpty() ? "" : "; ") << pa->Get(this, i);
	}
	return out;
}

void VectorObject::DesignPaint(ScalingDraw& draw) const
{
	Paint(draw);
}

void VectorObject::DragPaint(ScalingDraw& draw) const
{
	Paint(draw);
}

void VectorObject::SelPaint(ScalingDraw& draw, const Index<int>& sel, bool is_hot) const
{
	const VectorInfo& vi = Info();
	Color c = (is_hot ? LtMagenta() : LtBlue());
	Vector<bool> mrc, mpt;
	for(int i = 0; i < sel.GetCount(); i++)
		if(sel[i] & ARG_MASK)
			if(sel[i] & RECT_MASK)
				mrc.At(sel[i] >> INDEX_SHIFT, false) = true;
			else
				mpt.At(sel[i] >> INDEX_SHIFT, false) = true;
	if(const VectorGeomArg<Rect> *ra = vi.RectArgs()) {
		for(int i = 0, n = ra->GetCount(this); i < n; i++) {
			Rect rc = ra->Get(this, i) * draw;
			Point mid = rc.CenterPoint();
			bool hot = (i < mrc.GetCount() && mrc[i]);
			DrawHotRect(*draw.draw, rc.left - 5, rc.top - 5, 5, 5, hot, c);
			DrawHotRect(*draw.draw, mid.x - 2, rc.top - 5, 5, 5, hot, c);
			DrawHotRect(*draw.draw, rc.right, rc.top - 5, 5, 5, hot, c);
			DrawHotRect(*draw.draw, rc.left - 5, mid.y - 2, 5, 5, hot, c);
			DrawHotRect(*draw.draw, rc.right, mid.y - 2, 5, 5, hot, c);
			DrawHotRect(*draw.draw, rc.left - 5, rc.bottom, 5, 5, hot, c);
			DrawHotRect(*draw.draw, mid.x - 2, rc.bottom, 5, 5, hot, c);
			DrawHotRect(*draw.draw, rc.right, rc.bottom, 5, 5, hot, c);
		}
	}
	if(const VectorGeomArg<Point> *pa = vi.PointArgs()) {
		for(int i = 0, n = pa->GetCount(this); i < n; i++) {
			Point pt = pa->Get(this, i) * draw;
			bool hot = (i < mpt.GetCount() && mpt[i]);
			DrawHotRect(*draw.draw, pt.x - 2, pt.y - 2, 5, 5, hot, c);
		}
	}
}

Rect VectorObject::GetBox() const
{
	const VectorInfo& vi = Info();
	Rect box = Null;
	if(const VectorGeomArg<Rect> *ra = vi.RectArgs())
		for(int i = 0; i < ra->GetCount(this); i++)
			box |= ra->Get(this, i);
	if(const VectorGeomArg<Point> *pa = vi.PointArgs())
		for(int i = 0; i < pa->GetCount(this); i++)
			box |= pa->Get(this, i);
	return box;
}

One<VectorObject> VectorObject::Copy() const
{
	const VectorInfo& vi = Info();
	One<VectorObject> newobj = vi.New();
	if(const VectorGeomArg<Rect> *ra = vi.RectArgs()) {
		int count = ra->GetCount(this);
		if(ra->CanSetCount())
			ra->SetCount(~newobj, count);
		for(int i = 0; i < count; i++)
			ra->Set(~newobj, i, ra->Get(this, i));
	}
	if(const VectorGeomArg<Point> *pa = vi.PointArgs()) {
		int count = pa->GetCount(this);
		if(pa->CanSetCount())
			pa->SetCount(~newobj, count);
		for(int i = 0; i < count; i++)
			pa->Set(~newobj, i, pa->Get(this, i));
	}
	for(int i = 0; i < vi.GetCount(); i++)
		vi[i].Set(~newobj, vi[i].Get(this));
	return newobj;
}

Index<int> VectorObject::Contains(const Scaling& scaling, Rect rc) const
{
	const VectorInfo& vi = Info();
	Index<int> out;
	if(const VectorGeomArg<Rect> *ra = vi.RectArgs())
		for(int i = 0, n = ra->GetCount(this); i < n; i++)
			if(rc.Contains(ra->Get(this, i) * scaling))
				out.Add((i << INDEX_SHIFT) | RECT_MASK | ARG_MASK | TRACK_RECT);
	if(const VectorGeomArg<Point> *pa = vi.PointArgs())
		for(int i = 0, n = pa->GetCount(this); i < n; i++)
			if(rc.Contains(pa->Get(this, i) * scaling))
				out.Add((i << INDEX_SHIFT) | ARG_MASK | TRACK_POINT);
	return out;
}

int VectorObject::Nearest(const Scaling& scaling, Point pt, int& maxdistsquared) const
{
	const VectorInfo& vi = Info();
	int nearest = -1;
	if(const VectorGeomArg<Rect> *ra = vi.RectArgs())
		for(int i = 0, n = ra->GetCount(this); i < n; i++) {
			Rect u = ra->Get(this, i) * scaling;
			if(IsNull(u))
				continue;
			int s2 = min(min(pt.x - u.left, u.right - pt.x), min(pt.y - u.top, u.bottom - pt.y));
			if(s2 >= 0)
				s2 = -s2 * s2;
			else {
				Size d = u.Bind(pt) - pt;
				s2 = d.cx * d.cx + d.cy * d.cy;
			}
			if(tabs(s2) < tabs(maxdistsquared)) {
				nearest = (i << INDEX_SHIFT) | ARG_MASK | RECT_MASK | TRACK_RECT;
				maxdistsquared = s2;
			}
		}

	if(const VectorGeomArg<Point> *pa = vi.PointArgs())
		for(int i = 0, n = pa->GetCount(this); i < n; i++) {
			Point u = pa->Get(this, i) * scaling;
			Size d = u - pt;
			int s2 = d.cx * d.cx + d.cy * d.cy;
			if(s2 < tabs(maxdistsquared)) {
				nearest = (i << INDEX_SHIFT) | ARG_MASK | TRACK_POINT;
				maxdistsquared = s2;
			}
		}

	return nearest;
}

Point VectorObject::TrackPoint(int track) const
{
	const VectorInfo& vi = Info();
	if(track & ARG_MASK)
		if(track & RECT_MASK) {
			Rect rc = vi.RectArgs()->Get(this, track >> INDEX_SHIFT);
			Point pt = rc.TopLeft();
			if(~(track & (TRACK_MASK & ~TRACK_RECT_SIDES))) {
				if((track & (TRACK_RECT_LEFT | TRACK_RECT_RIGHT)) == TRACK_RECT_RIGHT)
					pt.x = rc.right;
				if((track & (TRACK_RECT_TOP | TRACK_RECT_BOTTOM)) == TRACK_RECT_BOTTOM)
					pt.y = rc.bottom;
			}
			return pt;
		}
		else
			return vi.PointArgs()->Get(this, track >> INDEX_SHIFT);
	return Null;
}

void VectorObject::Create(Point from, Point to, dword keyflags)
{
	const VectorInfo& vi = Info();
	Point rest = (from + to) >> 1;
	Rect rc(from, to);
	rc.Normalize();
	bool is_rc = false;
	if(const VectorGeomArg<Rect> *ra = vi.RectArgs()) {
		is_rc = ra->GetCount(this) > 0;
		for(int i = 0, n = ra->GetCount(this); i < n; i++)
			ra->Set(this, i, rc);
	}
	if(const VectorGeomArg<Point> *pa = vi.PointArgs()) {
		for(int i = 0, n = pa->GetCount(this); i < n; i++)
			pa->Set(this, i, is_rc ? rest : n == 1 ? to : i == 0 ? from : i == 1 ? to : rest);
	}
}

void VectorObject::Track(int track, Point from, Point to, dword keyflags)
{
	Size delta = to - from;
	const VectorInfo& vi = Info();
	int tm = track & TRACK_MASK;
	if(const VectorGeomArg<Rect> *ra = vi.RectArgs()) {
		if((tm & ~TRACK_RECT_SIDES) == 0) { // sides
			for(int i = 0, n = ra->GetCount(this); i < n; i++) {
				Rect rc = ra->Get(this, i);
				if(track & TRACK_RECT_LEFT)   rc.left   += delta.cx;
				if(track & TRACK_RECT_TOP)    rc.top    += delta.cy;
				if(track & TRACK_RECT_RIGHT)  rc.right  += delta.cx;
				if(track & TRACK_RECT_BOTTOM) rc.bottom += delta.cy;
				rc.Normalize();
				ra->Set(this, i, rc);
			}
		}
		else if(tm == TRACK_MOVE) {
			for(int i = 0, n = ra->GetCount(this); i < n; i++)
				ra->Set(this, i, ra->Get(this, i) + delta);
		}
		else if(tm == TRACK_SIZE) {
			for(int i = 0, n = ra->GetCount(this); i < n; i++)
				ra->Set(this, i, ra->Get(this, i).Inflated(Rect(delta)).Normalized());
		}
	}

	if(const VectorGeomArg<Point> *pa = vi.PointArgs()) {
		if(tm == TRACK_POINT && track & ARG_MASK) {
			int i = track >> INDEX_SHIFT;
			pa->Set(this, i, pa->Get(this, i) + delta);
		}
		else if(tm == TRACK_MOVE)
			for(int i = 0, n = pa->GetCount(this); i < n; i++)
				pa->Set(this, i, pa->Get(this, i) + delta);

	}
}

Size GetAlignOffset(const Rect& rc, Alignment halign, Alignment valign, Point pt)
{
	Point a = pt;
	switch(halign) {
		case ALIGN_LEFT:   a.x = rc.left; break;
		case ALIGN_CENTER: a.x = (rc.left + rc.right) >> 1; break;
		case ALIGN_RIGHT:  a.x = rc.right; break;
	}
	switch(valign) {
		case ALIGN_TOP:    a.y = rc.top; break;
		case ALIGN_CENTER: a.y = (rc.top + rc.bottom) >> 1; break;
		case ALIGN_BOTTOM: a.y = rc.bottom; break;
	}
	return pt - a;
}

void VectorObject::Align(Alignment halign, Alignment valign, Point anchor)
{
	const VectorInfo& vi = Info();
	if(const VectorGeomArg<Rect> *ra = vi.RectArgs())
		for(int i = 0; i < ra->GetCount(this); i++) {
			Rect rc = ra->Get(this, i);
			rc.Offset(GetAlignOffset(rc, halign, valign, anchor));
			ra->Set(this, i, rc);
		}
	if(const VectorGeomArg<Point> *pa = vi.PointArgs()) {
		Rect rc = Null;
		for(int i = 0; i < pa->GetCount(this); i++)
			rc |= pa->Get(this, i);
		Size delta = GetAlignOffset(rc, halign, valign, anchor);
		for(int i = 0; i < pa->GetCount(this); i++)
			pa->Set(this, i, pa->Get(this, i) + delta);
	}
}

void VectorObject::EqualSize(Size sz)
{
	const VectorInfo& vi = Info();
	if(const VectorGeomArg<Rect> * ra = vi.RectArgs())
		for(int i = 0; i < ra->GetCount(this); i++) {
			Rect rc = ra->Get(this, i);
			if(!IsNull(sz.cx))
				rc.right = rc.left + sz.cx;
			if(!IsNull(sz.cy))
				rc.bottom = rc.top + sz.cy;
			ra->Set(this, i, rc);
		}
}

Image VectorObject::Cursor(int track, dword keyflags) const
{
	if(!((track & TRACK_MASK) & ~TRACK_RECT_SIDES)) {
		switch(track & TRACK_RECT_SIDES) {
			case TRACK_RECT_LEFT:
			case TRACK_RECT_RIGHT:
				return CtrlImg::SizeHorz0();
			case TRACK_RECT_TOP:
			case TRACK_RECT_BOTTOM:
				return CtrlImg::SizeVert0();
			case TRACK_RECT_LEFT | TRACK_RECT_TOP:
			case TRACK_RECT_RIGHT | TRACK_RECT_BOTTOM:
				return CtrlImg::SizeHoVe0();
			case TRACK_RECT_LEFT | TRACK_RECT_BOTTOM:
			case TRACK_RECT_RIGHT | TRACK_RECT_TOP:
				return CtrlImg::SizeVeHo0();
		}
	}

	switch(track & TRACK_MASK) {
		case TRACK_SELECT: return CtrlImg::HandCursor();
		case TRACK_MOVE:   return VecImg::SizeAll();
		case TRACK_POINT:  return VecImg::track_point();
	}

	return Null;
}

Value VectorObject::GetLink(Point pt, Size sz) const
{
	const VectorInfo& vi = Info();
	int l = vi.FindArg("link");
	if(l >= 0)
		return vi[l].Get(this);
	return Null;
}

String VectorObject::ToString() const
{
	String out;
	const VectorInfo& vi = Info();
	out << vi.GetType() << ", ";
	if(const VectorGeomArg<Rect> *ra = vi.RectArgs()) {
		int n = ra->GetCount(this);
		if(n) {
			out << ".SetRect(";
			bool sc = ra->CanSetCount();
			if(sc)
				out << "Vector<Rect>()";
			for(int i = 0; i < n; i++) {
				Rect rc = ra->Get(this, i);
				out << NFormat("%s`Rect(%d, %d, %d, %d)",
					sc ? " << " : i ? ", " : "", rc.left, rc.top, rc.right, rc.bottom);
			}
			out << ')';
		}
	}
	if(const VectorGeomArg<Point> *pa = vi.PointArgs()) {
		int n = pa->GetCount(this);
		if(n) {
			out << ".SetPoint(";
			bool sc = pa->CanSetCount();
			if(sc)
				out << "Vector<Point>()";
			for(int i = 0; i < n; i++) {
				Point pt = pa->Get(this, i);
				out << NFormat("%s`Point(%d, %d)",
					sc ? " << " : i ? ", " : "", pt.x, pt.y);
			}
			out << ")";
		}
	}
	int npr = 0;
	for(int i = vi.GetCount(); --i >= 0;) {
		const VectorArg& viarg = vi[i];
		Value v = viarg.Get(this);
		bool usearg = false;
		if(viarg.dflt != v) {
			String fmtarg;
			if(IsNull(v))
				fmtarg = "Null";
			else if(v.GetType() == COLOR_V) {
				Color c(v);
				fmtarg = NFormat("Color(%d, %d, %d)", c.GetR(), c.GetG(), c.GetB());
			}
			else if(v.GetType() == STRING_V || v.GetType() == WSTRING_V)
				fmtarg = AsCString(String(v));
			else if(v.GetType() == FONT_V)
				fmtarg = FormatVectorFont(v);
			else
				fmtarg = StdFormat(v);
			out << '.' << viarg.name << '(' << fmtarg << ')';
		}
	}
	return out;
}

One<VectorObject> VectorObject::Load(CParser& parser)
{
	String type = parser.ReadId();
	parser.PassChar(',');
	const VectorInfo *vi = RegisteredObjects().Get(type, NULL);
	if(!vi)
		return NULL;
	One<VectorObject> obj = vi->New();
	for(int i = 0; i < vi->GetCount(); i++)
		(*vi)[i].Set(~obj, (*vi)[i].dflt);
	while(parser.Char('.')) {
		String id = parser.ReadId();
		if(id == "SetPoint") {
			if(const VectorGeomArg<Point> *pa = vi->PointArgs()) {
				Vector<Point> pts;
				parser.PassChar('(');
				bool lsh = false;
				bool first = true;
				if(parser.Id("Vector")) {
					parser.PassChar('<');
					parser.PassId("Point");
					parser.PassChar('>');
					parser.PassChar('(');
					parser.PassChar(')');
					lsh = true;
				}
				while(!parser.Char(')')) {
					if(lsh)
						parser.PassChar2('<', '<');
					else if(!first)
						parser.PassChar(',');
					first = false;
					Point pt;
					parser.PassId("Point");
					parser.PassChar('(');
					pt.x = parser.ReadInt();
					parser.PassChar(',');
					pt.y = parser.ReadInt();
					parser.PassChar(')');
					pts.Add(pt);
				}
				if(pts.GetCount() < pa->GetCount(~obj))
					pts.SetCountR(pa->GetCount(~obj), Point(0, 0));
				if(pa->CanSetCount())
					pa->SetCount(~obj, pts.GetCount());
				for(int i = 0, n = pa->GetCount(~obj); i < n; i++)
					pa->Set(~obj, i, pts[i]);
			}
		}
		else if(id == "SetRect") {
			if(const VectorGeomArg<Rect> *ra = vi->RectArgs()) {
				Vector<Rect> rcs;
				parser.PassChar('(');
				bool lsh = false;
				bool first = true;
				if(parser.Id("Vector")) {
					parser.PassChar('<');
					parser.PassId("Rect");
					parser.PassChar('>');
					parser.PassChar('(');
					parser.PassChar(')');
					lsh = true;
				}
				while(!parser.Char(')')) {
					if(lsh)
						parser.PassChar2('<', '<');
					else if(!first)
						parser.PassChar(',');
					first = false;
					Rect rc;
					parser.PassId("Rect");
					parser.PassChar('(');
					rc.left = parser.ReadInt();
					parser.PassChar(',');
					rc.top = parser.ReadInt();
					parser.PassChar(',');
					rc.right = parser.ReadInt();
					parser.PassChar(',');
					rc.bottom = parser.ReadInt();
					parser.PassChar(')');
					rcs.Add(rc);
				}
				if(rcs.GetCount() < ra->GetCount(~obj))
					rcs.SetCountR(ra->GetCount(~obj), Rect(0, 0, 0, 0));
				if(ra->CanSetCount())
					ra->SetCount(~obj, rcs.GetCount());
				for(int i = 0, n = ra->GetCount(~obj); i < n; i++)
					ra->Set(~obj, i, rcs[i]);
			}
		}
		else {
			int t = vi->FindArg(id);
			parser.PassChar('(');
			if(t >= 0) {
				const VectorArg& viarg = (*vi)[t];
				Value value;
				if(!parser.Id("Null"))
					switch(viarg.vtype) {
						case INT_V: {
							value = parser.ReadInt();
							break;
						}
						case DOUBLE_V: {
							value = parser.ReadDouble();
							break;
						}
						case STRING_V:
						case WSTRING_V: {
							value = parser.ReadString();
							break;
						}
						case COLOR_V: {
							Color c = Null;
							parser.PassId("Color");
							parser.PassChar('(');
							if(!parser.IsChar(')')) {
								int r = parser.ReadInt();
								parser.PassChar(',');
								int g = parser.ReadInt();
								parser.PassChar(',');
								int b = parser.ReadInt();
								c = Color(r, g, b);
							}
							parser.PassChar(')');
							value = c;
							break;
						}

						case FONT_V: {
							value = ParseVectorFont(parser);
							break;
						}

						default: NEVER();
					}
				viarg.Set(~obj, value);
			}
			parser.PassChar(')');
		}
	}
	return obj;
}

typedef VectorMap<String, const VectorInfo *> VectorEntryMap;

GLOBAL_VAR(VectorEntryMap, VectorObject::RegisteredObjects)
GLOBAL_VAR(VectorEntryMap, VectorObject::RegisteredVMIndex)

void VectorObject::RegisterInfo(const VectorInfo& vi)
{
	RegisteredObjects().GetAdd(vi.GetType()) = &vi;
	RegisteredVMIndex().GetAdd(vi.GetVMType()) = &vi;
}

VectorImage::VectorImage()
: data(new Data)
{
}

void VectorImage::Clone()
{
	VectorImage old_image = *this;
	data = new Data;
	data->size = old_image.data->size;
	data->name = old_image.data->name;
	Reserve(old_image.GetCount());
	for(int i = 0; i < old_image.GetCount(); i++)
		Add(old_image[i].Copy());
}

VectorImage::VectorImage(const Value& value)
{
	if(IsTypeRaw<VectorImage>(value))
		*this = ValueTo<VectorImage>(value);
	else if(IsNull(value))
		data = new Data;
	else {
		NEVER();
	}
}

void VectorImage::Paint(ScalingDraw& draw) const
{
	for(int i = 0; i < data->objects.GetCount(); i++)
		data->objects[i].Paint(draw);
}

void VectorImage::DesignPaint(ScalingDraw& draw) const
{
	for(int i = 0; i < data->objects.GetCount(); i++)
		data->objects[i].DesignPaint(draw);
}

void VectorImage::DragPaint(ScalingDraw& draw) const
{
	for(int i = 0; i < data->objects.GetCount(); i++)
		data->objects[i].DragPaint(draw);
}

VectorMap< int, Index<int> > VectorImage::Contains(const Scaling& scaling, Rect rc) const
{
	VectorMap< int, Index<int> > list;
	for(int pass = 0; list.IsEmpty() && pass <= 1; pass++)
		for(int i = 0; i < data->objects.GetCount(); i++)
			if(pass || rc.Contains(data->objects[i].GetBox() * scaling)) {
				Index<int> sel = data->objects[i].Contains(scaling, rc);
				if(!sel.IsEmpty())
					list.Add(i) = sel;
			}
	return list;
}

Point VectorImage::Nearest(const Scaling& scaling, Point pt, int maxdistsquared) const
{
	int nobj = -1, ntrack = -1;
	int mx = 1000000000;
	for(int i = 0; i < data->objects.GetCount(); i++) {
		int sel = data->objects[i].Nearest(scaling, pt, mx);
		if(sel >= 0) {
			nobj = i;
			ntrack = sel;
		}
	}
	if(mx > maxdistsquared)
		nobj = ntrack = -1;
	return Point(nobj, ntrack);
}

Point VectorImage::Nearest(const Scaling& scaling, Point pt, int maxdistsquared, const Vector<int>& selection) const
{
	int nobj = -1, ntrack = -1;
	int mx = 1000000000;
	for(int i = 0; i < selection.GetCount(); i++) {
		int sel = data->objects[selection[i]].Nearest(scaling, pt, mx);
		if(sel >= 0) {
			nobj = selection[i];
			ntrack = sel;
		}
	}
	if(mx > maxdistsquared)
		nobj = ntrack = -1;
	return Point(nobj, ntrack);
}

Point VectorImage::TrackPoint(Point track) const
{
	if(track.x < 0 || track.x >= data->objects.GetCount())
		return Null;
	return data->objects[track.x].TrackPoint(track.y);
}

Image VectorImage::Cursor(Point track, dword keyflags) const
{
	if(track.y < 0)
		return Null;
	if(track.x >= 0)
		return data->objects[track.x].Cursor(track.y, keyflags);
	switch((track.y & VectorObject::TRACK_MASK)) {
		case VectorObject::TRACK_IMAGE_SIZE: {
			switch(track.y >> VectorObject::INDEX_SHIFT) {
				case 1: return CtrlImg::SizeHorz0();
				case 2: return CtrlImg::SizeVert0();
				case 3: return CtrlImg::SizeHoVe0();
			}
			break;
		}
	}
	return Null;
}

/*
Point VectorImage::FindNearest(Point pt, int maxdistsquared, const VectorMap<int, int>& selection) const
{
	int best = -1, sel = 0, dist = 1000000000;
	for(int i = 0; i < selection.GetCount(); i++) {
		int x = selection.GetKey(i);
		int temp = dist;
		int osel = data->objects[x].NearestMask(pt, temp) & selection[i];
		if(osel && temp <= dist) {
			best = x;
			sel = osel;
			dist = temp;
		}
	}
	if(dist > maxdistsquared)
		return Point(-1, 0);
	return Point(best, sel);
}
*/

String VectorImage::SaveVec(int index) const
{
	String out;
	String fn = data->name;
	if(IsNull(fn))
		fn = NFormat("vi__%d", index);
	out << NFormat("VECTOR_BEGIN(%s, %d, %d)\n", fn, data->size.cx, data->size.cy);
	for(int i = 0; i < data->objects.GetCount(); i++)
		out << "\tVECTOR_OBJ(" << data->objects[i] << ")\n";
	out << "VECTOR_END\n";
	return out;
}

void VectorImage::LoadVec(CParser& parser)
{
	Clear();
	parser.PassId("VECTOR_BEGIN");
	parser.PassChar('(');
	data->name = parser.ReadId();
	parser.PassChar(',');
	data->size.cx = parser.ReadInt();
	parser.PassChar(',');
	data->size.cy = parser.ReadInt();
	parser.PassChar(')');
	while(parser.Id("VECTOR_OBJ")) {
		parser.PassChar('(');
		One<VectorObject> object = VectorObject::Load(parser);
		parser.PassChar(')');
		if(object)
			Add(object);
	}
	parser.PassId("VECTOR_END");
}

void VectorImage::SaveVM(Stream& out) const
{
	out.Put("VIMa");
	out.Put("n");
	String n = data->name;
	VectorSerialize(out, n);
	out.Put("s");
	Size s = data->size;
	VectorSerialize(out, s);
	out.Put("o");
	int count = data->objects.GetCount();
	VectorSerializePacked(out, count);
	for(int i = 0; i < data->objects.GetCount(); i++) {
		const VectorObject& obj = data->objects[i];
		const VectorInfo& info = obj.Info();
		StringStream data;
		data.Put(info.vm_type);
		if(const VectorGeomArg<Rect> *ra = info.RectArgs()) {
			int count = ra->GetCount(&obj);
			if(ra->CanSetCount())
				VectorSerializePacked(data, count);
			for(int i = 0; i < count; i++) {
				Rect rc = ra->Get(&obj, i);
				VectorSerialize(data, rc);
			}
		}
		if(const VectorGeomArg<Point> *pa = info.PointArgs()) {
			int count = pa->GetCount(&obj);
			if(pa->CanSetCount())
				VectorSerializePacked(data, count);
			for(int i = 0; i < count; i++) {
				Point pt = pa->Get(&obj, i);
				VectorSerialize(data, pt);
			}
		}
		for(int a = 0; a < info.GetCount(); a++) {
			const VectorArg& arg = info[a];
			Value v = arg.Get(&obj);
			if(v != arg.dflt) {
				data.Put(arg.vm_name);
				VectorSerialize(data, v, arg.vtype);
			}
		}
		String res = data.GetResult();
		VectorSerialize(out, res);
	}
	out.Put(".");
}

static String VMLoadTag(Stream& in)
{
	String out;
	for(;;) {
		int i = in.Get();
		if(i < 0)
			break;
		out.Cat(i);
		if(i < 'A' || i > 'Z')
			break;
	}
	return out;
}

void VectorImage::LoadVM(Stream& in)
{
	Clear();
	char tag[4];
	if(!in.GetAll(tag, 4) || memcmp(tag, "VIMa", 4)) {
		in.SetError();
		return;
	}
	for(;;) {
		int i = in.Get();
		if(i < 0 || i == '.')
			return;
		if(i == 'n')
			VectorSerialize(in, data->name);
		else if(i == 's')
			VectorSerialize(in, data->size);
		else if(i == 'o') {
			int c;
			VectorSerializePacked(in, c);
			Reserve(c);
			for(int i = 0; i < c; i++) {
				String odata;
				VectorSerialize(in, odata);
				StringStream indata(odata);
				String argtag = VMLoadTag(indata);
				if(const VectorInfo *info = VectorObject::RegisteredVMIndex().Get(argtag, NULL)) {
					One<VectorObject> object = info->New();
					if(const VectorGeomArg<Rect> *ra = info->RectArgs()) {
						int count = ra->GetCount(~object);
						if(ra->CanSetCount()) {
							VectorSerializePacked(indata, count);
							if(count >= 0 && count <= 100000)
								ra->SetCount(~object, count);
							else
								indata.SetError();
						}
						int i = 0;
						while(i < count) {
							Rect rc;
							VectorSerialize(indata, rc);
							if(indata.IsError())
								break;
							ra->Set(~object, i++, rc);
						}
						while(i < count)
							ra->Set(~object, i++, Rect(0, 0, 0, 0));
					}
					if(const VectorGeomArg<Point> *pa = info->PointArgs()) {
						int count = pa->GetCount(~object);
						if(pa->CanSetCount()) {
							VectorSerializePacked(indata, count);
							if(count >= 0 && count <= 100000)
								pa->SetCount(~object, count);
							else
								indata.SetError();
						}
						int i = 0;
						while(i < count) {
							Point pt;
							VectorSerialize(indata, pt);
							if(indata.IsError())
								break;
							pa->Set(~object, i++, pt);
						}
						while(i < count)
							pa->Set(~object, i++, Point(0, 0));
					}
					for(int a = 0; a < info->GetCount(); a++) {
						const VectorArg& arg = (*info)[a];
						arg.Set(~object, arg.dflt);
					}
					while(!indata.IsEof() && !indata.IsError()) {
						argtag = VMLoadTag(indata);
						int f = info->FindVMArg(argtag);
						if(f >= 0) {
							Value v;
							VectorSerialize(indata, v, (*info)[f].vtype);
							(*info)[f].Set(~object, v);
						}
						else
							indata.SetError();
					}
					if(!indata.IsError())
						Add(object);
				}
			}
		}
	}
}

void VectorImage::SerializeVM(Stream& stream)
{
	if(stream.IsStoring())
		SaveVM(stream);
	else
		LoadVM(stream);
}

Value VectorImage::GetLink(Point pt, Size sz) const
{
	if(sz.cx > 1 && sz.cy > 1) {
		VectorMap< int, Index<int> > cont = Contains(Scaling(), Rect(pt, sz));
		for(int i = 0; i < cont.GetCount(); i++) {
			Value v = data->objects[cont.GetKey(i)].GetLink(pt, sz);
			if(!IsNull(v))
				return v;
		}
	}
	return Value();
}

void VectorLineData::PaintCap(ScalingDraw& draw, int cap, int size, Color c, Point from, Point to) const
{
	Size dir = to - from;
	double len = Length(dir);
	if(!len)
		return;
	switch(cap) {
		case CAP_ARROW: case CAP_THIN_ARROW: {
			Point shift = from + iscale(dir, size, fceil(len));
			Size dif = iscale(Size(dir.cy, -dir.cx), size >> (cap == CAP_ARROW ? 1 : 2), fceil(len));
			Point poly[3] = { from, shift + dif, shift - dif };
			draw.draw->DrawPolygon(poly, 3, c, Null, Null);
			break;
		}
		case CAP_CIRCLE: {
			int r = size >> 1;
			Rect rc(from + iscale(dir, r, fround(Length(dir))), Size(1, 1));
			rc.Inflate(r);
			draw.draw->DrawEllipse(rc, c, Null, Null);
			break;
		}
		case CAP_END_CIRCLE: {
			Rect rc(from, Size(1, 1));
			rc.Inflate(size >> 1, size >> 1);
			draw.draw->DrawEllipse(rc, c, Null, Null);
			break;
		}
	}
}

Point VectorLineData::ShrinkCap(ScalingDraw& draw, int cap, int size, Point from, Point to) const
{
	double len = Length(from - to);
	if(!len)
		return from;
	switch(cap) {
		case CAP_CIRCLE: {
			return from + iscale(to - from, size, fceil(2 * len));
		}

		case CAP_ARROW: case CAP_THIN_ARROW: {
			return from + iscale(to - from, size, fceil(len));
		}
	}
	return from;
}

void VectorLineData::Paint(ScalingDraw& draw) const
{
	if(points.GetCount() < 2)
		return;
	Vector<Point> out;
	out.SetCount(points.GetCount());
	for(int i = 0; i < points.GetCount(); i++)
		out[i] = points[i] * draw;
	Point from = out[0], to = out.Top();
	int cfrom = draw.X(begin_size), cto = draw.X(end_size);
	out[0] = ShrinkCap(draw, begin_cap, cfrom, from, out[1]);
	out.Top() = ShrinkCap(draw, end_cap, cto, to, out[out.GetCount() - 2]);
	if(out.GetCount() == 2)
		draw.draw->DrawLine(out[0], out[1], line_width, line_color);
	else
		draw.draw->DrawPolyline(out, line_width, line_color);
	if(begin_cap)
		PaintCap(draw, begin_cap, cfrom, begin_color, from, out[1]);
	if(end_cap)
		PaintCap(draw, end_cap, cto, end_color, to, out[out.GetCount() - 2]);
}


class VectorLineCapDisplayCls : public Display {
public:
	virtual void Paint(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const;
};

void VectorLineCapDisplayCls::Paint(Draw& draw, const Rect& rc, const Value& v, Color ink, Color paper, dword style) const
{
	draw.DrawRect(rc, paper);
	Size size = rc.Size();
	int sz = min(size.cx, size.cy);
	Point p1(size.cy >> 1, size.cy >> 1);
	Point p2(size.cx - (size.cy >> 1), size.cy >> 1);
	draw.Clipoff(rc);
	ScalingDraw sdraw(&draw);
	VectorLineData data;
	data.SetPoint(Vector<Point>() << p1 << p2).LineWidth(2).LineColor(ink)
		.BeginCap(v).BeginSize(sz).BeginColor(ink).Paint(sdraw);
	draw.End();
}

const Display& VectorLineCapDisplay()
{
	static VectorLineCapDisplayCls d;
	return d;
}

const VectorInfo& VectorLine::ClassInfo()
{
	static VectorInfo i = VectorInfoFor<VectorLine>(
		"VectorLine", "l",
		NULL,
		VectorGeomArgDef(&VectorLine::points),
		Array<VectorArg>()
		<< VectorArgDef(&VectorLine::line_color,  "LineColor",    "l",  Black())
		<< VectorArgDef(&VectorLine::line_width,  "LineWidth",    "w",  0)
		<< VectorArgDef(&VectorLine::begin_cap,   "BeginCap",     "b",  CAP_NONE, VectorArg::EDIT_LINE_CAP)
		<< VectorArgDef(&VectorLine::begin_size,  "BeginSize",    "s",  10)
		<< VectorArgDef(&VectorLine::begin_color, "BeginColor",   "m",  Black())
		<< VectorArgDef(&VectorLine::end_cap,     "EndCap",       "e",  CAP_NONE, VectorArg::EDIT_LINE_CAP)
		<< VectorArgDef(&VectorLine::end_size,    "EndSize",      "t",  10)
		<< VectorArgDef(&VectorLine::end_color,   "EndColor",     "n",  Black())
		<< VectorArgDef(&VectorLine::link,        "Link",         "z",  Null)
	).DefaultName("Line")
	.Icon(VecImg::line_object);
	return i;
}

void VectorLine::Track(int track, Point start, Point end, dword keyflags)
{
	switch(track & TRACK_MASK) {
		case TRACK_LINE_INSERT: {
			int i = minmax(track >> INDEX_SHIFT, 0, points.GetCount());
			points.Insert(i, end);
			break;
		}
		case TRACK_LINE_REMOVE: {
			int i = minmax(track >> INDEX_SHIFT, 0, points.GetCount() - 1);
			if(points.GetCount() > 2 && i >= 0)
				points.Remove(i);
			break;
		}
		default: VectorObject::Track(track, start, end, keyflags);
	}
}

void VectorLine::DragPaint(ScalingDraw& draw) const
{
	if(points.GetCount() < 2)
		return;
	Vector<Point> out;
	out.SetCount(points.GetCount());
	for(int i = 0; i < points.GetCount(); i++)
		out[i] = points[i] * draw;
	if(out.GetCount() == 2)
		draw.draw->DrawLine(out[0], out[1], 0, Black());
	else
		draw.draw->DrawPolyline(out, 0, Black());
}

int VectorLine::Nearest(const Scaling& scaling, Point pt, int& maxdistsquared) const
{
	int s2, track = -1, md = 10000;
	for(int i = 0; i < points.GetCount(); i++)
		if((s2 = Squared(pt - points[i] * scaling)) < tabs(maxdistsquared)) {
			maxdistsquared = s2;
			md = s2;
			track = (i << INDEX_SHIFT) | TRACK_POINT | ARG_MASK;
		}
	if(md <= 100)
		return track;
	for(int i = 1; i < points.GetCount(); i++)
		if((s2 = GetLineDistanceSquared(points[i - 1] * scaling, points[i] * scaling, pt) + 10) < tabs(maxdistsquared)) {
			maxdistsquared = s2;
			track = TRACK_MOVE;
		}
	for(int i = 0; i <= points.GetCount(); i++) {
		Point addpt, rmpt;
		GetAddRmPoint(scaling, i, addpt, rmpt);
		int s2;
		if(!IsNull(addpt) && (s2 = Squared(addpt - pt)) < tabs(maxdistsquared)) {
			maxdistsquared = s2;
			track = (i << INDEX_SHIFT) | TRACK_LINE_INSERT;
		}
		if(!IsNull(rmpt) && (s2 = Squared(rmpt - pt)) < tabs(maxdistsquared)) {
			maxdistsquared = s2;
			track = (i << INDEX_SHIFT) | TRACK_LINE_REMOVE;
		}
	}
	return track;
}

Image VectorLine::Cursor(int track, dword keyflags) const
{
	switch(track & TRACK_MASK) {
		case TRACK_LINE_INSERT: return VecImg::line_insert_cursor();
		case TRACK_LINE_REMOVE: return VecImg::line_remove_cursor();
		default: return VectorObject::Cursor(track, keyflags);
	}
}

void VectorLine::Create(Point from, Point to, dword keyflags)
{
	points.SetCount(2);
	points[0] = from;
	points[1] = to;
}

static void PaintAddBox(Draw& draw, Point pt, bool is_hot)
{
	Color c = (is_hot ? Color(0, 192, 0) : Green());
	draw.DrawRect(pt.x - 4, pt.y - 4, 9, 1, c);
	draw.DrawRect(pt.x - 4, pt.y - 4, 1, 9, c);
	draw.DrawRect(pt.x - 4, pt.y + 4, 9, 1, c);
	draw.DrawRect(pt.x + 4, pt.y - 4, 1, 9, c);
	draw.DrawRect(pt.x - 2, pt.y, 5, 1, c);
	draw.DrawRect(pt.x, pt.y - 2, 1, 5, c);
}

static void PaintRemoveBox(Draw& draw, Point pt, bool is_hot)
{
	Color c = (is_hot ? Color(192, 0, 0) : Red());
	draw.DrawRect(pt.x - 4, pt.y - 4, 9, 1, c);
	draw.DrawRect(pt.x - 4, pt.y - 4, 1, 9, c);
	draw.DrawRect(pt.x - 4, pt.y + 4, 9, 1, c);
	draw.DrawRect(pt.x + 4, pt.y - 4, 1, 9, c);
	draw.DrawRect(pt.x - 2, pt.y, 5, 1, c);
}

Size VectorLine::GetOutVector(const Scaling& scaling, int i, int factor) const
{
	if(points.GetCount() <= 1)
		return Size(factor, 0);
	Size invec;
	if(i == 0)
		invec = points[i + 1] - points[i];
	else if(i >= points.GetCount() - 1)
		invec = points[points.GetCount() - 2] - points.Top();
	else
		invec = ScaleSize(points[i + 1] - points[i], 1000) + ScaleSize(points[i - 1] - points[i], 1000);
	return ScaleSize(-invec * scaling, factor);
}

void VectorLine::SelPaint(ScalingDraw& draw, const Index<int>& sel, bool is_hot) const
{
	VectorObject::SelPaint(draw, sel, is_hot);
	for(int i = 0; i <= points.GetCount(); i++) {
		Point addpt, rmpt;
		GetAddRmPoint(draw, i, addpt, rmpt);
		PaintAddBox(*draw.draw, addpt, is_hot);
		if(!IsNull(rmpt))
			PaintRemoveBox(*draw.draw, rmpt, is_hot);
	}
}

void VectorLine::GetAddRmPoint(const Scaling& scaling, int i, Point& addpt, Point& rmpt) const
{
	int fac = 12 + (scaling.X(line_width) >> 1);
	addpt = rmpt = Null;
	switch(points.GetCount()) {
		case 0: return;
		case 1: addpt = points[0] * scaling + Size(fac, 0); return;
	}
	Size outvec = GetOutVector(scaling, i, fac);
	Size side = (points.GetCount() <= 2 ? Size(0, 0) : ScaleSize(Size(outvec.cy, -outvec.cx), 8));
	Point pt = points[min(i, points.GetCount() - 1)] * scaling;
	if(i == 0) {
		addpt = pt + outvec + side;
		if(points.GetCount() > 2)
			rmpt = pt + outvec - side;
	}
	else if(i < points.GetCount()) {
		Size dir = pt - points[i - 1] * scaling;
		addpt = ((points[i - 1] * scaling + pt) >> 1) + ScaleSize(Size(dir.cy, -dir.cx), fac);
		if(points.GetCount() > 2) {
			rmpt = pt + outvec;
			if(i + 1 >= points.GetCount())
				rmpt -= side;
		}
	}
	else
		addpt = pt + outvec + side;
}

Vector<Point> VectorRectData::GetPoly(ScalingDraw& draw, int lw) const
{
	Size rdelta(draw.X(rounding), draw.Y(rounding));
	int newside = minmax<int>(max(rdelta.cx, rdelta.cy) >> 1, 0, 50);
	Vector<Point> out;
	Rect rc = rect * draw;
	int w1 = lw >> 1, w2 = (lw + 1) >> 1;
	rc.Deflate(w1, w1, w2, w2);
	if(newside <= 0) {
		out.SetCount(4);
		out[0] = rc.TopRight();
		out[1] = rc.TopLeft();
		out[2] = rc.BottomLeft();
		out[3] = rc.BottomRight();
	}
	else {
		out.SetCount(4 * (newside + 1));
		Point *d0 = out.Begin(), *d1 = d0 + 2 * (newside + 1), *d2 = d1, *d3 = out.End();
		Rect inner = rc.Deflated(rdelta);
		*d0++ = Point(rc.right, inner.top);
		*--d1 = Point(rc.left, inner.top);
		*d2++ = Point(rc.left, inner.bottom);
		*--d3 = Point(rc.right, inner.bottom);
		for(int i = 1; i < newside; i++) {
			double a = i * (M_PI / 2) / newside;
			int c = fround(rdelta.cx * cos(a)), s = fround(rdelta.cy * sin(a));
			*d0++ = Point(inner.right + c, inner.top - s);
			*--d1 = Point(inner.left - c, inner.top - s);
			*d2++ = Point(inner.left - c, inner.bottom + s);
			*--d3 = Point(inner.right + c, inner.bottom + s);
		}
		*d0++ = Point(inner.right, rc.top);
		*--d1 = Point(inner.left, rc.top);
		*d2++ = Point(inner.left, rc.bottom);
		*--d3 = Point(inner.right, rc.bottom);
		ASSERT(d0 == d1 && d2 == d3);
	}
	return out;
}

void VectorRectData::Paint(ScalingDraw& draw) const
{
	Rect drc = rect * draw, irc = drc;
	switch(style) {
		case STYLE_RECT: {
			if(!IsNull(outline_color)) {
				Size frmsize(max(draw.X(outline_width), 1), max(draw.Y(outline_width), 1));
				irc.Deflate(frmsize);
				DrawRectMinusRect(*draw.draw, drc, irc, outline_color);
			}
			draw.draw->DrawRect(irc, fill_color);
			break;
		}

		case STYLE_ROUNDED_RECT: {
			draw.draw->DrawPolygon(GetPoly(draw, outline_width), fill_color, outline_width, outline_color);
			break;
		}

		case STYLE_ELLIPSE: {
			int w2 = outline_width >> 1, w1 = (outline_width - 1) >> 1;
			draw.draw->DrawEllipse(drc.Deflated(w1), fill_color, outline_width, outline_color);
			break;
		}
	}

	int ow = IsNull(outline_color) ? 0 : outline_width;
	drc.Deflate(draw.X(ow + hmargin), draw.Y(ow + vmargin));
	Font font = GetScaledFont(draw);
	Size sz = draw.draw->GetTextSize(text, font);
	int x, y;
	switch(halign) {
		case ALIGN_LEFT:  x = drc.left; break;
		default:          x = (drc.left + drc.right - sz.cx) >> 1; break;
		case ALIGN_RIGHT: x = drc.right - sz.cx; break;
	}
	switch(valign) {
		case ALIGN_TOP:    y = drc.top; break;
		default:           y = (drc.top + drc.bottom - sz.cy) >> 1; break;
		case ALIGN_BOTTOM: y = drc.bottom - sz.cy; break;
	}
	draw.draw->DrawText(x, y, text, font, text_color);
}

Font VectorRectData::GetScaledFont(const ScalingDraw& draw) const
{
	int newht = draw.Y(font.GetHeight());
	int newwd = draw.X(draw.draw->GetFontInfo(font).GetAveWidth());
	return font().Height(newht ? newht : 1).Width(newwd);
}

void VectorRect::DragPaint(ScalingDraw& draw) const
{
	switch(style) {
		case STYLE_RECT: {
			DrawLineRect(*draw.draw, rect * draw);
			break;
		}

		case STYLE_ROUNDED_RECT: {
			draw.draw->DrawPolygon(GetPoly(draw, 0), Null, 0, Black());
			break;
		}

		case STYLE_ELLIPSE: {
			Rect rc = rect * draw;
			draw.draw->DrawEllipse(rc, Null, 0, Black());
			DrawLineRect(*draw.draw, rc, PEN_DOT);
			break;
		}
	}
}

/*
int VectorRect::Nearest(Point pt, int& maxdistsquared) const
{
	if(style != STYLE_ELLIPSE)
		return VectorObject::Nearest(pt, maxdistsquared);
	int s2 = GetEllipseDistanceSquared(rect, pt);
	if(tabs(s2) <= tabs(maxdistsquared)) {
		maxdistsquared = s2;
		return (0 << INDEX_SHIFT) | TRACK_RECT | RECT_MASK | ARG_MASK;
	}
	return -1;
}
*/

const VectorInfo& VectorRect::ClassInfo()
{
	static VectorInfo i = VectorInfoFor<VectorRect>(
		"VectorRect", "r",
			VectorGeomArgDef(&VectorRect::rect),
			NULL,
			Array<VectorArg>()
			<< VectorArgDef(&VectorRect::text_color,    "TextColor",    "x",  Black())
			<< VectorArgDef(&VectorRect::fill_color,    "FillColor",    "c",  White())
			<< VectorArgDef(&VectorRect::outline_color, "OutlineColor", "o",  Black())
			<< VectorArgDef(&VectorRect::outline_width, "OutlineWidth", "u",  0)
			<< VectorArgDef(&VectorRect::rounding,      "Rounding",     "r",  10)
			<< VectorArgDef(&VectorRect::style,         "Style",        "s",  STYLE_RECT, VectorArg::EDIT_RECT_STYLE)
			<< VectorArgDef(&VectorRect::font,          "SetFont",      "f",  Arial(20))
			<< VectorArgDef(&VectorRect::text,          "Text",         "t",  Null)
			<< VectorArgDef(&VectorRect::halign,        "AlignHorz",    "h",  ALIGN_CENTER, VectorArg::EDIT_ALIGN_HORZ)
			<< VectorArgDef(&VectorRect::valign,        "AlignVert",    "v",  ALIGN_CENTER, VectorArg::EDIT_ALIGN_VERT)
			<< VectorArgDef(&VectorRect::hmargin,       "MarginHorz",   "m",  0)
			<< VectorArgDef(&VectorRect::vmargin,       "MarginVert",   "n",  0)
			<< VectorArgDef(&VectorRect::link,          "Link",         "z",  Null)
	).DefaultName("Rectangular")
	.Icon(VecImg::rect_object);
	return i;
}

void VectorArcData::Paint(ScalingDraw& draw) const
{
	Rect rlim = rect * draw;
	rlim.Deflate(line_width >> 1);
	draw.draw->DrawArc(rlim, from * draw, to * draw, line_width, line_color);
}

void VectorArc::DesignPaint(ScalingDraw& draw) const
{
	VectorArcData::Paint(draw);
	Point center = (rect * draw).CenterPoint();
	draw.draw->DrawLine(center, from * draw, PEN_DOT);
	draw.draw->DrawLine(center, to * draw, PEN_DOT);
}

void VectorArc::DragPaint(ScalingDraw& draw) const
{
	Rect rlim = rect * draw;
	draw.draw->DrawArc(rlim, from * draw, to * draw, 0, Black());
	Point center = rlim.CenterPoint();
	draw.draw->DrawLine(center, from * draw, PEN_DOT);
	draw.draw->DrawLine(center, to * draw, PEN_DOT);
	DrawLineRect(*draw.draw, rlim, PEN_DOT);
}

int VectorArc::Nearest(const Scaling& scaling, Point pt, int& maxdistsquared) const
{
	int s2, track = -1;
	if((s2 = GetEllipseDistanceSquared(rect * scaling, pt)) < tabs(maxdistsquared)) {
		maxdistsquared = s2;
		track = (0 << INDEX_SHIFT) | TRACK_RECT | ARG_MASK | RECT_MASK;
	}
	if((s2 = Squared(pt - from * scaling)) < tabs(maxdistsquared)) {
		maxdistsquared = s2;
		track = (0 << INDEX_SHIFT) | TRACK_POINT | ARG_MASK;
	}
	if((s2 = Squared(pt - to * scaling)) < tabs(maxdistsquared)) {
		maxdistsquared = s2;
		track = (1 << INDEX_SHIFT) | TRACK_POINT | ARG_MASK;
	}
	if(track >= 0)
		return track;
	Point c = rect.CenterPoint();
	if((s2 = GetLineDistanceSquared(c, from * scaling, pt)) < tabs(maxdistsquared)) {
		maxdistsquared = s2;
		track = (0 << INDEX_SHIFT) | TRACK_POINT | ARG_MASK;
	}
	if((s2 = GetLineDistanceSquared(c, to * scaling, pt)) < tabs(maxdistsquared)) {
		maxdistsquared = s2;
		track = (1 << INDEX_SHIFT) | TRACK_POINT | ARG_MASK;
	}
	return track;
}

const VectorInfo& VectorArc::ClassInfo()
{
	static VectorInfo i = VectorInfoFor<VectorArc>(
		"VectorArc", "a",
			VectorGeomArgDef(&VectorArc::rect),
			VectorGeomArgDef(&VectorArc::from, &VectorArc::to),
			Array<VectorArg>()
			<< VectorArgDef(&VectorArc::line_color, "LineColor", "l",  Black())
			<< VectorArgDef(&VectorArc::line_width, "LineWidth", "w",  0)
			<< VectorArgDef(&VectorArc::link,       "Link",      "z",  Null)
	).DefaultName("Arc")
	.Icon(VecImg::arc_object);
	return i;
}
