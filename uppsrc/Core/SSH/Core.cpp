#include "SSH.h"

namespace Upp {

namespace SSH {
bool sTrace = false;
int  sTraceVerbose = 0;

// SSH diagnostic utilities.

String GetName(int type, int64 id)
{
	String s;
	switch(type) {
		case Ssh::SESSION:
			s = "Session";
			break;
		case Ssh::SFTP:
			s = "SFtp";
			break;
		case Ssh::CHANNEL:
			s = "Channel";
			break;
		case Ssh::SCP:
			s = "Scp";
			break;
		case Ssh::EXEC:
			s = "Exec";
			break;
		case Ssh::SHELL:
			s = "Shell";
			break;
		case Ssh::TUNNEL:
			s = "Tunnel";
			break;
		default:
			return "";
	}
	return pick(Format("SSH: %s, oid: %d: ", s, id));
}
}

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

// Ssh: SSH objects core class.

static StaticMutex sLoopLock;

bool Ssh::Run(Gate<>&& fn, bool abortable)
{
	auto Do = [=, &fn]()
	{
		Mutex::Lock __(sLoopLock);

		if(IsTimeout())
			ThrowError(-1, "Operation timed out.");

		if(abortable && ssh->status == ABORTED)
			ThrowError(-1, "Operation aborted.");
	
		if(ssh->socket && ssh->socket->IsError())
			ThrowError(-1, "[Socket error]: " << ssh->socket->GetErrorDesc());

		if(!ssh->init)
			ssh->init = Init();
		
		return !ssh->init || ! fn();
	};
	
	try {
		ssh->status = WORKING;
		ssh->start_time = msecs();
			
		while(Do())	Wait();

		ssh->status = IDLE;
	}
	catch(const Error& e) {
		SetError(e.code, e);
	}
	catch(...) {
		SetError(-1, "Unhandled exception.");
	}

	return !IsError();
}

void Ssh::Wait()
{
	UpdateClient();
	if(!ssh->socket || !ssh->session)
		return;
	dword q = 0, r = libssh2_session_block_directions(ssh->session);
	if(r & LIBSSH2_SESSION_BLOCK_INBOUND)
		q |= WAIT_READ;
	if(r & LIBSSH2_SESSION_BLOCK_OUTBOUND)
		q |= WAIT_WRITE;
	SocketWaitEvent we;
	we.Add(*ssh->socket, q);
	we.Wait(ssh->waitstep);
}

void Ssh::ThrowError(int rc, const String& reason)
{
	if(IsNull(reason) && ssh && ssh->session) {
		Buffer<char*> libmsg(256, 0);
		rc = libssh2_session_last_error(ssh->session, libmsg, nullptr, 0);
		throw Error(rc, *libmsg);
	}
	else
		throw Error(rc, reason);
}

void Ssh::SetError(int rc, const String& reason)
{
	ssh->status  = FAILED;
	ssh->error.a = rc;
	ssh->error.b = reason;
	if(ssh->socket) {
		ssh->socket->ClearAbort();
		ssh->socket->ClearError();
	}
	LLOG("Failed. Code = " << rc << ", " << reason);
}

int64 Ssh::GetNewId()
{
	static std::atomic<int64> objectid(0);
	return ++objectid;
}

Ssh::Ssh()
{
    ssh.Create();
    ssh->session        = nullptr;
    ssh->socket         = nullptr;
    ssh->init           = false;
    ssh->timeout        = Null;
    ssh->start_time     = 0;
    ssh->waitstep       = 10;
    ssh->chunk_size     = CHUNKSIZE;
    ssh->status         = IDLE;
    ssh->oid            = GetNewId();
    ssh->otype          = CORE;
}

Ssh::~Ssh()
{
}

INITIALIZER(SSH) {
	libssh2_init(0);
}
EXITBLOCK {
	libssh2_exit();
}
}