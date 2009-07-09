#include "CtrlLib/CtrlLib.h"

using namespace Upp;

class MyApp : public TopWindow {
public:
	virtual void Paint(Draw& w);
};

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White);
	for(int i = 0; i < 3600; i += 300)
		w.DrawText(300, 300, i, ".  just a test! 皆の日本語" + AsString(i), Arial(24));
}

GUI_APP_MAIN
{
	MyApp().Run();
}
