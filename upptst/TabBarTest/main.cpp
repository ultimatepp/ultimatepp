#include <CtrlLib/CtrlLib.h>
#include <TabBar/TabBar.h>

using namespace Upp;

struct MyApp : TopWindow {
	FileTabs tabs;
	
	MyApp() {
		SetFrame(tabs);
		tabs.MinTabCount(0);
		for(int i = 0; i < 3; i++)
			tabs.AddFile("file.txt");
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
