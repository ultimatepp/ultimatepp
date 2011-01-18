#ifndef _SysExex_h_
#define _SysExex_h_

#include <Core/Core.h>

NAMESPACE_UPP

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command, passing a command line to it and gathering the output
// from both stdout and stderr
bool SysExec(String const &command, String const &args, const VectorMap<String, String> &Environ, String &OutStr, String &ErrStr);
bool SysExec(String const &command, String const &args, String &OutStr, String &ErrStr);
bool SysExec(String const &command, String const &args, const VectorMap<String, String> &Environ, String &OutStr);
bool SysExec(String const &command, String const &args, String &OutStr);
bool SysExec(String const &command, String const &args);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command, passing a command line to it without waiting for its termination
intptr_t SysStart(String const &command, String const &args, const VectorMap<String, String> &Environ);
intptr_t SysStart(String const &command, String const &args);

///////////////////////////////////////////////////////////////////////////////////////////////
// executes an external command as Admin user, passing a command line to it without waiting for its termination
// it WILL prompt for user intervention on secure systems (linux - Vista+ OSes)
// return true on success, false otherwise
bool SysStartAdmin(String Password, String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysStartAdmin(String Password, String const &command, String const &args);

END_UPP_NAMESPACE

#endif
