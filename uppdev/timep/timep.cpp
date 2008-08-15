#include "windows.h"
#include "psapi.h"
#include "stdio.h"

typedef __int64            int64;

double tm64(int64 x)
{
	return double(x / 10000) / 1000;
}

int main (int argc, const char *argv []) {
	STARTUPINFO startup;
	PROCESS_INFORMATION pinfo;
	PROCESS_MEMORY_COUNTERS mem;
	HANDLE hProc;
	union {
		int64    time;
		FILETIME ft;
	}
	create, exit, kernel, user;

	GetStartupInfo (&startup);

	const char *s = strchr(GetCommandLine(), ' ');
	CreateProcess(NULL, s ? s + 1 : "", NULL, NULL, TRUE,
	              NORMAL_PRIORITY_CLASS, NULL, NULL, &startup, &pinfo);

	DuplicateHandle(GetCurrentProcess(), pinfo.hProcess, GetCurrentProcess(), &hProc,
	                PROCESS_QUERY_INFORMATION | SYNCHRONIZE, FALSE, 0);
	WaitForSingleObject(hProc, INFINITE);

	GetProcessMemoryInfo(hProc, &mem, sizeof(mem));

	GetProcessTimes (hProc, &create.ft, &exit.ft, &kernel.ft, &user.ft);
	printf("real:%6.3f  user:%6.3f   sys:%6.3f  page:%4d KB  work:%4d  KB\n",
	       tm64(exit.time - create.time), tm64(user.time), tm64(kernel.time),
	       mem.PeakPagefileUsage >> 10, mem.PeakWorkingSetSize >> 10);

	CloseHandle(pinfo.hThread); CloseHandle(pinfo.hProcess);
	CloseHandle(hProc);
	return 0;
}
