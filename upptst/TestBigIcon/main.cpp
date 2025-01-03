#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <TestBigIcon/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <TestBigIcon/test.iml>
#include <Draw/iml_source.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	win.LargeIcon(TestImg::Icon());
	win.Run();
}
