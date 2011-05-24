#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SetLayout/SetLayout.lay>
#include <CtrlCore/lay.h>

class MyApp : public WithLayout1<TopWindow> {
public:
	void Layout1()   { SetLayout_Layout1(*this); }
	void Layout2()   { SetLayout_Layout2(*this); }

	typedef MyApp CLASSNAME;

	MyApp()
	{
		CtrlLayout(*this, "Window title");
		layout1 <<= THISBACK(Layout1);
		layout2 <<= THISBACK(Layout2);
	}
};


GUI_APP_MAIN
{
	MyApp().Run();
}

