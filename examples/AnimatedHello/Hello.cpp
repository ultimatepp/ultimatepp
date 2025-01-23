// This is Upp conversion of Qt example, see
// http://doc.trolltech.com/3.0/hello-example.html

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class HelloWorld : public TopWindow {
public:
	virtual void LeftDown(Point, dword);
	virtual void Paint(Draw& w);

private:
	String text;
	
public:
	HelloWorld& Text(const String& t)     { text = t; Refresh(); return *this; }

	HelloWorld();
};

HelloWorld::HelloWorld()
{
	SetTimeCallback(-40, [=] { Refresh(); });
	BackPaint();
	Zoomable().Sizeable();
	SetRect(0, 0, 1000, 300);
}

void HelloWorld::LeftDown(Point, dword)
{
	EditText(text, "Text to display", "Text");
}

void HelloWorld::Paint(Draw& w)
{
	Font fnt = Roman(64);
	Size sz = GetSize();
	w.DrawRect(sz, White);
	Size tsz = GetTextSize(text, fnt);
	Point pos = (sz - tsz) / 2;
	for(int i = 0; i < text.GetLength(); i++) {
		int q = (i + GetTickCount() / 40) & 15;
		w.DrawText(pos.x, pos.y + sin(GetTickCount() / 100.0 + i / 2.0) * (sz.cy - fnt.GetCy()) / 4,
		           ~text + i, fnt, HsvColorf(q / 15.0, 1, 0.5), 1);
		pos.x += fnt[text[i]];
	}
}

GUI_APP_MAIN
{
	HelloWorld hw;
	hw.Title("Hello world example");
	hw.Text(Nvl(Join(CommandLine(), " "), "Hello world !"));
	hw.Run();
}
