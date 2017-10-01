#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	CoWork co;
	for(int i = 0; i < 50; i++)
		co & [] {
			for(;;) {
				if(CoWork::IsCanceled()) {
					LOG("CoWork was canceled");
					return;
				}
				Sleep(1);
			}
		};
	Sleep(100);
	co.Cancel();
}
