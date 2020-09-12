#include "AppWindow.h"

using namespace Upp;

AppWindow::AppWindow()
{
    Title("App Window");
    SetRect(0, 0, 250, 250);
    button.SetLabel("Hello world!");
    button << [=] { OnButtonClick(); };
    Add(button.HSizePos(25, 25).VSizePos(75, 75));
}

void AppWindow::OnButtonClick()
{
	if(PromptYesNo("Button was clicked. Do you want to quit?")) {
	    Break();
	}
}
