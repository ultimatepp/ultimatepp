////////////////////////////////////////////////////////////////////////////////////////////////////////
// a working (better ??) implementation of libsudo equivalent
// allows to run sudo either in sync or async mode
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SysExec_SudoLib_h_
#define _SysExec_SudoLib_h_

#include <Core/Core.h>

NAMESPACE_UPP

#ifdef PLATFORM_POSIX

// executes a command via sudo; if wait is true, will wait for command end, otherwise executes it in background
bool SudoExec(String user, String const &password, String const &args, VectorMap<String, String> const &env, bool wait);

#endif

END_UPP_NAMESPACE

#endif
