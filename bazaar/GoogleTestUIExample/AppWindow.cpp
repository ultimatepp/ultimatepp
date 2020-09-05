#include "AppWindow.h"

using namespace Upp;

AppWindow::AppWindow()
{
    Title("App Window");
    SetRect(0, 0, 200, 200);
    button.SetLabel("Hello world!");
    button << [=] { OnClick(); };
    Add(button.HSizePos(25, 25).VSizePos(50, 50));
}

void AppWindow::OnClick()
{
	if(PromptYesNo("Button was clicked. Do you want to quit?")) {
	    Break();
	}
}
