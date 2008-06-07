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
	
	void Animate()                        { Refresh(); }

public:
	typedef HelloWorld CLASSNAME;

	HelloWorld& Text(const String& t)     { text = t; Refresh(); return *this; }

	HelloWorld();
};

HelloWorld::HelloWorld()
{
	SetTimeCallback(-40, THISBACK(Animate));
	BackPaint();
	Zoomable().Sizeable();
	SetRect(0, 0, 260, 80);
}

void HelloWorld::LeftDown(Point, dword)
{
	Close();
}

void HelloWorld::Paint(Draw& w)
{
	Size sz = GetSize();
    static int sin_tbl[16] = {
        0, 38, 71, 92, 100, 92, 71, 38, 0, -38, -71, -92, -100, -92, -71, -38
	};
	w.DrawRect(sz, White);
	Size tsz = GetTextSize(text, Roman(32));
	Point pos = (sz - tsz) / 2;
	for(int i = 0; i < text.GetLength(); i++) {
		int q = (i + GetTickCount() / 40) & 15;
		w.DrawText(pos.x, pos.y + sin_tbl[q] * (sz.cy - 32) / 200,
		           ~text + i, Roman(32), HsvColorf(q / 15.0, 1, 0.5), 1);
		pos.x += Roman(32).Info()[text[i]];
	}
}

GUI_APP_MAIN
{
	HelloWorld hw;
	hw.Title("Hello world example");
	hw.Text(Nvl(Join(CommandLine(), " "), "Hello world !"));
	hw.Run();
}
