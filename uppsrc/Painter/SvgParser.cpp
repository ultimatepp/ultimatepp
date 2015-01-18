#include "Painter.h"

#define LLOG(x)

NAMESPACE_UPP

#include "SvgInternal.h"

void SvgParser::ResolveGradient(int i)
{
	Gradient& g = gradient[i];
	if(g.resolved || g.href.GetCount() < 2)
		return;
	int q = gradient.Find(g.href.Mid(1));
	g.resolved = true;
	if(q < 0)
		return;
	ResolveGradient(q);
	Gradient& g2 = gradient[q];
	if(g.stop.GetCount() == 0)
		g.stop <<= g2.stop;
	g.a.x = Nvl(Nvl(g.a.x, g2.a.x));
	g.a.y = Nvl(Nvl(g.a.y, g2.a.y));
	g.b.x = Nvl(g.b.x, g2.b.x); // In user-space units, needs to be replaced by cx, in normal with 1
	g.b.y = Nvl(Nvl(g.b.y, g2.b.y));
	g.c.x = Nvl(Nvl(g.c.x, g2.c.x), 0.5);
	g.c.y = Nvl(Nvl(g.c.y, g2.c.y), 0.5);
	g.f.x = Nvl(Nvl(g.f.x, g2.f.x), g.c.x);
	g.f.y = Nvl(Nvl(g.f.y, g2.f.y), g.c.y);
	g.r = Nvl(Nvl(g.r, g2.r), 1.0);
	g.transform = Nvl(g.transform, g2.transform);
	g.style = Nvl(Nvl(g.style, g2.style), GRADIENT_PAD);
}

void SvgParser::StartElement()
{
	state.Add();
	state.Top() = state[state.GetCount() - 2];
	bp.Begin();
	bp.Transform(Transform(Txt("transform")));
	Style(Txt("style"));
	for(int i = 0; i < GetAttrCount(); i++)
		ProcessValue(GetAttr(i), (*this)[i]);
	closed = false;
}

void SvgParser::EndElement()
{
	if(!closed) {
		sw.Stroke(0, Black()); // Finish path to allow new transformations, if not yet done
	}
	state.Drop();
	bp.End();
}

void SvgParser::DoGradient(int gi, bool stroke)
{
	State& s = state.Top();
	ResolveGradient(gi);
	Gradient& g = gradient[gi];
	if(g.stop.GetCount()) {
		for(int i = 0; i < g.stop.GetCount(); i++)
			sw.ColorStop(g.stop[i].offset, g.stop[i].color);
		Pointf a = g.a;
		Pointf b = g.b;
		Pointf c = g.c;
		Pointf f = g.f;
		Pointf r(g.r, g.r);
		Sizef sz = bp.boundingbox.GetSize();
		Pointf pos = bp.boundingbox.TopLeft();
		if(IsNull(b.x))
			b.x = g.user_space ? bp.boundingbox.right : 1.0;
		if(g.user_space) {
			a = (a - pos) / sz;
			b = (b - pos) / sz;
			c = (c - pos) / sz;
			f = (f - pos) / sz;
			r = r / sz;
		}
		Xform2D m;
		
		if(g.radial) {
			m.x.x = r.x;
			m.x.y = 0;
			m.y.x = 0;
			m.y.y = r.y;
			m.t = c;
			f = (f - c) / r;
		}
		else {
			Pointf d = b - a;
			m.x.x = d.x;
			m.x.y = -d.y;
			m.y.x = d.y;
			m.y.y = d.x;
			m.t = a;
		}
		m = m * Xform2D::Scale(sz.cx, sz.cy) * Xform2D::Translation(pos.x, pos.y);
		if(g.transform.GetCount())
			m = m * Transform(g.transform);
		RGBA c1 = g.stop[0].color;
		RGBA c2 = g.stop.Top().color;
		if(stroke)
			if(g.radial)
				sw.Stroke(s.stroke_width, f, c1, c2, m, g.style);
			else
				sw.Stroke(s.stroke_width, c1, c2, m, g.style);
		else
			if(g.radial)
				sw.Fill(f, c1, c2, m, g.style);
			else
				sw.Fill(c1, c2, m, g.style);
		bp.Finish(stroke * s.stroke_width);
		sw.ClearStops();
		closed = true;
	}
}

