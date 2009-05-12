#ifndef LimitSingleInstance_H
#define LimitSingleInstance_H

#include <CtrlLib/CtrlLib.h>

using namespace Upp;
//typedef void (WINAPI *PROCSWITCHTOTHISWINDOW) (HWND, BOOL); 
//PROCSWITCHTOTHISWINDOW SwitchToThisWindow;

void GetSwitchWindowProc();

HWND    GetAppHwnd(const char * app_name, bool skip_current);
HMODULE GetCurrentMod();
DWORD   GetCurrentPrc();

struct MMFData
{
	bool pending;
    char cmd[256 * 10];	
};

class AppInstance
{
  protected:
    DWORD  error;
    HANDLE mutex;
    HANDLE data;
    
    FileMapping filemap;
        
    
    //MMFData md;
    
  public:

    AppInstance(const char * mutex_name);
    ~AppInstance();
    BOOL IsAnotherInstance();
	bool PassCmd(HWND hwnd, dword msg, const Vector<String> &cmd);
	Vector<String> GetCmd();
	
	DWORD GetError() { return error; }
    
};

#endif
