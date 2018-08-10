#include "SSH.h"

namespace Upp {
/*
#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

int SshExec::Execute(const String& cmd, Stream& out, Stream& err)
{
	ComplexCmd(CHANNEL_EXEC, [=, &out, &err]() mutable {
		SshChannel::Open();
		SshChannel::Exec(cmd);
		Cmd(CHANNEL_SHELL, [=] { Unlock(); return true; });
		SshChannel::Get(out, ssh->chunk_size);
		SshChannel::GetStdErr(err);
		SshChannel::SendRecvEof();
		SshChannel::Close();
		SshChannel::CloseWait();
		Cmd(CHANNEL_RC,  [=]() mutable { SshChannel::GetExitCode(); return true; });
		Cmd(CHANNEL_SIGNAL, [=]() mutable { SshChannel::GetExitSignal(); return true; });
		
	});
	return exitcode;
}

AsyncWork<Tuple<int, String, String>> SshExec::AsyncRun(SshSession& session, const String& cmd)
{
	auto work = Upp::Async([=, cmd = String(cmd), &session]{
		SshExec worker(session);
		worker.NonBlocking();
	
		bool cancelled = false;
		int  waitstep  = worker.GetWaitStep();
		
		StringStream out, err;

		worker.Execute(cmd, out, err);
		
		while(worker.Do()) {
			if(!cancelled && CoWork::IsCanceled()) {
				worker.Cancel();
				cancelled = true;
			}
			Sleep(waitstep);
		}
		if(worker.IsError())
			throw Ssh::Error(worker.GetError(), worker.GetErrorDesc());

		return MakeTuple<int, String, String>(
			worker.GetExitCode(),
			pick(out.GetResult()),
			pick(err.GetResult())
			);
	});
	return pick(work);
}*/
}