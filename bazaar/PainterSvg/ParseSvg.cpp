#include "PainterSvg.h"

NAMESPACE_UPP

Color GetSvgColor(const char *color);
String GetValueStringXml(String str, const char* var);
Color GetColorXml(String text);

class Svg2DTransform {
public:
	Svg2DTransform() 						{Init();}
	Svg2DTransform(Svg2DTransform &transf) 	{*this = transf;}
	void Init() {
		transX = transY = 0;
		angle = 0;
		scaleX = scaleY = 1;
	}
	void Apply(Painter &sw) {
		sw.End();
		sw.Begin();
		sw.Translate(transX, transY);
		sw.Scale(scaleX, scaleY);
		sw.Rotate(angle);
	}
	void Translate(double x, double y) {
		transX += x;
		transY += y;
	}
	void Scale(double x, double y) {
		scaleX *= x;
		scaleY *= y;
	}
	void Rotate(double ang) {
		angle += ang;
	}
	Svg2DTransform &operator=(const Svg2DTransform &transf) {
		if (this == &transf)      // Same object?
      		return *this; 
		transX = transf.transX;
		transY = transf.transY;
		angle = transf.angle;
		scaleX = transf.scaleX;
		scaleY = transf.scaleY;
		return *this;
	}
	
private:
	double transX;
	double transY;
	double angle;
	double scaleX;
	double scaleY;
};

