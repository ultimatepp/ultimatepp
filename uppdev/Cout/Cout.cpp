#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	for(int i = 0; i < 10; i++) {
		Cout() << "This is just a test of cout!\n";
	}
	Cout() << "This is just a test of cout!" << EOL;
	FileOut out(ConfigFile("test.txt"));
	out << "Hello!" << EOL;
	out << "This ends with LF only!\n";
}
