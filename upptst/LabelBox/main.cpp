#include "LabelBox.h"

MyApp::MyApp()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	MyApp().Run();
}
