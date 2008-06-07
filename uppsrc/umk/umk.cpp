#include "stdio.h"
#include "Windows.h"

int main(int argc, const char *argv[])
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError  = GetStdHandle(STD_ERROR_HANDLE);

	char cmd[16384];
	GetModuleFileName(NULL, cmd, _MAX_PATH);
#ifdef flagDEBUG
	strcpy(strrchr(cmd, '\\'), "\\debugide.exe");
#else
	strcpy(strrchr(cmd, '\\'), "\\theide.exe");
#endif
	for(int i = 1; i < argc; i++) {
		strcat(cmd, " ");
		strcat(cmd, argv[i]);
	}
	if(!CreateProcess(NULL, cmd, &sa, &sa, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
		return 255;

	HANDLE hProcess = pi.hProcess;
	CloseHandle(pi.hThread);
	WaitForSingleObject(hProcess, INFINITE);
	DWORD ExitCode;
	GetExitCodeProcess(hProcess, &ExitCode);
	CloseHandle(pi.hProcess);
	return ExitCode;
}
