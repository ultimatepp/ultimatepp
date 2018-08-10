#include "SSH.h"

namespace Upp {
/*
#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool Scp::Open(int opcode, const String& path, int64 size, long mode)
{
	return Cmd(CHANNEL_OPEN, [=] {
		if(path.IsEmpty())
			SetError(-1, "Path is not set.");
		switch(opcode) {
			case CHANNEL_SCP_GET:
				*channel = libssh2_scp_recv2(ssh->session, path, &filestat);
				break;
			case CHANNEL_SCP_PUT:
				*channel = libssh2_scp_send64(ssh->session, path, mode, size, 0, 0);
				break;
			default:
				NEVER();
		}
		if(!*channel && !WouldBlock()) {
			LLOG("Unable to obtain a channel.");
			SetError(-1);
		}
		if(*channel) {
			LLOG("Scp channel obtained.");
			Unlock();
		}
		return *channel != NULL;
	});
}

bool Scp::Get(const String& path, Stream& out)
{
	Clear();
	return ComplexCmd(CHANNEL_SCP_GET, [=, &out]() mutable {
		Open(CHANNEL_SCP_GET, path, 0, 0);
		Cmd(CHANNEL_SCP_GET, [=, &out]{ return SshChannel::ReadStream(out, filestat.st_size); });
		SshChannel::SendRecvEof();
		SshChannel::Close();
		SshChannel::CloseWait();
	});
}

String Scp::Get(const String& path)
{
	Clear();
	ComplexCmd(CHANNEL_SCP_GET, [=]() mutable {
		Open(CHANNEL_SCP_GET, path, 0, 0);
		Cmd(CHANNEL_SCP_GET, [=]{ return SshChannel::ReadString((String&) result, filestat.st_size); });
		SshChannel::SendRecvEof();
		SshChannel::Close();
		SshChannel::CloseWait();
	});
	return !IsBlocking() ? Null : GetResult();
}

bool Scp::Put(Stream& in, const String& path, long mode)
{
	Clear();
	return ComplexCmd(CHANNEL_SCP_PUT, [=, &in]() mutable {
		Open(CHANNEL_SCP_PUT, path, in.GetSize(), mode);
		SshChannel::Put(in, in.GetSize());
		SshChannel::SendRecvEof();
		SshChannel::Close();
		SshChannel::CloseWait();
	});
}

bool Scp::Put(const String& in, const String& path, long mode)
{
	Clear();
	return ComplexCmd(CHANNEL_SCP_PUT, [=, &in]() mutable {
		Open(CHANNEL_SCP_PUT, path, in.GetLength(), mode);
		SshChannel::Put(in);
		SshChannel::SendRecvEof();
		SshChannel::Close();
		SshChannel::CloseWait();
	});
}

void Scp::StartAsync(int cmd, SshSession& session, const String& path, Stream& io, long mode,
						Gate<int64, int64, int64> progress, Event<int64, const void*, int> consumer)
{
	Scp worker(session);
	worker.NonBlocking();
	
	auto wid = worker.GetId();

	if(consumer)
		worker.WhenContent = [=, &consumer](const void* b, int l) {
			consumer(wid, b, l);
		};
	if(progress)
		worker.WhenProgress = [=, &progress](int64 d, int64 t) {
			return progress(wid, d, t);
		};
		
	switch(cmd) {
	case SshChannel::CHANNEL_SCP_GET:
		worker.Get(path, io);
		break;
	case SshChannel::CHANNEL_SCP_PUT:
		worker.Put(io, path, mode);
		break;
	default:
		NEVER();
	}
	
	bool cancelled = false;
	int  waitstep  = worker.GetWaitStep();
	
	while(worker.Do()) {
		if(!cancelled && CoWork::IsCanceled()) {
			worker.Cancel();
			cancelled = true;
		}
		Sleep(waitstep);
	}
	if(worker.IsError())
		throw Ssh::Error(worker.GetError(), worker.GetErrorDesc());
}

AsyncWork<String> Scp::AsyncGet(SshSession& session, const String& path, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, progress = pick(progress)]{
		StringStream data;
		Scp::StartAsync(SshChannel::CHANNEL_SCP_GET, session, path, data, 0, progress);
		return pick(data.GetResult());
	});
}

AsyncWork<void> Scp::AsyncGet(SshSession& session, const String& path, Stream& out, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, &out, progress = pick(progress)]{
		Scp::StartAsync(SshChannel::CHANNEL_SCP_GET, session, path, out, 0, progress);
	});
}

AsyncWork<void> Scp::AsyncPut(SshSession& session, String& in, const String& path, long mode, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, &in, progress = pick(progress)]{
		StringStream ss(in);
		Scp::StartAsync(SshChannel::CHANNEL_SCP_PUT, session, path, ss, mode, progress);
	});
}

AsyncWork<void> Scp::AsyncPut(SshSession& session, Stream& in, const String& path, long mode, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, &in, progress = pick(progress)]{
		Scp::StartAsync(SshChannel::CHANNEL_SCP_PUT, session, path, in, mode, progress);
	});
}

AsyncWork<void> Scp::AsyncGetToFile(SshSession& session, const String& src, const String& dest, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, progress = pick(progress)]{
		FileOut fo(dest);
		if(!fo)
			throw Ssh::Error(Format("Unable to open file '%s' for writing.", dest));
		Scp::StartAsync(SshChannel::CHANNEL_SCP_GET, session, src, fo, 0, progress);
	});
}

AsyncWork<void> Scp::AsyncPutToFile(SshSession& session, const String& src, const String& dest, long mode, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, progress = pick(progress)]{
		FileIn fi(src);
		if(!fi)
			throw Ssh::Error(Format("Unable to open file '%s' for reading.", src));
		Scp::StartAsync(SshChannel::CHANNEL_SCP_PUT, session, dest, fi, mode, progress);
	});
}

AsyncWork<void> Scp::AsyncConsumerGet(SshSession& session, const String& path, Event<int64, const void*, int> consumer)
{
	return Async([=, &session, consumer = pick(consumer)]{
		Scp::StartAsync(SshChannel::CHANNEL_SCP_GET, session, path, NilStream(), 0, Null, consumer);
	});
}
*/
}