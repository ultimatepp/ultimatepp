#include "Core/Core.h"

#if defined(PLATFORM_WIN32) 
	#include <shellapi.h>
	#include <Tlhelp32.h>
	#include <psapi.h>
	#include <Winioctl.h>
	#define CY tagCY
	// To compile in MinGW copy files Rpcsal.h, DispEx.h, WbemCli.h, WbemDisp.h, Wbemidl.h, 
	// WbemProv.h and WbemTran.h from \Microsoft SDKs\Windows\v6.1\Include to MinGW/include and 
	// wbemuuid.lib from \Microsoft SDKs\Windows\v6.1\Lib to MinGW/lib
	#include <rpcsal.h>	
	#include <Wbemidl.h>
	#include <winnls.h> 
	#include <vfw.h>
#endif
#ifdef PLATFORM_POSIX
	#include <sys/time.h>
	#include <sys/resource.h>
	
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/vfs.h> 
	#include <sys/utsname.h>
	#include <dirent.h>
	#include <pwd.h>
	#include <grp.h>
	#include <time.h>
	#include <signal.h>
	#include <sys/reboot.h>
//#include <linux/kd.h>
//#include <sys/ioctl.h>
	
	#include <X11/Xlib.h>
	#include <X11/Xos.h>
	#include <X11/Xfuncs.h>
	#include <X11/Xutil.h>
	#include <X11/Xatom.h>
	#include <X11/extensions/XTest.h>
#endif

using namespace Upp;

#include "SysInfo.h"


/////////////////////////////////////////////////////////////////////
// LaunchFile

#if defined(PLATFORM_WIN32)
bool LaunchFileCreateProcess(const String file)
{
	STARTUPINFOW startInfo;
    PROCESS_INFORMATION procInfo;

    ZeroMemory(&startInfo, sizeof(startInfo));
    startInfo.cb = sizeof(startInfo);
    ZeroMemory(&procInfo, sizeof(procInfo));

	WString wexec;
	wexec = Format("\"%s\" \"%s\"", GetExtExecutable(GetFileExt(file)), file).ToWString();
	WStringBuffer wsbexec(wexec);
	
    if(!CreateProcessW(NULL, wsbexec, NULL, NULL, FALSE, 0, NULL, NULL, &startInfo, &procInfo))  
        return false;

   	WaitForSingleObject(procInfo.hProcess, 0);

    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);	
	return true;
}

bool LaunchFileShellExecute(const String file)
{
	HINSTANCE ret;
	WString fileName(file);
	ret = ShellExecuteW(NULL, L"open", fileName, NULL, L".", SW_SHOWNORMAL);		

	return (int)ret > 32;
}
#endif

bool LaunchFile(const String file)
{
#if defined(PLATFORM_WIN32)
	if (!LaunchFileShellExecute(file))			// First try
	   	return LaunchFileCreateProcess(file);	// Second try
	return true;
#endif
#ifdef PLATFORM_POSIX
	int ret;
	if (GetDesktopManagerNew() == "gnome") 
		ret = system("gnome-open \"" + file + "\"");
	else if (GetDesktopManagerNew() == "kde") 
		ret = system("kfmclient exec \"" + file + "\" &"); 
	else if (GetDesktopManagerNew() == "enlightenment") {
		String mime = GetExtExecutable(GetFileExt(file));
		String program = mime.Left(mime.Find("."));		// Left side of mime executable is the program to run
		ret = system(program + " \"" + file + "\" &"); 
	} else 
		ret = system("xdg-open \"" + file + "\"");
	return (ret >= 0);
#endif
}


/////////////////////////////////////////////////////////////////////
// Hardware Info
#if defined(PLATFORM_WIN32) 
		
bool GetWMIInfo(String system, Array <String> &data, Array <Value> *ret[])
{
	HRESULT hRes;
	
	hRes = CoInitialize(NULL);
	if (hRes != S_OK && hRes != S_FALSE)
		return false;

	hRes = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT,
			RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0);
	if (hRes != S_OK && hRes != RPC_E_TOO_LATE) {
		CoUninitialize();
		return false;
	}
	IWbemLocator* pIWbemLocator = NULL;
	if (CoCreateInstance(CLSID_WbemAdministrativeLocator, NULL, 
		CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IUnknown, (void **)&pIWbemLocator) != S_OK) {
		CoUninitialize();
		return false;
	}
	
	BSTR bstrNamespace = SysAllocString(L"root\\cimv2");
	IWbemServices* pWbemServices = NULL;
	if (pIWbemLocator->ConnectServer(bstrNamespace, NULL, NULL, NULL, 0, NULL, NULL,
		&pWbemServices) != S_OK) {
		CoUninitialize();
		return false;
	}
	SysFreeString(bstrNamespace);
	
	hRes = CoSetProxyBlanket(pWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
			RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(hRes)) {
        pWbemServices->Release();
        pIWbemLocator->Release();     
        CoUninitialize();
        return false;
    }
	
	IEnumWbemClassObject* pEnumerator = NULL;
	String query;
	query << "Select * from " << system;
	WCHAR wquery[1024*sizeof(WCHAR)];
	MultiByteToWideChar(CP_UTF8, 0, query, -1, wquery, sizeof(wquery)/sizeof(wquery[0]));
	BSTR strQuery = SysAllocString(wquery);
	BSTR strQL = SysAllocString(L"WQL");
	hRes = pWbemServices->ExecQuery(strQL, strQuery, WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	if (hRes != S_OK) {
        pWbemServices->Release();
        pIWbemLocator->Release();     
        CoUninitialize();
        return false;
    }
	SysFreeString(strQuery);
	SysFreeString(strQL);

    IWbemClassObject *pClassObject;
    ULONG uReturn = 0;
    int row = 0;
	while (pEnumerator) {
        hRes = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn);
       	if(0 == uReturn)
            break;
		if(hRes != S_OK) {
	        pWbemServices->Release();
	        pIWbemLocator->Release(); 
	        pEnumerator->Release(); 
	        pClassObject->Release();   
	        CoUninitialize();
	        return false;
	    }
		for (int col = 0; col < data.GetCount(); ++col) {
			VARIANT vProp;
			BSTR strClassProp = SysAllocString(data[col].ToWString());
	        hRes = pClassObject->Get(strClassProp, 0, &vProp, 0, 0);
	        if(hRes != S_OK){
		        pWbemServices->Release();
		        pIWbemLocator->Release(); 
		        pEnumerator->Release(); 
		        pClassObject->Release();   
		        CoUninitialize();
		        return false;
		    }
			SysFreeString(strClassProp);        
			ret[col]->Add(GetVARIANT(vProp));
		}
		row++;
    }
	pIWbemLocator->Release();
	pWbemServices->Release();
	pEnumerator->Release();
	pClassObject->Release();
	CoUninitialize(); 
	
	return true;
}
bool GetWMIInfo(String system, String data, Value &res)
{
	Array <Value> arrRes;
	Array <Value> *arrResP[1];
	arrResP[0] = &arrRes;
	Array <String> arrData;
	arrData.Add(data);
	bool ret = GetWMIInfo("Win32_ComputerSystem", arrData, arrResP);
	if (ret)
		res = arrRes[0];
	return ret;
}
	
