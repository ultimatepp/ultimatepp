#include "Controls4U.h"
#include "DrawingCanvas.h"

String GetValueStringXml(String str, const char* var);
Color GetColorXml(String text);
void ParseG(GraphElemList &elems, XmlParser &xp, Svg2DTransform transf, SvgStyle style, const char *svgFolder);
	
void GraphElemList::Paint(Painter &sw, Svg2DTransform _transf, SvgStyle _style, bool firstLayer) {
	_transf += transf;
	_transf.Apply(sw);			
	_style += style;	
	_style.Apply(sw);	
	for (int i = 0; i < elems.GetCount(); ++i) {
		elems[i].Paint(sw, _transf, _style, false);
	}
	if (firstLayer)
		PaintLimits(sw);
}

void GraphElemList::Clear() {
	elems.Clear();
}

void SvgStyle::Set(String str) {
	String value;
	Color color;
	
	color = GetColorXml(GetValueStringXml(str, "stroke:"));
	if (!color.IsNullInstance())
		strokeColor = color;
	
	value = GetValueStringXml(str, "stroke-width:");
	if (value != "")
		strokeWidth = atoi(value);
	
	color = GetColorXml(GetValueStringXml(str, "fill:"));
	if (!color.IsNullInstance())
		strokeFill = color;
	
	value = GetValueStringXml(str, "opacity:");
	if (value != "")
		strokeOpacity = atof(value);
}

void SvgStyle::Apply(Painter &sw) {
	sw.Opacity(strokeOpacity);	
	if (!strokeFill.IsNullInstance())	
		sw.Fill(strokeFill);				
	sw.Stroke(strokeWidth, strokeColor);
}

SvgStyle &SvgStyle::operator=(const SvgStyle &style) {
	if (this == &style)      // Same object?
  		return *this; 
	strokeColor = style.strokeColor;
	strokeFill = style.strokeFill;
	strokeOpacity = style.strokeOpacity;
	strokeWidth = style.strokeWidth;
	return *this;
}

SvgStyle &SvgStyle::operator+=(const SvgStyle &style) {
	if (this == &style)      // Same object?
  		return *this;
  	if (!IsNull(style.strokeColor))
		strokeColor = style.strokeColor;
	if (!IsNull(style.strokeFill))
		strokeFill = style.strokeFill;
	if (!IsNull(style.strokeOpacity))
		strokeOpacity = style.strokeOpacity;
	if (!IsNull(style.strokeWidth))
		strokeWidth = style.strokeWidth;
	return *this;
}
	
String GetValueStringXml(String str, const char* var) {
	int pos, endpos;
	
	if ((pos = str.Find(var)) >= 0) {
		pos += strlen(var);
		endpos = str.Find(';', pos+1);
		if (endpos == -1)
			endpos = str.GetCount();
		return NormalizeSpaces(str.Mid(pos, endpos-pos));
	} else
		return "";
}

Color HtmlToColor(const char *str) {
	int col;
	sscanf(str+1, "%x", &col);
	return Color(col >> 16, (col >> 8) & 255, col & 255);
}

Color GetColorXml(String text) {
	text = ToLower(text);
	if (text == "none" || text.IsEmpty())
		return Null;
	else if (text[0] == '#') 
		return HtmlToColor(text);
	else
		return GetSvgColor(text);
}

Array<double> GetTransformArgs(String str, const char *command) {
	Array<double> args;
	int pos, endpos;

	if ((pos = str.Find(command)) < 0) 
		return args;
	
	pos += strlen(command);
	pos = 1 + str.Find('(', pos);
	
	while (true) {
		if ((endpos = str.Find(',', pos)) < 0) 
			break;
		double &d = args.Add();
		d = atof(str.Mid(pos, endpos-pos));
		pos = endpos+1;
	} 
	if (!args.IsEmpty()) {
		if ((endpos = str.Find(')', pos)) >= 0) {
			double &d = args.Add();
			d = atof(str.Mid(pos, endpos-pos));
		}
	}
	return args;
}



void SvgGet_Rect(GraphElemList &elems, XmlParser &xp) {
	RectElem &elem = static_cast<RectElem&>(elems.elems.Add(new RectElem()));
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x") 
			elem.x = atoi(xp[i]);
		else if (attr == "y") 
			elem.y = atoi(xp[i]);
		else if (attr == "width") 
			elem.width = atoi(xp[i]);
		else if (attr == "height") 
			elem.height = atoi(xp[i]);
		else if (attr == "style") 
			elem.style.Set(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				elem.transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				elem.transf.Scale(args[0], args[1]);			
		}
	}	
	elem.SetLimits();			
}

