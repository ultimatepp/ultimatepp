#include <CtrlLib/CtrlLib.h>

using namespace Upp;

void HotKey1()
{
	PromptOK("HotKey1");
}

void HotKey2()
{
	PromptOK("HotKey2");
}

GUI_APP_MAIN
{
	TopWindow win;
	int id = Ctrl::RegisterSystemHotKey(K_CTRL|K_F1, callback(HotKey1));
	Ctrl::RegisterSystemHotKey(K_ALT|K_CTRL|K_SHIFT|K_A, callback(HotKey2));
	win.Run();
	Ctrl::UnregisterSystemHotKey(id);
	win.Run();
}
