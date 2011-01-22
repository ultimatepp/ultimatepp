#include "ShellLib.h"
#include "ArgEnv.h"

#ifdef PLATFORM_WIN32

#include <Shellapi.h>

NAMESPACE_UPP

// executes a command via shell "runas" as admin user;
// if wait is true, will wait for command end, otherwise executes it in background
bool ShellExec(String const &args, VectorMap<String, String> const &env, bool wait)
{
	// we use ShellExecuteEx to start an Admin process

	// as ShellExecuteEx doesn't take an environment argument, but inherits
	// calling process one, we have to modify directly the environment before calling

	// copy original environment, for that we can use upp :-)
	// don't know if Environment() funcitons returns a cached one, so
	// to be sure we copy it
	VectorMap<String, String> prevEnv(Environment(), 1);
	
	// wipe original environment with windows api
	for(int i = 0; i < prevEnv.GetCount(); i++)
		SetEnvironmentVariable(prevEnv.GetKey(i), NULL);
	
	// insert new environment variables
	for(int i = 0; i < env.GetCount(); i++)
		SetEnvironmentVariable(env.GetKey(i), env[i]);
	
	// now we can use ShellExecute to raise process level
	SHELLEXECUTEINFO info =
	{
		sizeof(SHELLEXECUTEINFO),		// cbsize
		SEE_MASK_NOASYNC,				// fMask
		0,								// hwnd
		"runas",						// lpVerb
		args,							// lpFile
		0,								// lpParameters
		0,								// lpDirectory
		SW_SHOW,						// nShow
		0,								// hHinstApp -- result handle or error code
	/* REST AS DEFAULT -- NOT NEEDED
		LPVOID    lpIDList;
		LPCTSTR   lpClass;
		HKEY      hkeyClass;
		DWORD     dwHotKey;
		union {	HANDLE hIcon; HANDLE hMonitor; } DUMMYUNIONNAME;
		HANDLE    hProcess;
	*/
	};
	
	// if we shall wait for process termination, we shall get spawned process
	// hanlde too...
	if(wait)
		info.fMask |= SEE_MASK_NOCLOSEPROCESS;
	
	bool res = ShellExecuteEx(&info);
	
	// restore the environment
	for(int i = 0; i < prevEnv.GetCount(); i++)
		SetEnvironmentVariable(env.GetKey(i), NULL);
	for(int i = 0; i < env.GetCount(); i++)
		SetEnvironmentVariable(prevEnv.GetKey(i), prevEnv[i]);
	
	if(wait && res)
		WaitForSingleObject(info.hProcess, INFINITE);
	
	return res;
}

END_UPP_NAMESPACE

#endif
