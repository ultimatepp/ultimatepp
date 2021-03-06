#include "ScatterDraw.h"

namespace Upp {
	
inline bool Even(int val)	  	{return !(val%2);}

Vector <double> GetLineDash(String dash) {
	Vector<double> d;
	CParser p(dash);
	try {
		while(!p.IsEof())
			if(!p.Char(':')) {
				double data = p.ReadDouble();
				if (data > 0)
					d << data;
			}
	}
	catch(CParser::Error) {}

	if(d.GetCount() & 1) {
		Vector<double> dash1;
		dash1.Append(d);
		dash1.Append(d);
		return dash1;
	}		
	return d;
}

double GetDashLength(const char *dash) {
	double len = 0;
	CParser p(dash);
	try {
		while(!p.IsEof())
			if(!p.Char(':')) {
				double data = p.ReadDouble();
				len += data;
			}
	}
	catch(CParser::Error) {}
	if (len == 0)
		len = 100000;
	return len-1;
}

Vector <double> &GetDashedArray(String dash) {
	static VectorMap <String, Vector <double> > pats;
	
	int pos = pats.Find(dash);
	if (pos < 0) {
		pats.Add(dash, GetLineDash(dash));
		pos = pats.GetCount()-1;
	} 
	return pats.GetValues()[pos];
}

void ScatterDraw::ParseTextMultiline(const String &text, Upp::Font &fnt, Upp::Vector <String> &texts, 
		Upp::Vector <Size> &sizes) {
	Size ret(0, 0);
	int npos = 0;
	for (int pos = 0; npos != -1; pos = npos+1) {
		npos = text.Find('\n', pos);
		String &t = texts.Add();
		if (npos != -1)
			t = text.Mid(pos, npos-pos);
		else
			t = text.Mid(pos);
		Size &s = sizes.Add();
		s.cx = GetTextSize(t, fnt).cx;
		s.cy = GetTextSize(t, fnt).cy;
	}
}

Size GetTextSizeMultiline(Upp::Vector <Size> &sizes) {
	Size ret(0, 0);
	for (int i = 0; i < sizes.GetCount(); ++i) {
		if (sizes[i].cx > ret.cx)
			ret.cx = sizes[i].cx;
		ret.cy += sizes[i].cy;
	}
	return ret;
}

void DrawLine(Draw &w, double x0, double y0, double x1, double y1, double width, const Color &color) {
	w.DrawLine(int(x0), int(y0), int(x1), int(y1), int(width), color);
}

void DrawLine(Painter &w, double x0, double y0, double x1, double y1, double width, const Color &color) {
	w.Move(x0, y0).Line(x0, y0).Line(x1, y1).Stroke(width, color);
}

void DrawVArrow(Draw &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color) {
	DrawLine(w, x0, y0, x1, y1, width, color);
	if (fabs(y0 - y1) > 2*aHeight) {
		if (x0 > x1)
			Swap(x0, x1);
		if (y0 > y1)
			Swap(y0, y1);
		Vector <Point> arrowU, arrowL;
		arrowU << Point(int(x0), int(y0)) << Point(int(x0-aWidth), int(y0+aHeight)) << Point(int(x0+aWidth), int(y0+aHeight));
		w.DrawPolygon(arrowU, SColorHighlight());
		arrowL << Point(int(x0), int(y1)) << Point(int(x0-aWidth), int(y1-aHeight)) << Point(int(x0+aWidth), int(y1-aHeight));
		w.DrawPolygon(arrowL, SColorHighlight());
	}
}

void DrawVArrow(Painter &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color) {
	DrawLine(w, x0, y0, x1, y1, width, color);
	if (fabs(y0 - y1) > 2*aHeight) {
		if (x0 > x1)
			Swap(x0, x1);
		if (y0 > y1)
			Swap(y0, y1);
		w.Move(x0, y0).Line(x0-aWidth, y0+aHeight).Line(x0+aWidth, y0+aHeight).Fill(SColorHighlight());
		w.Move(x0, y1).Line(x0-aWidth, y1-aHeight).Line(x0+aWidth, y1-aHeight).Fill(SColorHighlight());
	}
}

void DrawHArrow(Draw &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color) {
	DrawLine(w, x0, y0, x1, y1, width, color);
	if (fabs(x0 - x1) > 2*aHeight) {
		if (x0 > x1)
			Swap(x0, x1);
		if (y0 > y1)
			Swap(y0, y1);
		Vector <Point> arrowL, arrowR;
		arrowL << Point(int(x0), int(y0)) << Point(int(x0+aHeight), int(y0+aWidth)) << Point(int(x0+aHeight), int(y0-aWidth));
		w.DrawPolygon(arrowL, SColorHighlight());
		arrowR << Point(int(x1), int(y0)) << Point(int(x1-aHeight), int(y0+aWidth)) << Point(int(x1-aHeight), int(y0-aWidth));
		w.DrawPolygon(arrowR, SColorHighlight());
	}
}

void DrawHArrow(Painter &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color) {
	DrawLine(w, x0, y0, x1, y1, width, color);
	if (fabs(x0 - x1) > 2*aHeight) {
		if (x0 > x1)
			Swap(x0, x1);
		if (y0 > y1)
			Swap(y0, y1);
		w.Move(x0, y0).Line(x0+aHeight, y0+aWidth).Line(x0+aHeight, y0-aWidth).Fill(SColorHighlight());
		w.Move(x1, y0).Line(x1-aHeight, y0+aWidth).Line(x1-aHeight, y0-aWidth).Fill(SColorHighlight());
	}
}

void DrawText(Draw &w, double x, double y, int angle, const String &text, Upp::Font font, Color color) {
	Vector<String> str = Split(text, '\n');
	int h = font.GetHeight() + font.GetDescent();
	for (int i = 0; i < str.GetCount(); ++i)
		w.DrawText(int(x), int(y + h*i), angle, str[i], font, color);
}

void DrawText(Painter &w, double x, double y, int angle, const String &text, Upp::Font font, Color color) {
	Vector<String> str = Split(text, '\n');
	int h = font.GetHeight() + font.GetDescent();
	w.Begin();
	w.Translate(x, y).Rotate(-angle*M_PI/1800.);
	for (int i = 0; i < str.GetCount(); ++i) {
		w.Text(0, 0, str[i], font).Fill(color);
		w.Translate(0, h);
	}
	w.End();
}

void Clip(Draw &w, double x, double y, double cx, double cy) {
	w.Clip(int(x), int(y), int(cx), int(cy));	
}

void Clip(Painter &w, double x, double y, double cx, double cy) {
	w.Rectangle(x, y, cx, cy).Clip();	
}

void ClipEnd(Draw &w) {
	w.End();	
}

void ClipEnd(Painter &) {
	;	
}

void DrawLineOpa(Draw& w, double x0, double y0, double x1, double y1, double scale, double opacity, 
				double thick, const Color &_color, String dash, const Color &background) {
	Vector<Pointf> p;
	p << Pointf(x0, y0) << Pointf(x1, y1);
	DrawPolylineOpa(w, p, scale, opacity, thick, _color, dash, background);
}

void DrawCircleOpa(Draw& w, double x, double y, double r, double scale, double opacity, 
				double thick, const Color &_color, String dash, const Color &background) {
	Vector<Pointf> p;
	for (double ang = 0; ang <= 2*M_PI; ang += 2*M_PI/50) 
		p << Pointf(int(x + r*cos(ang)), int(y + r*sin(ang)));
	DrawPolylineOpa(w, p, scale, opacity, thick, _color, dash, background);
}

void DashScaled(Painter& w, const String dash, double scale) {
	if (!dash.IsEmpty()) {		
		Vector<double> d;
		double start = 0;
		CParser p(dash);
		try {
			while(!p.IsEof())
				if(p.Char(':'))
					start = p.ReadDouble();
				else
					d.Add(scale*p.ReadDouble());
		}
		catch(CParser::Error) {}
		w.Dash(d, scale*start);
	}
}

void DrawLineOpa(Painter& w, double x0, double y0, double x1, double y1, double scale, 
				double opacity, double thick, const Color &color, String dash, 
				const Color &) {	
	w.Move(Pointf(x0, y0));
	w.Line(Pointf(x1, y1));
	DashScaled(w, dash, scale);
	w.Opacity(opacity);				// Before Stroke()
	w.Stroke(thick*scale, color);
}

void DrawCircleOpa(Painter& w, double x, double y, double r, double scale, 
				double opacity, double thick, const Color &color, String dash, 
				const Color &) {	
	w.Circle(x, y, r);
	DashScaled(w, dash, scale);
	w.Opacity(opacity);				// Before Stroke()
	w.Stroke(thick*scale, color);
}

void FillRectangleOpa(Draw &w, double x0, double y0, double x1, double y1,  
					double opacity, const Color &background, const Color &color) {
	if (IsNull(color))
		return;
	Color opacolor = GetOpaqueColor(color, background, opacity);
	if (x0 > x1) 
		Swap(x0, x1);
	if (y0 > y1) 
		Swap(y0, y1);
	w.DrawRect(int(x0), int(y0), abs(int(x1 - x0)), abs(int(y1 - y0)), opacolor);
}

void FillRectangleOpa(Painter &w, double x0, double y0, double x1, double y1,  
					double opacity, const Color &, const Color &color) {
	if (IsNull(color))
		return;
	w.Rectangle(x0, y0, x1 - x0, y1 - y0).Opacity(opacity).Fill(color);
}

void FillRectangle(Draw &w, double x0, double y0, double x1, double y1, const Color &color) {
	if (IsNull(color))
		return;
	w.DrawRect(int(x0), int(y0), int(x1 - x0), int(y1 - y0), color);
}

void FillRectangle(Painter &w, double x0, double y0, double x1, double y1, const Color &color) {
	if (IsNull(color))
		return;
	w.Rectangle(x0, y0, x1 - x0, y1 - y0).Fill(color);
}
						
void DrawRectangle(Draw &w, double x0, double y0, double x1, double y1, double scale, 
					double width, const Color &color) {
	int ix0 = int(x0);
	int iy0 = int(y0);
	int ix1 = int(x1);
	int iy1 = int(y1);
	int thick = int(width*scale);
	w.DrawLine(ix0, iy0, ix1, iy0, thick, color);
	w.DrawLine(ix1, iy0, ix1, iy1, thick, color);
	w.DrawLine(ix1, iy1, ix0, iy1, thick, color);
	w.DrawLine(ix0, iy1, ix0, iy0, thick, color);
}

void DrawRectangle(Painter &w, double x0, double y0, double x1, double y1, double scale, 
					double width, const Color &color) {
	w.Move(Pointf(x0, y0)).Line(Pointf(x1, y0)).Line(Pointf(x1, y1))
	 .Line(Pointf(x0, y1)).Line(Pointf(x0, y0)).Stroke(width*scale, color);
}
	
Pointf PointAtLen(const Pointf &p0, const Pointf &p1, double len) {
	Pointf ret;
	if (p1.y == p0.y) {
		ret.x = p0.x + ((p0.x < p1.x) ? len : -len);
		ret.y = p0.y;
	} else if (p1.x == p0.x) {
		ret.x = p0.x;
		ret.y = p0.y + ((p0.y < p1.y) ? len : -len);
	} else {
		double k = sqr((p1.x-p0.x)/(p1.y-p0.y));
		double deltax = len/sqrt(1/k + 1);
		double deltay = len/sqrt(k + 1);
		ret.x = p0.x + ((p0.x < p1.x) ? deltax : -deltax);
		ret.y = p0.y + ((p0.y < p1.y) ? deltay : -deltay);
	}
	return ret;
}

void DrawPolylineOpa(Draw& w, const Vector<Pointf> &p, double scale, double opacity, 
				double thick, const Color &_color, String dash, const Color &background) {
	ASSERT(!p.IsEmpty());
	Color color = GetOpaqueColor(_color, background, opacity) ;
	if (dash == LINE_SOLID) {
		//w.DrawPolyline(p, int(thick*scale), color);
		for (int i = 1; i < p.GetCount(); ++i) {
			if (!(IsNull(p[i-1]) || IsNull(p[i])))
				w.DrawLine(p[i-1], p[i], int(thick*scale), color);
		}
	} else {
		Vector <double> &pat = GetDashedArray(dash);
		if (pat.IsEmpty())
			return;
		int iPat = 0; 
		
		double len = pat[0]*scale;			// Pixels per bar
		Pointf begin, end;
		begin = p[0];
		for (int i = 1; i < p.GetCount();) {
			if (IsNull(begin) || IsNull(p[i])) 
				i++;
			else {
				double d = Distance(begin, p[i]);
				if (d >= len) 
					end = PointAtLen(begin, p[i], len);
				else {
					end = p[i];
					len -= d;
					++i;
				}
				if (Even(iPat)) 
					w.DrawLine(begin, end, int(thick*scale), color);
				if (d >= len) {
					iPat++;
					if (iPat == pat.GetCount())
						iPat = 0;				 
					len = pat[iPat]*scale;
				}
				begin = end;
			}
		}
	}		
}

void DrawPolylineOpa(Painter& w, const Vector<Pointf> &p, double scale, double opacity, 
				double thick, const Color &color, String dash, const Color &) {	
	ASSERT(!p.IsEmpty());
	bool broken = true;
	for (int i = 0; i < p.GetCount(); ++i) {
		if (IsNull(p[i]))
			broken = true;		
		else {
			if (broken) {
				w.Move(p[i]);
				broken = false;
			} else			
				w.Line(p[i]);
		}
	}
	DashScaled(w, dash, scale);
	w.Opacity(opacity);				// Before Stroke()
	w.Stroke(thick*scale, color);
}

void FillPolylineOpa(Draw& w, const Vector<Pointf> &p, double , double opacity, 
				const Color &background, const Color &fillColor) {
	ASSERT(!p.IsEmpty());
	Color opacolor = GetOpaqueColor(fillColor, background, opacity) ;

	Vector<Point> pi;
	pi.SetCount(p.GetCount());
	for (int i = 0; i < pi.GetCount(); ++i) {
		pi[i].x = static_cast<int>(p[i].x);
		pi[i].y = static_cast<int>(p[i].y);
	}
	w.DrawPolygon(pi, opacolor);
}

void FillPolylineOpa(Painter& w, const Vector<Pointf> &p, double , double opacity, 
				const Color &, const Color &fillColor) {	
	ASSERT(!p.IsEmpty());
	w.Move(p[0]);
	for (int i = 1; i < p.GetCount(); ++i) 
		w.Line(p[i]);
	w.Opacity(opacity);
	w.Fill(fillColor);		// Before Stroke()
}

Size GetTextSizeSpace(const String& text, Font font) {
	Size ret(0, 0);
	WString wtext(text);
	for (int i = 0; i < wtext.GetCount(); ++i)  
		ret.cx += font.GetLeftSpace(wtext[i]) + font.GetWidth(wtext[i]);
	ret.cy = font.GetHeight() + font.GetDescent();
	return ret;
}

}