class SvgStyle {
public:
	SvgStyle()					{Init();}
	SvgStyle(SvgStyle &transf)  {*this = transf;}
	void Init() {
		strokeWidth = 1;
		strokeFill = Null;
		strokeColor = Black();
		strokeOpacity = 1;
	}
	void Get(String str) {
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
	void Apply(Painter &sw) {
		sw.Opacity(strokeOpacity);	
		if (!strokeFill.IsNullInstance())	
			sw.Fill(strokeFill);				
		sw.Stroke(strokeWidth, strokeColor);
	}
	SvgStyle &operator=(const SvgStyle &style) {
		if (this == &style)      // Same object?
      		return *this; 
		strokeColor = style.strokeColor;
		strokeFill = style.strokeFill;
		strokeOpacity = style.strokeOpacity;
		strokeWidth = style.strokeWidth;
		return *this;
	}
	
public:
	int strokeWidth;
	Color strokeFill;
	Color strokeColor;
	double strokeOpacity;
};

Color GetSvgColor(const char *color) {
	struct Svg_color {
		const char *name;
		int r;
		int g;
		int b;
	} colors[] = {  {"aliceblue", 240, 248, 255}, 
					{"antiquewhite", 250, 235, 215}, 
					{"aqua", 0, 255, 255}, 
					{"aquamarine", 127, 255, 212}, 
					{"azure", 240, 255, 255}, 
					{"beige", 245, 245, 220}, 
					{"bisque", 255, 228, 196}, 
					{"black", 0, 0, 0}, 
					{"blanchedalmond", 255, 235, 205}, 
					{"blue", 0, 0, 255}, 
					{"blueviolet", 138, 43, 226}, 
					{"brown", 165, 42, 42}, 
					{"burlywood", 222, 184, 135}, 
					{"cadetblue", 95, 158, 160}, 
					{"chartreuse", 127, 255, 0}, 
					{"chocolate", 210, 105, 30}, 
					{"coral", 255, 127, 80}, 
					{"cornflowerblue", 100, 149, 237}, 
					{"cornsilk", 255, 248, 220}, 
					{"crimson", 237, 164, 61}, 
					{"cyan", 0, 255, 255}, 
					{"darkblue", 0, 0, 139}, 
					{"darkcyan", 0, 139, 139}, 
					{"darkgoldenrod", 184, 134, 11}, 
					{"darkgray", 169, 169, 169}, 
					{"darkgreen", 0, 100, 0}, 
					{"darkkhaki", 189, 183, 107}, 
					{"darkmagenta", 139, 0, 139}, 
					{"darkolivegreen", 85, 107, 47}, 
					{"darkorange", 255, 140, 0}, 
					{"darkorchid", 153, 50, 204}, 
					{"darkred", 139, 0, 0}, 
					{"darksalmon", 233, 150, 122}, 
					{"darkseagreen", 143, 188, 143}, 
					{"darkslateblue", 72, 61, 139}, 
					{"darkslategray", 47, 79, 79}, 
					{"darkturquoise", 0, 206, 209}, 
					{"darkviolet", 148, 0, 211}, 
					{"deeppink", 255, 20, 147}, 
					{"deepskyblue", 0, 191, 255}, 
					{"dimgray", 105, 105, 105}, 
					{"dodgerblue", 30, 144, 255}, 
					{"firebrick", 178, 34, 34}, 
					{"floralwhite", 255, 250, 240}, 
					{"forestgreen", 34, 139, 34}, 
					{"fuchsia", 255, 0, 255}, 
					{"gainsboro", 220, 220, 220}, 
					{"ghostwhite", 248, 248, 255}, 
					{"gold", 255, 215, 0}, 
					{"goldenrod", 218, 165, 32}, 
					{"gray", 128, 128, 128}, 
					{"green", 0, 128, 0}, 
					{"greenyellow", 173, 255, 47}, 
					{"honeydew", 240, 255, 240}, 
					{"hotpink", 255, 105, 180}, 
					{"indianred", 205, 92, 92}, 
					{"indigo", 75, 0, 130}, 
					{"ivory", 255, 255, 240}, 
					{"khaki", 240, 230, 140}, 
					{"lavender", 230, 230, 250}, 
					{"lavenderblush", 255, 240, 245}, 
					{"lawngreen", 124, 252, 0}, 
					{"lemonchiffon", 255, 250, 205}, 
					{"lightblue", 173, 216, 230}, 
					{"lightcoral", 240, 128, 128}, 
					{"lightcyan", 224, 255, 255}, 
					{"lightgoldenrodyellow", 250, 250, 210}, 
					{"lightgrey", 211, 211, 211}, 
					{"lightgreen", 144, 238, 144}, 
					{"lightpink", 255, 182, 193}, 
					{"lightsalmon", 255, 160, 122}, 
					{"lightseagreen", 32, 178, 170}, 
					{"lightskyblue", 135, 206, 250}, 
					{"lightslategray", 119, 136, 153}, 
					{"lightsteelblue", 176, 196, 222}, 
					{"lightyellow", 255, 255, 224}, 
					{"lime", 0, 255, 0}, 
					{"limegreen", 50, 205, 50}, 
					{"linen", 250, 240, 230}, 
					{"magenta", 255, 0, 255}, 
					{"maroon", 128, 0, 0}, 
					{"mediumaquamarine", 102, 205, 170}, 
					{"mediumblue", 0, 0, 205}, 
					{"mediumorchid", 186, 85, 211}, 
					{"mediumpurple", 147, 112, 219}, 
					{"mediumseagreen", 60, 179, 113}, 
					{"mediumslateblue", 123, 104, 238}, 
					{"mediumspringgreen", 0, 250, 154}, 
					{"mediumturquoise", 72, 209, 204}, 
					{"mediumvioletred", 199, 21, 133}, 
					{"midnightblue", 25, 25, 112}, 
					{"mintcream", 245, 255, 250}, 
					{"mistyrose", 255, 228, 225}, 
					{"moccasin", 255, 228, 181}, 
					{"navajowhite", 255, 222, 173}, 
					{"navy", 0, 0, 128}, 
					{"oldlace", 253, 245, 230}, 
					{"olive", 128, 128, 0}, 
					{"olivedrab", 107, 142, 35}, 
					{"orange", 255, 165, 0}, 
					{"orangered", 255, 69, 0}, 
					{"orchid", 218, 112, 214}, 
					{"palegoldenrod", 238, 232, 170}, 
					{"palegreen", 152, 251, 152}, 
					{"paleturquoise", 175, 238, 238}, 
					{"palevioletred", 219, 112, 147}, 
					{"papayawhip", 255, 239, 213}, 
					{"peachpuff", 255, 218, 185}, 
					{"peru", 205, 133, 63}, 
					{"pink", 255, 192, 203}, 
					{"plum", 221, 160, 221}, 
					{"powderblue", 176, 224, 230}, 
					{"purple", 128, 0, 128}, 
					{"red", 255, 0, 0}, 
					{"rosybrown", 188, 143, 143}, 
					{"royalblue", 65, 105, 225}, 
					{"saddlebrown", 139, 69, 19}, 
					{"salmon", 250, 128, 114}, 
					{"sandybrown", 244, 164, 96}, 
					{"seagreen", 46, 139, 87}, 
					{"seashell", 255, 245, 238}, 
					{"sienna", 160, 82, 45}, 
					{"silver", 192, 192, 192}, 
					{"skyblue", 135, 206, 235}, 
					{"slateblue", 106, 90, 205}, 
					{"slategray", 112, 128, 144}, 
					{"snow", 255, 250, 250}, 
					{"springgreen", 0, 255, 127}, 
					{"steelblue", 70, 130, 180}, 
					{"tan", 210, 180, 140}, 
					{"teal", 0, 128, 128}, 
					{"thistle", 216, 191, 216}, 
					{"tomato", 255, 99, 71}, 
					{"turquoise", 64, 224, 208}, 
					{"violet", 238, 130, 238}, 
					{"wheat", 245, 222, 179}, 
					{"white", 255, 255, 255}, 
					{"whitesmoke", 245, 245, 245}, 
					{"yellow", 255, 255, 0}, 
					{"yellowgreen", 154, 205, 50}, 
					{""}};
	
	for (int i = 0; colors[i].name[0] != '\0'; ++i)		// Naive algorithm to get the color
		if (strcmp(color, colors[i].name) == 0) 
			return Color(colors[i].r, colors[i].g, colors[i].b);
	return Null;
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

void SvgPaint_Rect(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	int x, y, width, height;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x") 
			x = atoi(xp[i]);
		else if (attr == "y") 
			y = atoi(xp[i]);
		else if (attr == "width") 
			width = atoi(xp[i]);
		else if (attr == "height") 
			height = atoi(xp[i]);
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				transf.Scale(args[0], args[1]);			
		}
	}				
	transf.Apply(sw);
	sw.Rectangle(x, y, width, height);	
	style.Apply(sw);
}

