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
	#include <X11/extensions/XTest.h>
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

#endif
