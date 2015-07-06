#include "Java.h"

NAMESPACE_UPP

String Java::GetDelimiter()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ";";
#else
	return ":";
#endif
}

END_UPP_NAMESPACE
