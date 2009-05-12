#include "Win32Utils.h"
#include <psapi.h>

int  g_process_id;
HWND g_process_hwnd;
//extern dword UM_COMMANDLINE;

HANDLE __fastcall GetProcess(const char * app_name, int &process_id, DWORD cur_process_id)
{
	char process_name[256];
	DWORD processes[1024], needed;
	
	if(!EnumProcesses(processes, sizeof(processes), &needed))
		return NULL;
	
	for(int i = 0; i < (int) (needed / sizeof(DWORD)); i++)
	{
		HANDLE hprc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
		
		if(hprc)
		{
			HMODULE mprc;
			DWORD needed;
			
			if(EnumProcessModules(hprc, &mprc, sizeof(mprc), &needed))
			{
				GetModuleBaseName(hprc, mprc, process_name, sizeof(process_name));
				if(strcmpi(process_name, app_name) == 0)
				{
					int prcid = processes[i];
					if(cur_process_id == 0)
					{
						process_id = prcid;
						return hprc;
					}
					else if(cur_process_id != prcid)
					{
						process_id = prcid;
						return hprc;
					}						
				}
			}
			CloseHandle(hprc);
		}
	}  
	return NULL;
}

HWND __fastcall FindWindowHandle(const char * class_name, int cnt)
{
	HWND hwnd = NULL;
	while(!hwnd && cnt > 0)
	{
		hwnd = FindWindow(class_name, NULL);
		if(hwnd)
			return hwnd;
		cnt--;
	}
	return NULL;
}

BOOL CALLBACK MyEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char class_name[256];
	GetClassName(hwnd, class_name, 256);
	
	static const char * upp_class = "UPP-CLASS";
	for(int i = 0; i < (int) strlen(upp_class); i++)
		if(upp_class[i] != class_name[i])
			return TRUE;
		
	DWORD process_id;
	int thread_id = GetWindowThreadProcessId(hwnd, &process_id);
	if(g_process_id == process_id)
	{
		g_process_hwnd = hwnd;
		return FALSE;
	}
	else
		return TRUE;
}

HWND GetAppHwnd(const char * app_name, bool skip_current)
{
	if(GetProcess(app_name, g_process_id, skip_current ? GetCurrentPrc() : 0) == NULL)
		return NULL;

	g_process_hwnd = NULL;
	::EnumWindows(MyEnumWindowsProc, 0);
	return g_process_hwnd;
}

HMODULE GetCurrentMod()
{
	return ::GetModuleHandle(NULL);
}

DWORD GetCurrentPrc()
{
	return ::GetCurrentProcessId();
}

void GetSwitchWindowProc()
{
//	HMODULE hUser32 = GetModuleHandle("user32"); 	
//	SwitchToThisWindow = PROCSWITCHTOTHISWINDOW;
//	GetProcAddress(hUser32, "SwitchToThisWindow");
}