void GetSystemInfo(String &manufacturer, String &productName, String &version, int &numberOfProcessors)
{ 
	manufacturer = "";
	Value vmanufacturer;
	if (GetWMIInfo("Win32_ComputerSystem", "manufacturer", vmanufacturer)) 
		manufacturer = TrimBoth(vmanufacturer);
	if (manufacturer.IsEmpty()) 
		manufacturer = FromSystemCharset(GetWinRegString("SystemManufacturer", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	if (manufacturer.IsEmpty()) {
		StringParse fileData = LoadFile(AppendFileName(GetSystemFolder(), "oeminfo.ini"));
		fileData.GoAfter("Manufacturer=");
		manufacturer = fileData.GetText("\r\n");
	}
	productName = "";
	Value vproductName;
	if (GetWMIInfo("Win32_ComputerSystem", "model", vproductName)) 
		productName = TrimBoth(vproductName);
	if (productName.IsEmpty())
		productName = FromSystemCharset(GetWinRegString("SystemProductName", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	if (productName.IsEmpty())
		productName = FromSystemCharset(GetWinRegString("Model", "SOFTWARE\\Microsoft\\PCHealth\\HelpSvc\\OEMInfo", HKEY_LOCAL_MACHINE));
	
	version = FromSystemCharset(GetWinRegString("SystemVersion", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	numberOfProcessors = atoi(GetEnv("NUMBER_OF_PROCESSORS"));
}
void GetBiosInfo(String &biosVersion, Date &biosReleaseDate)
{ 
	// Alternative is "wmic bios get name" and "wmic bios get releasedate"
	String strBios = FromSystemCharset(GetWinRegString("BIOSVersion", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));	
	if (strBios.IsEmpty())
		strBios = FromSystemCharset(GetWinRegString("SystemBiosVersion", "HARDWARE\\DESCRIPTION\\System", HKEY_LOCAL_MACHINE));	
	for (int i = 0; i < strBios.GetLength(); ++i) {
		if (strBios[i] == '\0')
			biosVersion.Cat(". ");
		else
			biosVersion.Cat(strBios[i]);
	}
	String strDate = FromSystemCharset(GetWinRegString("BIOSReleaseDate", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	if (strDate.IsEmpty())
		strDate = FromSystemCharset(GetWinRegString("SystemBiosDate", "HARDWARE\\DESCRIPTION\\System", HKEY_LOCAL_MACHINE));
	StrToDate(biosReleaseDate, strDate);
}
bool GetProcessorInfo(int number, String &vendor, String &identifier, String &architecture, int &speed)		
{
	String strReg = Format("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d", number);
	vendor = FromSystemCharset(GetWinRegString("VendorIdentifier", strReg, HKEY_LOCAL_MACHINE));	
	identifier = FromSystemCharset(GetWinRegString("ProcessorNameString", strReg, HKEY_LOCAL_MACHINE));	
	architecture = FromSystemCharset(GetWinRegString("Identifier", strReg, HKEY_LOCAL_MACHINE));	
	speed = GetWinRegInt("~MHz", strReg, HKEY_LOCAL_MACHINE);
	return true;
}	

bool GetVideoInfo(Array <Value> &name, Array <Value> &description, Array <Value> &videoProcessor, Array <Value> &ram, Array <Value> &videoMode)
{
	Array <Value> *res[5];
	res[0] = &name;
	res[1] = &description;
	res[2] = &videoProcessor;
	res[3] = &ram;
	res[4] = &videoMode;
	Array <String> data;
	data.Add("Name");
	data.Add("Description");
	data.Add("VideoProcessor");
	data.Add("AdapterRAM");
	data.Add("VideoModeDescription");
	if (!GetWMIInfo("Win32_VideoController", data, res))
		return false;
	
	for (int row = 0; row < ram.GetCount(); ++row) 
		ram[row] = (atoi(ram[row].ToString()) + 524288)/1048576;
	return true;
}
bool GetPackagesInfo(Array <Value> &name, Array <Value> &version, Array <Value> &vendor, 
Array <Value> &installDate, Array <Value> &caption, Array <Value> &description, Array <Value> &state)
{
	Array <Value> *res[7];
	res[0] = &name;
	res[1] = &version;
	res[2] = &vendor;
	res[3] = &installDate;	
	res[4] = &caption;	
	res[5] = &description;	
	res[6] = &state;	
	Array <String> data;
	data.Add("Name");
	data.Add("Version");
	data.Add("Vendor");
	data.Add("InstallDate2");
	data.Add("Caption");
	data.Add("Description");
	data.Add("InstallState");
	if (!GetWMIInfo("Win32_Product", data, res))
		return false;
	
	for (int i = 0; i < installDate.GetCount(); ++i) {
		String sdate = installDate[i];
		Time t(atoi(sdate.Left(4)), atoi(sdate.Mid(4, 2)), atoi(sdate.Mid(6, 2)), 
			   atoi(sdate.Mid(8, 2)), atoi(sdate.Mid(10, 2)), atoi(sdate.Mid(12, 2)));
		installDate[i] = t;
		int istate = state[i];
		switch (istate) {
		case -6:	state[i] = "Bad Configuration";	break;
		case -2:	state[i] = "Invalid Argument";	break;
		case -1:	state[i] = "Unknown Package";	break;
		case 1:		state[i] = "Advertised";		break;
		case 2:		state[i] = "Absent";			break;
		case 5:		state[i] = "Ok";				break;
		default: 	return false;
		}
	}
	return true;
}
#endif
#if defined (PLATFORM_POSIX)
void GetSystemInfo(String &manufacturer, String &productName, String &version, int &numberOfProcessors)
{
	manufacturer = LoadFile_Safe("/sys/devices/virtual/dmi/id/board_vendor");
	productName = LoadFile_Safe("/sys/devices/virtual/dmi/id/board_name");
	version = LoadFile_Safe("/sys/devices/virtual/dmi/id/product_version");
	
	StringParse cpu(LoadFile_Safe("/proc/cpuinfo"));	
	numberOfProcessors = 1;
	while (cpu.GoAfter("processor")) {
		cpu.GoAfter(":");
		numberOfProcessors = atoi(cpu.GetText()) + 1;
	} 
}
void GetBiosInfo(String &biosVersion, Date &biosReleaseDate)
{
	String biosVendor = LoadFile_Safe("/sys/devices/virtual/dmi/id/bios_vendor");
	biosVersion = LoadFile_Safe("/sys/devices/virtual/dmi/id/bios_version");
	StrToDate(biosReleaseDate, LoadFile_Safe("/sys/devices/virtual/dmi/id/bios_date"));
}
bool GetProcessorInfo(int number, String &vendor, String &identifier, String &architecture, int &speed)	
{
	StringParse cpu(LoadFile_Safe("/proc/cpuinfo"));
	
	int cpuNumber;
	do {
		if (!cpu.GoAfter("processor", ":"))
			return false;
		String sCpu = cpu.GetText();
		if (sCpu == "")
			return false;
		cpuNumber = atoi(sCpu);
	} while (cpuNumber != number);
		
	cpu.GoAfter("vendor_id", ":");
	vendor = cpu.GetText();
	cpu.GoAfter("cpu family", ":");
	String family = cpu.GetText();		// 6 means 686
	cpu.GoAfter("model", ":");
	String model = cpu.GetText();
	cpu.GoAfter("model name", ":");
	identifier = cpu.GetText("\n");
	cpu.GoAfter("stepping", ":");
	String stepping = cpu.GetText();
	architecture = Sys("uname -m");		// CPU type
	architecture << " Family " << family << " Model " << model << " Stepping " << stepping;		// And 64 bits ?? uname -m
	cpu.GoAfter_Init("cpu MHz", ":");
	speed = cpu.GetInt();
}
#endif

/////////////////////////////////////////////////////////////////////
// Memory Info

#if defined(__MINGW32__)
typedef struct _MEMORYSTATUSEX {
	DWORD dwLength;
	DWORD dwMemoryLoad;
	DWORDLONG ullTotalPhys;
	DWORDLONG ullAvailPhys;
	DWORDLONG ullTotalPageFile;
	DWORDLONG ullAvailPageFile;
	DWORDLONG ullTotalVirtual;
	DWORDLONG ullAvailVirtual;
	DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX,*LPMEMORYSTATUSEX;

WINBASEAPI BOOL WINAPI GlobalMemoryStatusEx(LPMEMORYSTATUSEX);

bool GetMemoryInfo(
	int &memoryLoad, 			// percent of memory in use		
	uint64 &totalPhys, 			// physical memory				
	uint64 &freePhys, 			// free physical memory			
	uint64 &totalPageFile,		// total paging file			
	uint64 &freePageFile,		// free paging file				
	uint64 &totalVirtual,		// total virtual memory			
	uint64 &freeVirtual)		// free virtual memory			
{
	MEMORYSTATUS status;	
	status.dwLength = sizeof (status);
	GlobalMemoryStatus(&status);

	memoryLoad          = status.dwMemoryLoad;
	totalPhys			= status.dwTotalPhys;
	freePhys			= status.dwAvailPhys;
	totalPageFile		= status.dwTotalPageFile;
	freePageFile		= status.dwAvailPageFile;
	totalVirtual		= status.dwTotalVirtual;
	freeVirtual	    	= status.dwAvailVirtual;
	
	return true;
}
#endif
#ifdef COMPILER_MSC

bool GetMemoryInfo(
	int &memoryLoad, 			// percent of memory in use		
	uint64 &totalPhys, 			// physical memory				
	uint64 &freePhys, 			// free physical memory			
	uint64 &totalPageFile,		// total paging file			
	uint64 &freePageFile,		// free paging file				
	uint64 &totalVirtual,		// total virtual memory			
	uint64 &freeVirtual)		// free virtual memory			
{
	MEMORYSTATUSEX status;	
	status.dwLength = sizeof (status);
	if (!GlobalMemoryStatusEx(&status))
		return false;
	memoryLoad          = status.dwMemoryLoad;
	totalPhys			= status.ullTotalPhys;
	freePhys			= status.ullAvailPhys;
	totalPageFile		= status.ullTotalPageFile;
	freePageFile		= status.ullAvailPageFile;
	totalVirtual		= status.ullTotalVirtual;
	freeVirtual	    	= status.ullAvailVirtual;
	
	return true;
}
#endif


#ifdef PLATFORM_POSIX
bool GetMemoryInfo(
int &memoryLoad, 			// percent of memory in use		
uint64 &totalPhys, 			// physical memory				
uint64 &freePhys, 			// free physical memory			
uint64 &totalPageFile,		// total paging file			
uint64 &freePageFile,		// free paging file				
uint64 &totalVirtual,		// total virtual memory			
uint64 &freeVirtual)
{
	StringParse meminfo = LoadFile_Safe("/proc/meminfo");
	if (meminfo == "")
		return false;
	meminfo.GoAfter_Init("MemTotal", ":");	totalPhys = 1024*meminfo.GetUInt64();
	meminfo.GoAfter_Init("MemFree", ":");	freePhys  = 1024*meminfo.GetUInt64();
	memoryLoad = (int)(100.*(totalPhys-freePhys)/totalPhys);
	meminfo.GoAfter_Init("SwapCached", ":");freePageFile = 1024*meminfo.GetUInt64();
	meminfo.GoAfter_Init("Cached", ":");	totalPageFile = 1024*meminfo.GetUInt64() + freePageFile;
	meminfo.GoAfter_Init("SwapTotal", ":");	totalVirtual = 1024*meminfo.GetUInt64();
	meminfo.GoAfter_Init("SwapFree", ":");	freeVirtual = 1024*meminfo.GetUInt64();
	
	return true;
}
#endif


/////////////////////////////////////////////////////////////////////
// Process list

#if defined(PLATFORM_WIN32) 

// Get the list of process identifiers.
bool GetProcessList(Array<long> &pid, Array<String> &pNames)
{
	PROCESSENTRY32 proc;
	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) 
		return false;
	proc.dwSize = sizeof(proc);
	long f = Process32First(hSnap, &proc);
	while (f) {
		pid.Add(proc.th32ProcessID);
		pNames.Add(proc.szExeFile);
       	f = Process32Next(hSnap, &proc);
	}
	CloseHandle(hSnap);
	return true;	
}
Array<long> GetProcessList()
{
	PROCESSENTRY32 proc;
	Array<long> ret;
	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) 
		return ret;
	proc.dwSize = sizeof(proc);
	long f = Process32First(hSnap, &proc);
	while (f) {
		ret.Add(proc.th32ProcessID);
       	f = Process32Next(hSnap, &proc);
	}
	CloseHandle(hSnap);
	return ret;	
}
String GetProcessName(long processID)
{
	WCHAR szProcessName[MAX_PATH];
    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    // Get the process name.
    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            GetModuleBaseNameW(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(WCHAR));
    }
    CloseHandle(hProcess);

    return FromSystemCharset(WString(szProcessName).ToString());
}
String GetProcessFileName(long processID)
{
	WCHAR szProcessName[MAX_PATH];
    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    // Get the process name.
    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            GetModuleFileNameExW(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(WCHAR));
    }
    CloseHandle(hProcess);

    return FromSystemCharset(WString(szProcessName).ToString());
}
BOOL CALLBACK EnumGetWindowsList(HWND hWnd, LPARAM lParam) 
{
	if (!hWnd)
		return TRUE;		// Not a window
	if (GetParent(hWnd) != 0)
		return TRUE;		// Child window
	Array<int> *ret = (Array<int> *)lParam;
	ret->Add((int)hWnd);
	return TRUE;
}
void GetWindowsList(Array<long> &hWnd, Array<long> &processId, Array<String> &name, Array<String> &fileName, Array<String> &caption)
{
	HANDLE hProcess;
	DWORD dwThreadId, dwProcessId;
	HINSTANCE hInstance;
	WCHAR str[MAX_PATH];
	
	EnumWindows(EnumGetWindowsList, (LPARAM)&hWnd);	
	for (int i = 0; i < hWnd.GetCount(); ++i) {
		hInstance = (HINSTANCE)GetWindowLong((HWND)hWnd[i], GWL_HINSTANCE);
		dwThreadId = GetWindowThreadProcessId((HWND)hWnd[i], &dwProcessId);
		processId.Add(dwProcessId);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
		if (GetModuleFileNameExW(hProcess, hInstance, str, sizeof(str)/sizeof(WCHAR)))
			fileName.Add(FromSystemCharset(WString(str).ToString()));
		else
			fileName.Add(t_("UNKNOWN"));	
		if (GetModuleBaseNameW(hProcess, hInstance, str, sizeof(str)/sizeof(WCHAR)))
			name.Add(FromSystemCharset(WString(str).ToString()));
		else
			name.Add(t_("UNKNOWN"));		
		CloseHandle(hProcess);
		if (IsWindowVisible((HWND)hWnd[i])) {
			int count = SendMessageW((HWND)hWnd[i], WM_GETTEXT, sizeof(str)/sizeof(WCHAR), (LPARAM)str);
			str[count] = '\0';
			caption.Add(FromSystemCharset(WString(str).ToString()));	
		} else
			caption.Add("");	
	}
}
Array<long> GetWindowsList()
{
	Array<long> ret;
	EnumWindows(EnumGetWindowsList, (LPARAM)&ret);	
	return ret;
}
BOOL CALLBACK TerminateAppEnum(HWND hwnd, LPARAM lParam)
{
	DWORD dwID ;
  	GetWindowThreadProcessId(hwnd, &dwID) ;
  	if(dwID == (DWORD)lParam)
     	PostMessage(hwnd, WM_CLOSE, 0, 0) ;
  	return TRUE ;
}
// pid	 	Process ID of the process to shut down.
// timeout 	Wait time in milliseconds before shutting down the process.
bool ProcessTerminate(long processId, int timeout)
{
  	// If we can't open the process with PROCESS_TERMINATE rights, then we give up immediately.
  	HANDLE hProc = ::OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, processId);
  	if(hProc == NULL)
    	return false ;
   	// TerminateAppEnum() posts WM_CLOSE to all windows whose PID matches your process's.
  	::EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM)processId) ;

	int ret;
  	// Wait on the handle. If it signals, great. If it times out,
  	// then you kill it.
  	int state = ::WaitForSingleObject(hProc, timeout);
  	if ((state == WAIT_TIMEOUT) || (state == WAIT_FAILED))
     	ret = ::TerminateProcess(hProc, 0);
	else
		ret = true;
	CloseHandle(hProc) ;
	return ret;
}

int GetProcessPriority(long pid)
{
	int priority;
	HANDLE hProc = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
  	if(hProc == NULL) 
    	return -1;
	priority = ::GetPriorityClass(hProc);
   	CloseHandle(hProc);
   	
   	switch(priority) {
   	case REALTIME_PRIORITY_CLASS:	priority = 10;	// Process that has the highest possible priority. The threads of a real-time priority class process preempt the threads of all other processes, including operating system processes performing important tasks. For example, a real-time process that executes for more than a very brief interval can cause disk caches not to flush or cause the mouse to be unresponsive.
   									break;
   	case HIGH_PRIORITY_CLASS:		priority = 8;	// Process that performs time-critical tasks that must be executed immediately for it to run correctly. The threads of a high-priority class process preempt the threads of normal or idle priority class processes. An example is the Task List, which must respond quickly when called by the user, regardless of the load on the operating system. Use extreme care when using the high-priority class, because a high-priority class CPU-bound application can use nearly all available cycles.
   									break;
	case ABOVE_NORMAL_PRIORITY_CLASS: priority = 6; // Process that has priority above NORMAL_PRIORITY_CLASS but below HIGH_PRIORITY_CLASS.
   									break;
	case NORMAL_PRIORITY_CLASS:		priority = 5;	// Process with no special scheduling needs.
   									break;	
	case BELOW_NORMAL_PRIORITY_CLASS: priority = 3;	// Process that has priority above IDLE_PRIORITY_CLASS but below NORMAL_PRIORITY_CLASS.
   									break;	
	case IDLE_PRIORITY_CLASS:		priority = 0;	// Process whose threads run only when the system is idle and are preempted by the threads of any process running in a higher priority class. An example is a screen saver. The idle priority class is inherited by child processes.
									break;
	default:						return -1;
   	}
  	return priority;	
}
 
bool SetProcessPriority(long pid, int priority)
{
	HANDLE hProc = ::OpenProcess(PROCESS_SET_INFORMATION , FALSE, pid);
  	if(hProc == NULL)
    	return false;
  	if (priority == 10)
  		priority = REALTIME_PRIORITY_CLASS;
  	else if (priority >= 8)
		priority = HIGH_PRIORITY_CLASS;
  	else if (priority >= 6)
		priority = ABOVE_NORMAL_PRIORITY_CLASS;
  	else if (priority >= 5)
		priority = NORMAL_PRIORITY_CLASS;	
  	else if (priority >= 3)
   		priority = BELOW_NORMAL_PRIORITY_CLASS;
  	else
  		priority = IDLE_PRIORITY_CLASS;
	int ret = ::SetPriorityClass(hProc, priority);		// SetProcessAffinityMask
   	CloseHandle(hProc) ;
  	return ret;	
}

#endif
#ifdef PLATFORM_POSIX			 //Check with ps

bool IsInteger(String s)
{
	for (int i = 0; i < s.GetCount(); ++i) {
		if (!isdigit(s[i]))
			return false;
	}
	return true;
}
bool GetProcessList(Array<long> &pid, Array<String> &pNames)
{
	FindFile ff;
	if(ff.Search("/proc/*")) {
		do {
			if (IsInteger(ff.GetName())) {
				String exe = Format("/proc/%s/exe", ff.GetName());
				StringBuffer exeb;
				exeb = exe;
				char procName[2048];
				int procNameLen = readlink(exeb, procName, sizeof(procName)-1);
				if (procNameLen > 0) {
					procName[procNameLen] = 0;
					pNames.Add(procName);
					pid.Add(atoi(ff.GetName()));
				}
			}
		} while(ff.Next());
	}
	return true;
}
Array<long> GetProcessList()
{
	FindFile ff;
	Array<long> pid; 
	if(ff.Search("/proc/*")) {
		do {
			if (IsInteger(ff.GetName())) {
				String exe = Format("/proc/%s/exe", ff.GetName());
				StringBuffer exeb;
				exeb = exe;
				char procName[2048];
				int procNameLen = readlink(exeb, procName, sizeof(procName)-1);
				if (procNameLen > 0) 
					pid.Add(atoi(ff.GetName()));
			}
		} while(ff.Next());
	}
	return pid;
}
String GetProcessName(long pid)
{
	return GetFileName(GetProcessFileName(pid));
}
// ls -l /proc/%d/fd gets also the files opened by the process
String GetProcessFileName(long pid)
{
	String ret = "";
	String exe = Format("/proc/%s/exe", FormatLong(pid));
	StringBuffer exeb;
	exeb = exe;
	char procName[2048];
	int procNameLen = readlink(exeb, procName, sizeof(procName)-1);
	if (procNameLen > 0) {
		procName[procNameLen] = 0;
		ret = procName;
	} 
	return ret;	
}

#ifdef PLATFORM_POSIX
#ifdef flagGUI
	#define SetX11ErrorHandler() {}
	#define SetSysInfoX11ErrorHandler()	{}
#else
	#define SetX11ErrorHandler() {}
	int SysInfoX11ErrorHandler(Display *, XErrorEvent *)	{return 0;}
	void SetSysInfoX11ErrorHandler()						{XSetErrorHandler(SysInfoX11ErrorHandler);}
#endif
#endif

void GetWindowsList_Rec (Display *dpy, Window w, int depth, Array<long> &wid) 
{ 
	if (depth > 3) // 1 is enough for Gnome. 2 is necessary for Xfce and Kde
		return; 

	wid.Add(w);

	Window root, parent; 
	unsigned int nchildren; 
	Window *children = NULL; 
	if (XQueryTree (dpy, w, &root, &parent, &children, &nchildren))  {
		for (int i = 0; i < nchildren; i++) {
			XWindowAttributes windowattr;
			XGetWindowAttributes(dpy, children[i], &windowattr);
			if (windowattr.map_state == IsViewable)
				GetWindowsList_Rec (dpy, children[i], depth + 1, wid); 
		}
	}
	if (children) 
		XFree((char *)children); 
	return; 
}
Array<long> GetWindowsList()
{
	Array<long> ret;	
	SetSysInfoX11ErrorHandler();
	
	Display *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return ret;
	}
	GetWindowsList_Rec (dpy, RootWindow(dpy, DefaultScreen(dpy)), 0, ret);
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	return ret;
}
void GetWindowsList(Array<long> &hWnd, Array<long> &processId, Array<String> &nameL, Array<String> &fileName, Array<String> &caption)
{
	SetSysInfoX11ErrorHandler();
	Display *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return;
	}
	GetWindowsList_Rec(dpy, RootWindow (dpy, DefaultScreen (dpy)), 0, hWnd);
	for (int i = 0; i < hWnd.GetCount(); ++i) {
		// Get window name
		XTextProperty tp;
		if (XGetWMName(dpy, hWnd[i], &tp) == 0) 
	    	caption.Add("");
        else {
        	if (tp.nitems > 0) {
           		int count = 0, i, ret;
           		char **list = NULL;
          		ret = XmbTextPropertyToTextList(dpy, &tp, &list, &count);
            	if((ret == Success || ret > 0) && list != NULL) {
                	String sret;
              		for(i = 0; i < count; i++)
              			sret << list[i]; // << " ";
              		XFreeStringList(list);
              		caption.Add(FromSystemCharset(sret));
          		} else 
              		caption.Add(FromSystemCharset((char *)tp.value));
      		} else 
      			caption.Add("");
        }
		// Get pid
		Atom atomPID = XInternAtom(dpy, "_NET_WM_PID", true);
		unsigned long pid = 0;
		if (atomPID == None)
			processId.Add(0L);
		else {
			Atom type;        
			int format;        
			unsigned long nItems;        
			unsigned long bytesAfter;        
			unsigned char *propPID = 0;
			if (0 == XGetWindowProperty(dpy, hWnd[i], atomPID, 0, 1024, false, XA_CARDINAL, &type, &format, &nItems, &bytesAfter, &propPID)) {
				if(propPID != 0) {
					pid = *((unsigned long *)propPID);
					processId.Add(pid);
					XFree(propPID);
				} else
					processId.Add(0L);
			} else
				processId.Add(0L);
		}
		if (pid != 0L)
			fileName.Add(GetProcessFileName(pid));
		else
			fileName.Add("");
		// Name and class
		XClassHint ch;
		ch.res_name = ch.res_class = NULL; 	
		Status status = XGetClassHint (dpy, hWnd[i], &ch);
		if (status != BadWindow) {
			if (ch.res_name)
				nameL.Add(ch.res_name);
			else
				nameL.Add("");
		} else
			nameL.Add("");
		if (ch.res_name) 
			XFree (ch.res_name); 
		if (ch.res_class) 
			XFree (ch.res_class); 
	}
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	return;
}    

