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
	win.Title("TEST");
	win.LargeIcon(TestImg::Icon());
	DLOG("*********** OPEN MAIN");
	win.OpenMain();
	DLOG("*********** SLEEP 500");
	Sleep(5000);
	DLOG("*********** RUN");
	win.Run();
}
