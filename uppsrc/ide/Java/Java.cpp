#include "Java.h"

namespace Upp {

String Java::GetDelimiter()
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ";";
#else
	return ":";
#endif
}

}