bool WindowKill(long wid)
{
	if (wid == 0)
		return false;
	
	Display *dpy = XOpenDisplay (NULL);
	if (!dpy) 
		return false;

	XSync (dpy, 0);			
	XKillClient (dpy, wid);
	XSync (dpy, 0);
	
	XCloseDisplay (dpy);
	return true;	
}

// Also possible to stop or cont
bool ProcessTerminate(long pid, int timeout)
{
	if (!ProcessExists(pid))
		return false;
	long wid = GetWindowIdFromProcessId(pid);		// Just in case
		
	// First... SIGTERM
	kill(pid, SIGTERM);	
	Sleep(timeout/3);
	if (!ProcessExists(pid))
		return true;
	// Second... SIGKILL
	kill(pid, SIGKILL);	
	Sleep(timeout/3);
	if (!ProcessExists(pid))
		return true;
	// Third ... WindowKill
	Sleep((int)(timeout/3));
	return WindowKill(wid);
}

int GetProcessPriority(long pid)
{
	int priority = getpriority(PRIO_PROCESS, pid);
	return 10 - (priority + 20)/4;		// Rescale -20/20 to 10/0
}
bool SetProcessPriority(long pid, int priority)
{
	priority = 20 - 4*priority;
	if (0 == setpriority(PRIO_PROCESS, pid, priority))
 		return true;	
	else
		return false;
}
#endif

long GetWindowIdFromCaption(String windowCaption, bool exactMatch)
{
	Array<long> wid, pid;
	Array<String> name, fileName, caption;
	GetWindowsList(wid, pid, name, fileName, caption);
	for (int i = 0; i < wid.GetCount(); ++i) {
		if (exactMatch) {
			String s = caption[i];
			if (caption[i] == windowCaption)
				return wid[i];
		} else {
			if (caption[i].Find(windowCaption) >= 0)
				return wid[i];
		}
	}
	return -1;
} 
long GetProcessIdFromWindowCaption(String windowCaption, bool exactMatch)
{
	Array<long> wid, pid;
	Array<String> name, fileName, caption;
	GetWindowsList(wid, pid, name, fileName, caption);
	for (int i = 0; i < wid.GetCount(); ++i) {
		if (exactMatch) {
			if (caption[i] == windowCaption)
				return pid[i];
		} else {
			if (caption[i].Find(windowCaption) >= 0)
				return pid[i];
		}
	}
	return -1;
}    
long GetProcessIdFromWindowId(long _wId)
{
	Array<long> wId, pid;
	Array<String> name, fileName, caption;
	GetWindowsList(wId, pid, name, fileName, caption);
	for (int i = 0; i < pid.GetCount(); ++i) {
		if (wId[i] == _wId)
			return pid[i];
	}
	return 0;
} 
long GetWindowIdFromProcessId(long _pid)
{
	Array<long> wId, pid;
	Array<String> name, fileName, caption;
	GetWindowsList(wId, pid, name, fileName, caption);
	for (int i = 0; i < pid.GetCount(); ++i) {
		if (pid[i] == _pid)
			return wId[i];
	}
	return 0;
} 

bool ProcessExists(long pid)
{
	return DirectoryExists(Format("/proc/%s", FormatLong(pid)));
}
/////////////////////////////////////////////////////////////////////
// Os Info

#if defined(PLATFORM_WIN32) 

#if !defined(PRODUCT_ULTIMATE)
//#define PRODUCT_UNDEFINED                       0x00000000
#define PRODUCT_ULTIMATE                        0x00000001
#define PRODUCT_HOME_BASIC                      0x00000002
#define PRODUCT_HOME_PREMIUM                    0x00000003
#define PRODUCT_ENTERPRISE                      0x00000004
#define PRODUCT_HOME_BASIC_N                    0x00000005
#define PRODUCT_BUSINESS                        0x00000006
#define PRODUCT_STANDARD_SERVER                 0x00000007
#define PRODUCT_DATACENTER_SERVER               0x00000008
#define PRODUCT_SMALLBUSINESS_SERVER            0x00000009
#define PRODUCT_ENTERPRISE_SERVER               0x0000000A
#define PRODUCT_STARTER                         0x0000000B
#define PRODUCT_DATACENTER_SERVER_CORE          0x0000000C
#define PRODUCT_STANDARD_SERVER_CORE            0x0000000D
#define PRODUCT_ENTERPRISE_SERVER_CORE          0x0000000E
#define PRODUCT_ENTERPRISE_SERVER_IA64          0x0000000F
#define PRODUCT_BUSINESS_N                      0x00000010
#define PRODUCT_WEB_SERVER                      0x00000011
#define PRODUCT_CLUSTER_SERVER                  0x00000012
#define PRODUCT_HOME_SERVER                     0x00000013
#define PRODUCT_STORAGE_EXPRESS_SERVER          0x00000014
#define PRODUCT_STORAGE_STANDARD_SERVER         0x00000015
#define PRODUCT_STORAGE_WORKGROUP_SERVER        0x00000016
#define PRODUCT_STORAGE_ENTERPRISE_SERVER       0x00000017
#define PRODUCT_SERVER_FOR_SMALLBUSINESS        0x00000018
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM    0x00000019
#define PRODUCT_UNLICENSED                      0xABCDABCD
#endif

#if !defined(SM_SERVERR2)
#define SM_SERVERR2             89
#endif
#if !defined(VER_SUITE_WH_SERVER)
#define VER_SUITE_WH_SERVER		0x00008000
#endif
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

