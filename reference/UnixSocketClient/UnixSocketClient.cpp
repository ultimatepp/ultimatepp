#include <Core/Core.h>

using namespace Upp;

// Start reference/UnixSocketServer before starting this program

String Request(const String r)
{
	Socket s;
	const String& path = NativePath(GetTempDirectory() + "/upp-unixsocket.sock");
	if(!s.ConnectFileSystem(path)) {
		Cout() << "Unable to connect to server!\n";
		SetExitCode(1);
		return String();
	}
	s.Put(r + '\n');
	return s.GetLine();
}

CONSOLE_APP_MAIN
{
	Cout() << Request("time") << '\n';
	Cout() << Request("33") << '\n';
}
