#include "SshBasics.h"

// SFtpStreamGet:
// Demonstrates a basic stream operation on an sftp remote file object.

void SFtpStreamGet(SshSession& session)
{
	const char *path = "/readme.txt";
	
	SFtp sftp(session);
	SFtpFileIn fi(sftp, path);
	while(!fi.IsEof()) {
		int64  pos  = fi.GetPos();
		String line = fi.GetLine();
		if(!line.IsEmpty())
			LOG(Format("Offset: %3d, Line: [%s]", pos, line));
	}
	if(fi.IsError())
		LOG(fi.GetErrorText());

}