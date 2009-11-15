#include <Web/Web.h>

using namespace Upp;

String Request(const String& r)
{
	Socket s;
	if(!ClientSocket(s, CommandLine().GetCount() ? CommandLine()[0] : "127.0.0.1", 3214)) {
		Cout() << "Unable to connect to server!\n";
		SetExitCode(1);
		return Null;
	}
	s.Write(r + '\n');
	return s.ReadUntil('\n');
}

CONSOLE_APP_MAIN
{
	Cout() << Request("time") << '\n';
	Cout() << Request("33") << '\n';
}
