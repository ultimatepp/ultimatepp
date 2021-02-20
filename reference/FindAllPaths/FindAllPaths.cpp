#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String dir = GetFileFolder(GetFileFolder(GetDataFile("x"))); // get nest directory for demo
	DUMPC(FindAllPaths(dir));
	DUMPC(FindAllPaths(dir, "*.h *.cpp"));
	DUMPC(FindAllPaths(dir, "*", FINDALLFOLDERS));
}
