#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class TestPopup : public TopWindow
{
	StaticRect r;
	
	public:
		typedef TestPopup CLASSNAME;
		TestPopup();
};

TestPopup::TestPopup()
{
	r.Color(Red());
	r.SetRect(100, 100, 200, 200);
	r.PopUp();
}

GUI_APP_MAIN
{
	TestPopup().Run();
}
