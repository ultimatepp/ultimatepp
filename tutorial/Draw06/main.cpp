#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	PrinterJob pd("My printer job");
	pd.CurrentPage(0);
	pd.MinMaxPage(0, 1);
	if(pd.Execute()) {
		Draw& w = pd.GetDraw();
		w.StartPage();
		w.DrawText(200, 1200, "Helo world!", Arial(600));
		w.EndPage();
		w.StartPage();
		w.DrawText(200, 1200, "Second page", Roman(600));
		w.EndPage();
	}
}