void SvgParser::StrokeFinishElement()
{
	State& s = state.Top();
	if(s.stroke_width > 0) {
		double o = s.opacity * s.stroke_opacity;
		if(o != 1) {
			sw.Begin();
			sw.Opacity(o);
		}
		if(s.stroke_gradient >= 0 && s.stroke_width > 0)
			DoGradient(s.stroke_gradient, true);
		else
		if(!IsNull(s.stroke) && s.stroke_width > 0) {
			sw.Stroke(s.stroke_width, s.stroke);
			bp.Finish(s.stroke_width);
			closed = true;
		}
		if(o != 1)
			sw.End();
	}
	EndElement();
}

void SvgParser::FinishElement()
{
	State& s = state.Top();
	double o = s.opacity * s.fill_opacity;
	if(o != 1) {
		sw.Begin();
		sw.Opacity(o);
	}
	if(s.fill_gradient >= 0)
		DoGradient(s.fill_gradient, false);
	else
	if(!IsNull(s.fill)) {
		sw.Fill(s.fill);
		bp.Finish(0);
		closed = true;
	}
	if(o != 1)
		sw.End();
	StrokeFinishElement();
}

void SvgParser::ParseGradient(bool radial)
{
	LLOG("ParseGradient " << Txt("id"));
	Gradient& g = gradient.Add(Txt("id"));
	g.radial = radial;
	g.user_space = Txt("gradientUnits") == "userSpaceOnUse";
	g.transform = Txt("gradientTransform");
	g.href = Txt("xlink:href");
	g.resolved = IsNull(g.href);
	double def = g.resolved ? 0.0 : (double)Null;
	double def5 = g.resolved ? 0.5 : (double)Null;
	g.c.x = Dbl("cx", def5);
	g.c.y = Dbl("cy", def5);
	g.r = Dbl("r", g.resolved ? 1.0 : (double)Null);
	g.f.x = Dbl("fx", g.c.x);
	g.f.y = Dbl("fy", g.c.y);
	g.a.x = Dbl("x1", def);
	g.a.y = Dbl("y1", def);
	g.b.x = Dbl("x2", Null);
	g.b.y = Dbl("y2", def);
	g.style = decode(Txt("spreadMethod"), "pad", GRADIENT_PAD, "reflect", GRADIENT_REFLECT,
	                 "repeat", GRADIENT_REPEAT, (int)Null);
	while(!End())
		if(TagE("stop")) {
			Stop &s = g.stop.Add();
			double offset = 0;
			String st = Txt("style");
			const char *style = st;
			double opacity = 1;
			Color  color;
			String key, value;
			for(;;) {
				if(*style == ';' || *style == '\0') {
					value = TrimBoth(value);
					if(key == "stop-color")
						color = GetColor(value);
					else
					if(key == "stop-opacity")
						opacity = StrDbl(value);
					else
					if(key == "offset")
						offset = StrDbl(value);
					value.Clear();
					key.Clear();
					if(*style == '\0')
						break;
					else
						style++;
				}
				else
				if(*style == ':') {
					key << TrimBoth(value);
					value.Clear();
					style++;
				}
				else
					value.Cat(*style++);
			}
			value = Txt("stop-color");
			if(value.GetCount())
				color = GetColor(value);
			value = Txt("stop-opacity");
			if(value.GetCount())
				opacity = StrDbl(value);
			s.color = clamp(int(opacity * 255 + 0.5), 0, 255) * color;
			s.offset = Nvl(Dbl("offset"), offset);
		}
		else
			Skip();
}

