#include "SvgDraw.h"

#include <CtrlLib/CtrlLib.h> //TODO: remove
#include <Web/Web.h>

NAMESPACE_UPP

void SvgDraw::Init(Size size)
{
	_nextId = 1;
	_indent = "";
	_size = size;
}

String SvgDraw::Finish()
{
	XmlTag svg("svg");
	svg("xmlns", "http://www.w3.org/2000/svg")
	   ("xmlns:xlink", "http://www.w3.org/1999/xlink")
	   ("width", _size.cx)
	   ("height", _size.cy);
	   
	StringBuffer final;
	final << XmlHeader() 
	      << XmlDocType("svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"")
	      << svg(String().Cat() << "<defs>\n" << ~_defs << "</defs>\n" << ~_body);
	return ~final;
}

void SvgDraw::BeginOp()
{
	_body << _indent << "<g";
	IndentMore();
	_offsetStack.Add(actual_offset);
}

void SvgDraw::EndOp()
{
	IndentLess();
	_body << _indent << "</g>\n";
	actual_offset = _offsetStack.Top();
	_offsetStack.Drop();
}

void SvgDraw::OffsetOp(Point p)
{
	Begin();
	AddAttr("transform", String().Cat() << "translate(" << p.x << ", " << p.y << ")");
	_body << ">\n";
	actual_offset += p;
}

bool SvgDraw::ClipOp(const Rect& r)
{
	Begin();
	AddAttr("clip-path", String().Cat() << "url(#" << AddPath(r) << ")");
	_body << ">\n";

	return false; // todo:
}

bool SvgDraw::ClipoffOp(const Rect& r)
{
	Begin();
	AddAttr("transform", String().Cat() << "translate(" << r.left << ", " << r.top << ")");
	AddAttr("clip-path", String().Cat() << "url(#" << AddPath(r.Offseted(-r.TopLeft())) << ")");
	_body << ">\n";
	actual_offset += r.TopLeft();
	
	return false; // todo:
}

bool SvgDraw::ExcludeClipOp(const Rect& r)
{
	//todo:
	PromptOK(String().Cat() << "ExcludeClipOp(" << r << ") not implemented");
	return false;
}

bool SvgDraw::IntersectClipOp(const Rect& r)
{
	//todo:
	PromptOK(String().Cat() << "IntersectClipOp(" << r << ") not implemented");
	return false;
}

Rect SvgDraw::GetClipOp() const
{
	//todo:
	PromptOK("GetClipOp() not implemented");
	return RectC(0, 0, 100, 100);
}

bool SvgDraw::IsPaintingOp(const Rect& r) const
{
	//todo:
	PromptOK(String().Cat() << "IsPaintingOp(" << r << ") not implemented");
	return false;
}

void SvgDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	StringBuffer style;
	style << "stroke:none;";
	style << "fill:rgb(" << color.GetR() << ',' << color.GetG() << ',' << color.GetB() << ");";
	
	XmlTag rect("rect");
	rect("x", x)("y", y)("width", cx)("height", cy)("style", style);
	AddTag(rect);
}

void SvgDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	//TODO: src, color
	PNGEncoder pngEncoder;
	String pngString = pngEncoder.SaveString(img);
	String dataUri = "data:image/png;base64," + Base64Encode(pngString);
	
	XmlTag image("image");
	image("x", x)("y", y)("width", cx)("height", cy)("xlink:href", dataUri);
	AddTag(image);
}

void SvgDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	StringBuffer style;
	style << "stroke:rgb(" << color.GetR() << ',' << color.GetG() << ',' << color.GetB() << ");";
	style << "stroke-width:" << width << ';';
	style << "stroke-linecap:round;";
	
	XmlTag line("line");
	line("x1", x1)("y1", y1)("x2", x2)("y2", y2)("style", style);
	AddTag(line);
}

void SvgDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
                                const int *counts, int count_count,
                                int width, Color color, Color doxor)
{
	//todo: doxor
	StringBuffer style;
	style << "stroke:rgb(" << color.GetR() << ',' << color.GetG() << ',' << color.GetB() << ");";
	style << "stroke-width:" << width << ';';
	style << "stroke-linecap:round;stroke-linejoin:round;";
	style << "fill:none;";
	
	for (int i = 0; i < count_count; i++)
	{
		XmlTag line("polyline");
		StringBuffer points;
		
		for (int j = 0; j < counts[i]; j++, vertices++)
			points << vertices->x << ' ' << vertices->y << ' ';
		
		line("points", points)("style", style);
		AddTag(line);
	}
}

void SvgDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
                                   const int *subpolygon_counts, int scc,
                                   const int *disjunct_polygon_counts, int dpcc,
                                   Color color, int width, Color outline,
                                   uint64 pattern, Color doxor)
{
	//todo: disjunct polygons, dpcc, pattern, doxor
	
	bool fill = !IsNull(color);
	bool stroke = !IsNull(outline) && !IsNull(width);
	if (!fill && !stroke) return;
	
	StringBuffer style;
	if (fill)
		style << "fill:rgb(" << color.GetR() << ',' << color.GetG() << ',' << color.GetB() << ");";
	if (stroke)
	{
		style << "stroke:rgb(" << outline.GetR() << ',' << outline.GetG() << ',' << outline.GetB() << ");";
		style << "stroke-width:" << width << ';';
		style << "stroke-linecap:round;stroke-linejoin:round;";
	}
	
	for (int i = 0; i < scc; i++)
	{
		XmlTag polygon("polygon");
		StringBuffer points;
		
		for (int j = 0; j < subpolygon_counts[i]; j++, vertices++)
			points << vertices->x << ' ' << vertices->y << ' ';
		
		polygon("points", points)("style", style);
		AddTag(polygon);
	}
}

void SvgDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	//TODO: support different pen styles: PEN_NULL, PEN_SOLID, PEN_DASH, PEN_DOT, PEN_DASHDOT, PEN_DASHDOTDOT
	if (pen == (int)Null)
		pen = 0;
	
	StringBuffer style;
	style << "stroke:rgb(" << pencolor.GetR() << ',' << pencolor.GetG() << ',' << pencolor.GetB() << ");";
	style << "stroke-width:" << pen << ';';
	style << "fill:rgb(" << color.GetR() << ',' << color.GetG() << ',' << color.GetB() << ");";
	
	XmlTag ellipse("ellipse");
	ellipse("cx", r.left + r.GetWidth() / 2)
	       ("cy", r.top + r.GetHeight() / 2)
	       ("rx", r.GetWidth() / 2)
	       ("ry", r.GetHeight() / 2)
	       ("style", style);
	AddTag(ellipse);
}

void SvgDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	PromptOK("DrawArcOp not implemented");
}

void SvgDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
	                    Color ink, int n, const int *dx)
{
	StringBuffer style;
	style << "stroke:none;";
	style << "fill:rgb(" << ink.GetR() << ',' << ink.GetG() << ',' << ink.GetB() << ");";
	style << "font-size:" << font.GetHeight() << "px;";
	style << "font-family:" << Font::GetFaceName(font.GetFace()) << ";";
	if (font.IsBold())
		style << "font-weight:bold;";
	if (font.IsItalic())
		style << "font-style:italic;";
	if (font.IsUnderline() && font.IsStrikeout())
		style << "text-decoration:underline line-through;";
	else if (font.IsUnderline())
		style << "text-decoration:underline;";
	else if (font.IsStrikeout())
		style << "text-decoration:line-through;";
	
	String xAttr;
	xAttr << x;
	if (dx != NULL)
	{
		int tempX = x;
		for (int i = 0; i < n; i++)
		{
			tempX += dx[i];
			xAttr << " " << tempX;
		}
	}
	
	XmlTag t("text");
	t("x", xAttr)("y", y + font.GetHeight())("style", style);
	if (angle != 0)
		t("transform", String().Cat() << "rotate(-" << (angle / 10) << ", " << x << ", " << y << ")");
	AddTag(t, DeXml(WString(text).ToString()));
}

String SvgDraw::GetNextId()
{
	return String().Cat() << "id" << _nextId++;
}

String SvgDraw::AddPath(Rect const &r)
{
	String id = GetNextId();
	XmlTag path("path");
	
	StringBuffer p;
	p << "M" << r.left << " " << r.top << " h" << r.Width() 
	  << " v" << r.Height() << " h-" << r.Width() << " Z";
	
	path("d", p);
	
	_defs << "\t<clipPath id=\"" << id << "\">\n\t\t" << path() << "\t</clipPath>\n" ;
	return id;
}

END_UPP_NAMESPACE
