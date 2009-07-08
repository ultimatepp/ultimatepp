#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	PrinterJob pj;
	if(PromptYesNo("Landscape?"))
		pj.Landscape();
	if(!PromptYesNo("Use default printer?"))
		if(!pj.Execute())
			return;
	Draw& w = pj;
	w.StartPage();
	const char *text = "Hello world!";
	Font font = Roman(300);
	Point p = Rect(w.GetPageSize()).CenterPos(GetTextSize(text, font));
	w.DrawText(p.x, p.y, "Hello world!", font);
	w.EndPage();
}
