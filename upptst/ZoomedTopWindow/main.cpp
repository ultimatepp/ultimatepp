#include "ZoomedTopWindow.h"

struct MyWin : TopWindow {
	MyWin();
};

MyWin::MyWin()
{
	Sizeable().Zoomable();
}



ZoomedTopWindow::ZoomedTopWindow()
{
	CtrlLayout(*this, "Window title");
	Zoomable().Sizeable();
	modal << [=] { MyWin().Run(); };
	modeless << [=] {
		static MyWin win;
		if(win.IsOpen())
			win.Close();
		else
			win.Open(this);
	};
}

GUI_APP_MAIN
{
	ZoomedTopWindow().Run();
}
