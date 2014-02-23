#include <TextDiffCtrl/TextDiffCtrl.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	TextDiffCtrl ctrl;
	win.Add(ctrl.SizePos());
	ctrl.Set(LoadFile("C:/u/upp.src/uppdev/AssistTest/main.cpp"), LoadFile("C:/u/upp.src/uppdev/ArrayCtrl/main.cpp"));
	win.Run();
}
