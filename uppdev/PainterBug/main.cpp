#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#define IMAGECLASS Images
#define IMAGEFILE <PainterBug/Pictures.iml>
#include <Draw/iml.h>

void InvRectPath(Painter& pw, const Rect& r)
{
	pw.Move(r.left, r.top).Line(r.left, r.bottom).Line(r.right, r.bottom).Line(r.right, r.top).Close();
}

static void sRenderLine(Painter& w, int x, int y, int dx, int dy, int cx, int cy, int len, byte pattern)
{
	for(int i = 0; i < len; i++)
		if((256 >> (i & 3)) & pattern)
			w.DrawRect(x + dx * i, y + dy * i, cx, cy, Black);
}

static void sRenderRect(Painter& w, const Rect& r, int n, byte pattern)
{
	sRenderLine(w, r.left, r.top, 1, 0, 1, n, r.GetWidth(), pattern);
	sRenderLine(w, r.left, r.bottom - 1, 1, 0, 1, n, r.GetWidth(), pattern);
	sRenderLine(w, r.left, r.top, 0, 1, n, 1, r.GetHeight(), pattern);
	sRenderLine(w, r.right - 1, r.top, 0, 1, n, 1, r.GetHeight(), pattern);
}

// This is Upp conversion of Qt example, see
// http://doc.trolltech.com/3.0/hello-example.html

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class HelloWorld : public TopWindow {
public:
	virtual void LeftDown(Point, dword);
	virtual void Paint(Draw& w);

private:
	int pos;
	int dir;
	String text;
	
	void Animate();

public:
	typedef HelloWorld CLASSNAME;

	HelloWorld& Text(const String& t)     { text = t; Refresh(); return *this; }

	HelloWorld();
};

HelloWorld::HelloWorld()
{
	BackPaint();
	Zoomable().Sizeable();
	pos = 0;
	dir = 1;
}

void HelloWorld::LeftDown(Point, dword)
{
	Close();
}

void HelloWorld::Animate()
{
	pos += dir;
	if(pos <= 0) {
		dir = 1;
		pos = 0;
	}
	if(pos >= 2000) {
		dir = -1;
		pos = 2000;
	}
	Refresh();
}

void HelloWorld::Paint(Draw& w)
{
	ImagePainter iw(50, 50);
//	iw.Clear(RGBAZero());
	iw.DrawImage(0, 0, CtrlImg::exclamation());
	Image m = iw;
	w.DrawRect(GetSize(), Gray);
	w.DrawImage(100, 100, m);
}

GUI_APP_MAIN
{
	HelloWorld hw;
	hw.Title("Hello world example");
	hw.Text(Nvl(Join(CommandLine(), " "), "Hello world !"));
	hw.Run();
}
