#include "SshBasics.h"

// OpenTcpTunnel:
// Demonstrates tcp-ip and port forwarding feature of the ssh protocol.

// This example requires upp/reference/SocketServer and upp/reference/SocketClient examples.
// SocketClient: Set the port number to 3215.

bool ServerSendRecv(SshSession& session, String& data)
{
	// SshTunnel <-> SocketServer
	SshTunnel tunnel(session);
	if(!tunnel.Connect("127.0.0.1", 3214)) {
		LOG("ServerSendRecv(): " << tunnel.GetErrorDesc());
		return false;
	}
	tunnel.Put(data + '\n');
	data = tunnel.GetLine();
	return !data.IsEmpty();
}

void ForwardTcpIp(SshSession& session)
{
	SshTunnel listener(session);
	if(!listener.Listen(3215, 5)) {
		LOG("ForwardTcpIp(): " << listener.GetErrorDesc());
		return;
	}
	LOG("SSH tunnel (server mode): Waiting for the requests to be tunneled...");
	for(;;) {
		SshTunnel tunnel(session);
		if(!tunnel.Accept(listener)) {
			LOG("ForwardTcpIp(): " << tunnel.GetErrorDesc());
			return;
		}
		// SocketClient <-> SshTunnel
		String data = tunnel.GetLine();
		LOG("Tunneled Request: " << data);
		if(!data.IsEmpty() && ServerSendRecv(session, data)) {
			LOG("Tunneled Response: " << data);
			tunnel.Put(data + '\n');
		}
	}
}