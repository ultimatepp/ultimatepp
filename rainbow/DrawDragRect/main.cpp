#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

void DDRect(RGBA *t, int dir, const byte *pattern, int pos, int count)
{
	while(count-- > 0) {
		byte p = pattern[7 & pos++];
		*t->r ^= p;
		*t->g ^= p;
		*t->b ^= p;
		t += dir;
	}
}

struct App : public Ctrl {
	uint64 pattern;
	int    pos;
	int    a;
	int    dir;

	void Paint(Draw& w)
	{
		DDUMP(pattern);
		Size sz = GetSize();
		w.DrawRect(sz, White());		
		DrawDragRect((SystemDraw&)w,
					 Rect(0, 0, 0, 0),
					 RectC(10, 10, a, a / 3),
					 Size(800, 600),
					 1,
					 Black(),
					 pattern);
	}
	
	virtual Image CursorImage(Point p, dword keyflags) {
		Animate();
		return Image::Arrow();
	}


	void Animate() {
		pattern = I64(0xf0f0) >> (pos++ & 7);
		if(Random() % 100 == 0)
			dir = (int)Random() % 3 - 1;
		a += dir;
		if(a < 0) {
			a = 0;
			dir = -dir;
		}
		if(a > 1000) {
			a = 1000;
			dir = -dir;
		}
//		if(++a > 900)
//			a = 0;
		DrawDragRect(*this,
					 Rect(0, 0, 0, 0),
					 RectC(10, 10, a, a / 3),
					 Size(800, 600),
					 1,
					 Black(),
					 pattern);
		PostCallback(THISBACK(Animate));
	}

	typedef App CLASSNAME;	

	App()
	{
		pattern = I64(9732030564846854595);
		pos = 0;
		a = 200;
		dir = 1;
//		PostCallback(THISBACK(Animate));
		SetCaret(0, 0, 20, 20);
	}
};

GUI_APP_MAIN
{
	App app;
	Ctrl::SetDesktop(app);
	app.SetFocus();
	Ctrl::EventLoop();
}
