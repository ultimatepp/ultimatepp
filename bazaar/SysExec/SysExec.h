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
bool SysStart(String const &command, String const &args, const VectorMap<String, String> &Environ);
bool SysStart(String const &command, String const &args);

END_UPP_NAMESPACE

#endif
