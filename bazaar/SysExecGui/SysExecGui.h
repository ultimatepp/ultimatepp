#ifndef _SysExecGui_SysExecGui_h
#define _SysExecGui_SysExecGui_h

#include <SysExec/SysExec.h>

NAMESPACE_UPP

///////////////////////////////////////////////////////////////////////////////////////////////
// SysXXXAdmin and SysXXXUser function with gui password frontend (for POSIX)
// For windows you don't have the password dialog, but do have uniform interface
// we propose them too -- they just don't show the interface prompt but the UAC one

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it without waiting for its termination
// it WILL prompt for user intervention on windows secure systems (Vista+ OSes), on which the password is useless
// on linux, will return an error if password is wrong
// return true on success, false otherwise
bool SysStartAdmin(String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysStartAdmin(String const &command, String const &args);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it without waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysStartUser(String const &user, String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysStartUser(String const &user, String const &command, String const &args);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it waiting for its termination
// it WILL prompt for user intervention on windows secure systems (Vista+ OSes), on which the password is useless
// on linux, will return an error if password is wrong
// return true on success, false otherwise
bool SysExecAdmin(String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysExecAdmin(String const &command, String const &args);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as required user, passing a command line to it waiting for its termination
// on linux, will return an error if password is required AND wrong
// on windows, by now... it just spawn the process without changing security level
// I still shall find a way to go back to user mode on windows
// return true on success, false otherwise
bool SysExecUser(String const &user, String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysExecUser(String const &user, String const &command, String const &args);

END_UPP_NAMESPACE

#endif
