#include "NeedAdd.h"


#ifdef PLATFORM_WIN32
LRESULT TopWindowSpecific::WindowProc(UINT message, WPARAM wParam, LPARAM lParam){
	
	if (message == WM_SYSCOMMAND && wParam == SC_MINIMIZE){
		TopWindow::WindowProc(message, wParam, lParam);
		WhenMinimize(); // callback
		return 0;
	}
	else
		return(TopWindow::WindowProc(message, wParam, lParam));
	//return 0;
}

#endif
