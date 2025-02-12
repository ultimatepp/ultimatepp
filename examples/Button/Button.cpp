#include <CtrlLib/CtrlLib.h>

// http://java.sun.com/docs/books/tutorial/uiswing/start/swingTour.html

using namespace Upp;

struct ButtonApp : TopWindow {
	int    count = 0;
	Button button;
	Label  label;

	void RefreshLabel()
	{
		label = Format("Number of button clicks %d", count);
	}

	ButtonApp()
	{
		Title("Button example");
		button << [=] { ++count; RefreshLabel(); };
		button.SetLabel("Click me!");
		Add(button.VCenterPosZ(30).HCenterPosZ(200));
		Add(label.BottomPosZ(0, 20).HCenterPosZ(200));
		label.SetAlign(ALIGN_CENTER);
		Sizeable().Zoomable();
		RefreshLabel();
	}
};

GUI_APP_MAIN
{
	ButtonApp().Run();
}
