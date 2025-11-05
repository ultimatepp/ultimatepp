#include "Debuggers.h"

using namespace Upp;

#if defined(PLATFORM_WIN32)

String Gdb::BreakRunning(int pid)
{
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if(!handle)
		return String() << "Failed to open process associated with " << pid << " PID.";

	String ret;
	BOOL is_wow_64 = FALSE;
	if(IsWow64Process(handle, &is_wow_64)) {
		if(sizeof(void*) == 8 && is_wow_64) {
			String out; // NOTE: this does not work anyway as we are not distributing theide32.exe anymore
			if(Sys(GetExeFolder() << "\\" << "theide32.exe --gdb_debug_break_process=" << pid, out) < 0)
				ret = "Failed to interrupt process via 32-bit TheIDE. Output from command is \"" << out << "\".";
		}
	}
	else
		 ret = "Failed to check that process is under wow64 emulation layer.";
	
	if(DebugBreakProcess(handle))
		return String().Cat() << "Failed to break process associated with " << pid << " PID.";
	
	CloseHandle(handle);
	
	return "";
}

#elif defined(PLATFORM_POSIX)

String Gdb::BreakRunning(int pid)
{
	if(kill(pid, SIGINT) == -1)
		return String().Cat() << "Failed to interrupt process associated with " << pid << " PID.";
	
	return "";
}

#endif