bool GetOsInfo(String &kernel, String &kerVersion, String &kerArchitecture, String &distro, String &distVersion, String &desktop, String &deskVersion)
{
   	OSVERSIONINFOEX osvi;
   	SYSTEM_INFO si;
   	PGNSI pGNSI;
   	PGPI pGPI;
   	BOOL bOsVersionInfoEx;
   	DWORD dwType;

   	ZeroMemory(&si, sizeof(SYSTEM_INFO));
   	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

   	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   	if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *) &osvi)))
      	return false;

   	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.
   	pGNSI = (PGNSI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
	if(NULL != pGNSI)
   		pGNSI(&si);
   	else 
   		GetSystemInfo(&si);

	kerVersion = Format("%d.%d", (int)osvi.dwMajorVersion, (int)osvi.dwMinorVersion);
	kernel = "Windows";
	if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
    	kerArchitecture = "64 bits";
	else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64)
      	kerArchitecture = "Itanium 64 bits";
   	else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL)
      	kerArchitecture = "32 bits";
	else
		kerArchitecture = "Unknown";	//PROCESSOR_ARCHITECTURE_UNKNOWN
   	if (VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && osvi.dwMajorVersion > 4 ) {
      	// Test for the specific product.
		if (osvi.dwMajorVersion == 6) {
			if (osvi.dwMinorVersion == 1) {
            	if (osvi.wProductType == VER_NT_WORKSTATION)
                	kernel.Cat(" Seven");
            	else 
					kernel.Cat(" Server 2008 R2");
			} else if (osvi.dwMinorVersion == 0) {
         		if (osvi.wProductType == VER_NT_WORKSTATION)
         			kernel.Cat(" Vista");
         		else
         			kernel.Cat(" Server 2008");
			}
         	pGPI = (PGPI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
         	pGPI(osvi.dwMajorVersion, osvi.dwMinorVersion, 
         		 osvi.wServicePackMajor, osvi.wServicePackMinor, &dwType);
         	switch(dwType) {
         	case PRODUCT_UNLICENSED:
               distro = "Unlicensed";
               break;
            case PRODUCT_ULTIMATE:
               distro = "Ultimate Edition";
               break;
            case PRODUCT_HOME_PREMIUM:
               distro = "Home Premium Edition";
               break;
            case PRODUCT_HOME_BASIC:	case PRODUCT_HOME_BASIC_N:
               distro = "Home Basic Edition";
               break;
            case PRODUCT_ENTERPRISE:
               distro = "Enterprise Edition";
               break;
            case PRODUCT_BUSINESS:		case PRODUCT_BUSINESS_N:
               distro = "Business Edition";
               break;
            case PRODUCT_STARTER:
               distro = "Starter Edition";
               break;
            case PRODUCT_CLUSTER_SERVER:
               distro = "Cluster Server Edition";
               break;
            case PRODUCT_DATACENTER_SERVER:
               distro = "Datacenter Edition";
               break;
            case PRODUCT_DATACENTER_SERVER_CORE:
               distro = "Datacenter Edition (core installation)";
               break;
            case PRODUCT_ENTERPRISE_SERVER:
               distro = "Enterprise Edition";
               break;
            case PRODUCT_ENTERPRISE_SERVER_CORE:
               distro = "Enterprise Edition (core installation)";
               break;
            case PRODUCT_ENTERPRISE_SERVER_IA64:
               distro = "Enterprise Edition for Itanium-based Systems";
               break;
            case PRODUCT_SMALLBUSINESS_SERVER:
               distro = "Small Business Server";
               break;
            case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
               distro = "Small Business Server Premium Edition";
               break;
            case PRODUCT_STANDARD_SERVER:
               distro = "Standard Edition";
               break;
            case PRODUCT_STANDARD_SERVER_CORE:
               distro = "Standard Edition (core installation)";
               break;
            case PRODUCT_WEB_SERVER:
               distro = "Web Server Edition";
               break;
         	}
      	} else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
         	if (GetSystemMetrics(SM_SERVERR2) )
         		kernel.Cat(" Server 2003 R2");
         	else if (osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER)
            	kernel.Cat(" Storage Server 2003");
     		else if (osvi.wSuiteMask & VER_SUITE_WH_SERVER)
            	kernel.Cat(" Home Server");
         	else if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	            kernel.Cat(" XP Professional x64 Edition");
         	else 
         		kernel.Cat(" Server 2003");
         	// Test for the server type.
         	if (osvi.wProductType != VER_NT_WORKSTATION ) {
            	if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 ) {
                	if(osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   		distro = "Datacenter Edition for Itanium-based Systems";
                	else if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   		distro = "Enterprise Edition for Itanium-based Systems";
            	}
			} else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 ) {
            	if(osvi.wSuiteMask & VER_SUITE_DATACENTER )
             		distro = "Datacenter x64 Edition";
                else if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   	distro = "Enterprise x64 Edition";
                else 
                	distro = "Standard x64 Edition";
            } else {
             	if (osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
                   	distro = "Compute Cluster Edition";
                else if(osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   	distro = "Datacenter Edition";
                else if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   	distro = "Enterprise Edition";
                else if (osvi.wSuiteMask & VER_SUITE_BLADE )
                   	distro = "Web Edition";
                else 
                	distro = "Standard Edition";
            }
    	} else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 ) {
         	kernel.Cat(" XP");
         	if(osvi.wSuiteMask & VER_SUITE_PERSONAL )
            	distro = "Home Edition";
         	else 
         		distro = "Professional";
      	} else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 ) {
         	kernel.Cat(" 2000");
         	if (osvi.wProductType == VER_NT_WORKSTATION)
            	distro = "Professional";
         	else {
            	if(osvi.wSuiteMask & VER_SUITE_DATACENTER)
               		distro = "Datacenter Server";
            	else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
               		distro = "Advanced Server";
            	else 
            		distro = "Server";
         	}
      	} 
       	// Include service pack (if any) and build number.
      	if(osvi.wServicePackMajor > 0)
			kerVersion.Cat(Format(" %s", osvi.szCSDVersion));

      	kerVersion.Cat(Format(" (Build %d)", (int)osvi.dwBuildNumber));
 	} else if (osvi.dwPlatformId == 1) {
 		switch(osvi.dwMinorVersion) {
 		case 0:
 			kernel.Cat(" 95");
 			break;
 		case 10:
 			kernel.Cat(" 98");
 			break;
 		case 90:
 			kernel.Cat(" Millennium");
 			break;
 		}
 		distro = "";
 	} else if (osvi.dwPlatformId == 2) {
    	switch(osvi.dwMajorVersion) {
    	kernel.Cat(" NT");
        case 3:
        	kernel.Cat(" 3.51");
        	break;
        case 4:
         	kernel.Cat(" 4.0");
         	break;
    	}
    	distro = "";
 	}
 	desktop = kernel;
 	distVersion = deskVersion = "";
   	return true;
}
#endif
#ifdef PLATFORM_POSIX

bool GetOsInfo_CheckLsb(String &distro, String &distVersion)
{
	StringParse lsb;			
	lsb = LoadFile_Safe("/etc/lsb-release");		
	if (lsb == "")
		return false;
	if(!lsb.GoAfter("DISTRIB_ID="))
		return false;				
	distro = ToLower(lsb.GetText());
	if (distro == "")
		return false;
	lsb.GoAfter_Init("DISTRIB_RELEASE=");
	distVersion = lsb.GetText();

	return true;
}
String GetDesktopManagerNew()
{
	String kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion;
	if (GetOsInfo(kernel, kerVersion, kerArchitecture, distro, distVersion, desktop, deskVersion))
	    return desktop;
	else
		return String("");
}
bool GetOsInfo(String &kernel, String &kerVersion, String &kerArchitecture, String &distro, String &distVersion, String &desktop, String &deskVersion)
{
	struct utsname buf;
	
	if (0 == uname(&buf)) {
		kernel = buf.sysname;
    	kerVersion = String(buf.release) + " " + String(buf.version);
        kerArchitecture = buf.machine;
	}
	if (kernel == "")
		kernel = LoadFile_Safe("/proc/sys/kernel/ostype");
	if (kernel == "")
		kernel = LoadFile_Safe("/proc/version");
	if (kernel == "") {
		if (Sys("sysctl_cmd -n kern.version").Find("FreeBSD") >= 0)
			kernel = "freebsd";
	}
	if (kerVersion == "")
		kerVersion = LoadFile_Safe("/proc/sys/kernel/osrelease") + " " + LoadFile_Safe("/proc/sys/kernel/version");
	if (kerArchitecture == "")
		kerArchitecture = Sys("uname -m");	// Kernel. See too /proc/version, /proc/version_signature and uname -a looking for architecture
	
	if (kernel == "")
		kernel = kerVersion = kerArchitecture = "UNKNOWN";	
	
	// Desktop
    if(GetEnv("GNOME_DESKTOP_SESSION_ID").GetCount() || GetEnv("GNOME_KEYRING_SOCKET").GetCount()) {
		desktop = "gnome";
		StringParse gnomeVersion = Sys("gnome-about --version");
		gnomeVersion.GoAfter("gnome-about");
		deskVersion = gnomeVersion.GetText();
	} else if(GetEnv("KDE_FULL_SESSION").GetCount() || GetEnv("KDEDIR").GetCount() || GetEnv("KDE_MULTIHEAD").GetCount()) {
        desktop = "kde"; 
        StringParse konsole = Sys("konsole --version");
        konsole.GoAfter("KDE:");
        deskVersion = konsole.GetText("\r\n");						
		if (deskVersion == "")		
			deskVersion = GetEnv("KDE_SESSION_VERSION");        
	} else {
		StringParse desktopStr;
		if (Sys("xprop -root _DT_SAVE_MODE").Find("xfce") >= 0)
			desktop = "xfce";
		else if ((desktopStr = Sys("xprop -root")).Find("ENLIGHTENMENT") >= 0) {
			desktop = "enlightenment";
			desktopStr.GoAfter("ENLIGHTENMENT_VERSION(STRING)", "=");
			desktopStr = desktopStr.GetText();
			if (desktopStr.GetText() == "Enlightenment")
				deskVersion = desktopStr.GetText();
		} else
			desktop = GetEnv("DESKTOP_SESSION");
	}
	if (desktop == "")
		desktop = deskVersion = "UNKNOWN";
	
	// Distro
	if (GetOsInfo_CheckLsb(distro, distVersion))
		;
	else if (FileExists("/usr/share/doc/ubuntu-minimal"))
		distro = "ubuntu";
	else if (FileExists("/etc/fedora-release")) {
		distro = "fedora";
		StringParse strFile = LoadFile_Safe("/etc/fedora-release");
		String str;
		do {
			str = strFile.GetText();
			if ((str != "fedora") && (str != "release"))
				distVersion << str << " ";
		} while (str != "");
	} else if (FileExists("/etc/redhat-release")) {
		distro = "redhat";
		distVersion = LoadFile_Safe("/etc/redhat-release");
	} else if (FileExists("/etc/SuSE-release")) {
		StringParse strFile = LoadFile_Safe("/etc/SuSE-release");
		distro = strFile.GetText();
		strFile.GoAfter_Init("VERSION", "=");
		distVersion = strFile.GetText();
	} else if (FileExists("/etc/mandrake-release")) {
		distro = "mandrake";			
		distVersion = LoadFile_Safe("/etc/mandrake-release");
	} else if (FileExists("/etc/mandriva-release")) {
		distro = "mandriva";	
		distVersion = LoadFile_Safe("/etc/mandriva-release");
	} else if (FileExists("/etc/aurox-release")) {
		distro = "aurox";
		distVersion = LoadFile_Safe("/etc/aurox-release");
	} else if (FileExists("/etc/altlinux-release")) {
		distro = "altlinux";			
		distVersion = LoadFile_Safe("/etc/altlinux-releas");
	} else if (FileExists("/etc/gentoo-release")) {
		distro = "gentoo";
		distVersion = LoadFile_Safe("/etc/gentoo-release");
	} else if (FileExists("/usr/portage")) {
		distro = "gentoo";
		distVersion = LoadFile_Safe("/usr/portage");
	} else if (FileExists("/etc/slackware-version")) {
		distro = "slackware";
		StringParse strFile = LoadFile_Safe("/etc/slackware-version");
		strFile.GetText();
		distVersion = strFile.GetText();
	} else if (FileExists("/etc/debian_version")) {
		distro = "debian";
		distVersion = LoadFile_Safe("/etc/debian_version");
	} else if (LoadFile_Safe("/etc/release").Find("Solaris") >= 0)
		distro = "solaris";
	else if (Sys("uname -r").Find("solaris") >= 0)
		distro = "solaris";
	else {					// If not try with /etc/osname_version
		distro = LoadFile_Safe("/etc/osname_version");
		distVersion = "";
	} 
	if (distro == "") 
		distro = LoadFile_Safe("/etc/issue");
	if (distro == "") 
		distro = distVersion = "UNKNOWN";
	
	return true;
}

#endif

/////////////////////////////////////////////////////////////////////
// Others

long    GetProcessId()			{return getpid();}

/////////////////////////////////////////////////////////////////////
// Drives list
#if defined(PLATFORM_WIN32)

Array<String> GetDriveList()
{
	char drvStr[26*4+1];		// A, B, C, ...
	Array<String> ret;
	
	int lenDrvStrs = ::GetLogicalDriveStrings(sizeof(drvStr), drvStr);
	// To get the error call GetLastError()
	if (lenDrvStrs == 0)
		return ret;
	
	ret.Add(drvStr);
	for (int i = 0; i < lenDrvStrs-1; ++i) {
		if (drvStr[i] == '\0') 
			ret.Add(drvStr + i + 1);
	}
	return ret;
}

bool GetDriveSpace(String drive, 
	//uint64 &totalBytes,	// To determine the total number of bytes on a disk or volume, use IOCTL_DISK_GET_LENGTH_INFO.
	uint64 &freeBytesUser, 	// Total number of free bytes on a disk that are available to the user who is associated with the calling thread.
	uint64 &totalBytesUser, // Total number of bytes on a disk that are available to the user who is associated with the calling thread.
	uint64 &totalFreeBytes)	// Total number of free bytes on a disk.
{
	StringBuffer sb(drive);	
	
	if(!GetDiskFreeSpaceEx(sb, (PULARGE_INTEGER)&freeBytesUser, (PULARGE_INTEGER)&totalBytesUser, (PULARGE_INTEGER)&totalFreeBytes))
		return false;
	//totalBytes = 0;
	return true;
}
// return true if mounted
bool GetDriveInformation(String drive, String &type, String &volume, /*uint64 &serial, */int &maxName, String &fileSystem)
{
	StringBuffer sb(drive);	
	
	switch (::GetDriveType(sb)) {
	case DRIVE_UNKNOWN: 	type = "Drive unknown";		break;
   	case DRIVE_NO_ROOT_DIR: type = "The root directory does not exist";	break;
	case DRIVE_REMOVABLE:
          switch (*sb) {
          case 'A':
          case 'B': 		type = "Floppy";		
          					volume = fileSystem = "";
          					/*serial = */maxName = 0;
          					return false;
          default: 			type = "Removable";	break;
          }
          break;
   	case DRIVE_FIXED:   	type = "Hard";		break;
    case DRIVE_REMOTE:  	type = "Network";	break;
    case DRIVE_CDROM:   	type = "Optical";	break;
    case DRIVE_RAMDISK: 	type = "RAM";		break;
   	}
   	char vol[MAX_PATH], fs[MAX_PATH];
   	long flags;
   	uint64 serial;
   	uint64 _maxName;
   	if(!::GetVolumeInformation(sb, vol, MAX_PATH, (LPDWORD)&serial, (LPDWORD)&_maxName, (LPDWORD)&flags, fs, MAX_PATH)) {
   		if (type == "Optical") {
   			volume = "";
   			fileSystem = "";
   			maxName = 0;
   			return true;
   		} else
   			return false;
   	}
   	volume = vol;
   	fileSystem = fs;
   	maxName = (int)_maxName;
   	
   	return true;
}

