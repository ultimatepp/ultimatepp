#include "SshBasics.h"

// ShellConsole:
// Demonstrates an interactive shell in console mode.

void ShellConsole(SshSession& session)
{
	SshShell shell(session);
	shell.Timeout(Null);
	if(!shell.Console("ansi"))
		LOG(shell.GetErrorDesc());
}