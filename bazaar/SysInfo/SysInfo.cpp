#include <Core/Core.h>

#if defined(PLATFORM_WIN32) 
#include <shellapi.h>
#include <Tlhelp32.h>
#include <psapi.h>
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

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xfuncs.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

using namespace Upp;

#include "SysInfo.h"

bool FileCat(const char *file, const char *appendFile)
{
	FileAppend f(file);
	if(!f.IsOpen())
		return false;
	FileIn fi(appendFile);
	if(!fi.IsOpen())
		return false;
	CopyStream(f, fi, fi.GetLeft());
	fi.Close();
	f.Close();
	if(f.IsError()) 
		return false;
	return true;
}

String FormatLong(long a)
{ 
	return Sprintf("%ld", a);
}

String Replace(String str, String find, String replace)
{
	String ret;
	int i = 0, j;
	int lenStr = str.GetCount();
	int lenReplace = replace.GetCount();
	while ((j = str.Find(find, i+1)) >= i) {
		ret += str.Mid(i, j-i) + replace;
		i = j + lenReplace-1;
		if (i >= lenStr)
			break;
	}
	ret += str.Mid(i, str.GetLength()-i);
	return ret;
}

void SearchFile_Each(String dir, String condFile, String text, Array<String> &files)
{
	FindFile ff(AppendFileName(dir, condFile));
	do {
		if(ff.IsFile()) {
			String p = AppendFileName(dir, ff.GetName());
			if (text == "")
				files.Add(p);
			else {
				FILE *fp = fopen(p, "r");
				if (fp != NULL) {
					int i = 0, c;
					while ((c = fgetc(fp)) != EOF) {
						if (c == text[i]) {
							++i;
							if (i == text.GetCount()) {
								files.Add(p);
								break;
							}
						} else {
							if (i != 0) 
								fseek(fp, -(i-1), SEEK_CUR);
							i = 0;
						}
					}
					fclose(fp);
				} else
					puts(Format("Error: %s", AppendFileName(dir, ff.GetName())));
			}
		} 
	} while (ff.Next());
	
	ff.Search(AppendFileName(dir, "*"));
	do {
		String name = ff.GetName();
		if(ff.IsDirectory() && name != "." && name != "..") {
			String p = AppendFileName(dir, name);
			SearchFile_Each(p, condFile, text, files);
		}
	} while (ff.Next()); 
}

Array<String> SearchFile(String dir, String condFile, String text)
{
	Array<String> ret;
	SearchFile_Each(dir, condFile, text, ret);	
	return ret;
}

/////////////////////////////////////////////////////////////////////
// LaunchCommand
int LaunchCommand(const char *cmd, void (*readCallBack)(String &))
{
	LocalProcess p;
	if(!p.Start(cmd))
		return -1;
	String str;
	while(p.IsRunning()) {
		str = p.Get();
		if (readCallBack)
			readCallBack(str);
		DoEvents();
	}
	str = p.Get();
	if (readCallBack)
		readCallBack(str);
	return p.GetExitCode();
}
int LaunchCommand(const char *cmd, String &ret)
{
	ret = "";
	LocalProcess p;
	if(!p.Start(cmd))
		return -1;
	while(p.IsRunning()) {
		ret.Cat(p.Get());
		DoEvents();
	}
	ret.Cat(p.Get());
	return p.GetExitCode();
}
String LaunchCommand(const char *cmd)
{
	String ret;
	LaunchCommand(cmd, ret);
	return ret;
}

/////////////////////////////////////////////////////////////////////
// LaunchFile

