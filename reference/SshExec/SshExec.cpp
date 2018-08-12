#include <Core/Core.h>
#include <Core/SSH/SSH.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
//	Ssh::Trace();

	const char *cmd = "ls -l /pub/example";
	String cout, cerr;

	SshSession session;
	if(session.Timeout(30000).Connect("demo:password@test.rebex.net:22")) {
		SshExec exec = session.CreateExec();
		if(exec(cmd, cout, cerr) >= 0) {
			LOG("Stdout:\n" << cout);
			LOG("Stderr:\n" << cerr);
			LOG("Exit code: " << exec.GetExitCode());
		}
		else
			LOG(exec.GetErrorDesc());
	}
	else
		LOG(session.GetErrorDesc());
}
