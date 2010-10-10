#ifndef _ProtectStatus_h_
#define _ProtectStatus_h_

#include <Core/Core.h>

NAMESPACE_UPP

// server error codes
typedef enum {
	PROTECT_OK = 0,						// no error (unused)
	PROTECT_HTTP_ERROR,					// error on HTTP communication with server
	PROTECT_BAD_REQUEST,				// missing POST data on request
	PROTECT_MISSING_IV,					// missing Initialization Vector on POST data
	PROTECT_BAD_DATA,					// missing mandatory fields in POST DATA
	PROTECT_NOT_CONNECTED,				// not connected to server -- must connect first
	PROTECT_CONNECTION_EXPIRED,			// server connection timeout -- should refresh more often
	PROTECT_LICENSE_EXPIRED,			// testing license expired
	PROTECT_UNREGISTERED,				// product unregistered
	PROTECT_LICENSES_NUMBER_EXCEEDED,	// number of product licenses exceeded (too many apps running)
	PROTECT_MAIL_ALREADY_USED,			// e-mail already used (and expired), can't register
	PROTECT_INVALID_APP_VERSION			// your license isn't valid for this APP version, should upgrade
} ProtectStatus;

// gets human-readable error message
extern String ProtectMessage(int m);

// server request reasons
typedef enum {
	PROTECT_UNKNOWN_REASON,	// internal error
	PROTECT_CONNECT,		// establish connection to server
	PROTECT_DISCONNECT,		// frees server connection
	PROTECT_REFRESH,		// refreshes server connection (to restart timeout)
	PROTECT_GETKEY,			// gets application key
	PROTECT_REGISTER,		// registers app for timed demo
	PROTECT_GETLICENSEINFO	// gets info about license (name, expiration date, app version....)
} ProtectReasons;

// get reason in string format
extern String ProtectReasonStr(int r);

// get reason in enum format
extern int ProtectReason(String const &s);

END_UPP_NAMESPACE

#endif
