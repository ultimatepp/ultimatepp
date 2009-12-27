#include "service.h"

#ifdef PLATFORM_WIN32

#include <wincon.h>
#include <winsvc.h>

#ifndef _MULTITHREADED
#error Service must be compiled in MT mode!
#endif

#endif

#ifdef PLATFORM_POSIX
#include <signal.h>
#endif

NAMESPACE_UPP

volatile bool service_stopped = false;
volatile bool service_paused = false;
volatile bool service_exited = false;

void StopService()
{
	service_stopped = true;
	int start = msecs();
	while(msecs(start) < 10000 && !service_exited)
		Sleep(100);
}

void PauseService()
{
	service_paused = true;
}

void ContinueService()
{
	service_paused = false;
}

#ifdef PLATFORM_POSIX
#ifdef PLATFORM_POSIX
static void QuitSignal(int signal)
{
	RLOG("Received signal " << signal << ", quitting...");
	service_stopped = true;
}

INITBLOCK {
	/*SetExeTitle(WEBMAP_EXETITLE);*/
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, &QuitSignal);
	signal(SIGTERM, &QuitSignal);
	signal(SIGKILL, &QuitSignal);
	/* setenv("DISPLAY", ":0", false); */
}
#endif

#endif

#ifdef PLATFORM_WIN32

static char service_name[256];

static String ThreadId()
{
	return String().Cat() << "[T" << FormatIntHex(GetCurrentThreadId()) << "]";
}

static BOOL WINAPI EventHandler(DWORD CtrlType)
{
	RLOG("EventHandler (CtrlType = " << (int)CtrlType << ")");
	if(CtrlType == CTRL_LOGOFF_EVENT)
		RLOG("LOGOFF_EVENT (ignored)");
	else {
		RLOG("exiting");
		service_stopped = true;
	}
	return true;
}

void InstallConsoleCtrlHandler()
{
	SetConsoleCtrlHandler(&EventHandler, true);
}

SERVICE_STATUS        ServiceStatus;
SERVICE_STATUS_HANDLE ServiceStatusHandle;

struct ServiceHandle {
	ServiceHandle(SC_HANDLE sc = 0) : sc(sc) {}
	~ServiceHandle()                        { Close(); }

	void           Close()                  { if(sc) { CloseServiceHandle(sc); sc = 0; } }
	SC_HANDLE      operator = (SC_HANDLE h) { Close(); return sc = h; }

	operator SC_HANDLE() const { return sc; }

	SC_HANDLE sc;
};

bool InstallService(String name, String display_name, String& cmdline, String& status)
{
	ServiceHandle scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!scm) {
		status = "Cannot open service manager";
		return false;
	}
	ServiceHandle me = OpenService(scm, name, SC_MANAGER_ALL_ACCESS);
	if(me) {
		SERVICE_STATUS srvstat;
		if(ControlService(me, SERVICE_CONTROL_STOP, &srvstat)) {
			int start = msecs();
			while(QueryServiceStatus(me, &srvstat) && srvstat.dwCurrentState != SERVICE_STOPPED && msecs(start) < 10000)
				Sleep(100);
		}
		if(!DeleteService(me)) {
			status = NFormat("Error deleting existing service: %s", GetLastErrorMessage());
			return false;
		}
		me.Close();
	}

	me = CreateService(scm, name, display_name,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		cmdline, 0, 0, 0, 0, 0);
	if(!me) {
		status = NFormat("Error creating service: %s", GetLastErrorMessage());
		return false;
	}
	if(!StartService(me, 0, NULL)) {
		status = NFormat("Error starting service: %s", GetLastErrorMessage());
		return false;
	}
	return true;
}

