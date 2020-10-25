#ifndef _SysInfo_SysInfo_in_h_
#define _SysInfo_SysInfo_in_h_

#include "Core/Core.h"

#if defined(PLATFORM_WIN32) 
	#include <shellapi.h>
	#include <Tlhelp32.h>
	#include <psapi.h>
	#include <Winioctl.h>
	#define CY tagCY
	// To compile in MinGW you will probably need to get files Rpcsal.h, WbemCli.h, WbemProv.h 
	// and for sure you will need wbemuuid.lib, even deleting libwbemuuid.a, as it is uncomplete
	#include <rpcsal.h>	
	#include <wbemcli.h>
	#include <wbemprov.h>
	#include <winnls.h> 
	#include <vfw.h>
	typedef ACCESS_MASK REGSAM;
	#include <PowrProf.h>
	#include <lm.h>
	#include <winsock2.h>
	#include <Ws2tcpip.h>
	#ifndef PROCESS_QUERY_LIMITED_INFORMATION
	#define PROCESS_QUERY_LIMITED_INFORMATION 0x1000
	#endif	
#endif
#ifdef PLATFORM_POSIX
	#include <sys/time.h>
	#include <sys/resource.h>
	#include <sys/ioctl.h>
	#include <net/if.h>
	
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/stat.h>
#ifndef PLATFORM_OSX
	#include <sys/vfs.h>
#endif
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
#ifndef flagNO_XTEST
	#include <X11/extensions/XTest.h>
#endif
	#include <X11/XKBlib.h>
	
	#include <arpa/inet.h>
	#ifdef flagGUI
		#define SetX11ErrorHandler();
		#define SetSysInfoX11ErrorHandler();
	#else
		#define SetX11ErrorHandler();
		int SysInfoX11ErrorHandler(_XDisplay *, XErrorEvent *);
		void SetSysInfoX11ErrorHandler();					
	#endif

#endif

#define _Browser_Browser_h
#include "SysInfo.h"
#ifdef _WIN32
#include "OSwin.h"
#endif

namespace Upp {

#if defined(__GNUC__) && !defined(COMPILER_CLANG)
#define __cdecl __attribute__((cdecl))
#endif
	
class CrashHandler {
public:
    CrashHandler();
    virtual ~CrashHandler() {};

private:
#if defined(PLATFORM_WIN32) 
    static LONG WINAPI UnhandledHandler(EXCEPTION_POINTERS *p);
    static void __cdecl SEHHandler(unsigned u, EXCEPTION_POINTERS* p);
#endif
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif
    static void __cdecl TerminateHandler();
    static void __cdecl UnexpectedHandler();

    static void __cdecl PureCallHandler();

    static void __cdecl InvalidParameterHandler(const wchar_t* expression, 
        const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);

    static void __cdecl NewHandler();
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    static void SigabrtHandler(int);
    static void SigfpeHandler(int);
    static void SigintHandler(int);
    static void SigillHandler(int);
    static void SigsegvHandler(int);
    static void SigtermHandler(int);
};

}

#endif
