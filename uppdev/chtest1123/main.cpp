#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow
{
	virtual void Paint(Draw& w)
	{
		w.DrawRect(GetSize(), White());
		ChPaint(w, GetSize(), Button::StyleScroll().look[0]);
	}

	App()
	{
	}
};

GUI_APP_MAIN
{
	App().Run();
}