/*
bool ForceForegroundWindow(HWND hwnd)
{
	const int SPI_GETFOREGROUNDLOCKTIMEOUT = 0x2000; 
  	const int SPI_SETFOREGROUNDLOCKTIMEOUT = 0x2001; 
	DWORD ForegroundThreadID;
  	DWORD ThisThreadID;
  	DWORD timeout; 

	if(IsIconic(hwnd))
		ShowWindow(hwnd, SW_RESTORE);

 	if(GetForegroundWindow() == hwnd)
 		return true
	else
	{

		OSVERSIONINFO of;
		of.dwOSVersionInfoSize = sizeof(of);
		GetVersionEx(&of);
		return of.dwMajorVersion >= 5 && of.dwMinorVersion >= 1;

	    // Windows 98/2000 doesn't want to foreground a window when some other 
	    // window has keyboard focus 

    	if(of.dwMajorVersion > 4 || (of.dwMajorVersion == 4 && of.dwMinorVersion > 0)) 
    	{

			// Code from Karl E. Peterson, www.mvps.org/vb/sample.htm 
			// Converted to Delphi by Ray Lischner 
			// Published in The Delphi Magazine 55, page 16 

      		ForegroundThreadID = GetWindowThreadProcessID(GetForegroundWindow(), NULL); 
      		ThisThreadID := GetWindowThreadPRocessId(hwnd,nil); 
      if AttachThreadInput(ThisThreadID, ForegroundThreadID, true) then 
begin 
        BringWindowToTop(hwnd); //IE 5.5 related hack 
        SetForegroundWindow(hwnd); 
        AttachThreadInput(ThisThreadID, ForegroundThreadID,false); 
        Result := (GetForegroundWindow = hwnd); 
      end; 
      if not Result then begin 

        // Code by Daniel P. Stasinski <dan...@karemor.com> 

        SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0,@timeout, 0); 
        SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0,TObject(0), 
SPIF_SENDCHANGE); 
        BringWindowToTop(hwnd); //IE 5.5 related hack 
        SetForegroundWindow(hWnd); 
        SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 
0,TObject(timeout), SPIF_SENDCHANGE); 
      end; 
    end 
    else begin 
      BringWindowToTop(hwnd); //IE 5.5 related hack 
      SetForegroundWindow(hwnd); 
    end; 

    Result := (GetForegroundWindow = hwnd); 
  end; 
end;
*/


AppInstance::AppInstance(const char * mutex_name)
{
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
	
	if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
	{
		error = GetLastError();
		return;
	}
	if(!SetSecurityDescriptorDacl(&sd, TRUE, (PACL) NULL, FALSE))
	{
		error = GetLastError();
		return;
	}
	
	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = (LPVOID) &sd;
	sa.bInheritHandle = FALSE;
	
	mutex = CreateMutex(&sa, FALSE, (LPCSTR) mutex_name);
	error = GetLastError();
}

AppInstance::~AppInstance()
{
	if(mutex)
	{
		CloseHandle(mutex);
		mutex = NULL;
	}
}

BOOL AppInstance::IsAnotherInstance()
{
	return (ERROR_ALREADY_EXISTS == error);
}

bool AppInstance::PassCmd(HWND hwnd, dword msg, const Vector<String> &cmd)
{
	int cnt = cmd.GetCount();
	int size = cnt * 1024;
		
    data = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, size, "ultimateplayer.map");
    if(!data)
    {
		error = GetLastError();
		//PromptOK(Format("P1 %d", (int)error));
        return false;
    }
    
    void * p = ::MapViewOfFile(data, FILE_MAP_WRITE, 0, 0, 0);
    if(!p)
    {
		error = GetLastError();
		//PromptOK(Format("P2 %d", (int)error));
		CloseHandle(data);
		return false;        
    }

	//ONCELOCK {
		MemStream s(p, size);
		s.Put32(cnt);
		for(int i = 0; i < cnt; i++)
		{
			//PromptOK(Format("%d", i));
			s.Put32(cmd[i].GetCount());
			s.Put(cmd[i]);
		}
		::UnmapViewOfFile(p);
		::PostMessage(hwnd, msg, 0, 0);
	//}
	
	return true;	
}

Vector<String> AppInstance::GetCmd()
{
	Vector<String> cmd;
	
	data = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "ultimateplayer.map");
	if(!data)
	{
		error = GetLastError();
		//PromptOK(Format("1 %d", (int)error));
		return cmd;
	}
	void * p = ::MapViewOfFile(data, FILE_MAP_READ, 0, 0, 0);
	if(!p)
	{
		error = GetLastError();
		//PromptOK(Format("2 %d", (int)error));
		return cmd;
	}

	//ONCELOCK {
		
//		int cnt = *((int *) p);
//		MemStream s((void *) ((byte *) p  + 4), cnt * 1024);

		MemStream s(p, ::GetFileSize(data, NULL));
		int cnt = s.Get32();

		char str[1024];
		
		for(int i = 0; i < cnt; i++)
		{
			int len = s.Get32();
			s.GetAll((void *)str, len);
			str[len] = '\0';
			cmd.Add(str);
		}
		::UnmapViewOfFile(p);
		CloseHandle(data);
	//}
	
	
	return cmd;
}

