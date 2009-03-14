#include <TextDiffCtrl/TextDiffCtrl.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	TextDiffCtrl ctrl;
	win.Add(ctrl.SizePos());
	ctrl.Set(LoadFile("u:/file1.txt"), LoadFile("u:/file2.txt"));
	win.Run();
}
