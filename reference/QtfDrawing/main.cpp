#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DrawingDraw dw(100, 100);
	dw.DrawRect(0, 0, 100, 100, White);
	dw.DrawEllipse(10, 10, 80, 80, Red, 5, Blue);
	dw.DrawRect(50, 50, 50, 50, Yellow);
	QtfRichObject pict(CreateDrawingObject(dw.GetResult(), Size(500, 500), Size(500, 500)));
	String qtf;
	qtf << "[A6 This is some drawing in QTF: " << pict << "&";
	qtf << "[C3*@B And now in the table: {{1:1 " << pict << ":: Another cell " << pict << "}}";
	PromptOK(qtf);
}