void SvgGet_Image(GraphElemList &elems, XmlParser &xp, const char *svgFolder) {
	ImageElem &elem = static_cast<ImageElem&>(elems.elems.Add(new ImageElem()));
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x") 
			elem.x = atoi(xp[i]);
		else if (attr == "y") 
			elem.y = atoi(xp[i]);
		else if (attr == "width") 
			elem.width = atoi(xp[i]);
		else if (attr == "height") 
			elem.height = atoi(xp[i]);
		else if (attr == "xlink:href") {
			elem.fileName = xp[i];
			if (!FileExists(elem.fileName))
				elem.fileName = AppendFileName(svgFolder, elem.fileName);
		} else if (attr == "style") 
			elem.style.Set(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				elem.transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				elem.transf.Scale(args[0], args[1]);			
		}
	}	
	elem.SetLimits();			
}

void SvgGet_Ellipse(GraphElemList &elems, XmlParser &xp) {
	EllipseElem &elem = static_cast<EllipseElem&>(elems.elems.Add(new EllipseElem()));
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "cx") 
			elem.x = atoi(xp[i]);
		else if (attr == "cy") 
			elem.y = atoi(xp[i]);
		else if (attr == "rx") 
			elem.width = atoi(xp[i]);
		else if (attr == "ry") 
			elem.height = atoi(xp[i]);
		else if (attr == "style") 
			elem.style.Set(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				elem.transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				elem.transf.Scale(args[0], args[1]);			
		}
	}	
	elem.SetLimits();						
}

void SvgGet_Line(GraphElemList &elems, XmlParser &xp) {
	LineElem &elem = dynamic_cast<LineElem&>(elems.elems.Add(new LineElem()));
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x1") 
			elem.x1 = atoi(xp[i]);
		else if (attr == "y1") 
			elem.y1 = atoi(xp[i]);
		else if (attr == "x2") 
			elem.x2 = atoi(xp[i]);
		else if (attr == "y2") 
			elem.y2 = atoi(xp[i]);
		else if (attr == "style") 
			elem.style.Set(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				elem.transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				elem.transf.Scale(args[0], args[1]);			
		}
	}	
	elem.SetLimits();			
}

Array<Pointf> GetPolygonPointsXml(String &strpoints) {
	Array<Pointf> points;
	int newpos, pos;
	char separator;
	
	if (strpoints.Find(',') >= 0)
		separator = ',';
	else
		separator = ' ';
	
	pos = newpos = 0;
	while (newpos != strpoints.GetCount()) {
		int x, y;
		if ((newpos = strpoints.Find(separator, pos)) < 0)
			break;
		x = atoi(strpoints.Mid(pos, newpos+1));
		pos = newpos+1;
		if ((newpos = strpoints.Find(' ', pos+1 < strpoints.GetCount()? pos+1: pos)) < 0)	// Jump possible spaces 
			newpos = strpoints.GetCount();
		y = atoi(strpoints.Mid(pos, newpos+1));
		pos = newpos+1;
		Pointf &point = points.Add();
		point.x = x;
		point.y = y;
	}
	return points;
}

void SvgGet_Polygon(GraphElemList &elems, XmlParser &xp) {
	PolygonElem &elem = static_cast<PolygonElem&>(elems.elems.Add(new PolygonElem()));
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "points") {
			String strpoints;
			strpoints = NormalizeSpaces(xp[i]);
			elem.points = GetPolygonPointsXml(strpoints);
		} else if (attr == "style") 
			elem.style.Set(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				elem.transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				elem.transf.Scale(args[0], args[1]);			
		}
	}	
	elem.SetLimits();		
}

void SvgGet_Title(GraphElemList &elems, XmlParser &xp) {
	elems.title = xp.ReadText();
}

