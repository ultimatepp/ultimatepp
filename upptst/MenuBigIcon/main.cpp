#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : public TopWindow {
	bool use_maxiconsize = false;
	virtual void RightDown(Point p, dword keyflags);
};

void MyApp::RightDown(Point p, dword keyflags)
{
	MenuBar b;
	if(use_maxiconsize)
		b.MaxIconSize(Size(12, 30));
	b.Add("Use max icon size", Upp::CtrlImg::error(), [=] { use_maxiconsize = !use_maxiconsize; }).Check(use_maxiconsize);
	b.Add("Action 2", Upp::CtrlImg::save_as(), [] { PromptOK("Action 2 invoked"); });
	if(!b.Execute())
		PromptOK("Menu canceled");
}

GUI_APP_MAIN
{
	MyApp app;
	app.Run();
}