#elif defined(PLATFORM_POSIX)

Array<String> GetDriveList()
{
	Array<String> ret;
	// Search for mountable file systems
	String mountableFS;
	StringParse sfileSystems(LoadFile_Safe("/proc/filesystems"));
	String str;
	while (true) {
		str = sfileSystems.GetText();	
		if (str == "")
			break;
		else if (str != "nodev")
			mountableFS << str << ".";
		else 
			str = sfileSystems.GetText();
	}
	// Get mounted drives
	StringParse smounts(LoadFile_Safe("/proc/mounts"));
	StringParse smountLine(smounts.GetText("\r\n"));
	do {
		String devPath 	 = smountLine.GetText();
		String mountPath = smountLine.GetText();
		String fs        = smountLine.GetText();
		if ((mountableFS.Find(fs) >= 0) && (mountPath.Find("/dev") < 0) && (mountPath.Find("/rofs") < 0))		// Is mountable
			ret.Add(mountPath);
		smountLine = smounts.GetText("\r\n");
	} while (smountLine != "");
	
	return ret;
}
bool GetDriveSpace(String drive, 
	//uint64 &totalBytes,	// To determine the total number of bytes on a disk or volume, use IOCTL_DISK_GET_LENGTH_INFO.
	uint64 &freeBytesUser, 	// Total number of free bytes on a disk that are available to the user who is associated with the calling thread.
	uint64 &totalBytesUser, // Total number of bytes on a disk that are available to the user who is associated with the calling thread.
	uint64 &totalFreeBytes)	// Total number of free bytes on a disk.
{
	freeBytesUser = totalBytesUser = totalFreeBytes = 0;
	
	StringParse space = Sys("df -T");
	if (space == "")
		return false;
	
	while (drive != space.GetText())
		;
	space.MoveRel(-10);	space.GoBeginLine(); 
	space.GetText();	space.GetText();	// Jumps over device path and filesystem
	totalBytesUser = 1024*space.GetUInt64();
	space.GetText();						// Jumps over used space
	freeBytesUser  = totalFreeBytes = 1024*space.GetUInt64();
	return true;
}

// return true if mounted
bool GetDriveInformation(String drive, String &type, String &volume, /*uint64 &serial, */int &maxName, String &fileSystem)
{
	StringParse info = Sys("mount -l");
	if (info  == "")
		return false;
	String straux;
	while (drive != (straux = info.GetText()))
		if (straux == "")
			return false;
	       
	if("type" != info.GetText())		// Jumps over "type"
		return false;

	fileSystem = info.GetText();
	String details = info.GetText();
	info.GoAfter("[");
	volume = info.GetText("]");
	//serial = 0;			// Unknown		
	if ((fileSystem == "udf" || fileSystem == "iso9660") && details.Find("ro") >=0)
		type = "Optical";
	else if (details.Find("flush") >=0)
		type = "Removable";
	else
		type = "Hard";
		
	struct statfs buf;
	if (0 == statfs(drive, &buf)) {
		//puts(Format("%x", buf.f_type)); // Filesystem type
		maxName = buf.f_namelen; 	
	} else {
		maxName = 0;
	}	
}

#endif

#if defined (PLATFORM_WIN32)
unsigned __int64 spstart, spstop;
unsigned __int64 nCtr, nFreq, nCtrStop;

#if defined(__MINGW32__)
int GetCpuSpeed()
{
    if(!QueryPerformanceFrequency((LARGE_INTEGER *) &nFreq)) 
    	return 0;

    QueryPerformanceCounter((LARGE_INTEGER *)&nCtrStop);
    nCtrStop += nFreq/10000;
    
    __asm__(".byte 0x0F");
    __asm__(".byte 0x31");
    __asm__("mov %eax,_spstart");
    __asm__("mov %edx,4+(_spstart)");
	
    do
        QueryPerformanceCounter((LARGE_INTEGER *)&nCtr);
    while (nCtr < nCtrStop);

    __asm__(".byte 0x0F");
    __asm__(".byte 0x31");
    __asm__("mov %eax,_spstop");
    __asm__("mov %edx,4+(_spstop)");

    return (int)((spstop-spstart)/100); 
}
#elif defined(_MSC_VER)
int GetCpuSpeed()
{
    if(!QueryPerformanceFrequency((LARGE_INTEGER *) &nFreq)) 
    	return 0;
    QueryPerformanceCounter((LARGE_INTEGER *) &nCtrStop);
    nCtrStop += nFreq/10000;								
    
    _asm {
     	__asm _emit 0x0f 
     	__asm _emit 0x31
    	mov DWORD PTR spstart, eax
   		mov DWORD PTR [spstart + 4], edx
    }
    do 
         QueryPerformanceCounter((LARGE_INTEGER *) &nCtr);
    while (nCtr < nCtrStop);
    _asm {
        __asm _emit 0x0f 
        __asm _emit 0x31
        mov DWORD PTR spstop, eax
        mov DWORD PTR [spstop + 4], edx
 	}
	return (int)((spstop-spstart)/100);
}
#endif 
#endif
#if defined(PLATFORM_POSIX)

#define RDTSC_READ(tm) __asm__ __volatile__ (".byte 0x0f; .byte 0x31" :"=a" (tm))
#define COUNT_SEC   (double)tv.tv_sec + (1.e-6)*tv.tv_usec

int GetCpuSpeed()
{
	struct timeval tv;
  	double cnt1,cnt2;
  	unsigned long start,end;

  	RDTSC_READ(start);
  	gettimeofday(&tv, 0);
  	cnt1 = COUNT_SEC + 0.01;

  	do {
    	gettimeofday(&tv, 0);
    	cnt2 = COUNT_SEC;
  	} while(cnt2 < cnt1);

  	RDTSC_READ(end);

  	return (int)((end-start)/10000);
}
#endif
#if defined(PLATFORM_WIN32)

#define SHTDN_REASON_MINOR_OTHER 0

