#include "SshBasics.h"

// SFtpAsyncGet: DEmonstrates multiple file downloads, using worker threads.

AsyncWork<void> AsyncGet(SshSession& session, const String& path)
{
	auto worker = Upp::Async([=, &session] {
		LOG("Downloading " << path);
		SFtp sftp(session);
		String file = sftp.LoadFile(path);
		if(sftp.IsError())
			throw Ssh::Error(Format("Worker #%d: %s", sftp.GetId(), sftp.GetErrorDesc()));
		LOG("File " << GetFileName(path) << " is successfully downloaded.");
	});
	return pick(worker);
}

void CheckError(AsyncWork<void>& w)
{
	try {
		w.Get();
	}
	catch(const Ssh::Error& e) {
		LOG(e);
	}
}

void SFtpAsyncGet(SshSession& session)
{
	const int MAXDOWNLOADS = 4;
	const char *path = "/pub/example/";
	
	SFtp browser(session);
	SFtp::DirList ls;
	
	if(!browser.ListDir(path, ls)) { // Get a dir listing to extract file names on-the-fly.
		LOG(browser.GetErrorDesc());
		return;
	}

	Array<AsyncWork<void>> workers;
	
	for(const auto& e : ls) {
		if(!e.IsFile() || (e.GetSize() > 65535))
			continue;
		if(workers.GetCount() == MAXDOWNLOADS)
			break;
		workers.Add(AsyncGet(session, AppendFileName(path, e.GetName())));
	}
	
	while(!workers.IsEmpty()) {
		for(int i = 0; i < workers.GetCount(); i++) {
			auto& worker = workers[i];
			if(worker.IsFinished()) {
				CheckError(worker);
				workers.Remove(i);
				break;
			}
			Sleep(1);
		}
	}
}
