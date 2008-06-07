#include <Ole/Ctrl/OleCtrl.h>

#ifdef flagMAIN

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpReserved)
{
	switch(fdwReason) {
		case DLL_PROCESS_ATTACH: {
			Ctrl::InitWin32(hinstDll);
			AppInitEnvironment__();
			break;
		}
		case DLL_PROCESS_DETACH: {
			Ctrl::CloseTopCtrls();
			break;
		}
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}
	return true;
}

#endif
