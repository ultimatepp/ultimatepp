#include "SSH.h"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool Scp::Open(int cmd, const String& path, libssh2_struct_stat* fs,
               int64 size, dword flags, long mode)
{
	return Run([=]() mutable {
		LIBSSH2_CHANNEL *ch = nullptr;
		switch(cmd) {
			case SFtp::READ:
				ch = libssh2_scp_recv2(ssh->session, path, fs);
				break;
			case SFtp::WRITE:
				ch = libssh2_scp_send64(ssh->session, path, mode, size, 0, 0);
				break;
			default:
				NEVER();
		}
		if(!ch && !WouldBlock()) {
			LLOG("Unable to obtain a channel.");
			SetError(-1);
		}
		if(ch) {
			channel = MakeOne<LIBSSH2_CHANNEL*>(ch);
			LLOG("Channel obtained.");
		}
		return !ch;
	});
}

// TODO: These methods should better be implemented using a "ScpStream"?
bool Scp::SaveFile(const char *path, const String& data)
{
	// TODO
	return false;
}

String Scp::LoadFile(const char *path)
{
	// TODO
	return Null;
}

bool Scp::SaveFile(const char *path, Stream& in)
{
	// TODO
	return false;
}

bool Scp::LoadFile(Stream& out, const char *path)
{
	// TODO
	return false;
}
}