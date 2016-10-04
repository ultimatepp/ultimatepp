#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DDUMP(GetSystemDirectory());
	DDUMP(GetFileLength(AppendFileName(GetSystemDirectory(), "sqlncli10.dll")));
	DDUMP(GetFileLength("\\\\office\\holly$\\sqlncli.msi"));
}
