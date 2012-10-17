#ifndef _SysExex_h_
#define _SysExex_h_

#include <Core/Core.h>

#ifdef PLATFORM_WIN32
#include "SysInfo.h"
#endif

NAMESPACE_UPP

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command, passing a command line to it and gathering the output
// from both stdout and stderr
bool SysExec(String const &command, String const &args, const VectorMap<String, String> &Environ, String &OutStr, String &ErrStr);
bool SysExec(String const &command, String const &args, String &OutStr, String &ErrStr);
bool SysExec(String const &command, String const &args, const VectorMap<String, String> &Environ, String &OutStr);
bool SysExec(String const &command, String const &args, String &OutStr);
bool SysExec(String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysExec(String const &command, String const &args);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command, passing a command line to it without waiting for its termination
// optionally returns pid of started process
bool SysStart(String const &command, String const &args, const VectorMap<String, String> &Environ, intptr_t *pid = NULL);
bool SysStart(String const &command, String const &args, intptr_t *pid = NULL);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it without waiting for its termination
// it WILL prompt for user intervention on windows secure systems (Vista+ OSes), on which the password is useless
// on linux, will return an error if password is wrong
// return true on success, false otherwise
bool SysStartAdmin(String const &password, String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysStartAdmin(String const &password, String const &command, String const &args);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it without waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysStartUser(String const &user, String const &password, String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysStartUser(String const &user, String const &password, String const &command, String const &args);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it waiting for its termination
// it WILL prompt for user intervention on windows secure systems (Vista+ OSes), on which the password is useless
// on linux, will return an error if password is wrong
// return true on success, false otherwise
bool SysExecAdmin(String const &password, String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysExecAdmin(String const &password, String const &command, String const &args);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysExecUser(String const &user, String const &password, String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysExecUser(String const &user, String const &password, String const &command, String const &args);

END_UPP_NAMESPACE

#endif
