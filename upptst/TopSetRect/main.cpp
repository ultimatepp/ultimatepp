#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	
	App() {
		SetRect(0, 0, 640, 480);
		Sizeable().Zoomable().CenterScreen();
	}
	
	void Paint(Draw& w) {
		Rect r = GetRect();
		Size sz = GetSize();
		w.DrawRect(sz, White());
		w.DrawText(40, 40, String() << r << " " << sz);
	}

	bool Key(dword key, int count) override
	{
		Rect r = GetRect();

		if(key == K_UP)
			r.OffsetVert(-10);
		else
		if(key == K_DOWN)
			r.OffsetVert(10);
		else
		if(key == K_LEFT)
			r.OffsetHorz(-10);
		else
		if(key == K_RIGHT)
			r.OffsetHorz(10);
		else
		if(key == K_SPACE)
			r.right += 10;	// See especially this. Even this one moves the window downwards on GTK.
		else
		if(key == K_DELETE)
			r = Rect(0, 0, 640, 480);
		else
		if(key == K_INSERT)
			r = Rect(0, 10, 640, 480);

		r.SetSize(Size(640, 480));
		SetRect(r);
		Refresh();
		return true;
	}
};

GUI_APP_MAIN
{
	App().Run();
}
