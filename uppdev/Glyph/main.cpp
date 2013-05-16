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
	fw.Cy(sz.cy);
	fw.draw = &w;
	fw.Init(sz);
	
	fw.DrawText(0, 0, "Ahoj!", Roman(400));
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
