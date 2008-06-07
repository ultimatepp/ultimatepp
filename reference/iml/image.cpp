#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define  IMAGEFILE  <iml/image.iml>
#define  IMAGECLASS MyImg
#include <Draw/iml.h>

struct App : TopWindow {
	void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, SGray);
		Size isz = MyImg::Smiley().GetSize();
		w.DrawImage((sz.cx - isz.cx) / 2, (sz.cy - isz.cy) / 2, MyImg::Smiley());
	}

	App()
	{
		Sizeable();
		BackPaint();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
