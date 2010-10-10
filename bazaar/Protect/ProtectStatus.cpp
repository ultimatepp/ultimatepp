#include "ProtectStatus.h"

NAMESPACE_UPP

// server error messages
const char *__ProtectMessages[] =
{
	tt_("NO ERROR"),
	tt_("HTTP SERVER ERROR"),
	tt_("BAD REQUEST"),
	tt_("MISSING INITIALIZATION VECTOR ON DATA"),
	tt_("BAD REQUEST DATA"),
	tt_("NOT CONNECTED TO SERVER"),
	tt_("SERVER CONNECTION EXPIRED"),
	tt_("TESTING LICENSE EXPIRED"),
	tt_("UNREGISTERED APPLICATION"),
	tt_("LICENSES NUMBER EXCEEDED"),
	tt_("EMAIL ALREADY USED AND EXPIRED"),
	tt_("INVALID APPLICATION VERSION, PLEASE UPGRADE")
};

String ProtectMessage(int m)
{
	if(m >= 0 && m < sizeof(__ProtectMessages) / sizeof(const char *))
		return GetLngString(__ProtectMessages[m]);
	else
		return t_("UNKNOWN ERROR");
}

// server request reasons
const char *__ProtectReasons[] =
{
	"UNKNOWN_REASON",
	"CONNECT",			// establish connection to server
	"DISCONNECT",		// frees server connection
	"REFRESH",			// refreshes server connection (to restart timeout)
	"GETKEY",			// gets application key
	"REGISTER",			// registers app for timed demo
	"GETLICENSEINFO"	// gets info about license (name, expiration date, app version....)
};

// get reason in string format
String ProtectReasonStr(int r)
{
	if(r >= 0 && r < sizeof(__ProtectReasons) / sizeof(const char *))
		return __ProtectReasons[r];
	else
		return __ProtectReasons[0];
}

// get reason in enum format
int ProtectReason(String const &s)
{
	for(int i = 0; i < sizeof(__ProtectReasons) / sizeof(const char *); i++)
		if(s == __ProtectReasons[i])
			return i;
	return 0;
}

END_UPP_NAMESPACE