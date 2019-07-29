#include <CtrlLib/CtrlLib.h>
#include <TextDiffCtrl/TextDiffCtrl.h>

using namespace Upp;

GUI_APP_MAIN
{
	PatchDiff d;
	d.Open("C:/Users/cxl/Downloads/xxx2.diff", Vector<String>() << "C:/u/upp.src/uppsrc/ide");
	d.Run();
}
