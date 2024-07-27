#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <ImlUHDDark/Test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <ImlUHDDark/Test.iml>
#include <Draw/iml_source.h>

struct Test : TopWindow {
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		int x = 0;
		for(int mode = 0; mode < 4; mode++) {
			SColorPaper_Write(mode & 1 ? Black() : White());
			SetUHDMode(mode & 2);
			w.DrawRect(x, 0, sz.cx, sz.cy, SColorPaper());
			w.DrawText(x, 0, get_i(mode, "Normal", "Dark", "UHD", "UHD Dark"), StdFont(), LtMagenta());
			int y = 200;
			w.DrawImage(x + 10, y += 40, TestImg::Test1());
			w.DrawImage(x + 10, y += 40, TestImg::Test2());
			w.DrawImage(x + 10, y += 40, TestImg::Test3());
			w.DrawImage(x + 10, y += 40, TestImg::Test4());
			w.DrawImage(x + 10, y += 40, TestImg::Test5());
			w.DrawImage(x + 10, y += 40, TestImg::Test6());
			x += 200;
		}
	}
};

GUI_APP_MAIN
{
	Test().Run();
}
