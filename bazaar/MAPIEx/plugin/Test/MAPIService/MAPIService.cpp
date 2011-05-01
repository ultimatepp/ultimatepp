////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File: MAPIService.cpp
// Description: Example of using CMAPIEx in a windows service
//
// Copyright (C) 2005-2011, Noel Dillabough
//
// This source code is free to use and modify provided this notice remains intact and that any enhancements
// or bug fixes are posted to the CodeProject page hosting this class for the community to benefit.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MAPIService.h"

// To make deploying the service easier, I'm linking MAPIEx as a library; to use the DLL instead comment out the following lines 
// and change the dependency to MAPIEx
#undef AFX_EXT_CLASS
#define AFX_EXT_CLASS

#include "../../MAPIEx.h"

// Service globals
#define SERVICE_NAME _T("MAPIExService")
SERVICE_STATUS status; 
SERVICE_STATUS_HANDLE hStatus;
CMAPIEx mapi;

#ifdef UNICODE
#define PRINTF wprintf
#else
#define PRINTF printf
#endif

void SetStatus(DWORD dwStatus, DWORD dwExitCode=0)
{
	status.dwCurrentState=dwStatus;
	status.dwWin32ExitCode=dwExitCode;
	SetServiceStatus(hStatus, &status);
}

void ControlHandler(DWORD dwControl) 
{ 
	if(dwControl==SERVICE_CONTROL_STOP || dwControl==SERVICE_CONTROL_SHUTDOWN) 
	{
		SetStatus(SERVICE_STOPPED);
		mapi.Logout();
		CMAPIEx::Term();
	}
} 

BOOL InstallService(SC_HANDLE hManager)
{
	TCHAR szPath[_MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), szPath, _MAX_PATH);

	SC_HANDLE hService=CreateService(hManager, SERVICE_NAME, SERVICE_NAME, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, szPath, NULL, NULL, NULL, NULL, NULL);
	if(!hService) return FALSE;

	SERVICE_DESCRIPTION desc;
	desc.lpDescription=_T("MAPIEx sample service");
	ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &desc);
	CloseServiceHandle(hService);
	return TRUE;
}

long CALLBACK OnNewMessage(LPVOID lpvContext, ULONG cNotification, LPNOTIFICATION lpNotifications)
{
	// I'm just doing a beep here when a new mail arrives 
	MessageBeep(-1);
	return 0;
}

void ServiceMain(int argc, char** argv) 
{ 
	memset(&status, 0, sizeof(SERVICE_STATUS));
	status.dwServiceType=SERVICE_WIN32;
	status.dwCurrentState=SERVICE_START_PENDING;
	status.dwControlsAccepted=SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

	hStatus=RegisterServiceCtrlHandler(SERVICE_NAME, (LPHANDLER_FUNCTION)ControlHandler);
	if(!hStatus) return;

	// initialize MAPI, and turn on notifications for new mail
	if(!CMAPIEx::Init(0, TRUE) || !mapi.Login(NULL, 0, TRUE) || !mapi.OpenMessageStore() || !mapi.Notify(OnNewMessage, NULL, fnevNewMail)) 
	{
		SetStatus(SERVICE_STOPPED, -1);
		return;
	}

	// sleep while running, checking every few seconds
	SetStatus(SERVICE_RUNNING);
	while(status.dwCurrentState==SERVICE_RUNNING) 
	{
		Sleep(4000);
	}
	ControlHandler(SERVICE_CONTROL_SHUTDOWN);
}

void main(int argc, char* argv[]) 
{ 
	if(argc>1)
	{
		SC_HANDLE hManager=OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if(hManager) 
		{
			SC_HANDLE hService=OpenService(hManager, SERVICE_NAME, SERVICE_ALL_ACCESS);

			if(!_tcscmp(argv[1], _T("-i"))) 
			{
				if(InstallService(hManager)) PRINTF("Installed %s successfully.\n", SERVICE_NAME);
				else PRINTF("Failed to install %s.\n", SERVICE_NAME);
			} else if(!_tcscmp(argv[1], _T("-u"))) 
			{
				if(hService && DeleteService(hService)) PRINTF("Uninstalled %s successfully.\n", SERVICE_NAME);
				else PRINTF("Failed to uninstall %s.\n", SERVICE_NAME);
			} else {
				PRINTF(_T("Usage:\n"));
				PRINTF(_T("\t-i\tInstall Service\n"));
				PRINTF(_T("\t-u\tUninstall Service\n"));
			}
			if(hService) CloseServiceHandle(hService);
			CloseServiceHandle(hManager);
		}
		return;
	}

	SERVICE_TABLE_ENTRY table[]={
		{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};
	StartServiceCtrlDispatcher(table);  
}
