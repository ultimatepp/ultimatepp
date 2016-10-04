#include <CtrlLib/CtrlLib.h>
#define IMAGECLASS Img
#define IMAGEFILE <Clock/main.iml>
#include <Draw/iml.h>

using namespace Upp;


struct App : TopWindow
{
	Clock clock;
	Button btn;
	LineCtrl vm, hm;
	DropTime dt;
	DropDate dd;
	
	App()
	{
		Title("Clock");
		Add(btn.LeftPos(5, 100).TopPos(5, 24));
//		btn <<= THISBACK(ShowClock);
		//clock.LeftPos(10, 150).TopPos(10, 150);
		//Add(clock);
//		Add(vm.LeftPos(150, 10).TopPos(5, 100));
//		Add(hm.LeftPos(150, 150).TopPos(140, 10));
		Add(dt.LeftPos(5, 100).TopPos(180, 19));
		Add(dd.LeftPos(115, 100).TopPos(180, 19));
		Sizeable().Zoomable();
		//dt.DropDateTime();
		//dt.DropDate();
	}
	
/*	void ShowClock()
	{
		Rect r = btn.GetScreenRect();
		//clock.SetRect(r.left, r.bottom, 150, 150);
		clock.PopUp(this, Rect(r.left, r.bottom, r.left + 150, r.bottom + 150));
	}
*/	
	typedef App CLASSNAME;
};

GUI_APP_MAIN
{
	App().Run();
}