void SvgParser::Poly(bool line)
{
	Vector<Point> r;
	String value = Txt("points");
	try {
		CParser p(value);
		while(!p.IsEof()) {
			Pointf n;
			n.x = p.ReadDouble();
			p.Char(',');
			n.y = p.ReadDouble();
			r.Add(n);
			p.Char(',');
		}
	}
	catch(CParser::Error) {}
	if(r.GetCount()) {
		StartElement();
		bp.Move(r[0].x, r[0].y);
		for(int i = 1; i < r.GetCount(); ++i)
			bp.Line(r[i].x, r[i].y);
		if(line)
			StrokeFinishElement();
		else
			FinishElement();
	}
}

double ReadNumber(CParser& p)
{
	while(!p.IsEof() && (!p.IsDouble() || p.IsChar('.')))
		p.SkipTerm();
	return p.ReadDouble();
}

Rectf GetSvgViewBox(XmlParser& xml)
{
	String v = xml["viewBox"];
	Rectf r = Null;
	if(v.GetCount()) {
		try {
			CParser p(v);
			r.left = ReadNumber(p);
			r.top = ReadNumber(p);
			r.right = r.left + ReadNumber(p);
			r.bottom = r.right + ReadNumber(p);
		}
		catch(CParser::Error) {
			r = Null;
		}
	}
	return r;
}

Sizef GetSvgSize(XmlParser& xml)
{
	Sizef sz;
	sz.cx = StrDbl(xml["width"]);
	sz.cy = StrDbl(xml["height"]);
	if(IsNull(sz.cx) || IsNull(sz.cy))
		sz = Null;
	return sz;
}

Pointf GetSvgPos(XmlParser& xml)
{
	Pointf p;
	p.x = StrDbl(xml["x"]);
	p.y = StrDbl(xml["y"]);
	if(IsNull(p.x) || IsNull(p.y))
		p = Null;
	return p;
}

void SvgParser::ParseG() {
#ifdef _DEBUG
	LLOG("====== TAG " << (IsTag() ? PeekTag() : String()));
#endif
	if(Tag("defs")) {
		while(!End())
			if(Tag("linearGradient"))
				ParseGradient(false);
			else
			if(Tag("radialGradient"))
				ParseGradient(true);
			else // TODO: Implement pattern, filter, ...
				Skip();
	}
	else
	if(Tag("linearGradient"))
		ParseGradient(false);
	else
	if(Tag("radialGradient"))
		ParseGradient(true);
	else
	if(TagE("rect")) {
		StartElement();
		bp.RoundedRectangle(Dbl("x"), Dbl("y"), Dbl("width"), Dbl("height"), Dbl("rx"), Dbl("ry"));
		FinishElement();
	}
	else
	if(TagE("ellipse")) {
		StartElement();
		bp.Ellipse(Dbl("cx"), Dbl("cy"), Dbl("rx"), Dbl("ry"));
		FinishElement();
	}
	else
	if(TagE("circle")) {
		StartElement();
		Pointf c(Dbl("cx"), Dbl("cy"));
		double r = Dbl("r");
		bp.Ellipse(c.x, c.y, r, r);
		FinishElement();
	}
	else
	if(TagE("line")) {
		StartElement();
		Pointf a(Dbl("x1"), Dbl("y1"));
		Pointf b(Dbl("x2"), Dbl("y2"));
		bp.Move(a);
		bp.Line(b);
		FinishElement();
	}
	else
	if(TagE("polygon"))
		Poly(false);
	else
	if(TagE("polyline"))
		Poly(true);
	else
	if(TagE("path")) {
		StartElement();
		bp.Path(Txt("d"));
		FinishElement();
	}
	else
	if(TagE("image")) {
		StartElement();
		String fileName = Txt("xlink:href");
		String data;
		resloader(fileName, data);
		if(data.GetCount()) {
			Image img = StreamRaster::LoadFileAny(fileName);
			if(!IsNull(img)) {
				bp.Rectangle(Dbl("x"), Dbl("y"), Dbl("width"), Dbl("height"));
				sw.Fill(StreamRaster::LoadFileAny(fileName), Dbl("x"), Dbl("y"), Dbl("width"), 0);
			}
		}
		EndElement();
	}
	else
	if(Tag("text")) {
		StartElement();
		String text = ReadText();
		text.Replace("\n", " ");
		text.Replace("\r", "");
		text.Replace("\t", " ");
		Font fnt = state.Top().font;
		bp.Text(Dbl("x"), Dbl("y") - fnt.GetAscent(), text, fnt);
		FinishElement();
		PassEnd();	
	}
	else
	if(Tag("g")) {
		StartElement();
		while(!End())
			ParseG();
		EndElement();
	}
	else
	if(Tag("svg")) {
		Sizef sz = GetSvgSize(*this);
		if(!IsNull(sz)) {
			Pointf p = Nvl(GetSvgPos(*this), Pointf(0, 0));
			Rectf vb = Nvl(GetSvgViewBox(*this), sz);
			//TODO: For now, we support "xyMid meet" only
			bp.Translate(p.x, p.y);
			bp.Scale(min(sz.cx / vb.GetWidth(), sz.cy / vb.GetHeight()));
			bp.Translate(-vb.TopLeft());
			StartElement();
			while(!End())
				ParseG();
			EndElement();
		}
	}
	else
		Skip();
}

