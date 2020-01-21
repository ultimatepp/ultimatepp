#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	int id = Ctrl::RegisterSystemHotKey(K_CTRL|K_F1, [] { PromptOK("Ctrl+F1"); });
	Ctrl::RegisterSystemHotKey(K_ALT|K_CTRL|K_SHIFT|K_A, [] { PromptOK("Ctrl+Alt+Shift+A"); });
	win.Run();
	Ctrl::UnregisterSystemHotKey(id);
	win.Run();
}
