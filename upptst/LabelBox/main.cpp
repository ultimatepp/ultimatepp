#include "LabelBox.h"

MyApp::MyApp()
{
	CtrlLayout(*this, "Window title");
	center.SetAlign(ALIGN_CENTER);
	right.SetAlign(ALIGN_RIGHT);
}

GUI_APP_MAIN
{
	MyApp().Run();
}
