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
void Ssh::Exit()
{
	ssh->timeout = 5000;
	ssh->start_time = 0;
	ssh->ccmd = -1;
	ssh->queue.Clear();
}

void Ssh::Check()
{
	if(IsTimeout())
		SetError(-1000, "Operation timed out.");
	if(ssh->status == CANCELLED)
		SetError(-1001, "Operation cancelled.");
}

// TODO: Merge Cmd & ComplexCmd.
bool Ssh::Cmd(int code, Function<bool()>&& fn)
{
	if(ssh->status != CLEANUP)
		ssh->status = WORKING;
	if(!IsComplexCmd())
		ssh->queue.Clear();
	ssh->queue.AddTail() = MakeTuple<int, Gate<>>(code, pick(fn));
	if(IsBlocking() && !IsComplexCmd())
		while(Do0());
	return !IsError();
}

bool Ssh::ComplexCmd(int code, Function<void()>&& fn)
{
	bool b = IsComplexCmd(); // Is this complex command a part of another complex command?
	ssh->ccmd = code;
	if(ssh->status != CLEANUP)
		ssh->status = WORKING;
	if(!b)
		ssh->queue.Clear();
	fn();
	if(!b && IsBlocking())
		while(Do0());
	return !IsError();
}

bool Ssh::Do0()
{
	try {
		if(ssh->start_time == 0)
			ssh->start_time = msecs();
INTERLOCKED {
		if(!ssh->init) {
			ssh->init = Init();
		}
		else
		if(!ssh->queue.IsEmpty()) {
			auto cmd = ssh->queue.Head().Get<Gate<>>();
			if(cmd())
				ssh->queue.DropHead();
			else
				Wait();
		}
}
		if(ssh->queue.IsEmpty()) {
			switch(ssh->status) {
				case CLEANUP:
					ssh->status = FAILED;
					break;
				case WORKING:
					ssh->status = FINISHED;
					break;
				case FAILED:
					break;
			}
			ssh->ccmd = -1;
			ssh->start_time =  0;
		}
		else Check();
	}
	catch(Error& e) {
		Cleanup(e);
	}
	return IsWorking();
}

bool Ssh::Do()
{
	ASSERT(!IsBlocking());
	return Do0();
}

dword Ssh::GetWaitEvents()
{
	ssh->events = 0;
	if(ssh->socket && ssh->session)
		ssh->events = libssh2_session_block_directions(ssh->session);
	return !!(ssh->events & LIBSSH2_SESSION_BLOCK_INBOUND) * WAIT_READ +
	       !!(ssh->events & LIBSSH2_SESSION_BLOCK_OUTBOUND) * WAIT_WRITE;
}

bool Ssh::Cleanup(Error& e)
{
	ssh->queue.Clear();
	ssh->start_time = 0;
	auto b = ssh->status == CLEANUP;
	ssh->status = FAILED;
	if(b)
		return false;
	ssh->error  = MakeTuple<int, String>(e.code, e);
	LLOG("Failed." << " Code = " << e.code << ", " << e);
	return !b && e.code != -1000; // Make sure we don't loop on timeout errors.
}

void Ssh::Wait()
{
	while(!IsTimeout()) {
		ssh->wait();
		if(!IsBlocking() || !ssh->socket)
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
		rc = libssh2_session_last_error(ssh->session, libmsg, NULL, 0);
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
    ssh->session        = NULL;
    ssh->socket         = NULL;
    ssh->init           = false;
    ssh->noloop			= false;
    ssh->timeout        = Null;
    ssh->start_time     = 0;
    ssh->waitstep       = 10;
    ssh->chunk_size     = CHUNKSIZE;
    ssh->status         = FINISHED;
    ssh->ccmd           = -1;
    ssh->oid            = GetNewId();
    ssh->otype          = 0;
    ssh->events         = WAIT_READ | WAIT_WRITE;
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