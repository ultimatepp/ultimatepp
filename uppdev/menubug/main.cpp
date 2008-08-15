#include "menubug.h"



MenuBug::MenuBug()
{

}

void MenuBug::RightDown(Point pt, dword keyflags)
{
	MenuBar menu;
	menu.Add("Pokus", Callback());
	menu.Execute(pt + GetScreenView().TopLeft());
}

GUI_APP_MAIN
{
	MemoryWatch(0x10408C8);
	MenuBug().Run();
}