void SvgPaint_Image(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style, 
					const char *svgFolder) {
	int x, y, width, height;
	String fileName;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x") 
			x = atoi(xp[i]);
		else if (attr == "y") 
			y = atoi(xp[i]);
		else if (attr == "width") 
			width = atoi(xp[i]);
		else if (attr == "height") 
			height = atoi(xp[i]);
		else if (attr == "xlink:href") 
			fileName = xp[i];
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				transf.Scale(args[0], args[1]);			
		}
	}				
	if (!FileExists(fileName))
		fileName = AppendFileName(svgFolder, fileName);
	transf.Apply(sw);
	sw.Rectangle(x, y, width, height).Fill(StreamRaster::LoadFileAny(fileName), x, y, width, 0).Stroke(0, Black());
	style.Apply(sw);
}

void SvgPaint_Ellipse(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	int x, y, width, height;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "cx") 
			x = atoi(xp[i]);
		else if (attr == "cy") 
			y = atoi(xp[i]);
		else if (attr == "rx") 
			width = atoi(xp[i]);
		else if (attr == "ry") 
			height = atoi(xp[i]);
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				transf.Scale(args[0], args[1]);			
		}
	}							
	transf.Apply(sw);
	sw.Ellipse(x, y, width, height);	
	style.Apply(sw);
}

void SvgPaint_Line(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	int x1, y1, x2, y2;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x1") 
			x1 = atoi(xp[i]);
		else if (attr == "y1") 
			y1 = atoi(xp[i]);
		else if (attr == "x2") 
			x2 = atoi(xp[i]);
		else if (attr == "y2") 
			y2 = atoi(xp[i]);
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				transf.Scale(args[0], args[1]);			
		}
	}				
	transf.Apply(sw);	
	sw.Move(x1, y1);
	sw.Line(x2, y2);	
	style.Apply(sw);
}

Array<Point> GetPolygonPointsXml(String &strpoints) {
	Array<Point> points;
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
		Point &point = points.Add();
		point.x = x;
		point.y = y;
	}
	return points;
}

