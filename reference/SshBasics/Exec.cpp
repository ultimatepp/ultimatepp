#include "SshBasics.h"

// ExecListDir:
// Demonstrates a remote command execution.

void ExecListDir(SshSession& session)
{
	const char *cmdline = "ls -l /pub/example";
	
	SshExec exec(session);
	String cout, cerr;
	int exit_code = exec(cmdline, cout, cerr);
	if(!exec.IsError()) {
		DUMP(exit_code);
		LOG("Stdout:\n" << cout);
		LOG("Stderr:\n" << cerr);
		return;
	}
	LOG(exec.GetErrorDesc());
	
	// Or you can use one of the helper functions instead:
	// LOG("Stdout:\n" << SshExecute(session, cmdline));
}