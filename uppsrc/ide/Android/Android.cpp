#include "Android.h"

NAMESPACE_UPP

String Android::GetScriptExt()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ".bat";
#else
	return "";
#endif	
}

String Android::GetCmdExt()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ".cmd";
#else
	return "";
#endif		
}

END_UPP_NAMESPACE
