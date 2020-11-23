#include "SSH.h"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool Scp::OpenRead(const String& path, ScpAttrs& attrs)
{
	Zero(attrs);
	return Run([=, &attrs]() mutable {
		LIBSSH2_CHANNEL *ch = libssh2_scp_recv2(ssh->session, path, &attrs);
		if(!ch && !WouldBlock()) {
			LLOG("Unable to open file " << path);
			ThrowError(-1);
		}
		if(ch) {
			channel = MakeOne<LIBSSH2_CHANNEL*>(ch);
			LLOG("File " << path << " opened.");
		}
		return ch;
	});
}

bool Scp::OpenWrite(const String& path, int64 size, long mode)
{
	return Run([=]() mutable {
		LIBSSH2_CHANNEL *ch = libssh2_scp_send64(ssh->session, path, mode, size, 0, 0);
		if(!ch && !WouldBlock()) {
			LLOG("Unable to open file " << path);
			ThrowError(-1);
		}
		if(ch) {
			channel = MakeOne<LIBSSH2_CHANNEL*>(ch);
			LLOG("File " << path << " opened.");
		}
		return ch;
	});
}

bool Scp::Load(Stream& s, ScpAttrs a, int64 maxsize)
{
	bool nowait = false;
	int64 done_ = 0;
	int64 size  = a.st_size;
	String msg;
	
	if(size < 0 || size >= maxsize) {
		msg = "Invald stream size.";
	}
	else {
		WhenProgress(0, size);
		Buffer<byte> chunk(ssh->chunk_size);
		while(done_ < size && !IsEof() && !IsError()) {
			int n = Get(chunk, (int) min<int64>(size - done_, ssh->chunk_size));
			if(n > 0) {
				done_ += n;
				s.Put(chunk, n);
				if((nowait = WhenProgress(done_, size))) {
					msg = "File transfer is aborted.";
					break;
				}
			}
		}
	}
	return Shut(msg, nowait);
}

bool Scp::Save(Stream& s)
{
	bool nowait = false;
	int64 done_ = 0;
	int64 size  = s.GetSize();
	String msg;
	
	WhenProgress(0, size);
	Buffer<byte> chunk(ssh->chunk_size);
	while(done_ < size && !IsEof() && !IsError()) {
		int l = s.Get(chunk, (int) min<int64>(size - done_, ssh->chunk_size));
		int n = Put(chunk, l);
		if(n > 0) {
			done_ += n;
			if(n < l)
				s.Seek(n);
			if((nowait = WhenProgress(done_, size))) {
				msg = "File transfer is aborted.";;
				break;
			}
		}
	}
	return Shut(msg, nowait);
}

bool Scp::SaveFile(const char *path, const String& data)
{
	StringStream s(data);
	return OpenWrite(path, s.GetSize()) && Save(s);
}

String Scp::LoadFile(const char *path)
{
	StringStream s;
	ScpAttrs attrs;
	if(OpenRead(path, attrs))
		Load(s, attrs, INT_MAX);
	return pick(s.GetResult());
}

bool Scp::SaveFile(const char *path, Stream& in)
{
	 return OpenWrite(path, in.GetSize()) && Save(in);
}

bool Scp::LoadFile(Stream& out, const char *path)
{
	ScpAttrs attrs;
	return OpenRead(path, attrs) && Load(out, attrs);
}
}