bool Shutdown(String action)
{
	unsigned int flag;
	if (action == "logoff")
		flag = EWX_LOGOFF;
//	else if (action == "poweroff")
//		flag = EWX_POWEROFF;
	else if (action == "reboot")
		flag = EWX_REBOOT;
	else if (action == "shutdown")
		flag = EWX_SHUTDOWN;
	else
		return false;
	
   	HANDLE hToken; 
   	TOKEN_PRIVILEGES tkp; 
 
   // Get a token for this process. 
   	if (!OpenProcessToken(GetCurrentProcess(), 
     	TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
 		return false; 
 
   // Get the LUID for the shutdown privilege. 
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
 
   tkp.PrivilegeCount = 1;  // one privilege to set    
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
   // Get the shutdown privilege for this process. 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
        (PTOKEN_PRIVILEGES)NULL, 0); 
 
   if (GetLastError() != ERROR_SUCCESS) 
      return false; 
 
   // Shut down the system and force all applications to close. 
   if (!ExitWindowsEx(flag | EWX_FORCE, SHTDN_REASON_MINOR_OTHER)) 
      return false; 

   return true;
}
#endif

#ifdef PLATFORM_POSIX
bool Shutdown(String action)
{
	if (action == "logoff") {
		kill(1, SIGTSTP);
		sync();
		signal(SIGTERM, SIG_IGN);
		setpgrp();
		kill(-1, SIGTERM);
		sleep(1);
		kill(-1, SIGHUP); //* Force PPPD's down, too *
		sleep(1);
		kill(-1, SIGKILL);
		sync();
		sleep(1);
	} else if (action == "shutdown") {
#if __GNU_LIBRARY__ > 5
		reboot(0xCDEF0123);
#else
		reboot(0xfee1dead, 672274793, 0xCDEF0123);
#endif
	} else if (action == "reboot") {		// LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2
#if __GNU_LIBRARY__ > 5
		reboot(0x01234567);
#else
		reboot(0xfee1dead, 672274793, 0x01234567);
#endif
	} 
	exit(0);
	return true; 
}
#endif

String BytesToString(uint64 _bytes)
{
	String ret;
	uint64 bytes = _bytes;
	
	if (bytes >= 1024) {
		bytes /= 1024;
		if (bytes >= 1024) {
			bytes /= 1024;
			if (bytes >= 1024) {
				bytes /= 1024;
				if (bytes >= 1024) {
					bytes /= 1024;
					ret = Format("%.2f%s", _bytes/(1024*1024*1024*1024.), "Tb");
				} else
					ret = Format("%.2f%s", _bytes/(1024*1024*1024.), "Gb");
			} else
				ret = Format("%.2f%s", _bytes/(1024*1024.), "Mb");
		} else
			ret = Format("%.2f%s", _bytes/1024., "Kb");
	} else
		ret << _bytes << "b";
	return ret;
}

void GetCompilerInfo(String &name, int &version, String &date)
{
	name = "";
	version = 0;
	date = __DATE__;
	#if defined(WIN32) 
		#if defined(__MINGW32__)
			name = "mingw";
			version = __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__;	// __VERSION__
		#elif defined(COMPILER_MSC) 
			name = "msc";
			version = _MSC_FULL_VER;
		#elif defined (__BORLANDC__)
			name = "borlandc"
			version = __BORLANDC__;
		#elif defined (__WATCOMC__)
			name = "watcomc"
			version = __WATCOMC__;
		#endif
	#elif defined (PLATFORM_POSIX)
	    #if defined(__GNUC__)
	    	name = "gnuc";
	    	version = __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__;
	    #endif
    #elif defined (__APPLE__) 
    	// In a next future?
	#endif
}
	

#ifdef PLATFORM_POSIX
bool GetBatteryStatus(bool &discharging, int &percentage, int &remainingMin)
{
	percentage = 100;
	Array<String> files = SearchFile("/proc/acpi/battery", "state");
	if (files.GetCount() == 0)
		return false;
	StringParse state = LoadFile_Safe(files[0]);
	if (state == "")
		return false;
	bool present;
	if(!state.GoAfter_Init("present", ":"))
		return false;			
	present = state.GetText() == "yes";
	if (!present)
		return false;	// No battery inserted
	state.GoAfter_Init("charging state", ":");	 	discharging = state.GetText() == "discharging";
	int presentRate, remainingCapacity;
	state.GoAfter_Init("present rate", ":");		presentRate = state.GetInt();
	state.GoAfter_Init("remaining capacity", ":");	remainingCapacity = state.GetInt();
	if (presentRate == 0 || !discharging)
		remainingMin = 10000;
	else
		remainingMin = (int)((60.*remainingCapacity)/presentRate);
	
	int designCapacity,lastFullCapacity;
	String vendor, type, model, serial;
	if (!GetBatteryInfo(present/*, designCapacity, lastFullCapacity, vendor, type, model, serial*/))
		percentage = (int)((100.*remainingCapacity)/lastFullCapacity);
	return true;
}
bool GetBatteryInfo(bool &present/*, int &designCapacity, int &lastFullCapacity, String &vendor, String &type, String &model, String &serial*/)
{
	Array<String> files = SearchFile("/proc/acpi/battery", "info");
	if (files.GetCount() == 0)
		return false;
	StringParse info = LoadFile_Safe(files[0]);
	if (info == "")
		return false;
	info.GoAfter_Init("present", ":");			present = info.GetText() == "yes";
	/*
	info.GoAfter_Init("design capacity", ":");	designCapacity = info.GetInt();
	info.GoAfter_Init("last full capacity", ":");lastFullCapacity = info.GetInt();
	info.GoAfter_Init("OEM info", ":");		 	vendor = info.GetText();
	info.GoAfter_Init("battery type", ":");		type = info.GetText();
	info.GoAfter_Init("model number", ":");		model = info.GetText();
	info.GoAfter_Init("serial number", ":");	serial = info.GetText();
	*/
	return true;
}

bool OpenCDTray(String drive)
{
	String dummy;
	return Sys("eject", dummy) > 0;	
}
bool CloseCDTray(String drive)
{
	String dummy;
	return Sys("eject -t", dummy) > 0;	
}

#endif
#if defined(PLATFORM_WIN32)
bool GetBatteryStatus(bool &discharging, int &percentage, int &remainingMin)
{
	SYSTEM_POWER_STATUS power;
	
	if(::GetSystemPowerStatus(&power) == 0)
		return false;
	
	if (power.ACLineStatus == 1)
		discharging = false;
	else
		discharging = true;
	percentage = power.BatteryLifePercent;
	remainingMin = (int)(power.BatteryLifeTime/60);
	
	return true;
}
bool GetBatteryInfo(bool &present/*, int &designCapacity, int &lastFullCapacity, String &vendor, String &type, String &model, String &serial*/)	
{
	SYSTEM_POWER_STATUS power;
	
	if(::GetSystemPowerStatus(&power) == 0)
		return false;
	if (power.BatteryFlag == 128)
		return false;			// No battery
	
	//designCapacity = (int)(power.BatteryFullLifeTime/60.);
	
	present = true;
	//power.ACLineStatus == 0;
	//lastFullCapacity = 0;
	//vendor = type = model = serial = "UNKNOWN";
	
	return true;
}

bool DriveOpenClose(String drive, bool open)
{
	int operation;
	if (open)
		operation = IOCTL_STORAGE_EJECT_MEDIA;
	else
		operation = IOCTL_STORAGE_LOAD_MEDIA;
	if (drive.IsEmpty())
		return false;
	else if (drive.GetCount() == 1)
		drive += ":";
	else {
		drive = drive.Left(2);
		if (drive[1] != ':')
			return false;
	}
	HANDLE hDrive;
	hDrive = CreateFile("\\\\.\\" + drive, GENERIC_READ || GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hDrive == INVALID_HANDLE_VALUE)
		return false;
	bool ret = false;
	DWORD dummyBytesReturned;
	if (DeviceIoControl(hDrive, operation, 0, 0, 0, 0, &dummyBytesReturned, 0))
		ret = true;
  	CloseHandle(hDrive);
  	return ret;
}

bool OpenCDTray(String drive)
{
	return DriveOpenClose(drive, true);
}

bool CloseCDTray(String drive)
{
	return DriveOpenClose(drive, false);
}

#endif

void Mouse_LeftClick()
{
    Mouse_LeftDown(); 
    Mouse_LeftUp(); 
}
void Mouse_RightClick()
{
    Mouse_RightDown();
    Mouse_RightUp();
}
void Mouse_MiddleClick()
{
    Mouse_MiddleDown();
    Mouse_MiddleUp();
}
void Mouse_LeftDblClick()
{
	Mouse_LeftClick();
	Mouse_LeftClick();
}
void Mouse_MiddleDblClick()
{
	Mouse_MiddleClick();
	Mouse_MiddleClick();
}
void Mouse_RightDblClick()
{
	Mouse_RightClick();
	Mouse_RightClick();
}

struct KeyCodes {
	String key; 
	int code;
}; 

#if defined(PLATFORM_WIN32)

Array <String> GetWinRegSubkeys(const String& key, HKEY base) {
	HKEY hkey;
	Array <String> subkeys;
	if(RegOpenKeyEx(base, key, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return subkeys;
	char temp[_MAX_PATH];
	dword len;
	for(dword dw = 0; len = sizeof(temp), RegEnumKeyEx(hkey, dw, temp, &len, 0, 0, 0, 0) == ERROR_SUCCESS; dw++)
		subkeys.Add(temp);
	RegCloseKey(hkey);
	return subkeys;
}

void Mouse_LeftDown()
{
    mouse_event (MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}
void Mouse_LeftUp()
{
    mouse_event (MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
void Mouse_MiddleDown()
{
    mouse_event (MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
}
void Mouse_MiddleUp()
{
    mouse_event (MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
}
void Mouse_RightDown()
{
    mouse_event (MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
}
void Mouse_RightUp()
{
    mouse_event (MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}

bool PutWindowPlacement(HWND hwnd, RECT rcNormalPosition, POINT ptMinPosition, POINT ptMaxPosition, long showcmd, long flags)
{
    WINDOWPLACEMENT place;
        
    place.ptMinPosition = ptMinPosition;
    place.ptMaxPosition = ptMaxPosition;
    place.rcNormalPosition = rcNormalPosition;
    place.showCmd = showcmd;
    place.flags = flags;
    place.length = sizeof(place);
    return SetWindowPlacement(hwnd, &place);
}
bool TakeWindowPlacement(HWND hwnd, RECT &rcNormalPosition, POINT &ptMinPosition, POINT &ptMaxPosition, long &showcmd)
{
    WINDOWPLACEMENT place;
    
    place.length = sizeof(place);
    bool ret = GetWindowPlacement(hwnd, &place);
    ptMinPosition = place.ptMinPosition;
    ptMaxPosition = place.ptMaxPosition;
    rcNormalPosition = place.rcNormalPosition;
    showcmd = place.showCmd;     //SW_SHOWMAXIMIZED, SW_SHOWMINIMIZED, SW_SHOWNORMAL
    //flags = place.flags;        // Always 0
    
    return ret;
}
bool Window_GetRect(long windowId, long &left, long &top, long &right, long &bottom)
{
	RECT rcNormalPosition;
    POINT ptMinPosition, ptMaxPosition;
    long showcmd;	
    
	TakeWindowPlacement((HWND)windowId, rcNormalPosition, ptMinPosition, ptMaxPosition, showcmd);
	
	left = rcNormalPosition.left;
	top = rcNormalPosition.top;
	right = rcNormalPosition.right;
	bottom = rcNormalPosition.bottom;
	
	return true;
}
void Window_SetRect(long windowId, long left, long top, long right, long bottom)
{
	RECT rcNormalPosition;
    POINT ptMinPosition, ptMaxPosition;
    long showcmd;	
    
	TakeWindowPlacement((HWND)windowId, rcNormalPosition, ptMinPosition, ptMaxPosition, showcmd);
	
	rcNormalPosition.left = left;
	rcNormalPosition.top = top;
	rcNormalPosition.right = right;
	rcNormalPosition.bottom = bottom;
	PutWindowPlacement((HWND)windowId, rcNormalPosition, ptMinPosition, ptMaxPosition, showcmd, 0);
}
bool Mouse_SetPos(long xMove, long yMove, long windowId)
{
    long left, top, right, bottom;
    
    if (windowId != 0) {
        Window_GetRect(windowId, left, top, right, bottom);
        xMove = xMove + left;
        yMove = yMove + top;
    }
    SetCursorPos(xMove, yMove);
    //DoEvents();
    
    return true;
}
bool Mouse_GetPos(long &x, long &y)
{
    POINT p;
    
    GetCursorPos (&p);
    x = p.x;
    y = p.y;
    
    return true;
}

KeyCodes keyCodes[60] = {
	"NUMPAD7", 	VK_NUMPAD7, 	"BACK", 	VK_BACK, 
	"NUMPAD8", 	VK_NUMPAD8,		"TAB", 		VK_TAB,
	"NUMPAD9", 	VK_NUMPAD9, 	"RETURN", 	VK_RETURN,
	"MULTIPLY", VK_MULTIPLY, 	"SHIFT",	VK_SHIFT,
	"ADD",		VK_ADD, 	 	"CONTROL",	VK_CONTROL,
	"SEPARATOR", VK_SEPARATOR,	"MENU", 	VK_MENU,
	"SUBTRACT", VK_SUBTRACT, 	"PAUSE", 	VK_PAUSE,
	"DECIMAL",	VK_DECIMAL,		"CAPITAL", 	VK_CAPITAL,
	"DIVIDE",	VK_DIVIDE, 		"ESCAPE",	VK_ESCAPE,
	"F1", 		VK_F1, 			"SPACE", 	VK_SPACE,
	"F2", 		VK_F2,	 		"END",		VK_END,
	"F3",		VK_F3, 			"HOME",		VK_HOME,
	"F4",		VK_F4, 			"LEFT", 	VK_LEFT,
	"F5",		VK_F5,		 	"UP", 		VK_UP,
	"F6",		VK_F6,		 	"RIGHT",	VK_RIGHT,
	"F7",		VK_F7,		 	"DOWN",		VK_DOWN,
	"F8",		VK_F8,		 	"PRINT",	VK_PRINT,
	"F9",		VK_F9,		 	"SNAPSHOT",	VK_SNAPSHOT,
	"F10",		VK_F10,		 	"INSERT",	VK_INSERT,
	"F11",		VK_F11,		 	"DELETE",	VK_DELETE,
	"F12",		VK_F12,		 	"LWIN",		VK_LWIN,
	"NUMLOCK",	VK_NUMLOCK,	 	"RWIN",		VK_RWIN,
	"SCROLL",	VK_SCROLL,	 	"NUMPAD0",	VK_NUMPAD0,
	"LSHIFT",	VK_LSHIFT,	 	"NUMPAD1", 	VK_NUMPAD1,
	"RSHIFT",	VK_RSHIFT,	 	"NUMPAD2",	VK_NUMPAD2,
	"LCONTROL",	VK_LCONTROL, 	"NUMPAD3",	VK_NUMPAD3,
	"RCONTROL",	VK_RCONTROL, 	"NUMPAD4",	VK_NUMPAD4,
	"LMENU",	VK_LMENU, 		"NUMPAD5",	VK_NUMPAD5,
	"RMENU",	VK_RMENU, 		"NUMPAD6",	VK_NUMPAD6,
	/*"PGUP", 	XK_Page_Up, 	"PGDOWN", 	XK_Page_Down
	"CAPSLOCK", XK_Caps_Lock, 	"BACKSPACE",XK_BackSpace	*/
	""
};

void PressKeyVK(int keyVK, bool hold = false, bool release = false, bool compatible = false)
{    
    long nScan, nExtended;
        
    nScan = MapVirtualKey(keyVK, 2);
    nExtended = 0;
    if (nScan == 0)
        nExtended = KEYEVENTF_EXTENDEDKEY;
    nScan = MapVirtualKey(keyVK, 0);
    
    if (compatible)
        nExtended = 0;
    
    if (!release)
        keybd_event ((BYTE)keyVK, (BYTE)nScan, nExtended, 0);
    
    if (!hold)
        keybd_event ((BYTE)keyVK, (BYTE)nScan, KEYEVENTF_KEYUP | nExtended, 0);
}

#if defined(__MINGW32__)
	#define	MAPVK_VK_TO_VSC		0
	#define	MAPVK_VSC_TO_VK   	1
	#define	MAPVK_VK_TO_CHAR  	2
	#define	MAPVK_VSC_TO_VK_EX 	3
#endif
#define	MAPVK_VK_TO_VSC_EX 	4

// This is less nice but more compatible for Notepad and MSWord for example
void PressKey(wchar key, bool hold = false, bool release = false)
{
	String numStr = FormatIntDec(key, 5, '0');
	PressKeyVK(VK_LMENU, true);
	PressKeyVK(VK_NUMPAD0 + numStr[0] - '0');
	PressKeyVK(VK_NUMPAD0 + numStr[1] - '0');
	PressKeyVK(VK_NUMPAD0 + numStr[2] - '0');
	PressKeyVK(VK_NUMPAD0 + numStr[3] - '0');
	PressKeyVK(VK_NUMPAD0 + numStr[4] - '0');
	PressKeyVK(VK_LMENU, false, true);
}
/*
void PressKey(wchar key, bool hold = false, bool release = false)
{
	bool caps, num, scroll;
	if (IsLetter(key)) {
    	GetKeyLockStatus(caps, num, scroll);
    	if (caps) 
    		SetKeyLockStatus(false, num, scroll);	
    }
 	char buff[120];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILANGUAGE, buff, sizeof(buff));
	HKL hKeyboardLayout = ::LoadKeyboardLayout(buff, KLF_ACTIVATE);  
    SHORT nVK = VkKeyScanExW(key, hKeyboardLayout);
	if (nVK == -1) {	// Last resource !!
		String numStr = FormatIntDec(key, 4, '0');
		PressKeyVK(VK_LMENU, true);
		PressKeyVK(VK_NUMPAD0 + numStr[0] - '0');
		PressKeyVK(VK_NUMPAD0 + numStr[1] - '0');
		PressKeyVK(VK_NUMPAD0 + numStr[2] - '0');
		PressKeyVK(VK_NUMPAD0 + numStr[3] - '0');
		PressKeyVK(VK_LMENU, false, true);
    	return;
	}
    UINT nScan = MapVirtualKeyExW(nVK, MAPVK_VK_TO_CHAR, hKeyboardLayout);
    long nExtended = 0;
    if (nScan == 0)
        nExtended = KEYEVENTF_EXTENDEDKEY;
    
    nScan = MapVirtualKeyExW(nVK, MAPVK_VK_TO_VSC, hKeyboardLayout);
    
    bool shift, ctrl, alt;
    
    shift = nVK & 0x100;
    ctrl  = nVK & 0x200;
    alt   = nVK & 0x400;    
    nVK   = nVK & 0xFF;
    
    if (!release) {
        if (shift)
            keybd_event (VK_SHIFT, 0, 0, 0);
        if (ctrl)
            keybd_event (VK_CONTROL, 0, 0, 0);
        if (alt)
            keybd_event (VK_MENU, 0, 0, 0);
    
        keybd_event ((BYTE)nVK, (BYTE)nScan, nExtended, 0);
    }
    if (!hold) {
        keybd_event ((BYTE)nVK, (BYTE)nScan, KEYEVENTF_KEYUP | nExtended, 0);
    
   		if (shift)
            keybd_event (VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        if (ctrl)
            keybd_event (VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
        if (alt)
            keybd_event (VK_MENU, 0, KEYEVENTF_KEYUP, 0);
    }
    if (IsLetter(key) && caps) 
    	SetKeyLockStatus(true, num, scroll);	
}
*/

bool GetKeyLockStatus(bool &caps, bool &num, bool &scroll)
{
    caps = GetKeyState(VK_CAPITAL);
    num = GetKeyState(VK_NUMLOCK);
    scroll = GetKeyState(VK_SCROLL);
    return true;
}
bool SetKeyLockStatus(bool caps, bool num, bool scroll)
{
	bool capsnow, numnow, scrollnow;
	
	GetKeyLockStatus(capsnow, numnow, scrollnow);
	if (capsnow != caps)
		PressKeyVK(VK_CAPITAL);
	if (numnow != num)
		PressKeyVK(VK_NUMLOCK);
	if (scrollnow != scroll)
		PressKeyVK(VK_SCROLL);
	return true;
}

#if defined(__MINGW32__) 
	#define labs(x)	labs((Upp::int64)(x))
#elif defined(_MSC_VER)
	#define labs(x)	abs(x)
#endif

bool Window_SaveCapture(long windowId, String fileName, int left, int top, int width, int height)
{
	if (windowId == 0)
		windowId = (long)GetDesktopWindow();

	if (GetFileExt(fileName) != ".bmp")
		fileName += ".bmp";
	 
	RECT rc;
	GetWindowRect ((HWND)windowId, &rc); 

	if (left == -1)
		left = rc.left;
	if (top == -1)
		top = rc.top;
	if (width == -1)
		width	= rc.right-rc.left;
	if (height == -1)
		height	= rc.bottom-rc.top;

	HDC hDC = GetDC(0);
	HDC memDC = CreateCompatibleDC (hDC);
	HBITMAP hb = CreateCompatibleBitmap (hDC, width, height);
	HBITMAP OldBM = (HBITMAP)SelectObject(memDC, hb);
	BitBlt(memDC, 0, 0, width, height , hDC, left, top , SRCCOPY);

    FILE *file = NULL;
  	BITMAPINFO bmpInfo;
    BITMAPFILEHEADER bmpFileHeader;
    ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    GetDIBits(hDC, hb, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);
   	if(bmpInfo.bmiHeader.biSizeImage <= 0)
     	bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth*labs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount+7)/8;
   	char *cbuf = new char[bmpInfo.bmiHeader.biSizeImage];
   	LPVOID buf = cbuf;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	GetDIBits(hDC, hb, 0, bmpInfo.bmiHeader.biHeight, buf, &bmpInfo, DIB_RGB_COLORS);
	if((file = _wfopen(fileName.ToWString(),L"wb")) == NULL)
  		return false;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+bmpInfo.bmiHeader.biSizeImage;
	bmpFileHeader.bfType = 19778;
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&bmpInfo.bmiHeader,sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(buf,bmpInfo.bmiHeader.biSizeImage, 1, file);
	
	delete[] cbuf;
	fclose(file);

	SelectObject(hDC, OldBM);
	DeleteObject(hb);
	DeleteDC(memDC);
	ReleaseDC(0, hDC);
	
	return true;
}

class ScreenGrab {
private:	
	enum {GRAB_MODE_DESKTOP, GRAB_MODE_WINDOW, GRAB_MODE_RECT};
	static HDC lDCDest;
	static HGDIOBJ lDib;
	static void *lDibPtr;
	static HGDIOBJ lBmpOld;
	static BITMAPINFO tBmpInfo;
	static PAVIFILE lAVIPtrFile;
	static PAVISTREAM lAVIPtrStrm;
	static AVISTREAMINFO tAVIHdr;
	static AVIFILEINFO tAVIFile;
	long lAVICnt;
	double frameRate;
	long numFrames; 		// number of frames in video stream
	long firstFrame; 		// position of the first video frame
	String fileName;
	bool viewMouse;
	bool opened;
	int grabMode;
	int hwnd;
	int left;
	int top; 
	int width;
	int height;

	bool AVIOpen(bool create = true);
	bool AVIWrite();
	void AVIClose();
	HICON GetCursorHandle();
	bool DIBCreate(HWND lHandleSource, long lWidth, long lHeight, long lPosX, long lPosY);
	void DIBClean();
	bool ScreenshotMemory();

public:
	ScreenGrab(String fileName, int secsFrame = 1, bool viewMouse = true);
	~ScreenGrab();
	
	bool IniGrabDesktop();
	bool IniGrabWindow(long handle);
	bool IniGrabDesktopRectangle(int left, int top, int width, int height);
	bool Grab(unsigned duration);
	bool GrabSnapshot();
	void Close();
};

bool Record_Desktop(String fileName, int duration, int secsFrame, bool viewMouse)
{
	ScreenGrab grab(fileName, secsFrame, viewMouse);
	if (!grab.IniGrabDesktop())
		return false;
	if (!grab.Grab(duration))
		return false;
	grab.Close();
	return true;
}
bool Record_DesktopRectangle(String fileName, int duration, int left, int top, int width, int height, int secsFrame, bool viewMouse)
{
	ScreenGrab grab(fileName, secsFrame, viewMouse);
	if (!grab.IniGrabDesktopRectangle(left, top, width, height))
		return false;
	if (!grab.Grab(duration))
		return false;
	grab.Close();
	return true;
}
bool Record_Window(String fileName, int duration, long handle, int secsFrame, bool viewMouse)
{
	ScreenGrab grab(fileName, secsFrame, viewMouse);
	if (!grab.IniGrabWindow(handle))
		return false;
	if (!grab.Grab(duration))
		return false;
	grab.Close();
	return true;
}

bool ScreenGrab::AVIOpen(bool create)
{
    long lRet, mode, res;
    
    AVIFileInit();
    if (create)
        mode = OF_CREATE | OF_WRITE;
    else
        mode = OF_SHARE_DENY_WRITE;

    lRet = AVIFileOpen(&lAVIPtrFile, fileName, mode, 0);
    if (lRet == AVIERR_OK && create) {
      	tAVIHdr.fccType = streamtypeVIDEO;
        tAVIHdr.fccHandler = 0;
       	tAVIHdr.dwScale = 100;		
       	tAVIHdr.dwRate = (DWORD)(tAVIHdr.dwScale*frameRate);	
       	//tAVIHdr.dwQuality = -1;
       	tAVIHdr.dwSuggestedBufferSize = tBmpInfo.bmiHeader.biSizeImage;
        SetRect(&(tAVIHdr.rcFrame), 0, 0, tBmpInfo.bmiHeader.biWidth, tBmpInfo.bmiHeader.biHeight);
        lRet = AVIFileCreateStream(lAVIPtrFile, &lAVIPtrStrm, &tAVIHdr);
        if (lRet == AVIERR_OK) {
            lRet = AVIStreamSetFormat(lAVIPtrStrm, 0, &(tBmpInfo.bmiHeader), sizeof(tBmpInfo.bmiHeader));
            if (lRet == AVIERR_OK) 
                lAVICnt = 0;
        }
    } else {
        res = AVIFileGetStream(lAVIPtrFile, &lAVIPtrStrm, streamtypeVIDEO, 0);
        if (res != AVIERR_OK)
            return false;
        firstFrame = AVIStreamStart(lAVIPtrStrm);
        if (firstFrame != -1) 
			return false;
        numFrames = AVIStreamLength(lAVIPtrStrm); 
        if (numFrames == -1)
        	return false;
        res = AVIFileInfo(lAVIPtrFile, &tAVIFile, sizeof(tAVIFile));
        if (res != AVIERR_OK)
        	return false;
        res = AVIStreamInfo(lAVIPtrStrm, &tAVIHdr, sizeof(tAVIHdr));
        if (res != AVIERR_OK) 
			return false;
    }
    return true;
}

bool ScreenGrab::AVIWrite()
{
    HRESULT  lRet;
    
    lRet = AVIStreamWrite(lAVIPtrStrm, lAVICnt, 1, lDibPtr, tBmpInfo.bmiHeader.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
    if (lRet == AVIERR_OK) {
        lAVICnt++;
        return true;
    } else
        return false;
}

void ScreenGrab::AVIClose()
{
    if (lAVIPtrStrm != 0) 
    	AVIStreamClose(lAVIPtrStrm);
    if (lAVIPtrFile != 0) 
		AVIFileClose(lAVIPtrFile);
    AVIFileExit();
}

HICON ScreenGrab::GetCursorHandle()
{
    HWND lHandle;
    POINT lpPos;
    long lThreadID;
    long lCurrentThreadID;
 
  	GetCursorPos(&lpPos);
    lHandle = WindowFromPoint(lpPos);
    lThreadID = GetWindowThreadProcessId(lHandle, 0);
    lCurrentThreadID = GetWindowThreadProcessId((HWND)GetWindowIdFromProcessId(GetProcessId()), 0); 
    HICON ret;
    if (lThreadID != lCurrentThreadID) {
        if (AttachThreadInput(lCurrentThreadID, lThreadID, true)) {
            ret = GetCursor();
            AttachThreadInput(lCurrentThreadID, lThreadID, false);
        }
    } else
        ret = GetCursor();
   
    return ret;
}

bool ScreenGrab::DIBCreate(HWND lHandleSource, long lWidth, long lHeight, long lPosX, long lPosY) 
{
    HDC lDCSource, lDCSourceDesktop;
    POINT lpCursorPos;
    
    lDCSource = GetWindowDC(lHandleSource);
    lDCSourceDesktop = GetWindowDC(0);
    
    bool ret = false;
    if (lDCSource != 0 && lDCSourceDesktop != 0) {
        lDCDest = CreateCompatibleDC(lDCSource);
        if (lDCDest != 0) {
            tBmpInfo.bmiHeader.biSize = sizeof(tBmpInfo.bmiHeader);
          	tBmpInfo.bmiHeader.biWidth = lWidth;
          	tBmpInfo.bmiHeader.biHeight = lHeight;
			tBmpInfo.bmiHeader.biPlanes = 1;
			tBmpInfo.bmiHeader.biBitCount = 24;
			tBmpInfo.bmiHeader.biCompression = 0;
			tBmpInfo.bmiHeader.biSizeImage = ((tBmpInfo.bmiHeader.biWidth * 3 + 3) & 0xFFFFFFFC) * tBmpInfo.bmiHeader.biHeight;
            lDib = CreateDIBSection(lDCDest, &tBmpInfo, 0, &lDibPtr, 0, 0);
            if (lDib != 0) {
                lBmpOld = SelectObject(lDCDest, lDib);
                BitBlt(lDCDest, 0, 0, lWidth, lHeight, lDCSourceDesktop, lPosX, lPosY, 0xCC0020);
                if (viewMouse) {
                    GetCursorPos(&lpCursorPos);
                    DrawIcon(lDCDest, lpCursorPos.x - lPosX, lpCursorPos.y - lPosY, GetCursorHandle());
                }
                ret = true;
            }
        }
    }
    ReleaseDC(lHandleSource, lDCSource);
    ReleaseDC(0, lDCSourceDesktop);
    return ret;
}

void ScreenGrab::DIBClean()
{
    SelectObject(lDCDest, lBmpOld);
    DeleteDC(lDCDest);
    DeleteObject(lDib);
}

bool ScreenGrab::ScreenshotMemory() 
{
    HWND lHandle;
    RECT lpRect;
    long lWidth, lHeight;
    long lPosX, lPosY;
    
	switch (grabMode) {
	case GRAB_MODE_DESKTOP:
    	lHandle = GetDesktopWindow();
    	GetWindowRect(lHandle, &lpRect);
    	lWidth = lpRect.right - lpRect.left;
    	lHeight = lpRect.bottom - lpRect.top;
    	lPosX = lpRect.left;
    	lPosY = lpRect.top;
    	break;
	case GRAB_MODE_WINDOW:
    	lHandle = (HWND)hwnd;
    	GetWindowRect(lHandle, &lpRect);
    	lWidth = lpRect.right - lpRect.left;
    	lHeight = lpRect.bottom - lpRect.top;
    	lPosX = lpRect.left;
    	lPosY = lpRect.top;
    	break;
	case GRAB_MODE_RECT:
    	lHandle = GetDesktopWindow();
    	GetWindowRect(lHandle, &lpRect);
    	lWidth = width;
    	lHeight = height;
    	lPosX = left;
    	lPosY = top;
    	break;
    default:
    	throw Exc(t_("Unknown grab mode"));
    	return false;
	}
    if (DIBCreate(lHandle, lWidth, lHeight, lPosX, lPosY))
        return true;
  	else
    	return false;
}

HDC ScreenGrab::lDCDest;
HGDIOBJ ScreenGrab::lDib;
void *ScreenGrab::lDibPtr;
HGDIOBJ ScreenGrab::lBmpOld;
PAVIFILE ScreenGrab::lAVIPtrFile;
PAVISTREAM ScreenGrab::lAVIPtrStrm;
BITMAPINFO ScreenGrab::tBmpInfo;
AVISTREAMINFO ScreenGrab::tAVIHdr;
AVIFILEINFO ScreenGrab::tAVIFile;

ScreenGrab::ScreenGrab(String _fileName, int secsFrame, bool _viewMouse)
{
	opened = false;
	fileName = _fileName;
	viewMouse = _viewMouse;
	frameRate = 1./secsFrame;
}

ScreenGrab::~ScreenGrab()
{
	Close();
}

void ScreenGrab::Close()
{
	if (!opened)
		return;
    AVIClose();
    DIBClean();
    opened = false;
}

bool ScreenGrab::IniGrabDesktop()
{
	opened = true;
	grabMode = GRAB_MODE_DESKTOP;
	if (!ScreenshotMemory())
    	return false;
  	if (!AVIOpen())
  		return false;
	return true;
}

bool ScreenGrab::IniGrabWindow(long handle)
{
	opened = true;
	grabMode = GRAB_MODE_WINDOW;
	hwnd = handle;
	if (!ScreenshotMemory())
    	return false;
  	if (!AVIOpen())
  		return false;
	return true;
}

bool ScreenGrab::IniGrabDesktopRectangle(int _left, int _top, int _width, int _height)
{
	opened = true;	
	grabMode = GRAB_MODE_RECT;
	left = _left;
	top = _top;
	width = _width;
	height = _height;
	if (!ScreenshotMemory())
    	return false;
  	if (!AVIOpen())
  		return false; 	
	return true;	
}
	
bool ScreenGrab::Grab(unsigned duration)
{
	if (!opened)
		return false;
    TimeStop timer;
    timer.Reset();
    while (timer.Elapsed() < duration*1000) {
        if (!ScreenshotMemory())
        	return false;
        if (!AVIWrite())
            return false;
        while (timer.Elapsed() < (lAVICnt*1000.)/frameRate)
	        Sleep(10);//DoEvents();
    }
    return true;
}

bool ScreenGrab::GrabSnapshot()
{
	if (!opened)
		return false;
  	if (!ScreenshotMemory())
       	return false;
   	if (!AVIWrite())
      	return false;
    return true;
}

#endif

#ifdef PLATFORM_POSIX

bool Window_GetRect(long windowId, long &left, long &top, long &right, long &bottom)
{
	SetSysInfoX11ErrorHandler();
	Display *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return false;
	}
	bool ret = false;
	Window rt;
	int x, y, rx, ry; 
	unsigned int width, height, bw, depth_; 
	if (XGetGeometry(dpy, windowId, &rt, &x, &y, &width, &height, &bw, &depth_)) { 
		left = x;
		top = y;
		right = x + width;
		bottom = y + height;
		ret = true;
	}
//	Window child; 		
//		if (XTranslateCoordinates (dpy, windowId, rt, 0, 0, &rx, &ry, &child))  
//			printf ("%d %d", rx - bw, ry - bw); 
	
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	return ret; 
}

bool Mouse_GetPos(long &x, long &y)
{
	SetSysInfoX11ErrorHandler();
	Display *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return false;
	}   
	bool ret = false;
	Window root, child;
	Window r = DefaultRootWindow(dpy);
	int retx, rety;
	int wx, wy;
	unsigned int keys_buttons;
   	if (XQueryPointer(dpy, r, &root, &child, &retx, &rety, &wx, &wy, &keys_buttons)) {
   		x = wx;
   		y = wy;
   		ret = true;
	} else 
		x = y = -1;
		
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	
	return ret;
}

bool Mouse_SetPos(long x, long y, long windowId)
{
	SetSysInfoX11ErrorHandler();
	Display *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return false;
	}   
    long left, top, right, bottom;
    Window r = DefaultRootWindow(dpy);
    if (windowId != 0) {
        Window_GetRect(windowId, left, top, right, bottom);
        x = x + left;
        y = y + top;
    }     
	XWarpPointer(dpy, None, r, 0, 0, 0, 0, x, y);
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	
	return true;
}

// libxtst-dev
void Mouse_FakeClick(int button, int press) {
	Display *dpy = XOpenDisplay(NULL);
	XTestFakeButtonEvent(dpy, button, press, CurrentTime);
	XFlush(dpy);
	XCloseDisplay(dpy);
}

void Mouse_LeftDown() {
	Mouse_FakeClick(1, True);
}
void Mouse_LeftUp() {
	Mouse_FakeClick(1, False);
}
void Mouse_MiddleDown() {
	Mouse_FakeClick(2, True);
}
void Mouse_MiddleUp() {
	Mouse_FakeClick(2, False);
}
void Mouse_RightDown() {
	Mouse_FakeClick(3, True);
}
void Mouse_RightUp() {
	Mouse_FakeClick(3, False);
}

void PressKeyVK(int key, Display *dpy = NULL) {
	bool local = false;
	if (!dpy) {
		if (!(dpy = XOpenDisplay(NULL)))
			return;
		local = true;
	}
	XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, key), True, CurrentTime);
	XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, key), False, CurrentTime);
	if (local) {
		XFlush(dpy);
		XCloseDisplay(dpy);
	}
}

void PressKey(wchar key, Display *dpy = NULL) {
	bool local = false;
	if (!dpy) {
		if (!(dpy = XOpenDisplay(NULL)))
			return;
		local = true;
	}
	wchar k = key;
	if (key > 0x00ff)
    	key = key | 0x01000000;
 	
 	bool shift = false;
	KeyCode code = XKeysymToKeycode(dpy, key);
	if (code != 0) { 
		if (XKeycodeToKeysym(dpy, code, 0) != key) {
			if (XKeycodeToKeysym(dpy, code, 1) == key) 
				shift = true;
			else
				code = 0;
		}
	} else {  
		int firstKeycode, maxKeycode;
		int keysymsPerKeycode;
		
		XDisplayKeycodes(dpy, &firstKeycode, &maxKeycode);
		KeySym *keysyms = XGetKeyboardMapping(dpy, firstKeycode, maxKeycode-firstKeycode+1, &keysymsPerKeycode);
      	int indx = (maxKeycode-firstKeycode-1)*keysymsPerKeycode;
		keysyms[indx] = key;
      	XChangeKeyboardMapping(dpy, firstKeycode, keysymsPerKeycode, keysyms, maxKeycode-firstKeycode);
      	XSync(dpy, False);
      	code = maxKeycode-1;
      	if (XKeycodeToKeysym(dpy, code, 0) != key) {
			if (XKeycodeToKeysym(dpy, code, 1) == key) 
				shift = true;
		}
    }
	if (code != 0) {
		if (shift)
			XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, XK_Shift_L), True, CurrentTime);	
		XTestFakeKeyEvent(dpy, code, True,  CurrentTime);
		XTestFakeKeyEvent(dpy, code, False, CurrentTime);
	 	if (shift)
			XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, XK_Shift_L), False, CurrentTime);	
	} 	
	if (local) {
 		XFlush(dpy);
 		XCloseDisplay(dpy);
	}
}

