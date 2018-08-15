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
			SetError(-1);
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
			SetError(-1);
		}
		if(ch) {
			channel = MakeOne<LIBSSH2_CHANNEL*>(ch);
			LLOG("File " << path << " opened.");
		}
		return ch;
	});
}

bool Scp::Load(Stream& s, ScpAttrs a, int64 maxlen)
{
	int64 done_ = 0;
	int64 size  = a.st_size;
	String msg;

	if(size >= INT_MAX) {
		msg = "Max size exceeded.";
	}
	else
	while(done_ < size && !IsEof() && !IsError()) {
		int csz = (int) min<int64>(size - done_, ssh->chunk_size);
		Buffer<char> chunk(csz, 0);
		int n = Get(chunk, csz);
		if(n > 0) {
			done_ += n;
			s.Put(chunk, n);
			if(WhenProgress(done_, size)) {
				msg = "File transfer aborted.";
				break;
			}
		}
	}
	return Shut(msg);
}

bool Scp::Save(Stream& s)
{
	int64 done_ = 0;
	int64 size  = s.GetSize();
	String msg;
	Buffer<char> chunk(ssh->chunk_size, 0);

	while(done_ < size && !IsEof() && !IsError()) {
		int l = s.Get(chunk, min<int64>(size - done_, ssh->chunk_size));
		int n = Put(chunk, l);
		if(n > 0) {
			done_ += n;
			if(n < l)
				s.Seek(n);
			if(WhenProgress(done_, size)) {
				msg = "File transfer aborted.";;
				break;
			}
		}
	}
	return Shut(msg);
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