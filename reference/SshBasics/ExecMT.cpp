#include "SshBasics.h"

// ExecAsyncListDir: Demonstrates remote command execution in worker threads.

AsyncWork<void> AsyncListDir(SshSession& session, const String& path)
{
	auto worker = Upp::Async([=, &session] {
		SshExec exec(session);
		String cout, cerr;
		int exit_code = exec("ls -l " + path, cout, cerr);
		if(exec.IsError())
			throw Ssh::Error(Format("Worker #%d: %s", exec.GetId(), exec.GetErrorDesc()));
		LOG("Directory: " << path);
		LOG("Exit code: " << exit_code);
		LOG("Stdout:\n" << cout);
		LOG("Stderr:\n" << cerr);
	});
	return pick(worker);
}

void GetResult(AsyncWork<void>& w)
{
	try {
		w.Get();
	}
	catch(const Ssh::Error& e) {
		LOG(e);
	}
}

void ExecAsyncListDir(SshSession& session)
{
	const char *path1 = "/";
	const char *path2 = "/pub/example/";
	
	auto worker1 = AsyncListDir(session, path1);
	auto worker2 = AsyncListDir(session, path2);
	
	GetResult(worker2);
	GetResult(worker1);
}