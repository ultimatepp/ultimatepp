#include "SshBasics.h"

// X11Forwarding:
// Demonstrates an interactive shell with X11 forwarding, in console mode.

// This example requires a running X server.

void X11Forwarding(SshSession& session)
{
	SshShell x11shell(session);
	x11shell.Timeout(Null);
	session.WhenX11 = [&x11shell](SshX11Handle xhandle)
	{
		x11shell.AcceptX11(xhandle);

	};
	if(!x11shell.ForwardX11().Console("ansi"))
		LOG(x11shell.GetErrorDesc());
}