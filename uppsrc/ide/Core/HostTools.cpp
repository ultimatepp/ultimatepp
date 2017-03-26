#include "Core.h"

String LocalHostTools::NormalizeExecutablePath(const String& path) const
{
	return ::NormalizeExePath(path);
}