bool UninstallService(String name, String& status)
{
	ServiceHandle scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!scm) {
		status = "Cannot open service manager";
		return false;
	}

	ServiceHandle me = OpenService(scm, name, SC_MANAGER_ALL_ACCESS);
	if(!me) {
		status = "Cannot open service.";
		return false;
	}
	SERVICE_STATUS srvstat;
	if(ControlService(me, SERVICE_CONTROL_STOP, &srvstat)) {
		int start = msecs();
		while(QueryServiceStatus(me, &srvstat) && srvstat.dwCurrentState != SERVICE_STOPPED && msecs(start) < 10000)
			Sleep(100);
	}
	if(!DeleteService(me)) {
		status = NFormat("Error deleting service: %s", GetLastErrorMessage());
		return false;
	}
	return true;
}

static void WINAPI ServiceSignal(DWORD OpCode)
{
	switch(OpCode) {
		case SERVICE_CONTROL_PAUSE: {
			RLOG(ThreadId() << "service pause");
			ServiceStatus.dwControlsAccepted = 0;
			ServiceStatus.dwCheckPoint = 1;
			ServiceStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
			if(!SetServiceStatus(ServiceStatusHandle, &ServiceStatus))
				break;
			PauseService();
			ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE
				| SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;;
			ServiceStatus.dwCheckPoint = 0;
			ServiceStatus.dwCurrentState = SERVICE_PAUSED;
			SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
			break;
		}

		case SERVICE_CONTROL_CONTINUE: {
			RLOG(ThreadId() << "service continue");
			ServiceStatus.dwControlsAccepted = 0;
			ServiceStatus.dwCheckPoint = 1;
			ServiceStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
			if(!SetServiceStatus(ServiceStatusHandle, &ServiceStatus))
				break;
			ContinueService();
			ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE
				| SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
			ServiceStatus.dwCheckPoint = 0;
			ServiceStatus.dwCurrentState = SERVICE_RUNNING;
			SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
			break;
		}

		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP: {
			RLOG(ThreadId() << "service stop / shutdown");
			ServiceStatus.dwControlsAccepted = 0;
			ServiceStatus.dwCheckPoint = 1;
			ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			ServiceStatus.dwWaitHint = 5000;
			if(!SetServiceStatus(ServiceStatusHandle, &ServiceStatus))
				break;
			StopService();
			ServiceStatus.dwCheckPoint = 0;
			ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
			break;
		}

		default: {
			break;
		}
	}
}

static void WINAPI ServiceInit(DWORD argc, LPTSTR *argv)
{
	if(!(ServiceStatusHandle = RegisterServiceCtrlHandler(service_name, &ServiceSignal)))
		return;
	ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = 0;
	ServiceStatus.dwWin32ExitCode = NO_ERROR;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 1;
	ServiceStatus.dwWaitHint = 1000;

	if(!SetServiceStatus(ServiceStatusHandle, &ServiceStatus))
		return;

	RLOG(ThreadId() << "initializing service");
	bool inited = InitService();
	if(inited) {
		RLOG(ThreadId() << "service successfully initialized");
		ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE
			| SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	}
	else {
		RLOG(ThreadId() << "service initialization failed");
		ServiceStatus.dwControlsAccepted = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	}

	ServiceStatus.dwCheckPoint = 0;
	if(!SetServiceStatus(ServiceStatusHandle,&ServiceStatus))
		return;

	if(inited) {
		RLOG(ThreadId() << "running service");
		RunService();
		RLOG(ThreadId() << "exiting service thread");
		service_exited = true;
	}
}

void RunServiceDispatcher(String service_name_)
{
	ASSERT(service_name_.GetLength() < __countof(service_name));
	strcpy(service_name, service_name);
	static SERVICE_TABLE_ENTRY servicetable[] = {
		{ service_name, &ServiceInit },
		{ NULL, NULL }
	};

	RLOG(ThreadId() << "starting service control dispatcher");
	StartServiceCtrlDispatcher(servicetable);
	RLOG(ThreadId() << "exiting main thread");
	return;
}

END_UPP_NAMESPACE

#endif
