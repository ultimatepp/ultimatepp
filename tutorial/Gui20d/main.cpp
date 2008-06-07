#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Gui20d/myapp.lay>
#include <CtrlCore/lay.h>

GUI_APP_MAIN
{
	WithMyAppLayout<TopWindow> app;
	CtrlLayoutOKCancel(app, "MyApp");
	switch(app.Run()) {
	case IDOK:
		PromptOK(String().Cat() << "OK: " << ~app.date);
		break;
	case IDCANCEL:
		Exclamation("Canceled");
	}
}
