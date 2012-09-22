#include <Core/Core.h>

using namespace Upp;

String Request(const String& r)
{
	TcpSocket s;
	if(!s.Connect(CommandLine().GetCount() ? CommandLine()[0] : "127.0.0.1", 3214)) {
		Cout() << "Unable to connect to server!\n";
		SetExitCode(1);
		return Null;
	}
	s.Put(r + '\n');
	return s.GetLine();
}

// Start reference/SocketServer before starting this program

CONSOLE_APP_MAIN
{
	Cout() << Request("time") << '\n';
	Cout() << Request("33") << '\n';
}
