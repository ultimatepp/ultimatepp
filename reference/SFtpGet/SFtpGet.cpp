#include <Core/Core.h>
#include <Core/SSH/SSH.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
//	Ssh::Trace();

	const char *file = "/pub/example/readme.txt";

	SshSession session;
	if(session.Timeout(30000).Connect("demo:password@test.rebex.net:22")) {
		SFtp sftp = session.CreateSFtp();
		String out = sftp.Get(file);
		LOG((!sftp.IsError() ? out : sftp.GetErrorDesc()));
	}
	else
		LOG(session.GetErrorDesc());
}
