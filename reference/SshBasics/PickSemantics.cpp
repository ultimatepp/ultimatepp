#include "SshBasics.h"

// SshPick:
// Demonstrates the pick (move) semantics for ssh objects.

void SshPick(SshSession& session)
{
	SshSession psession = pick(session);	// All Ssh-based objects are pickable.
	if(!session)
		LOG("SshSession object is picked.");
	SFtpGet(psession);
}