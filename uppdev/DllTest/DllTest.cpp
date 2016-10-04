#include <windows.h>
#include "DllTest.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason,  LPVOID lpReserved)
{
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
