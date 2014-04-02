#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	for(;;) {
		Cout() << "Press ENTER to open FileOut\n";
		ReadStdIn();
		FileOut out(ConfigFile("testfile"));
		if(out) {
			Cout() << "file opened\nPress ENTER to close\n";
			ReadStdIn();
		}
		else
			Cout() << "NOT opened\n";
	}
}