bool GetKeyLockStatus0(bool &caps, bool &num, bool &scroll, Display *dpy) {
	int x, y, xx, yy;
	Window dm1, dm2;
	unsigned int sKbdState;
	
	if(!XQueryPointer(dpy, DefaultRootWindow(dpy), &dm1, &dm2, &x, &y, &xx, &yy, &sKbdState))
		return false;

	caps   = sKbdState & LockMask;
	num    = sKbdState & Mod2Mask;
	scroll = sKbdState & Mod5Mask;
	
	return true;
}

bool GetKeyLockStatus(bool &caps, bool &num, bool &scroll) {
	Display *dpy;
	if (!(dpy = XOpenDisplay(NULL)))
		return false;

	if (!GetKeyLockStatus0(caps, num, scroll, dpy)) {
		XCloseDisplay(dpy);
		return false;
	}
	XFlush(dpy);
	XCloseDisplay(dpy);
	return true;
}

bool SetKeyLockStatus(bool caps, bool num, bool scroll) {
	Display *dpy;
	if (!(dpy = XOpenDisplay(NULL)))
		return false;
			
	bool oldcaps, oldnum, oldscroll; 
	if (!GetKeyLockStatus0(oldcaps, oldnum, oldscroll, dpy)) {
		XCloseDisplay(dpy);
		return false;
	}
	if (caps != oldcaps) 
		PressKeyVK(XK_Caps_Lock, dpy);
	if (num != oldnum) 
		PressKeyVK(XK_Num_Lock, dpy);
	if (scroll != oldscroll) 
		PressKeyVK(XK_Scroll_Lock, dpy);

	XFlush(dpy);
	XCloseDisplay(dpy);
	return true;
}

