#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class MyWindow : public TopWindow {
public:
	typedef Color CLASSNAME;
	MyWindow();
private:
	ArrayCtrl arrayctrl;
	Array<ProgressIndicator> ind;
};


MyWindow::MyWindow()
{
	arrayctrl.AddColumn("Text");
	arrayctrl.AddColumn("Option").Ctrls<Option>();
	arrayctrl.AddColumn("Progress");
	
	arrayctrl.Add("One", false);
	arrayctrl.SetCtrl(0, 2, ind.Add());
	arrayctrl.Add("Two", true);
	arrayctrl.SetCtrl(1, 2, ind.Add());
	
	ind[0].Set(25, 100);
	ind[1].Set(50, 100);
	
	Add(arrayctrl.SizePos());
	
	Sizeable().Zoomable();
	SetRect(100, 100, 500, 400);
}


GUI_APP_MAIN
{
	MyWindow().Title("Test App").Run();
}
