#include <CtrlLib/CtrlLib.h>


	void ColorPop()
	{
		ColorPopUp chooser;
		chooser.PopUp(&myButton);
	}

	typedef MyAppWindow CLASSNAME;

	MyAppWindow()
	{
		Title("My application").Zoomable().Sizeable();
		AddFrame(menu);
		Add(myButton.LeftPos(10, 40).TopPos(10, 20));
		menu.Set(THISBACK(MainMenu));
		myButton.WhenAction = THISBACK(ColorPop);
	}
};


GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}