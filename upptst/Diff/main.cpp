#include <CtrlLib/CtrlLib.h>
#include <TextDiffCtrl/TextDiffCtrl.h>

using namespace Upp;

GUI_APP_MAIN
{
	PatchDiff d;
	d.Open("C:/Users/cxl/patch.diff", Vector<String>() << "C:/xxx/classic/uppsrc/ide");
	d.Run();
}
