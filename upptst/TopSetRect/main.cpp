#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	
	App() {
		SetRect(0, 0, 640, 480);
		Sizeable().Zoomable().CenterScreen();
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

		SetRect(r);
		return true;
	}
};

GUI_APP_MAIN
{
	App().Run();
}
