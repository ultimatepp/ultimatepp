#include "SshBasics.h"

// SFtpAsyncGet2: Demonstrates multiple file downloads, using a parallelization loop.


void SFtpAsyncGet2(SshSession& session)
{
	const int MAXDOWNLOADS = 4;
	const char *path = "/pub/example/";

	SFtp::DirList ls;
	{
		// Get a remote dir listing.
		SFtp browser(session);
		if(!browser.ListDir(path, ls)) {
			RLOG(browser.GetErrorDesc());
			return;
		}
	}

	// Filter the dir list.
	auto files =  FilterRange(ls, [](const SFtp::DirEntry& e) { return e.IsFile() && e.GetSize() <= 65536; });

	// Loop over.
	CoFor(min(files.GetCount(), MAXDOWNLOADS), [&files, &path, &session](int i){
		const SFtp::DirEntry& e = files[i];
		String fpath = AppendFileName(path, e.GetName());
		RLOG("Downloading " << fpath);
		SFtp sftp(session);
		String file = sftp.LoadFile(fpath);
		if(sftp.IsError())
			RLOG(Format("Worker #%d: %s", sftp.GetId(), sftp.GetErrorDesc()));
		else
			RLOG("File " << e.GetName() << " is successfully downloaded.");
	});
}
