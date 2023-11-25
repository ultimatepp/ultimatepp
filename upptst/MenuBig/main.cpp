#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : public TopWindow {
	virtual void RightDown(Point p, dword keyflags);
};

void MyApp::RightDown(Point p, dword keyflags)
{
	MenuBar b;
	for(int i = 0; i < 60; i++)
		b.Add("Action " + AsString(i), Upp::CtrlImg::save_as(), [] { PromptOK("invoked"); });
	b.Execute();
}

GUI_APP_MAIN
{
	MyApp app;
	app.Run();
}
