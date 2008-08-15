#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGEFILE <Chameleon/Chameleon.iml>
#define IMAGECLASS ChImg
#include <Draw/iml.h>

GUI_APP_MAIN
{
	TopWindow myapp;
	myapp.Background(PaintRect(ColorDisplay(), Red()));
	Button    button;
	button.Transparent();
	myapp.Add(button.HSizePos(50, 50).VSizePos(50, 50));
	Button::StyleNormal().Write().look[0] = ChImg::B();
	myapp.Sizeable();
	myapp.Run();
}
