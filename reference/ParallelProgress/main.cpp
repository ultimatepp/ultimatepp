#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Progress pi("Working on many items at once...", 100);
	std::atomic<int> ii(0);
	CoDo([&] {
		for(int i = ii++; i < 100; i = ii++) {
			for(int q = 0; q < 1000; q++) {
				if(pi.Canceled()) // ideally call Canceled every 1-10ms
					return;
				Sleep(1); // work simulation
			}
			pi.Step();
		}
	});
}
