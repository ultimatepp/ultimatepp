#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : public TopWindow {
	virtual void RightDown(Point p, dword keyflags);
};

void MyApp::RightDown(Point p, dword keyflags)
{
	MenuBar b;
	b.Add("Action", [] { PromptOK("Action invoked"); });
	if(!b.Execute())
		PromptOK("Menu canceled");
}

GUI_APP_MAIN
{
	MyApp app;
	app.Run();
}
