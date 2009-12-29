#ifndef _TCore_service_service_h_
#define _TCore_service_service_h_

#include <Web/Web.h>

#ifdef PLATFORM_WIN32

extern bool InitService(); // implemented by main program
extern void RunService(); // implemented by main program

NAMESPACE_UPP

extern volatile bool service_stopped;
extern volatile bool service_paused;

void InstallConsoleCtrlHandler();
bool InstallService(String name, String display_name, String& cmdline, String& status);
bool UninstallService(String name, String& status);
void RunServiceDispatcher(String service_name);

END_UPP_NAMESPACE

#endif

#endif
