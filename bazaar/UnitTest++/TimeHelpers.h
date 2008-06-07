#include "Config.h"

//#if !defined PLATFORM_WIN32
#ifndef flagWIN32
    #include "Posix/TimeHelpersPosix.h"
#else
    #include "Win32/TimeHelpersWin32.h"
#endif
