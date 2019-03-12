#include "Painter.h"

#define LLOG(x)

namespace Upp {

#include "SvgInternal.h"

void SvgParser::Reset()
{
	state.Add();
	State& s = state.Top();
	s.fill_gradient = s.stroke_gradient = -1;
	s.fill = Black();
	s.stroke = Null;
	s.opacity = s.fill_opacity = s.stroke_opacity = s.stroke_width = 1;
	s.dash_offset = 0;
	s.font = Serif(24);
	s.text_anchor = 0;
}

void SvgParser::ProcessValue(const String& key, const String& value_)
{
	State& s = state.Top();
	String value = TrimBoth(value_);
	value = TrimBoth(value);
	LLOG("ATTR " << key << " = " << value);
	if(value != "inherit") {
		if(key == "opacity")
			s.opacity = Nvl(StrDbl(value), 1.0);
		else
		if(key == "fill") {
			if(value.StartsWith("url(#")) {
				value = value.Mid(5);
				int q = value.Find(')');
				if(q >= 0)
					value.Trim(q);
				s.fill_gradient = gradient.Find(value);
				s.fill = Null;
			}
			else {
				s.fill_gradient = -1;
				s.fill = GetColor(value);
			}
		}
		else
		if(key == "fill-opacity")
			s.fill_opacity = Nvl(StrDbl(value), 1.0);
		else
		if(key == "fill-rule")
			sw.EvenOdd(value == "evenodd");
		else
		if(key == "stroke") {
			if(value.StartsWith("url(#")) {
				value = value.Mid(5);
				int q = value.Find(')');
				if(q >= 0)
					value.Trim(q);
				s.stroke_gradient = gradient.Find(value);
				s.stroke = Null;
			}
			else {
				s.stroke = GetColor(value);
				s.stroke_gradient = -1;
			}
		}
		else
		if(key == "stroke-opacity")
			s.stroke_opacity = Nvl(StrDbl(value), 1.0);
		else
		if(key == "stroke-width")
			s.stroke_width = Nvl(StrDbl(value), 1.0);
		else
		if(key == "stroke-linecap")
			sw.LineCap(decode(value, "round", LINECAP_ROUND, "square", LINECAP_SQUARE, LINECAP_BUTT));
		else
		if(key == "stroke-linejoin")
			sw.LineJoin(decode(value, "round", LINEJOIN_ROUND, "bevel", LINEJOIN_BEVEL, LINEJOIN_MITER));
		else
		if(key == "miter-limit")
			sw.MiterLimit(max(1.0, StrDbl(value)));
		else
		if(key == "stroke-dasharray") {
			s.dash_array = value;
			sw.Dash(s.dash_array, s.dash_offset);
		}
		else
		if(key == "stroke-dashoffset") {
			s.dash_offset = StrDbl(value);
			sw.Dash(s.dash_array, s.dash_offset);
		}
		else
		if(key == "font") {
			// TODO(?)
		}
		else
		if(key == "font-family") {
			int face = Font::SANSSERIF;
			if(findarg(value, "courier", "monospace") >= 0)
				face = Font::MONOSPACE;
			if(findarg(value, "roman;serif") >= 0)
				face = Font::SERIF;
			s.font.Face(face);
		}
		else
		if(key == "font-size")
			s.font.Height(atoi(value));
		else
		if(key == "font-style")
			s.font.Italic(findarg(value, "italic", "oblique") >= 0);
		else
		if(key == "font-weight")
			s.font.Bold(findarg(value, "bold", "bolder") >= 0 || atoi(value) >= 500);
		else
		if(key == "text-anchor")
			s.text_anchor = decode(value, "left", 0, "middle", 1, "right", 2, 0);
	}
}

void SvgParser::Style(const char *style)
{
	String key, value;
	for(;;) {
		if(*style == ';' || *style == '\0') {
			ProcessValue(key, value);
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
}

Xform2D SvgParser::Transform(const char *transform)
{
	Xform2D mx;
	try {
		CParser p(transform);
		while(!p.IsEof()) {
			String kind = ToLower(p.ReadId());
			Vector<double> r;
			p.Char('(');
			while(!p.IsEof() && !p.Char(')')) {
				r.Add(p.ReadDouble());
				p.Char(',');
			}
			if(r.GetCount() >= 1) {
				LLOG("transform " << kind << r);
				if(kind == "translate" && r.GetCount() >= 2)
					mx = Xform2D::Translation(r[0], r[1]) * mx;
				else
				if(kind == "rotate") {
					if(r.GetCount() >= 3)
						mx = Xform2D::Translation(-r[1], -r[2]) * mx;
					mx = Xform2D::Rotation(r[0] * M_2PI / 360) * mx;
					if(r.GetCount() >= 3)
						mx = Xform2D::Translation(r[1], r[2]) * mx;
				}
				else
				if(kind == "scale" && r.GetCount() >= 1)
					mx = Xform2D::Scale(r[0], r[min(r.GetCount() - 1, 1)]) * mx;
				else {
					Xform2D m;
					if(kind == "skewx")
						m.x.y = atan(r[0] * M_2PI / 360);
					else
					if(kind == "skewy")
						m.y.x = atan(r[0] * M_2PI / 360);
					else
					if(kind == "matrix" && r.GetCount() >= 6) {
						m.x.x = r[0];
						m.y.x = r[1];
						m.x.y = r[2];
						m.y.y = r[3];
						m.t.x = r[4];
						m.t.y = r[5];
					}
					mx = m * mx;
				}
			}
			p.Char(',');
		}
	}
	catch(CParser::Error) {
	}
	return mx;
}

}
