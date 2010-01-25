#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ImageDraw idw(1200, 600);
	idw.DrawRect(0, 0, 1200, 600, White);
	idw.DrawText(0, 0, "(Hello)", Serif(400));
	
	Image m = idw;
	PNGEncoder().SaveFile("u:/h.png", m);
	
	DrawingDraw dw(350, 250);
	dw.DrawImage(0, 0, 350, 250, m);
	QtfRichObject pict(CreateDrawingObject(dw.GetResult(), Size(350, 250), Size(350, 250)));

	PromptOK(String("[A5 Rendering problem: ") + pict.ToString());
}