void SvgPaint_Polygon(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	Array<Point> points;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "points") {
			String strpoints;
			strpoints = NormalizeSpaces(xp[i]);
			points = GetPolygonPointsXml(strpoints);
		} else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				transf.Scale(args[0], args[1]);			
		}
	}			
	transf.Apply(sw);
	sw.Move(points[0].x, points[0].y);
	for (int i = 1; i < points.GetCount(); ++i)				
		sw.Line(points[i].x, points[i].y);
	style.Apply(sw);
}

void SvgPaint_Text(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	int x, y;
	String font;
	int factor = 20;			// Factor to get better resolution text
	
	Font f(Font::ARIAL, 12);
	StringBuffer stext = xp.ReadText();
	
	int i, j;
	for (i = j = 0; i < stext.GetCount(); ++i, ++j) {
		if (stext[i] == '\n')
			stext[j] = ' ';	
		else if (stext[i] == '\r')
			--j;
		else
			stext[j] = stext[i];
	}
	stext.SetCount(j);
	String text = stext;
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x") 
			x = atoi(xp[i]);
		else if (attr == "y") 
			y = atoi(xp[i]);
		else if (attr == "style") { 
			style.Get(xp[i]);
			
			if (style.strokeFill.IsNullInstance())
				style.strokeFill = style.strokeColor;
			String fontText;
			fontText = GetValueStringXml(xp[i], "font-family:");
			if (fontText.Find("Roman") >= 0)
				f.Face(Font::ROMAN);
			else if (fontText.Find("Arial") >= 0)
				f.Face(Font::ARIAL);			
			else if (fontText.Find("Courier") >= 0)
				f.Face(Font::COURIER);		
			f.Height(atoi(GetValueStringXml(xp[i], "font-size:"))*factor);
			if (f.GetHeight() == 0)
				f.Height(12*factor);
		} else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				transf.Scale(args[0], args[1]);			
		}
	}
	transf.Translate(x, y-f.GetHeight()/factor);
	transf.Scale(1./factor, 1./factor);						
	transf.Apply(sw);	
	sw.Text(0, 0, text, f);
	style.Apply(sw);
}

void SvgPaint_Path(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle &style) {
	String path;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "d") 
			path = xp[i];
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				transf.Scale(args[0], args[1]);			
		}
	}		
	transf.Apply(sw);	
	if (!path.IsEmpty())
		sw.Path(path);
	style.Apply(sw);
}

void SvgPaint_G(Painter& sw, XmlParser &xp, Svg2DTransform &transf, SvgStyle &style) {
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				transf.Scale(args[0], args[1]);
		}
	}		
}

void ParseG(Painter& p, XmlParser &xp, Svg2DTransform transf, SvgStyle style, const char *svgFolder) {
	if(xp.TagE("rect")) 
		SvgPaint_Rect(p, xp, transf, style);
	else if(xp.TagE("ellipse"))
		SvgPaint_Ellipse(p, xp, transf, style);				
	else if(xp.TagE("line"))
		SvgPaint_Line(p, xp, transf, style);				
	else if(xp.TagE("polygon"))
		SvgPaint_Polygon(p, xp, transf, style);				
	else if(xp.TagE("polyline"))
		SvgPaint_Polygon(p, xp, transf, style);	
	else if(xp.TagE("path"))
		SvgPaint_Path(p, xp, transf, style);	
	else if(xp.TagE("image"))
		SvgPaint_Image(p, xp, transf, style, svgFolder);		
	else if(xp.Tag("text")) {
		SvgPaint_Text(p, xp, transf, style);
		xp.PassEnd();	
	} else if(xp.Tag("g")) {
		Svg2DTransform transfG(transf);
		SvgStyle styleG(style);
		
		SvgPaint_G(p, xp, transfG, styleG);
					
		while(!xp.End()) 
			ParseG(p, xp, transfG, styleG, svgFolder);
	} else
		xp.Skip();
}

void ParseSVG(Painter& p, const char *svgFile, const char *svgFolder) {
	XmlParser xp(svgFile);
	while(!xp.IsTag())
		xp.Skip();
	xp.PassTag("svg");
	Svg2DTransform transf;
	SvgStyle style;
	p.Begin();
	while(!xp.End()) {
		transf.Init();
		style.Init();
		ParseG(p, xp, transf, style, svgFolder);
	}
	p.End();
}

END_UPP_NAMESPACE
