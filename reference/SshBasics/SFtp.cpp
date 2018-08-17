#include "SshBasics.h"

// SFtpGet:
// Demonstrates a file download, using sftp.

void SFtpGet(SshSession& session)
{
	const char *path = "/readme.txt";
	
	SFtp sftp(session);
	String file = sftp.LoadFile(path);
	LOG((!sftp.IsError() ? file : sftp.GetErrorDesc()));
}