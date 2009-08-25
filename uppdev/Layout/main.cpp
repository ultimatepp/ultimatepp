#include "Layout.h"

Layout::Layout()
{
	CtrlLayoutOK(*this, "Test");
	dl.Tip("Tip!");
}

GUI_APP_MAIN {
	Layout().Run();
}