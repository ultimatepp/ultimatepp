#include "ShellLib.h"
#include "ArgEnv.h"

#ifdef PLATFORM_WIN32

#include <Shellapi.h>

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////
// as I found that ShellExecuteEx don't take environment from current process
// but gets it from registry, the only way I found to pass it to spawned process
// is to modify the current user's registry keys directly; so, here a function that
// reads current environment and replaces it with a given one in registry for
// current user (KEY : HKCU/Environment
// it takes the new envoronment, replaces registry one's and returns it
struct RegData : Moveable<RegData>
{
	DWORD type;
	String data;
	
	RegData(DWORD t, const String &d)
	{
		type = t;
		data = d;
	}
};

typedef VectorMap<String, RegData> RegMap;

// builds a regmap from an anvironment
// (all values are set as REG_SZ strings)
RegMap BuildEnvMap(VectorMap<String, String> const &env)
{
	RegMap res;
	for(int i = 0; i < env.GetCount(); i++)
		res.Add(env.GetKey(i), RegData((DWORD)REG_SZ, String(~env[i], env[i].GetCount()+1)));
	return res;
}

bool ReplaceRegEnv(RegMap &prevEnv, RegMap const &newEnv)
{
	prevEnv.Clear();

	// opens the environment key for reading
	HKEY hKey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return false;
	
	// gets buffer sizes for names and values... sigh
	DWORD nValues, maxValueNameLen, maxValueLen;
	if(RegQueryInfoKey(
		hKey, 
	  	NULL,				//  LPTSTR lpClass,
		NULL,				//  LPDWORD lpcClass,
		NULL,				//  LPDWORD lpReserved,
		NULL,				//  LPDWORD lpcSubKeys,
		NULL,				//  LPDWORD lpcMaxSubKeyLen,
		NULL,				//  LPDWORD lpcMaxClassLen,
		&nValues,			//  LPDWORD lpcValues,
		&maxValueNameLen,	//  LPDWORD lpcMaxValueNameLen,
  		&maxValueLen,		//  LPDWORD lpcMaxValueLen,
  		NULL,				//  LPDWORD lpcbSecurityDescriptor,
		NULL				//  PFILETIME lpftLastWriteTime
	) != ERROR_SUCCESS)
	{
		LOG("ERROR OPENING REGISTRY");
		return false;
	}
	LOG("Num values is " << nValues);
	
	// read all user environment variables
	Buffer<char>name(maxValueNameLen + 1);
	Buffer<BYTE>value(maxValueLen + 1);
	DWORD nameLen, valueLen, valueType;
	for(DWORD i = 0; i < nValues; i++)
	{
		nameLen = maxValueNameLen + 1;
		valueLen = maxValueLen + 1;
		int r;
		if( (r = RegEnumValue(
			hKey,
			i,				//  DWORD dwIndex,
			name,			//  LPTSTR lpValueName,
			&nameLen,		//  LPDWORD lpcchValueName,
			NULL,			//  LPDWORD lpReserved,
			&valueType,		//  LPDWORD lpType,
			value,			//  LPBYTE lpData,
			&valueLen		//  LPDWORD lpcbData
		)) != ERROR_SUCCESS)
		{
			LOG("ERROR ENUMERATING VALUES, IDX is " << i << "  error code is " << r);
			RegCloseKey(hKey);
			return false;
		}
		name[nameLen] = 0;
		value[valueLen] = 0;

		prevEnv.Add(~name, RegData(valueType, String(~value, valueLen)));
	}

	// close environment and reopen it for write/delete
	RegCloseKey(hKey);
	if(RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return false;
	
	// wipe all previous values from environment
	for(DWORD i = 0; i < nValues; i++)
	{
		if(RegDeleteValue(hKey, prevEnv.GetKey(i)) != ERROR_SUCCESS)
		{
			LOG("ERROR DELETING VALUE " << prevEnv.GetKey(i));
			RegCloseKey(hKey);
			return false;
		}
	}
	
	// now add new values inside registry
	for(int i = 0; i < newEnv.GetCount(); i++)
	{
		if(RegSetValueEx(
			hKey,
			newEnv.GetKey(i),			//  LPCTSTR lpValueName,
			0,							//  DWORD Reserved,
			newEnv[i].type,				//  DWORD dwType,
			newEnv[i].data,				//  const BYTE *lpData,
			newEnv[i].data.GetCount()	//  DWORD cbData
			) != ERROR_SUCCESS
		)
		{
			LOG("ERROR ADDING VALUE " << newEnv.GetKey(i));
			RegCloseKey(hKey);
			return false;
		}
	}
	
	RegCloseKey(hKey);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
// executes a command via shell "runas" as admin user;
// if wait is true, will wait for command end, otherwise executes it in background
bool ShellExec(String const &args, VectorMap<String, String> const &env, bool wait)
{
	// we use ShellExecuteEx to start an Admin process

	// as ShellExecuteEx doesn't take an environment argument, but gets user's default one
	// we must replace in inside registry before calling
	RegMap prevEnv;
	if(!ReplaceRegEnv(prevEnv, BuildEnvMap(env)))
	{
		LOG("ERROR SETTING ENVIRONMENT FOR SHELLEXECUTEEX");
		return false;
	}

	// now we can use ShellExecute to raise process level
	SHELLEXECUTEINFO info =
	{
		sizeof(SHELLEXECUTEINFO),		// cbsize
		SEE_MASK_FLAG_DDEWAIT,			// fMask
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
	RegMap dummyEnv;
	if(!ReplaceRegEnv(dummyEnv, prevEnv))
	{
		LOG("ERROR RESETTING ENVIRONMENT");
		return false;
	}
	
	if(wait && res)
		WaitForSingleObject(info.hProcess, INFINITE);
	
	return res;
}

END_UPP_NAMESPACE

#endif