bool Window_SaveCapture(long windowId, String fileName, int left, int top, int width, int height)
{
	if (GetFileExt(fileName) != ".xwd")
		fileName += ".xwd";
	
	String command;
	if (windowId == 0)
		command = "xwd -root -silent -out \"" + fileName + "\"";
	else
		command = "xwd -id " + FormatLong(windowId) + " -silent -out \"" + fileName + "\"";
	
	String strret;
	return Sys(command, strret) >= 0;
}

KeyCodes keyCodes[] = {
	"NUMPAD7", 	XK_KP_7,	 	"BACK", 	XK_BackSpace, 
	"NUMPAD8", 	XK_KP_8,		"TAB", 		XK_Tab,
	"NUMPAD9", 	XK_KP_9, 		"RETURN", 	XK_Return,
	"MULTIPLY", XK_KP_Multiply,	"SHIFT",	XK_Shift_Lock,
	"ADD",		XK_KP_Add, 	 	"CONTROL",	XK_Control_L,
	"SEPARATOR", XK_KP_Separator,"MENU", 	XK_Super_L,
	"SUBTRACT", XK_KP_Subtract,	"PAUSE", 	XK_Pause,
	"DECIMAL",	XK_KP_Decimal,	/*"CAPITAL", 	VK_CAPITAL,*/
	"DIVIDE",	XK_KP_Divide,	"ESCAPE",	XK_Escape,
	"F1", 		XK_F1, 			"SPACE", 	XK_KP_Space,
	"F2", 		XK_F2,	 		"END",		XK_End,
	"F3",		XK_F3, 			"HOME",		XK_Home,
	"F4",		XK_F4, 			"LEFT", 	XK_Left,
	"F5",		XK_F5,		 	"UP", 		XK_Up,
	"F6",		XK_F6,		 	"RIGHT",	XK_Right,
	"F7",		XK_F7,		 	"DOWN",		XK_Down,
	"F8",		XK_F8,		 	"PRINT",	XK_Sys_Req,
	"F9",		XK_F9,		 	/*"SNAPSHOT",	VK_SNAPSHOT,*/
	"F10",		XK_F10,		 	"INSERT",	XK_Insert,
	"F11",		XK_F11,		 	"DELETE",	XK_Delete,
	"F12",		XK_F12,		 	"LWIN",		XK_Meta_L,
	"NUMLOCK",	XK_Num_Lock, 	"RWIN",		XK_Meta_R,
	"SCROLL",	XK_Scroll_Lock,	"NUMPAD0",	XK_KP_0,
	"LSHIFT",	XK_Shift_L,	 	"NUMPAD1", 	XK_KP_1,
	"RSHIFT",	XK_Shift_R,	 	"NUMPAD2",	XK_KP_2,
	"LCONTROL",	XK_Control_L, 	"NUMPAD3",	XK_KP_3,
	"RCONTROL",	XK_Control_R, 	"NUMPAD4",	XK_KP_4,
	"LMENU",	XK_Super_L,		"NUMPAD5",	XK_KP_5,
	"RMENU",	XK_Super_R, 	"NUMPAD6",	XK_KP_6,
	"PGUP", 	XK_Page_Up, 	"PGDOWN", 	XK_Page_Down,
	"CAPSLOCK", XK_Caps_Lock, 	"BACKSPACE",XK_BackSpace,
	""
};

#endif

int GetKeyCode(String key) {
	for (int i = 0; keyCodes[i].code != 0; ++i)
		if (keyCodes[i].key == key)
			return keyCodes[i].code;
	return 0;
}

void Keyb_SendKeys(String text, long finalDelay, long delayBetweenKeys)
{
	bool inKey = false;
	String key = "";
	WString wtext(text);
	for (int i = 0; i < wtext.GetCount(); ++i) {
		bool vk = false;
		Sleep(delayBetweenKeys);
		wchar c = wtext[i];
		if (c == '{')
			inKey = true;
		else if (c == '}') {
			if (key == "{") 
				c = '{';
			else {
				c = GetKeyCode(key);
				vk = true;
			}
			inKey = false;
			key = "";
		} else if (inKey == 1)
			key.Cat(c);
		else if (c == '\n') {
			c = GetKeyCode("RETURN");
			vk = true;
		}
 		if (inKey == false) {
			if (!vk) 
 				PressKey(c);
			else
				PressKeyVK(c);
		}
	}
	Sleep(finalDelay);
}

bool Snap_Desktop(String fileName)
{
	return Window_SaveCapture(0, fileName);
}
bool Snap_DesktopRectangle(String fileName, int left, int top, int width, int height)
{
	return Window_SaveCapture(0, fileName, left, top, width, height);
}
bool Snap_Window(String fileName, long handle)
{
	return Window_SaveCapture(handle, fileName);
}

#ifdef PLATFORM_POSIX

void SetDesktopWallPaper(const char *path)
{
	String desktopManager = GetDesktopManagerNew();
	
	if (desktopManager == "gnome") {
		Sys("gconftool-2 -t str -s /desktop/gnome/background/picture_filename \"" + String(path) + "\"");
		String mode;
		if (*path == '\0')
			mode = "none";		// Values "none", "wallpaper", "centered", "scaled", "stretched"
		else
			mode = "stretched";
		Sys("gconftool-2 -t str -s /desktop/gnome/background/picture_options \"" + mode + "\"");	
	} else if (desktopManager == "kde") {
			// 1: disabled, only background color
			// 2: tiled with first image in top left corner
			// 3: tiled with first image centered
			// 4: centered stretched with proportions kept until one side hits screen, space filled by background color
			// 5: same as 4, though wallpaper aligned to top left and space after stretching filled by tiling
			// 6: stretched to fit screen 
		int mode;
		if (*path == '\0')
			mode = 1;
		else
			mode = 6;
		Sys("dcop kdesktop KBackgroundIface setWallpaper \"" + String(path) + "\" " + AsString(mode));
	} else
		throw Exc(t_("Not possible to change Desktop bitmap"));
}

#endif

#if defined(PLATFORM_WIN32)

void SetDesktopWallPaper(const char *path)
{
    if (0 == SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (LPVOID)path, SPIF_UPDATEINIFILE || SPIF_SENDWININICHANGE))
        throw Exc(Format(t_("Error %s changing Desktop bitmap"), AsString(GetLastError())));
}
#endif
