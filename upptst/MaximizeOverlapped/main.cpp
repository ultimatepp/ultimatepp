#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow w;
	w.Sizeable().Zoomable();
	w.SetMinSize(Size(600, 600));
	w.Title("Maximize me and close me - it should be possible to 'unmaximize' this window after it reopens");

	TopWindow main;
	main.Open();
	for(;;) {
		if(!w.IsOpen())
			w.OpenMain();
		if(!main.IsOpen())
			break;
		Ctrl::ProcessEvents();
	}
}
