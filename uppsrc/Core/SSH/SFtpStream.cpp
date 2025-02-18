#include "SSH.h"

namespace Upp {

void SFtpStream::SetPos(int64 pos)
{
	sftp->Seek(handle, pos);
}

void SFtpStream::SetStreamSize(int64 size)
{
	// TODO
}

dword SFtpStream::Read(int64 at, void *ptr, dword size)
{
	SetPos(at);
	int n = sftp->Get(handle, ptr, size);
	if(sftp->IsError()) SetError();
	return dword(n);
}

void SFtpStream::Write(int64 at, const void *data, dword size)
{
	SetPos(at);
	sftp->Put(handle, data, size);
	if(sftp->IsError()) SetError();
}

void SFtpStream::Close()
{
	if(handle) {
		Flush();
		sftp->Close(handle);
		handle = nullptr;
	}
}

bool SFtpStream::IsOpen() const
{
	return handle;
}

bool SFtpStream::Open(SFtp& sftp_, const char *filename, dword mode, int acm)
{
	if(IsOpen())
		Close();
	sftp = &sftp_;
	int iomode = mode & ~SHAREMASK;
	handle = sftp->Open(filename,
						iomode == READ
							? SFtp::READ
							: iomode == CREATE
								? SFtp::READ|SFtp::WRITE|SFtp::CREATE|SFtp::TRUNCATE
								: SFtp::READ|SFtp::WRITE,
						acm
						);
				
	if(handle) {
		SFtpAttrs attrs;
		Zero(attrs);
		if(iomode != CREATE && !sftp->GetAttrs(handle, attrs)) {
			sftp->Close(handle);
			handle = nullptr;
			return false;
		}
		OpenInit(mode, attrs.filesize);
	}
	return handle;
}

SFtpStream::SFtpStream(SFtp& sftp, const char *filename, dword mode, int acm)
: SFtpStream()
{
	Open(sftp, filename, mode, acm);
}

SFtpStream::SFtpStream()
: sftp(nullptr)
, handle(nullptr)
{
}

SFtpStream::~SFtpStream()
{
	Close();
}

};