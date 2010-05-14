#include "EditField.h"

void EditFieldApp::Set()
{
	ef1 <<= "Just a test!";
	ef1.SetReadOnly();
}

EditFieldApp::EditFieldApp()
{
	CtrlLayoutOKCancel(*this, "Test");
	set <<= THISBACK(Set);
	ef1.NullText("(default)");
	ef2.AutoSize();
}

GUI_APP_MAIN
{
	EditFieldApp app;
	app.Run();
}