void SvgGet_Text(GraphElemList &elems, XmlParser &xp) {
	TextElem &elem = static_cast<TextElem&>(elems.elems.Add(new TextElem()));
	
	elem.text = xp.ReadText();
	elem.text.Replace("\n", " ");
	elem.text.Replace("\r", "");
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x") 
			elem.x = atoi(xp[i]);
		else if (attr == "y") 
			elem.y = atoi(xp[i]);
		else if (attr == "style") { 
			elem.style.Set(xp[i]);
			
			if (elem.style.strokeColor.IsNullInstance())
				elem.style.strokeColor = Black();
			if (elem.style.strokeFill.IsNullInstance())
				elem.style.strokeFill = elem.style.strokeColor;
			String fontText;
			fontText = GetValueStringXml(xp[i], "font-family:");
			if (fontText.Find("Roman") >= 0)
				elem.f.Face(Font::ROMAN);
			else if (fontText.Find("Arial") >= 0)
				elem.f.Face(Font::ARIAL);			
			else if (fontText.Find("Courier") >= 0)
				elem.f.Face(Font::COURIER);		
			elem.f.Height(atoi(GetValueStringXml(xp[i], "font-size:"))*elem.factor);
			if (elem.f.GetHeight() == 0)
				elem.f.Height(12*elem.factor);
		} else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				elem.transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				elem.transf.Scale(args[0], args[1]);			
		}
	}
	elem.SetLimits();
}

void SvgGet_Path(GraphElemList &elems, XmlParser &xp) {
	PathElem &elem = static_cast<PathElem&>(elems.elems.Add(new PathElem()));
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "d") 
			elem.path = xp[i];
		else if (attr == "style") 
			elem.style.Set(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				elem.transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				elem.transf.Scale(args[0], args[1]);			
		}
	}	
	elem.SetLimits();	
}

void SvgGet_G(GraphElemList &elems, XmlParser &xp, const char *svgFolder) {
	GraphElemList &elem = static_cast<GraphElemList&>(elems.elems.Add(new GraphElemList()));
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "style") 
			elem.style.Set(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				elem.transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				elem.transf.Scale(args[0], args[1]);
		}
	}	
	while(!xp.End()) 
		ParseG(elem, xp, elem.transf, elem.style, svgFolder);	
	elem.SetLimits();
}

void ParseG(GraphElemList &elems, XmlParser &xp, Svg2DTransform transf, SvgStyle style, const char *svgFolder) {
	if(xp.TagE("rect"))  
		SvgGet_Rect(elems, xp);
	else if(xp.TagE("ellipse"))
		SvgGet_Ellipse(elems, xp);				
	else if(xp.TagE("line"))
		SvgGet_Line(elems, xp);				
	else if(xp.TagE("polygon"))
		SvgGet_Polygon(elems, xp);				
	else if(xp.TagE("polyline"))
		SvgGet_Polygon(elems, xp);	
	else if(xp.TagE("path"))
		SvgGet_Path(elems, xp);	
	else if(xp.TagE("image"))
		SvgGet_Image(elems, xp, svgFolder);		
	else if(xp.Tag("text")) {
		SvgGet_Text(elems, xp);
		xp.PassEnd();	
	} else if(xp.Tag("g")) 
		SvgGet_G(elems, xp, svgFolder);
	else if(xp.Tag("title")) {
		SvgGet_Title(elems, xp);
		xp.PassEnd();	
	} else
		xp.Skip();
}

bool ParseSVG(DrawingCanvas &canvas, const char *svgFile, const char *svgFolder) {
	XmlParser xp(svgFile);
	while(!xp.IsTag())
		xp.Skip();
	xp.PassTag("svg");
	Svg2DTransform transf;
	SvgStyle style;
	style.SetStrokeFill(Black()).SetStrokeOpacity(1);
	//p.Begin();
	while(!xp.End()) {
		transf.Init();
		style.Init();
		ParseG(canvas.elemList, xp, transf, style, svgFolder);
	}
	//p.End();
	return true;
}


DrawingCanvas::DrawingCanvas() {
	transparent = false;
	translateX = translateY = 0;
	rotate = 0;
	scale = 1;
	
	quality = MODE_ANTIALIASED;	// 1 or 2 MODE_NOAA        = 1, MODE_SUBPIXEL    = 2,
	linejoin = LINEJOIN_MITER;
	linecap = LINECAP_BUTT;
	transparent = false;
	opacity = 1;
	scaleFactor = 1.2;

	focusMove.focusMoving = false;
	selectionWindow.isSelected = false;
}
		
