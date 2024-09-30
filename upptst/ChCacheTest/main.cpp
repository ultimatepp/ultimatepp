#include <CtrlLib/CtrlLib.h>

using namespace Upp;


GUI_APP_MAIN
{
	for(;;) {
		Progress progress;
		progress.Create();
		progress.SetTotal(20000);
		for(int i=0;i<20000;i++){
			if(progress.Canceled()) return;
			progress.SetPos(i);
			progress.SetText(Format("MemoryUsedKb %d", MemoryUsedKb()));
		}
	}
}
