#include "SSH.h"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool SshTunnel::IsValid()
{
	bool b = false;
	
	switch(mode) {
		case CONNECT:	b = channel || listener; break;
		case LISTEN:	b = channel || listener; break;
		case ACCEPT:	b = channel; break;
		default: NEVER();
	}
	if(b)
		SetError(-1, "Invalid channel instance.");
	return !b;
}

void SshTunnel::Exit()
{
	if(!listener)
		return;
	
	Run([=]() mutable{
		int rc = libssh2_channel_forward_cancel(*listener);
		if(!WouldBlock(rc) && rc < 0) ThrowError(rc);
		if(rc == 0)	listener.Clear();
		return !rc;
	});
}

bool SshTunnel::Connect(const String& host, int port)
{
	mode = CONNECT;
	
	if(!IsValid())
		return false;

	return Run([=]() mutable{
		LIBSSH2_CHANNEL *ch = libssh2_channel_direct_tcpip(ssh->session, host , port);
		if(!ch && !WouldBlock()) ThrowError(-1);
		if(ch) {
			channel = MakeOne<LIBSSH2_CHANNEL*>(ch);
			LLOG("Direct tcp-ip connection to " << host << ":" << port << " is established.");
		}
		return ch;
	});
}

bool SshTunnel::Connect(const String& url)
{
	UrlInfo u(url);
	if(!u.host.IsEmpty() && u.port.IsEmpty())
		return Connect(u.host, StrInt(u.port));
	SetError(-1, "Malformed proxy connection URL.");
	return false;
}

bool SshTunnel::Listen(const String& host, int port, int* bound_port, int listen_count)
{
	mode = LISTEN;

	if(!IsValid())
		return false;
	
	return Run([=]() mutable {
		LIBSSH2_LISTENER *lsn = libssh2_channel_forward_listen_ex(
			ssh->session,
			host.IsEmpty() ? nullptr : ~host,
			port,
			bound_port ? bound_port : nullptr,
			listen_count
		);
		if(!lsn && !WouldBlock())
			ThrowError(-1);
		if(lsn) {
			listener = MakeOne<LIBSSH2_LISTENER*>(lsn);
			LLOG("Started listening on port #" << port);
		}
		return lsn;
	});
}

bool SshTunnel::Accept(SshTunnel& listener)
{
	mode = ACCEPT;

	if(!IsValid())
		return false;
	
	if(!listener.listener) {
		SetError(-1, "Invalid listener.");
		return false;
	}

	return Run([=, &listener]() mutable {
		LIBSSH2_CHANNEL *ch = libssh2_channel_forward_accept(*listener.listener);
		if(!ch && !WouldBlock()) ThrowError(-1);
		if(ch) {
			channel = MakeOne<LIBSSH2_CHANNEL*>(ch);
			LLOG("Connection accepted.");
		}
		return ch;
	});
}
}