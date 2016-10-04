#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>
#include "glyph.h"

using namespace Upp;

#define IMAGECLASS GlyphImg
#define IMAGEFILE <Glyph/glyph.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS GlyphImg
#define IMAGEFILE <Glyph/glyph.iml>
#include <Draw/iml_source.h>


struct MyApp : TopWindow {
	Point p;
	int   width;
	
	virtual void Paint(Draw& w);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
	
	MyApp() { width = 2; p = Point(0, 0); Sizeable().Zoomable(); }
};

Image MyApp::CursorImage(Point p, dword keyflags)
{
	return Image::Arrow();
	
	static Image img;
	ONCELOCK {
		SImageDraw w(64, 64);
		w.DrawRect(0, 0, 64, 64, White());
		w.DrawText(2, 2, "A", Roman(60), Black());
		for(int x = -2; x <= 2; x++)
			for(int y = -2; y <= 2; y++)
				w.Alpha().DrawText(2 + x, 2 + y, "A", Roman(60), White());
		img = w;
	}
	return img;
	return GlyphImg::cursor1();
}

void MyApp::MouseMove(Point p_, dword keyflags)
{
	p = p_;
	Refresh();
}

void MyApp::MouseWheel(Point p, int zdelta, dword keyflags)
{
	width = minmax(1, 1000, width + sgn(zdelta));
	Title(AsString(width));
	Refresh();
}

void PolyPolyPolygon(Draw& sw)
{
	Point points[12][2][4];
	int subpolygon_counts[12][2];
	int disjunct_counts[12];
	Point lpoints[12][5];
	int lcounts[12][2];
	Pointf center(400, 300);
	static const double out_delta = 2 * M_PI / 24;
	static const double in_delta = 2 * M_PI / 48;
	for(int i = 0; i < 12; i++) {
		double angle = i * 2 * M_PI / 12;
		points[i][0][0] = Polar(center, 200, angle);
		points[i][0][1] = Polar(center, 120, angle - out_delta);
		points[i][0][2] = Polar(center, 40, angle);
		points[i][0][3] = Polar(center, 120, angle + out_delta);
		points[i][1][0] = Polar(center, 160, angle);
		points[i][1][1] = Polar(center, 120, angle + in_delta);
		points[i][1][2] = Polar(center, 80, angle);
		points[i][1][3] = Polar(center, 120, angle - in_delta);
		subpolygon_counts[i][0] = 4;
		subpolygon_counts[i][1] = 4;
		disjunct_counts[i] = subpolygon_counts[i][0] + subpolygon_counts[i][1];
		
		lpoints[i][0] = Polar(center, 260, angle + in_delta);
		lpoints[i][1] = Polar(center, 230, angle);
		lpoints[i][2] = Polar(center, 260, angle - in_delta);
		lcounts[i][0] = 3;
		lpoints[i][3] = Polar(center, 280, angle + in_delta);
		lpoints[i][4] = Polar(center, 280, angle - in_delta);
		lcounts[i][1] = 2;
	}
	sw.DrawPolyPolyPolygon(points[0][0], sizeof(points) / sizeof(Point),
		subpolygon_counts[0], sizeof(subpolygon_counts) / sizeof(int),
		disjunct_counts, sizeof(disjunct_counts) / sizeof(int),
		LtBlue(), 4, LtRed(), I64(0xF0F0F0F00F0F0F0F));
	sw.DrawPolyPolyline(lpoints[0], sizeof(lpoints) / sizeof(Point),
		lcounts[0], sizeof(lcounts) / sizeof(int),
		4, Color(0, 192, 0));
}

void MyApp::Paint(Draw& w)
{
	Size sz = GetSize();

	w.DrawRect(sz, White());
	
	Vector<Rect> clip;

	clip.Add(RectC(50, 50, 10, 10));
	clip.Add(RectC(100, 100, 20, 20));
	clip.Add(RectC(200, 100, 40, 40));
	clip.Add(RectC(200, 100, 40, 40));
	clip.Add(RectC(200, 200, 40, 40));
	clip.Add(RectC(220, 210, 30, 30));

	SeedRandom();
	for(int i = 0; i < 20; i++)
		clip.Add(RectC(Random(sz.cx), Random(sz.cy), Random(100), Random(100)));

//	clip.Add(sz);

	TestDraw fw;
	fw.draw = &w;
	fw.Init(sz);
	
//	fw.DrawText(100, 100, "Ahoj!", Roman(400));

	PolyPolyPolygon(fw);

	RichText txt = ParseQTF(LoadFile(GetDataFile("text.qtf")));
	if(1) {
		RTIMING("SDraw");
		txt.Paint(Zoom(2, 10), fw, 0, 0, sz.cx);
	}
	if(0) {
		RTIMING("Draw");
		txt.Paint(Zoom(2, 10), w, 0, 0, sz.cx);
	}
	if(1) {
		RTIMING("Painter");
		ImageBuffer ib(sz);
		BufferPainter sw(ib);
		sw.Clear(White());
		txt.Paint(Zoom(2, 10), sw, 0, 0, sz.cx);
	}
	
/*
	fw.cloff.Add();
	fw.cloff.Top().offset = Point(0, 0);
	for(int i = 0; i < clip.GetCount(); i++) {
		w.DrawRect(clip[i], LtGray());
		fw.cloff.Top().clip.Add(clip[i]);
	}
*/
//	String txt = LoadFile(GetDataFile("text.qtf"));
//	ParseQTF(txt).Paint(Zoom(2, 10), fw, 0, 0, sz.cx);

//	fw.DrawText(sz.cx / 2, sz.cy / 2, -3600 * Bearing(p - sz / 2) / M_2PI,
//	            "Hello world! abcdefghijklmnopqrstuv", Roman(40).Underline(), Red());
	fw.DrawImage(p.x, p.y, CtrlImg::reporticon(), RectC(0, 0, 20, 12), Blue());
}

GUI_APP_MAIN
{
	MyApp().Run();
}