void DrawingCanvas::Paint(Draw& w) {
	Size sz = GetSize();
	if(transparent) {
		for(int y = 0; y + 32 < sz.cy; y += 32)
			for(int x = 0; x + 32 < sz.cx; x += 32)
				w.DrawRect(x, y, 32, 32, (x ^ y) & 32 ? Color(254, 172, 120) : Color(124, 135, 253));
	}
	ImageBuffer ib(sz);
	BufferPainter sw(ib, quality);
	DoPaint(sw);
	if (selectionWindow.isSelected) { 
		sw.Rectangle(selectionWindow.selected.left, selectionWindow.selected.top, 
			selectionWindow.selected.GetWidth(), selectionWindow.selected.GetHeight())
			.Opacity(0.4).Dash("1").Stroke(1, Black()).Fill(LtGray());
		//sw.RectPath(selectionWindow.selected).Dash("1").Stroke(0.5, Black());
	}
	w.DrawImage(0, 0, ib);
}

void DrawingCanvas::DoPaint(Painter& sw) {
	sw.Translate(translateX, translateY);
	sw.Rotate(rotate);
	sw.Scale(scale);
	sw.Opacity(opacity);
	sw.LineCap(linecap);
	sw.LineJoin(linejoin);
	{ PAINTER_TIMING("FILL");
		if(transparent)
			sw.Clear(RGBAZero());
		else
			sw.Clear(White());
	}
	Svg2DTransform trans;
	SvgStyle style;
	style.SetStrokeFill(Null).SetStrokeColor(Black()).SetStrokeOpacity(1).SetStrokeWidth(1);
	
	sw.Begin();
	elemList.Paint(sw, trans, style, true);
	sw.End();
}


bool LoadSvg(DrawingCanvas &canvas, String fileName) {
	if (!FileExists(fileName))
		return false;

	return ParseSVG(canvas, LoadFileBOM(fileName), GetFileDirectory(fileName));
}

void DrawingCanvas::MouseWheel(Point p, int zdelta, dword keyflags) {
	Size sz = GetSize();
	double factor;
	if(zdelta > 0)
		factor = scaleFactor;
	else
		factor = 1/scaleFactor;
	
	scale *= factor;
	translateX = sz.cx*(1-factor)/2. + translateX*factor;
	translateY = sz.cy*(1-factor)/2. + translateY*factor;
	
	Refresh();
}

void DrawingCanvas::MiddleDown(Point p, dword keyflags) {
	focusMove.lastFocusPoint = p;
    focusMove.focusMoving = true;
}

void DrawingCanvas::MiddleUp(Point p, dword keyflags) {
    focusMove.focusMoving = false;
}

void DrawingCanvas::MouseLeave() {
    focusMove.focusMoving = false;
}

void DrawingCanvas::MouseMove(Point p, dword keyflags) {
    if (focusMove.focusMoving) {
     	translateX -= focusMove.lastFocusPoint.x - p.x;
      	translateY -= focusMove.lastFocusPoint.y - p.y;
      	focusMove.lastFocusPoint = p;

        Refresh();
    }
    if (selectionWindow.isSelected) {
      	selectionWindow.selected.right = p.x;
		selectionWindow.selected.bottom = p.y;  
		
		Refresh();
    }
}

void DrawingCanvas::LeftDown(Point p, dword keyflags) {
	selectionWindow.isSelected = true;
	selectionWindow.selected.left = selectionWindow.selected.right = p.x;
	selectionWindow.selected.top = selectionWindow.selected.bottom = p.y;
}

void DrawingCanvas::LeftUp(Point p, dword keyflags) {
	if (selectionWindow.isSelected) {
		Refresh();
		selectionWindow.isSelected = false;
	}
}

PainterCanvas::PainterCanvas() {
	rotate = 0;
	/*translateX = translateY = 0;
	scale = 1;*/
	
	mode = MODE_ANTIALIASED;	// MODE_NOAA, MODE_SUBPIXEL
	linejoin = LINEJOIN_MITER;
	linecap = LINECAP_BUTT;
	opacity = 1;
	scaleFactor = 1.2;
	backColor = Null;
	backImage = Null;
	colorUnderBackgroundImage = false;
	alwaysFitInCanvas = true;
	Layout();
	showWindow = true;
	
	focusMove.focusMoving = false;
}
		
