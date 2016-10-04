#include "SvgDraw.h"

#include <CtrlLib/CtrlLib.h> //TODO: remove
#include <Web/Web.h>

NAMESPACE_UPP

void SvgDraw::Init(int xPixels, int yPixels)
{
	_nextId = 1;
	
	_svgNode.CreateTag("svg");
	_svgNode.SetAttr("xmlns", "http://www.w3.org/2000/svg");
	_svgNode.SetAttr("xmlns:xlink", "http://www.w3.org/1999/xlink");
	_svgNode.SetAttr("width", xPixels);
	_svgNode.SetAttr("height", yPixels);
	_svgNode.SetAttr("viewBox", String().Cat() << "0 0 " << xPixels << " " << yPixels);
	
	_defsNode = &_svgNode.Add("defs");
	_currentParentNode = &_svgNode;
}

String SvgDraw::Finish()
{
	return XmlHeader() + 
		XmlDocType("svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"") +
		AsXML(_svgNode, 0);
}

void SvgDraw::BeginOp()
{
	_parentNodes.Add(_currentParentNode);
	XmlNode &_newParentNode = _currentParentNode->Add("g");
	_currentParentNode = &_newParentNode;
	////_offsetStack.Add(actual_offset);
}

void SvgDraw::EndOp()
{
	ASSERT(_parentNodes.GetCount() > 0);
	////ASSERT(_offsetStack.GetCount() > 0);
	
	_currentParentNode = _parentNodes.Top();
	_parentNodes.Drop();
	////actual_offset = _offsetStack.Top();
	////_offsetStack.Drop();
}

void SvgDraw::OffsetOp(Point p)
{
	Begin();
	_currentParentNode->SetAttr("transform", String().Cat() << "translate(" << p.x << ", " << p.y << ")");
	////actual_offset += p;
}

bool SvgDraw::ClipOp(const Rect& r)
{
	Begin();
	XmlNode &clipPath = _defsNode->Add("clipPath");
	XmlNode &path = clipPath.Add("path");
	
	String pathD;
	pathD << "M" << r.left << " " << r.top;
	pathD << " h" << r.Width() << " v" << r.Height() << " h-" << r.Width() << " Z";
	
	String id = GetNextId();
	
	path.SetAttr("d", pathD);
	clipPath.SetAttr("id", id);
	
	_currentParentNode->SetAttr("clip-path", String().Cat() << "url(#" << id << ")");

	return false; // todo:
}

bool SvgDraw::ClipoffOp(const Rect& r)
{
	Begin();
	XmlNode &clipPath = _defsNode->Add("clipPath");
	XmlNode &path = clipPath.Add("path");
	
	String pathD;
	pathD << "M0 0";
	pathD << " h" << r.Width() << " v" << r.Height() << " h-" << r.Width() << " Z";
	
	String id = GetNextId();
	
	path.SetAttr("d", pathD);
	clipPath.SetAttr("id", id);
	
	_currentParentNode->SetAttr("transform", String().Cat() << "translate(" << r.left << ", " << r.top << ")");
	////actual_offset += r.TopLeft();
	_currentParentNode->SetAttr("clip-path", String().Cat() << "url(#" << id << ")");
	
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
	
	XmlNode &rect = _currentParentNode->Add("rect");
	rect.SetAttr("x", x);
	rect.SetAttr("y", y);
	rect.SetAttr("width", cx);
	rect.SetAttr("height", cy);
	rect.SetAttr("style", style);
}

void SvgDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	//TODO: src, color
	PNGEncoder pngEncoder;
	String pngString = pngEncoder.SaveString(img);
	String dataUri = "data:image/png;base64," + Base64Encode(pngString);
	
	XmlNode &image = _currentParentNode->Add("image");
	image.SetAttr("x", x);
	image.SetAttr("y", y);
	image.SetAttr("width", cx);
	image.SetAttr("height", cy);
	image.SetAttr("xlink:href", dataUri);
}

void SvgDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	//TODO: Why is the standard width = 0  ?!?
	StringBuffer style;
	style << "stroke:rgb(" << color.GetR() << ',' << color.GetG() << ',' << color.GetB() << ");";
	style << "stroke-width:" << width << ';';
	style << "stroke-linecap:round;";
	
	XmlNode &line = _currentParentNode->Add("line");
	line.SetAttr("x1", x1);
	line.SetAttr("y1", y1);
	line.SetAttr("x2", x2);
	line.SetAttr("y2", y2);
	line.SetAttr("style", style);
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
		XmlNode &polyline = _currentParentNode->Add("polyline");
		StringBuffer points;
		
		for (int j = 0; j < counts[i]; j++, vertices++)
			points << vertices->x << ' ' << vertices->y << ' ';
		
		polyline.SetAttr("points", points);
		polyline.SetAttr("style", style);
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
		style << "stroke:rgb(" << outline.GetR() << ',' << outline.GetG() << ',' << outline.GetB() << ");";
	if (stroke)
		style << "stroke-width:" << width << ';';
	
	for (int i = 0; i < scc; i++)
	{
		XmlNode &polygon = _currentParentNode->Add("polygon");
		StringBuffer points;
		
		for (int j = 0; j < subpolygon_counts[i]; j++, vertices++)
			points << vertices->x << ' ' << vertices->y << ' ';
		
		polygon.SetAttr("points", points);
		polygon.SetAttr("style", style);
	}
}

void SvgDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor)
{
	//TODO: why is the pen default Null and not 0 or 1?
	if (pen == (int)Null)
		pen = 0;
	
	StringBuffer style;
	style << "stroke:rgb(" << pencolor.GetR() << ',' << pencolor.GetG() << ',' << pencolor.GetB() << ");";
	style << "stroke-width:" << pen << ';';
	style << "fill:rgb(" << color.GetR() << ',' << color.GetG() << ',' << color.GetB() << ");";
	
	XmlNode &ellipse = _currentParentNode->Add("ellipse");
	ellipse.SetAttr("cx", r.left + r.GetWidth() / 2);
	ellipse.SetAttr("cy", r.top + r.GetHeight() / 2);
	ellipse.SetAttr("rx", r.GetWidth() / 2);
	ellipse.SetAttr("ry", r.GetHeight() / 2);
	ellipse.SetAttr("style", style);
}

void SvgDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	PromptOK("DrawArcOp not implemented");
}

void SvgDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
	                    Color ink, int n, const int *dx)
{
	//TODO: angle
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
	
	XmlNode &textNode = _currentParentNode->Add("text");
	textNode.SetAttr("x", xAttr);
	textNode.SetAttr("y", y + font.GetHeight());
	textNode.SetAttr("style", style);
	textNode.AddText(WString(text).ToString());
}

String SvgDraw::GetNextId()
{
	return String().Cat() << "id" << _nextId++;
}

END_UPP_NAMESPACE
