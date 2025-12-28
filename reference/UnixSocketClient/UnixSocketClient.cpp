#include <Core/Core.h>

using namespace Upp;

// Start reference/UnixSocketServer before starting this program
String GetSocketPath()
{
	String temp;
#ifdef PLATFORM_WIN32
	temp = GetEnv("TEMP");
#else
	temp = GetTempPath();
#endif
	return AppendFileName(temp, "upp-unixsocket.socket");
}

String Request(const String r)
{
	Socket s;
	if(!s.ConnectFileSystem(GetSocketPath())) {
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
