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

struct ColorRenderer : DDARasterizer {
	Draw *draw;
	Color color;

	virtual void PutHorz(int x, int y, int cx) {
		draw->DrawRect(x, y, cx, 1, color);
	}
	virtual void PutVert(int x, int y, int cy) {
		draw->DrawRect(x, y, 1, cy, color);
	}
};

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

	w.DrawRect(sz, Gray());
	
	Vector<Rect> clip;
	clip.Add(RectC(50, 50, 10, 10));
	clip.Add(RectC(100, 100, 20, 20));
	clip.Add(RectC(200, 100, 40, 40));
	clip.Add(RectC(200, 100, 40, 40));
	clip.Add(RectC(200, 200, 40, 40));
	clip.Add(RectC(220, 210, 30, 30));

	
	TestDraw fw;
	fw.draw = &w;
	fw.cloff.Add();
	fw.cloff.Top().offset = Point(0, 0);
	for(int i = 0; i < clip.GetCount(); i++) {
		w.DrawRect(clip[i], LtGray());
		fw.cloff.Top().clip.Add(clip[i]);
	}
	
	fw.DrawImage(p.x, p.y, CtrlImg::reporticon(), RectC(0, 0, 20, 12), Blue());
//	fw.DrawRect(p.x, p.y, 16, 16, Red());
}

GUI_APP_MAIN
{
	MyApp().Run();
}
