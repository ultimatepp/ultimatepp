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
		case Ssh::TCPTUNNEL:
			s = "TCP Tunnel";
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
void Ssh::Check()
{
	auto sock = ssh->socket;

	if(IsTimeout())
		SetError(-1, "Operation timed out.");

	if(ssh->status == ABORTED || (sock && ssh->socket->IsAbort()))
		SetError(-1, "Operation aborted.");

	if(sock && ssh->socket->IsError())
		SetError(-1, "[Socket error]: " << ssh->socket->GetErrorDesc());
}

bool Ssh::Run(Gate<>&& fn)
{
	try {
		if(InProgress())
			SetError(-1, "An operation is already in progress.");

		ssh->status = WORKING;
		ssh->start_time = msecs();


		while(InProgress()) {
			Mutex::Lock __m(StaticMutex);
			if(!ssh->init)
				ssh->init = Init();
			if(ssh->init && fn())
				break;
			Wait();
			Check();
		}
		ssh->status = IDLE;
	}
	catch(const Error& e) {
		ssh->status  = FAILED;
		ssh->error.a = e.code;
		ssh->error.b = e;
		if(ssh->socket) {
			ssh->socket->ClearAbort();
			ssh->socket->ClearError();
		}
		LLOG("Failed. Code = " << e.code << ", " << e);
	}
	return !IsError();
}

dword Ssh::GetWaitEvents()
{
	dword events = 0;
	if(ssh->socket && ssh->session)
		events = libssh2_session_block_directions(ssh->session);
	return !!(events & LIBSSH2_SESSION_BLOCK_INBOUND) * WAIT_READ +
	       !!(events & LIBSSH2_SESSION_BLOCK_OUTBOUND) * WAIT_WRITE;
}

void Ssh::Wait()
{
	while(!IsTimeout()) {
		ssh->whenwait();
		if(!ssh->socket)
			return;
		SocketWaitEvent we;
		AddTo(we);
		if(we.Wait(ssh->waitstep) || ssh->noloop)
			return;
	}
}

void Ssh::SetError(int rc, const String& reason)
{
	if(IsNull(reason) && ssh && ssh->session) {
		Buffer<char*> libmsg(256);
		rc = libssh2_session_last_error(ssh->session, libmsg, nullptr, 0);
		throw Error(rc, *libmsg);
	}
	else
		throw Error(rc, reason);
}

int64 Ssh::GetNewId()
{
	static int64 objectid;
	return objectid == INT64_MAX ? objectid = 1	: ++objectid;
}

Ssh::Ssh()
{
    ssh.Create();
    ssh->session        = nullptr;
    ssh->socket         = nullptr;
    ssh->init           = false;
    ssh->noloop			= false;
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
	RLOG("Initializing libssh2...");
	libssh2_init(0);
}
EXITBLOCK {
	RLOG("Deinitializing libssh2...");
	libssh2_exit();
}
}