bool SvgParser::Parse() {
	try {
		while(!IsTag())
			Skip();
		PassTag("svg");
		bp.Begin();
		while(!End())
			ParseG();
		bp.End();
	}
	catch(XmlError e) {
		return false;
	}
	return true;
}

SvgParser::SvgParser(const char *svg, Painter& sw)
:	XmlParser(svg),
    sw(sw), bp(sw)	
{
	Reset();
}

bool ParseSVG(Painter& p, const char *svg, Callback2<String, String&> resloader, Rectf *boundingbox)
{
	SvgParser sp(svg, p);
	sp.bp.compute_svg_boundingbox = boundingbox;
	sp.resloader = resloader;
	if(!sp.Parse())
		return false;
	if(boundingbox)
		*boundingbox = sp.bp.svg_boundingbox;
	return true;
}

bool RenderSVG(Painter& p, const char *svg, Callback2<String, String&> resloader)
{
	return ParseSVG(p, svg, resloader, NULL);
}

bool RenderSVG(Painter& p, const char *svg)
{
	return RenderSVG(p, svg, Callback2<String, String&>());
}

void GetSVGDimensions(const char *svg, Sizef& sz, Rectf& viewbox)
{
	viewbox = Null;
	sz = Null;
	try {
		XmlParser xml(svg);
		while(!xml.IsTag())
			xml.Skip();
		xml.PassTag("svg");
		viewbox = GetSvgViewBox(xml);
		sz = GetSvgSize(xml);
	}
	catch(XmlError e) {
	}
}

Rectf GetSVGBoundingBox(const char *svg)
{
	NilPainter nil;
	Rectf bb;
	if(!ParseSVG(nil, svg, Callback2<String, String&>(), &bb))
		return Null;
	return bb;
}

Image RenderSVGImage(Size sz, const char *svg, Callback2<String, String&> resloader)
{
	Rectf f = GetSVGBoundingBox(svg);
	Sizef iszf = GetFitSize(f.GetSize(), Sizef(sz.cx, sz.cy) - 10.0);
	Size isz((int)ceil(iszf.cx), (int)ceil(iszf.cy));
	if(isz.cx <= 0 || isz.cy <= 0)
		return Null;
	ImageBuffer ib(isz);
	BufferPainter sw(ib);
	sw.Clear(White());
	sw.Scale(min(isz.cx / f.GetWidth(), isz.cy / f.GetHeight()));
	sw.Translate(-f.left, -f.top);
	RenderSVG(sw, svg, resloader);
	return ib;
}

Image RenderSVGImage(Size sz, const char *svg)
{
	return RenderSVGImage(sz, svg, Callback2<String, String&>());
}

END_UPP_NAMESPACE
