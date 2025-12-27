#include <Core/Core.h>

using namespace Upp;

// Start reference/UnixSocketServer before starting this program

CONSOLE_APP_MAIN
{
#ifdef PLATFORM_POSIX
	auto Request = [](const String& r)
	{
		Socket s;
		if(!s.ConnectFileSystem("/tmp/upp-unixsocket.sock")) {
			Cout() << "Unable to connect to server!\n";
			SetExitCode(1);
			return String();
		}
		s.Put(r + '\n');
		return s.GetLine();
	};

	Cout() << Request("time") << '\n';
	Cout() << Request("33") << '\n';
#else
	Cout() << "This example requires a POSIX compliant operating system...\r\n"
	SetExitCode(1);
#endif
}