void PainterCanvas::Paint(Draw& w) {
	Size sz = GetSize();
	ImageBuffer ib(sz);
	BufferPainter sw(ib, mode);
	
	sw.Translate(translateX, translateY);
	sw.Rotate(rotate);
	sw.Scale(scale);
	sw.Opacity(opacity);
	sw.LineCap(linecap);
	sw.LineJoin(linejoin);
	//if (/*IsNull(backImage) || */colorUnderBackgroundImage) 
	//	sw.Clear(backColor);
	//else
	//	sw.Clear(RGBAZero());
	if (backColor)
		sw.Clear(backColor);
	else
		sw.Clear(Gray());
	//else {
	//	Color c = Null;
	//	sw.Clear(c);
	//}

	if (canvasSize.cx > 0 && canvasSize.cy > 0) {
		if (!IsNull(backImage)) 
			sw.Rectangle(0, 0, canvasSize.cx, canvasSize.cy).Fill(backImage, 0, 0, canvasSize.cx, 0)
				.Stroke(0, Black());
		
		WhenPaint(sw);
	}
	w.DrawImage(0, 0, ib);	
	
	if (!showWindow/* || canvasSize.cx <= 0 || canvasSize.cy <= 0*/) 
		return;
	
	int twidth = 100;
	int theight = int(twidth*double(canvasSize.cy)/canvasSize.cx); 
	int tx = sz.cx-twidth-20;
	int ty = sz.cy-theight-20;	

	ImageBuffer tib(twidth, theight);
	BufferPainter tsw(tib, mode);
	
	tsw.Scale(double(twidth)/canvasSize.cx);

	if (IsNull(backImage) || colorUnderBackgroundImage) 
		tsw.Clear(backColor);
	 
	if (!IsNull(backImage)) 
		tsw.Rectangle(0, 0, canvasSize.cx, canvasSize.cy).Fill(backImage, 0, 0, canvasSize.cx, 0)
			.Stroke(0, Black());
	
	if (canvasSize.cx > 0 && canvasSize.cy > 0) 
		WhenPaint(tsw);
	
	w.DrawImage(tx, ty, twidth, theight, tib);
	DrawRectLine(w, tx-1, ty-1, twidth+1, theight+1, 1, Black());
	
	double rateRep = double(twidth)/(canvasSize.cy*scale);
	DrawRectLine(w, int(tx-translateX*rateRep-1), int(ty-translateY*rateRep-1), 
				1+int(twidth*sz.cx/((canvasSize.cx - translateX*rateRep)*scale)), 
				1+int(theight*sz.cy/((canvasSize.cy - translateY*rateRep)*scale)), 1, Black());	
}

void PainterCanvas::Layout() {
	if (alwaysFitInCanvas) 
		FitInCanvas();	
}

void PainterCanvas::MouseWheel(Point p, int zdelta, dword keyflags) {
	double factor;
	if(zdelta > 0)
		factor = scaleFactor;
	else
		factor = 1/scaleFactor;
	scale *= factor;
	
	Size sz = GetSize();
	translateX = sz.cx*(1-factor)/2. + translateX*factor;
	translateY = sz.cy*(1-factor)/2. + translateY*factor;
	
	Refresh();
}

void PainterCanvas::FitInCanvas() {
	Rect r = FitInFrame(GetSize(), GetCanvasSize());

	double scalex = GetSize().cx/double(GetCanvasSize().cx);
	double scaley = GetSize().cy/double(GetCanvasSize().cy);
	
	scale = min(scalex, scaley);
	translateX = r.left;
	translateY = r.top;

	Refresh();
}	

void PainterCanvas::MiddleDown(Point p, dword keyflags) {
	focusMove.lastFocusPoint = p;
    focusMove.focusMoving = true;
}

void PainterCanvas::MiddleUp(Point p, dword keyflags) {
    focusMove.focusMoving = false;
}

void PainterCanvas::MouseLeave() {
    focusMove.focusMoving = false;
}

void PainterCanvas::MouseMove(Point p, dword keyflags) {
    if (focusMove.focusMoving) {
     	translateX -= focusMove.lastFocusPoint.x - p.x;
      	translateY -= focusMove.lastFocusPoint.y - p.y;
      	focusMove.lastFocusPoint = p;

        Refresh();
    }
}

void PainterCanvas::SetBackground(Image &image)	{
	backImage = image; 
	if (backImage.GetSize() != canvasSize) {
		SetCanvasSize(backImage.GetSize()); 
		Layout();
	}
}

