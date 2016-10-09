#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SetLayout/SetLayout.lay>
#include <CtrlCore/lay.h>

class MyApp : public WithLayout1<TopWindow> {
public:
	MyApp()
	{
		CtrlLayout(*this, "Window title");
		layout1 << [=] { SetLayout_Layout1(*this); };
		layout2 << [=] { SetLayout_Layout2(*this); };
	}
};


GUI_APP_MAIN
{
	MyApp().Run();
}

