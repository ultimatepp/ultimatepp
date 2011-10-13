#include "SysInfo.h"

#ifdef PLATFORM_WIN32

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////
// utility function to see if running Vista or newer OSs
bool IsVistaOrLater(void)
{
    OSVERSIONINFO osvi;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);
    
    return (osvi.dwMajorVersion >= 6);
}
 
////////////////////////////////////////////////////////////////////////////////////
// utility function to see if running Xp or newer OSs
bool IsXpOrLater(void)
{
    OSVERSIONINFO osvi;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);
    
    return (osvi.dwMajorVersion > 5 || (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion >= 1));
}

#ifdef __MINGW32__ 
extern "C" BOOL WINAPI CheckTokenMembership(HANDLE,PSID,PBOOL);
#endif

static BOOL IsGroupMember(DWORD dwRelativeID, BOOL bProcessRelative, BOOL* pIsMember)
{
    HANDLE hToken, hDupToken;
    PSID pSid = NULL;
    SID_IDENTIFIER_AUTHORITY SidAuthority = SECURITY_NT_AUTHORITY;

    if (!pIsMember)
    {
        SetLastError(ERROR_INVALID_USER_BUFFER);
        return FALSE;
    }

    if (bProcessRelative || !OpenThreadToken(GetCurrentThread(), TOKEN_QUERY | TOKEN_DUPLICATE, TRUE, &hToken))
    {
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &hToken))
            return FALSE;
    }

    if (!DuplicateToken(hToken, SecurityIdentification, &hDupToken))
    {
        CloseHandle(hToken);
        return FALSE;
    }

    CloseHandle(hToken);
    hToken = hDupToken;

    if (!AllocateAndInitializeSid(&SidAuthority, 2,
            SECURITY_BUILTIN_DOMAIN_RID, dwRelativeID, 0, 0, 0, 0, 0, 0,
            &pSid))
    {
        CloseHandle(hToken);
        return FALSE;
    }

    if (!CheckTokenMembership(hToken, pSid, pIsMember))
    {
        CloseHandle(hToken);
        FreeSid(pSid);

        *pIsMember = FALSE;
        return FALSE;
    }

    CloseHandle(hToken);
    FreeSid(pSid);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// utility functions to check whether an app is running in elevated mode
bool IsUserAdministrator(void)
{
	BOOL isAdmin;
	
	// always an admin for XP and previous versions
	if(!IsVistaOrLater())
		return true;
	
	// check if running in admin mode for Vista or newers
    IsGroupMember(DOMAIN_ALIAS_RID_ADMINS, FALSE, &isAdmin);
    return isAdmin;
}

END_UPP_NAMESPACE

#endif