bool LaunchFile(const String file)
{
#if defined(PLATFORM_WIN32)
	HINSTANCE ret;
	WString fileName(file);
	ret = ShellExecuteW(NULL, L"open", fileName, NULL, L".", SW_SHOWMINIMIZED);		//SW_SHOWDEFAULT
	return (int)ret > 32;
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
String GetExtExecutable(String ext)
{
	String exeFile = "";
	if (ext[0] != '.')
		ext = String(".") + ext;
#if defined(PLATFORM_WIN32)
	String file = AppendFileName(GetHomeDirectory(), String("pru") + ext);
	SaveFile(file, "   ");
	if (!FileExists(file)) {
		exeFile = "Error getting exe";
		puts("ERROR");
		return exeFile;
	}
	HINSTANCE ret;
	WString fileW(file);
	WCHAR exe[1024];
	ret = FindExecutableW(fileW, NULL, exe);
	if ((long)ret > 32)
		exeFile = WString(exe).ToString();
	DeleteFile(file);
#endif
#ifdef PLATFORM_POSIX
	StringParse mime;
	//if (LaunchCommand(String("xdg-mime query filetype ") + file, mime) >= 0) 	// xdg-mime query filetype does not work properly in Enlightenment
	mime = LoadFile_Safe("/etc/mime.types");	// Search in /etc/mime.types the mime type for the extension
	if ((mime.GoAfter_Init(String(" ") + ext.Right(ext.GetCount()-1))) || (mime.GoAfter_Init(String("\t") + ext.Right(ext.GetCount()-1)))) {
		mime.GoBeginLine();
		mime = mime.GetText();
		exeFile = TrimRight(LaunchCommand(String("xdg-mime query default ") + mime));
	}
#endif
	return exeFile;
}

/////////////////////////////////////////////////////////////////////
// Special Folders

String GetFirefoxDownloadFolder()
{
	String profilesPath = "mozilla/firefox";
#ifdef PLATFORM_POSIX	// Is hidden
	profilesPath = String(".") + profilesPath;
#endif
	StringParse profiles = LoadFile(AppendFileName(GetAppDataFolder(), AppendFileName(profilesPath, "profiles.ini")));
	if (!profiles.GoAfter("Path")) return "";
	if (!profiles.GoAfter("=")) return "";
	String path = profiles.GetText();
	String pathPrefs = AppendFileName(AppendFileName(AppendFileName(GetAppDataFolder(), profilesPath), path), "prefs.js");
	StringParse prefs = LoadFile(pathPrefs);
	if (!prefs.GoAfter("\"browser.download.dir\"")) {
		if (!prefs.GoAfter("\"browser.download.lastDir\""))
			return "";		
	}
	if (!prefs.GoAfter(",")) return "";
	return prefs.GetText();
}

#if defined(PLATFORM_WIN32)
String GetShellFolder(const char *local, const char *users) 
{
	String ret = FromSystemCharset(GetWinRegString(local, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
									   HKEY_CURRENT_USER));
	if (ret == "" && *users != '\0')
		return FromSystemCharset(GetWinRegString(users, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
									   HKEY_LOCAL_MACHINE));
	return ret;
}
String GetIEDownloadFolder() 
{
	String ret = FromSystemCharset(GetWinRegString("Download Directory", "Software\\Microsoft\\Internet Explorer", HKEY_CURRENT_USER));
	if (ret == "")
		ret =  FromSystemCharset(GetWinRegString("Save Directory", "Software\\Microsoft\\Internet Explorer\\Main", HKEY_CURRENT_USER)); 
	return ret;
}
String GetDesktopFolder()	{return GetShellFolder("Desktop", "Common Desktop");}
String GetProgramsFolder()	{return FromSystemCharset(GetWinRegString("ProgramFilesDir", "Software\\Microsoft\\Windows\\CurrentVersion", HKEY_LOCAL_MACHINE));}
String GetAppDataFolder()	{return GetShellFolder("AppData", "Common AppData");}
String GetMusicFolder()		{return GetShellFolder("My Music", "CommonMusic");}
String GetPicturesFolder()	{return GetShellFolder("My Pictures", "CommonPictures");}
String GetVideoFolder()		{return GetShellFolder("My Video", "CommonVideo");}
String GetPersonalFolder()	{return GetShellFolder("Personal", 0);}
String GetTemplatesFolder()	{return GetShellFolder("Templates", "Common Templates");}
String GetDownloadFolder()	
{
	String browser = GetExtExecutable("html");
	browser = ToLower(browser);
	if (browser.Find("iexplore") >= 0)
		return GetIEDownloadFolder();
	else if (browser.Find("firefox") >= 0)
		return GetFirefoxDownloadFolder();
	return GetDesktopFolder();		// I do not know to do it in other browsers !!
};
String GetTempFolder()
{
	String ret;
	if ((ret = GetEnv("TEMP")) == "")	// One or the other one
		ret = GetEnv("TMP");
	return ret;
}
String GetOsFolder()
{
	char ret[MAX_PATH];
	::GetWindowsDirectory(ret, MAX_PATH);
	return String(ret);
}
String GetSystemFolder()
{
	char ret[MAX_PATH];
	::GetSystemDirectory(ret, MAX_PATH);
	return String(ret);
}
#endif
#ifdef PLATFORM_POSIX
String GetPathXdg(String xdgConfigHome, String xdgConfigDirs)
{
	String ret = "";
	if (FileExists(ret = AppendFileName(xdgConfigHome, "user-dirs.dirs"))) ;
  	else if (FileExists(ret = AppendFileName(xdgConfigDirs, "user-dirs.defaults"))) ;
  	else if (FileExists(ret = AppendFileName(xdgConfigDirs, "user-dirs.dirs"))) ;
  	return ret;
}
String GetPathDataXdg(String fileConfig, const char *folder) 
{
	StringParse fileData = LoadFile(fileConfig);
	
	if (!fileData.GoAfter(folder)) return "";
	if (!fileData.GoAfter("=")) return "";
	
	String ret = "";
	StringParse path = fileData.GetText();
	if(path.GoAfter("$HOME/")) 
		ret = AppendFileName(GetHomeDirectory(), path.Right());
	else if (!FileExists(path))
		ret = AppendFileName(GetHomeDirectory(), path);
	
	return ret;		
}
String GetShellFolder(const char *local, const char *users) 
{
 	String xdgConfigHome = GetEnv("XDG_CONFIG_HOME");
  	if (xdgConfigHome == "")		// By default
  		xdgConfigHome = AppendFileName(GetHomeDirectory(), ".config");
  	String xdgConfigDirs = GetEnv("XDG_CONFIG_DIRS");
  	if (xdgConfigDirs == "")			// By default
  		xdgConfigDirs = "/etc/xdg";
  	String xdgFileConfigData = GetPathXdg(xdgConfigHome, xdgConfigDirs);
  	String ret = GetPathDataXdg(xdgFileConfigData, local);
  	if (ret == "" && *users != '\0')
  		return GetPathDataXdg(xdgFileConfigData, users);
  	else
  		return ret;
}
String GetDesktopFolder()	
{
	String ret = GetShellFolder("XDG_DESKTOP_DIR", "DESKTOP");
	if (ret == "")
		return AppendFileName(GetHomeDirectory(), "Desktop");
	else
		return ret;
}
String GetProgramsFolder()	{return String("/usr/bin");}
String GetAppDataFolder()	{return GetHomeDirectory();};
String GetMusicFolder()		{return GetShellFolder("XDG_MUSIC_DIR", "MUSIC");}
String GetPicturesFolder()	{return GetShellFolder("XDG_PICTURES_DIR", "PICTURES");}
String GetVideoFolder()		{return GetShellFolder("XDG_VIDEOS_DIR", "VIDEOS");}
String GetPersonalFolder()	{return GetShellFolder("XDG_DOCUMENTS_DIR", "DOCUMENTS");}
String GetTemplatesFolder()	{return GetShellFolder("XDG_TEMPLATES_DIR", "XDG_TEMPLATES_DIR");}
String GetDownloadFolder()	
{
	String browser = GetExtExecutable("html");
	browser = ToLower(browser);
	if (browser.Find("firefox") >= 0)
		return GetFirefoxDownloadFolder();
	return GetShellFolder("XDG_DOWNLOAD_DIR", "DOWNLOAD");
};
String GetTempFolder()
{
	return GetHomeDirectory();		
}
String GetOsFolder()
{
	return String("/bin");
}
String GetSystemFolder()
{
	return String("");
}
#endif

/////////////////////////////////////////////////////////////////////
// Hardware Info
#if defined(PLATFORM_WIN32) 
void GetSystemInfo(String &manufacturer, String &productName, String &version, int &numberOfProcessors)
{
	manufacturer = FromSystemCharset(GetWinRegString("SystemManufacturer", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	if (manufacturer == "") {
		StringParse fileData = LoadFile(AppendFileName(GetSystemFolder(), "oeminfo.ini"));
		fileData.GoAfter("Manufacturer=");
		manufacturer = fileData.GetText("\r\n");
	}
	productName = FromSystemCharset(GetWinRegString("SystemProductName", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	if (productName == "")
		productName = FromSystemCharset(GetWinRegString("Model", "SOFTWARE\\Microsoft\\PCHealth\\HelpSvc\\OEMInfo", HKEY_LOCAL_MACHINE));
	version = FromSystemCharset(GetWinRegString("SystemVersion", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));
	numberOfProcessors = atoi(GetEnv("NUMBER_OF_PROCESSORS"));
}
void GetBiosInfo(String &biosVersion, Date &biosReleaseDate)
{
	String strBios = FromSystemCharset(GetWinRegString("BIOSVersion", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE));	
	for (int i = 0; i < strBios.GetLength(); ++i) {
		if (strBios[i] == '\0')
			biosVersion.Cat(". ");
		else
			biosVersion.Cat(strBios[i]);
	}
	StrToDate(biosReleaseDate, FromSystemCharset(GetWinRegString("BIOSReleaseDate", "HARDWARE\\DESCRIPTION\\System\\BIOS", HKEY_LOCAL_MACHINE)));
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
	architecture = LaunchCommand("uname -m");		// CPU type
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
uint64 &freeVirtual)			// free virtual memory			
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
uint64 &freeVirtual)			// free virtual memory			
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

// This system files are like pipes: it is not possible to get the length to size the buffer
String LoadFile_Safe(String fileName)
{
	int fid = open(fileName, O_RDONLY);
	if (fid < 0)
		return String("");
	int size = 1024, nsize;
	StringBuffer s;
	char buf[size];
	while((nsize = read(fid, buf, size)) == size) 
		s.Cat(buf, size);
	s.Cat(buf, nsize-1);
	close(fid);
	return s;
}

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
			fileName.Add("UNKNOWN");	
		if (GetModuleBaseNameW(hProcess, hInstance, str, sizeof(str)/sizeof(WCHAR)))
			name.Add(FromSystemCharset(WString(str).ToString()));
		else
			name.Add("UNKNOWN");		
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
#ifdef CTRLLIB_H
	void SetX11ErrorHandler();
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
              			sret << list[i] << " ";
              		XFreeStringList(list);
              		caption.Add(FromSystemCharset(sret));
          		} else 
              		caption.Add(FromSystemCharset((char *)tp.value));
      		} else 
      			caption.Add("");
        }
		// Get pid
		Atom atomPID = XInternAtom(dpy, "_NET_WM_PID", True);
		unsigned long pid = 0;
		if (atomPID == None)
			processId.Add(0L);
		else {
			Atom type;        
			int format;        
			unsigned long nItems;        
			unsigned long bytesAfter;        
			unsigned char *propPID = 0;
			if (0 == XGetWindowProperty(dpy, hWnd[i], atomPID, 0, 1024, False, XA_CARDINAL, &type, &format, &nItems, &bytesAfter, &propPID)) {
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
		// Window dimensions
		//	Window rt;
		//	int x, y, rx, ry; 
		//	unsigned int width, height, bw, depth_; 
		//	if (XGetGeometry(dpy, w, &rt, &x,&y,&width,&height,&bw,&depth_)) { 
		//		Window child; 
		//		printf (" %ux%u+%d+%d", width, height, x, y); 
		//		if (XTranslateCoordinates (dpy, w,rt,0,0,&rx,&ry,&child))  
		//			printf (" +%d+%d", rx - bw, ry - bw); 
		//	} 
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
#if defined(__MINGW32__)

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
   	if (VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && osvi.dwMajorVersion > 4 ) {
      	// Test for the specific product.
      	if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 ) {
         	if( osvi.wProductType == VER_NT_WORKSTATION )
         		kernel.Cat(" Vista");
         	else
         		kernel.Cat(" Server 2008");
         	pGPI = (PGPI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
         	pGPI( 6, 0, 0, 0, &dwType);

         	switch( dwType ) {
            case PRODUCT_ULTIMATE:
               distro = "Ultimate Edition";
               break;
            case PRODUCT_HOME_PREMIUM:
               distro = "Home Premium Edition";
               break;
            case PRODUCT_HOME_BASIC:
               distro = "Home Basic Edition";
               break;
            case PRODUCT_ENTERPRISE:
               distro = "Enterprise Edition";
               break;
            case PRODUCT_BUSINESS:
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
      	}
      	if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
         	if (GetSystemMetrics(SM_SERVERR2) )
         		kernel.Cat(" Server 2003 R2");
         	else if ( osvi.wSuiteMask==VER_SUITE_STORAGE_SERVER )
            	kernel.Cat(" Storage Server 2003");
         	else if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
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
    	}
    	if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 ) {
         	kernel.Cat(" XP");
         	if(osvi.wSuiteMask & VER_SUITE_PERSONAL )
            	distro = "Home Edition";
         	else 
         		distro = "Professional";
      	}
      	if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 ) {
         	kernel.Cat(" 2000");
         	if ( osvi.wProductType == VER_NT_WORKSTATION )
            	distro = "Professional";
         	else {
            	if(osvi.wSuiteMask & VER_SUITE_DATACENTER )
               		distro = "Datacenter Server";
            	else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
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
		if (LaunchCommand("sysctl_cmd -n kern.version").Find("FreeBSD") >= 0)
			kernel = "freebsd";
	}
	if (kerVersion == "")
		kerVersion = LoadFile_Safe("/proc/sys/kernel/osrelease") + " " + LoadFile_Safe("/proc/sys/kernel/version");
	if (kerArchitecture == "")
		kerArchitecture = LaunchCommand("uname -m");	// Kernel. See too /proc/version, /proc/version_signature and uname -a looking for architecture
	
	if (kernel == "")
		kernel = kerVersion = kerArchitecture = "UNKNOWN";	
	
	// Desktop
    if(GetEnv("GNOME_DESKTOP_SESSION_ID").GetCount() || GetEnv("GNOME_KEYRING_SOCKET").GetCount()) {
		desktop = "gnome";
		StringParse gnomeVersion = LaunchCommand("gnome-about --version");
		gnomeVersion.GoAfter("gnome-about");
		deskVersion = gnomeVersion.GetText();
	} else if(GetEnv("KDE_FULL_SESSION").GetCount() || GetEnv("KDEDIR").GetCount() || GetEnv("KDE_MULTIHEAD").GetCount()) {
        desktop = "kde"; 
        StringParse konsole = LaunchCommand("konsole --version");
        konsole.GoAfter("KDE:");
        deskVersion = konsole.GetText("\r\n");						
		if (deskVersion == "")		
			deskVersion = GetEnv("KDE_SESSION_VERSION");        
	} else {
		StringParse desktopStr;
		if (LaunchCommand("xprop -root _DT_SAVE_MODE").Find("xfce") >= 0)
			desktop = "xfce";
		else if ((desktopStr = LaunchCommand("xprop -root")).Find("ENLIGHTENMENT") >= 0) {
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
	else if (LaunchCommand("uname -r").Find("solaris") >= 0)
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
//uint64 &totalBytes,		// To determine the total number of bytes on a disk or volume, use IOCTL_DISK_GET_LENGTH_INFO.
uint64 &freeBytesUser, 	// Total number of free bytes on a disk that are available to the user who is associated with the calling thread.
uint64 &totalBytesUser, 	// Total number of bytes on a disk that are available to the user who is associated with the calling thread.
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
   	if(!::GetVolumeInformation(sb, vol, MAX_PATH, (LPDWORD)&serial, (LPDWORD)&_maxName, (LPDWORD)&flags, fs, MAX_PATH))
   		return false;
   	volume = vol;
   	fileSystem = fs;
   	maxName = (int)maxName;
   	
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
//uint64 &totalBytes,		// To determine the total number of bytes on a disk or volume, use IOCTL_DISK_GET_LENGTH_INFO.
uint64 &freeBytesUser, 	// Total number of free bytes on a disk that are available to the user who is associated with the calling thread.
uint64 &totalBytesUser, 	// Total number of bytes on a disk that are available to the user who is associated with the calling thread.
uint64 &totalFreeBytes)	// Total number of free bytes on a disk.
{
	freeBytesUser = totalBytesUser = totalFreeBytes = 0;
	
	StringParse space = LaunchCommand("df -T");
	if (space == "")
		return false;
	
	while (drive != space.GetText())
		;
	space.MoveRel(-10);		space.GoBeginLine(); 
	space.GetText();	space.GetText();	// Jumps over device path and filesystem
	totalBytesUser = 1024*space.GetUInt64();
	space.GetText();						// Jumps over used space
	freeBytesUser  = totalFreeBytes = 1024*space.GetUInt64();
	return true;
}

// return true if mounted
bool GetDriveInformation(String drive, String &type, String &volume, /*uint64 &serial, */int &maxName, String &fileSystem)
{
	StringParse info = LaunchCommand("mount -l");
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

String BytesToKMGT(uint64 _bytes)
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
					ret = Format("%.1f%s", _bytes/(1024*1024*1024*1024.), "Tb");
				} else
					ret = Format("%.1f%s", _bytes/(1024*1024*1024.), "Gb");
			} else
				ret = Format("%.1f%s", _bytes/(1024*1024.), "Mb");
		} else
			ret = Format("%.1f%s", _bytes/1024., "Kb");
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
#endif

// More info. No superuser permissions required
// /var/log/installer/status 	Gives a list a software packages installed
// mime.types 					Gives the mime-types list
// lshw							Hardware info. A lot
// lspci -vv					Hardware info. A lot, best organized

// Hack
// biosdecode
// dmidecode --type 0
// sysinfo
