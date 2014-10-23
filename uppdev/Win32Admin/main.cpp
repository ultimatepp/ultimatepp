#include <CtrlLib/CtrlLib.h>

#include <Shellapi.h>

using namespace Upp;

bool IsAdmin()
{
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;
	// Initialize SID.
	if( !AllocateAndInitializeSid( &NtAuthority,
	                               2,
	                               SECURITY_BUILTIN_DOMAIN_RID,
	                               DOMAIN_ALIAS_RID_ADMINS,
	                               0, 0, 0, 0, 0, 0,
	                               &AdministratorsGroup))
	{
	    // Initializing SID Failed.
	    return false;
	}
	// Check whether the token is present in admin group.
	BOOL IsInAdminGroup = FALSE;
	if( !CheckTokenMembership( NULL,
	                           AdministratorsGroup,
	                           &IsInAdminGroup ))
	{
	    // Error occurred.
	    IsInAdminGroup = FALSE;
	}
	// Free SID and return.
	FreeSid(AdministratorsGroup);

	return IsInAdminGroup;
}

GUI_APP_MAIN
{
	if(IsAdmin()) {
		RLOG("Now I am admin, everything is fine.");
		PromptOK("I am an admin!");
	}
	else {
		RLOG("Not an admin, starting as admin");
		PromptOK("Not an admin, running as admin");
		ShellExecute(NULL, "runas", GetExeFilePath(), 0, 0, SW_SHOWNORMAL);
		return;
	}

	RLOG("Doing admin stuff...");
	getchar();
}
