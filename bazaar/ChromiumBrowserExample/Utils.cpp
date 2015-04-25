#include "Utils.h"

using namespace Upp;


#ifdef PLATFORM_LINUX

#define DOCK	"gsettings set org.onboard.window docking-enabled true"
#define BOTTOM	"gsettings set org.onboard.window docking-edge 'bottom'"

void Upp::ShowKeyboard(bool show)
{
	if (show){
		Size sz = GetScreenSize();
		int e = system(Format(DOCK ";" BOTTOM ";onboard -l Phone -x %d -y %d -s %d`x%d &", 0, sz.cy - sz.cy / 4, sz.cx, sz.cy / 4));
	}else{
		Sys("killall onboard");
	}
}

#elif defined(PLATFORM_WIN32)

#include <TlHelp32.h>

void Upp::ShowKeyboard(bool show)
{
	if (show){

		Sys("cmd.exe /c \"C:\\Program Files\\Common Files\\Microsoft Shared\\ink\\TabTip.exe\"");

	}else{

		const char * procname = "TabTip.exe";
	    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	    PROCESSENTRY32 pEntry;
	    pEntry.dwSize = sizeof (pEntry);
	    BOOL hRes = Process32First(hSnapShot, &pEntry);
	    while (hRes)
	    {
	        if (strcmp(pEntry.szExeFile, procname) == 0)
	        {
	            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
	                                          (DWORD) pEntry.th32ProcessID);
	            if (hProcess != NULL)
	            {
	                TerminateProcess(hProcess, 9);
	                CloseHandle(hProcess);
	            }
	        }
	        hRes = Process32Next(hSnapShot, &pEntry);
	    }
	    CloseHandle(hSnapShot);

	}

}

#endif
