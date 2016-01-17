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
		w.DrawImage(0, 0, MyImg::Smiley());
		w.DrawImage(isz.cx, 0, MyImg::Get("Smiley"));
		w.DrawImage(0, isz.cy, GetImlImage("MyImg:Smiley"));
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
