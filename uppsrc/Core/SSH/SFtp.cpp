#include "SSH.h"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)


bool SFtp::Cleanup(Error& e)
{
	if(!Ssh::Cleanup(e) || !IsComplexCmd())
		return false;
	ssh->ccmd = -1;
	if(sftp->handle) {
		LLOG("Cleaning up...");
		ssh->status = CLEANUP;
		Close(NULL);
	}
	return true;
}

bool SFtp::Init()
{
	if(!ssh->session)
		SetError(-1, "SSH session is invalid.");
	sftp->session = libssh2_sftp_init(ssh->session);
	if(sftp->session) {
		LLOG("Session successfully initialized.");
		return true;
	}
	if(!WouldBlock())
		SetError(-1);
	return false;
}

void SFtp::Exit()
{
	Cmd(SFTP_EXIT, [=]() mutable {
		if(!sftp->session)
			return true;
		if(WouldBlock(libssh2_sftp_shutdown(sftp->session)))
			return false;
		sftp->session = NULL;
		sftp->handle = NULL;
		ssh->init = false;
		LLOG("Session deinitalized.");
		return true;
	});
}

int SFtp::FStat(SFtpHandle handle, SFtpAttrs& a, bool set)
{
	int rc = libssh2_sftp_fstat_ex(HANDLE(handle), &a, set);
	return WouldBlock(rc) ?  1 : rc;
}

int SFtp::LStat(const String& path, SFtpAttrs& a, int type)
{
	int rc = libssh2_sftp_stat_ex(sftp->session, ~path, path.GetLength(), type, &a);
	return WouldBlock(rc) ?  1 : rc;
}

SFtpHandle SFtp::Open(const String& path, dword flags, long mode)
{
	auto b = Cmd(SFTP_OPEN, [=]() mutable {
		ASSERT(sftp->session);
		sftp->handle = libssh2_sftp_open(sftp->session, path, flags, mode);
		if(!sftp->handle && !WouldBlock())
			SetError(-1);
		if(sftp->handle)
			LLOG(Format("File '%s' is successfully opened.", path));
		return sftp->handle != NULL;
	});
	return b ? sftp->handle : NULL;
}

bool SFtp::Close(SFtpHandle handle)
{
	return Cmd(SFTP_CLOSE, [=]() mutable {
		int rc = libssh2_sftp_close_handle(HANDLE(handle));
		if(!WouldBlock(rc) && rc != 0)
			SetError(-1, "Unable to close handle.");
		if(rc == 0) {
			LLOG("File handle freed.");
			sftp->handle = NULL;
		}
		return rc == 0;
	});
}

bool SFtp::Rename(const String& oldpath, const String& newpath)
{
	return Cmd(SFTP_RENAME, [=]() mutable {
		ASSERT(sftp->session);
		int rc = libssh2_sftp_rename(sftp->session, oldpath, newpath);
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(rc == 0)
			LLOG(Format("'%s' is successfully renamed to '%s'", oldpath, newpath));
		return rc == 0;
	});
}

bool SFtp::Delete(const String& path)
{
	return Cmd(SFTP_DELETE, [=]() mutable {
		ASSERT(sftp->session);
		int rc = libssh2_sftp_unlink(sftp->session, path);
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(rc == 0)
			LLOG(Format("File '%s' is successfully deleted", path));
		return rc == 0;
	});
}

bool SFtp::Sync(SFtpHandle handle)
{
	return Cmd(SFTP_SYNC, [=]() mutable {
		int rc = libssh2_sftp_fsync(HANDLE(handle));
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(rc == 0)
			LLOG("File successfully synchronized to disk.");
		return rc == 0;
	});
}

SFtp& SFtp::Seek(SFtpHandle handle, int64 position)
{
	Cmd(SFTP_SEEK, [=]() mutable {
		LLOG("Seeking to offset " << position);
		libssh2_sftp_seek64(HANDLE(handle), position);
		return true;
	});
	return *this;
}

int64 SFtp::GetPos(SFtpHandle handle)
{
	Cmd(SFTP_TELL, [=]() mutable {
		sftp->value = (int64) libssh2_sftp_tell64(HANDLE(handle));
		LLOG("File position: " << sftp->value);
		return true;
	});
	return !IsBlocking() ? Null : pick(sftp->value);
}

bool SFtp::DataRead(SFtpHandle handle, int64 size, Event<const void*, int>&& fn, bool str)
{
	if(OpCode() == SFTP_START) {
		if(FStat(HANDLE(handle), *sftp->finfo, false) <= 0) OpCode() = SFTP_GET;
		else return false;
	}

	auto peek = size > 0;
	auto csz = peek ? min(size, int64(ssh->chunk_size)) : ssh->chunk_size;
	int  rc = 0;
	do {
		Buffer<char> buffer(csz);
		rc = libssh2_sftp_read(HANDLE(handle), buffer, csz);
		if(rc < 0) {
			if(!WouldBlock(rc))
				SetError(rc);
			return false;
		}
		if(rc > 0) {
			const auto& fn_ = WhenContent ? WhenContent : fn;
			if(peek && sftp->done < size)
				fn_(buffer, sftp->done + rc > size ? size : rc);
			else
			if(!peek)
				fn_(buffer, rc);
			sftp->done += rc;
			if(peek && sftp->done == size)
				break;
			if(WhenProgress(sftp->done, sftp->finfo.GetSize()))
				SetError(-1, "Read aborted.");
			ssh->start_time = msecs();
		}
	}
	while(rc > 0);
	LLOG(Format("%d of %d bytes successfully read.", sftp->done, peek ? size : sftp->finfo.GetSize()));
	if(peek || str)
		sftp->value = WhenContent ? Null : pick(sftp->stream.GetResult());
	return true;
}

bool SFtp::DataWrite(SFtpHandle handle, Stream& in, int64 size)
{
	auto poke = size > 0;
	ssize_t write_size = poke ? size : in.GetSize();
	int rc = 0;
	do {
		auto s = in.Get(ssh->chunk_size);
		rc = libssh2_sftp_write(HANDLE(handle), s.Begin(), s.GetLength());
		if(rc < 0 && !WouldBlock(rc))
			SetError(rc);
		if(rc != 0) {
			if(rc < s.GetLength())
				in.SeekCur(-(s.GetLength() - (rc > 0 ? rc : 0)));
			if(rc < 0)
				return false;
			sftp->done += rc;
			if(WhenProgress(sftp->done, write_size))
				SetError(-1, "File write aborted");
			ssh->start_time = msecs();
		}
	}
	while(sftp->done < write_size);
	LLOG(Format("%ld of %ld bytes successfully written.", (int64) sftp->done, (int64) write_size));
	return true;
}

int SFtp::Get(SFtpHandle handle, void *ptr_, int size)
{
	int done = 0;
	char *ptr = (char *)ptr_;
	return Cmd(SFTP_START, [=, &done]() mutable {
		if(OpCode() == SFTP_START) {
			if(FStat(HANDLE(handle), *sftp->finfo, false) <= 0) OpCode() = SFTP_GET;
			else return false;
		}
		while(done < size) {
			int sz = min(ssh->chunk_size, size - done);
			int rc = libssh2_sftp_read(handle, ptr, sz);
			if(WouldBlock(rc))
				return false;
			if(rc < 0)
				SetError(rc);
			done += rc;
			if(rc != sz)
				break;
			if(WhenProgress(done, size))
				SetError(-1, "Read aborted.");
			ssh->start_time = msecs();
		}
		LLOG(Format("%d of %d bytes successfully read.", done, size));
		return true;
	}) ? done : -1;
}

bool SFtp::Put(SFtpHandle handle, const void *ptr_, int size)
{
	char *ptr = (char *)ptr_;
	return Cmd(SFTP_START, [=]() mutable {
		int rc = 0;
		int done = 0;
		while(done < size) {
			int sz = min(ssh->chunk_size, size - done);
			int rc = libssh2_sftp_write(handle, ptr, sz);
			if(WouldBlock(rc))
				return false;
			if(rc != sz)
				if(rc >= 0)
					SetError(-1, "Out of space");
				else
					SetError(rc);
			ptr += sz;
			done += sz;
			if(WhenProgress(done, size))
				SetError(-1, "File write aborted");
			ssh->start_time = msecs();
		}
		LLOG(Format("%d of %d bytes successfully written.", done, size));
		return true;
	});
}

bool SFtp::SaveFile(const char *path, const String& data)
{
	SFtpFileOut out(*this, path);
	return SaveStream(out, data);
}

String SFtp::LoadFile(const char *path)
{
	SFtpFileIn in(*this, path);
	return LoadStream(in);
}

bool SFtp::SaveFile(const char *path, Stream& in)
{
	SFtpFileOut out(*this, path);
	return CopyStream(out, in) >= 0;
}

void SFtp::LoadFile(Stream& out, const char *path)
{
	SFtpFileIn in(*this, path);
	CopyStream(in, out) >= 0;
}

bool SFtp::Get(SFtpHandle handle, Stream& out)
{
	sftp->done  = 0;
	sftp->finfo = Null;
	return Cmd(SFTP_START, [=, &out]() mutable {
		return DataRead(HANDLE(handle), 0, [&out](const void* b, int l) { out.Put(b, l); });
	});
}

String SFtp::Get(SFtpHandle handle)
{
	sftp->done  = 0;
	sftp->finfo = Null;
	sftp->stream.Create();
	Cmd(SFTP_START, [=]() mutable {
		return DataRead(HANDLE(handle), 0, [=](const void* b, int l) { sftp->stream.Put(b, l); }, true);
	});
	return !IsBlocking() ? Null : pick(sftp->value);
}

bool SFtp::Get(const String& path, Stream& out)
{
	return ComplexCmd(SFTP_GET, [=, &out]() mutable {
		OpenRead(path);
		Get(NULL, out);
		Close(NULL);
	});
}

bool SFtp::Get(const String& path, Stream& out, int64 offset)
{
	return ComplexCmd(SFTP_GET, [=, &out]() mutable {
		OpenRead(path);
		Seek(NULL, offset);
		Get(NULL, out);
		Close(NULL);
	});
}

String SFtp::Get(const String& path)
{
	ComplexCmd(SFTP_GET, [=]() mutable {
		OpenRead(path);
		Get(NULL);
		Close(NULL);
	});
	return !IsBlocking() ? Null : pick(sftp->value);
}

bool SFtp::Put(SFtpHandle handle, Stream& in)
{
	sftp->done = 0;
	return Cmd(SFTP_PUT, [=, &in]() mutable {
		return DataWrite(HANDLE(handle), in, 0);
	});
}

bool SFtp::Put(Stream& in, const String& path, dword flags, long mode)
{
	return ComplexCmd(SFTP_PUT, [=, &in]() mutable {
		Open(path, flags, mode);
		Put(NULL, in);
		Close(NULL);
	});
}

bool SFtp::Put(Stream& in, const String& path)
{
	return Put(in, path, CREATE | WRITE | TRUNCATE,	IWUSR | IRALL);
}

bool SFtp::Put(Stream& in, const String& path, int64 offset)
{
	return ComplexCmd(SFTP_PUT, [=, &in] {
		OpenWrite(path);
		Seek(NULL, offset);
		Put(NULL, in);
		Close(NULL);
	});
}

bool SFtp::Append(Stream& in, const String& path, long mode)
{
	return Put(in, path, WRITE | CREATE | APPEND, mode);
}

bool SFtp::Append(Stream& in, const String& path)
{
	return Put(in, path, WRITE | CREATE | APPEND, IRALL | IWUSR);
}

String SFtp::Peek(const String& path, int64 offset, int64 length)
{
	ComplexCmd(SFTP_PEEK, [=]() mutable {
		sftp->done = 0;
		sftp->stream.Create();
		OpenRead(path);
		Seek(NULL, offset);
		Cmd(SFTP_START, [=]{
			return DataRead(NULL, length, [=](const void* b, int l) { sftp->stream.Put(b, l); }, true);
		});
		Close(NULL);
	});
	return !IsBlocking() ? Null : pick(sftp->value);
}

bool SFtp::Poke(const String& data, const String& path, int64 offset, int64 length)
{
	return ComplexCmd(SFTP_POKE, [=]() mutable {
		sftp->done = 0;
		sftp->stream.Open(data);
		OpenWrite(path);
		Seek(NULL, offset);
		Cmd(SFTP_PUT, [=]{ return DataWrite(HANDLE(NULL), sftp->stream, length); });
		Close(NULL);
	});
}

SFtpHandle	 SFtp::OpenDir(const String& path)
{
	auto b = Cmd(SFTP_OPENDIR, [=]() mutable {
		ASSERT(sftp->session);
		sftp->handle = libssh2_sftp_opendir(sftp->session, path);
		if(!sftp->handle && !WouldBlock())
			SetError(-1);
		if(sftp->handle)
			LLOG(Format("Directory '%s' is successfully opened.", path));
		return sftp->handle != NULL;
	});
	return b ? sftp->handle : NULL;
}

bool SFtp::MakeDir(const String& path, long mode)
{
	return Cmd(SFTP_MAKEDIR, [=]() mutable {
		ASSERT(sftp->session);
		int rc = libssh2_sftp_mkdir(sftp->session, path, mode);
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(rc == 0)
			LLOG(Format("Directory '%s' is succesfully created.", path));
		return rc == 0;
	});
}

bool SFtp::RemoveDir(const String& path)
{
	return Cmd(SFTP_REMOVEDIR, [=]() mutable {
		ASSERT(sftp->session);
		int rc = libssh2_sftp_rmdir(sftp->session, path);
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(rc == 0)
			LLOG(Format("Directory '%s' is succesfully deleted.", path));
		return rc == 0;
	});
}

bool SFtp::ListDir(SFtpHandle handle, DirList& list)
{
	return Cmd(SFTP_LISTDIR, [=, &list]() mutable {
		char label[512];
		char longentry[512];
		SFtpAttrs attrs;
		int rc = 0;
		do {
			rc = libssh2_sftp_readdir_ex(
					HANDLE(handle),
					label, sizeof(label),
					longentry, sizeof(longentry),
					&attrs
			);
			if(rc < 0) {
				if(!WouldBlock(rc))
					SetError(rc);
				return false;
			}
			if(rc > 0) {
				DirEntry& entry	= list.Add();
				entry.filename	= label;
				*entry.a		= attrs;
				entry.valid		= true;
//				DUMP(entry)
			}
		}
		while(rc > 0);
		LLOG(Format("Directory listing is successful. (%d entries)", list.GetCount()));
		return true;
	});
}

bool SFtp::ListDir(const String& path, DirList& list)
{
	return ComplexCmd(SFTP_LISTDIR, [=, &list]() mutable {
		OpenDir(path);
		ListDir(NULL, list);
		Close(NULL);
	});
}

String SFtp::GetWorkDir()
{
	ComplexCmd(SFTP_LINK, [=]() mutable {
		SymLink(".", NULL, LIBSSH2_SFTP_REALPATH);
	});
	return !IsBlocking() ? Null : pick(sftp->value);
}

bool SFtp::SymLink(const String& path, String* target, int type)
{
	if(type == LIBSSH2_SFTP_SYMLINK)
		return Cmd(SFTP_LINK, [=]() mutable {
			ASSERT(sftp->session);
			ASSERT(target);
			int rc = libssh2_sftp_symlink_ex(
						sftp->session,
						path,
						path.GetLength(),
						const_cast<char*>(target->Begin()),
						target->GetLength(),
						type
					);
			if(!WouldBlock(rc) && rc != 0)
				SetError(rc);
			if(rc == 0)
				LLOG(Format("Symbolic link '%s' for path '%s' is successfult created.", target, path));
			return rc == 0;
		});
	else
		return Cmd(SFTP_LINK, [=]() mutable {
			ASSERT(sftp->session);
			Buffer<char> buf(512, 0);
			int rc = libssh2_sftp_symlink_ex(
						sftp->session,
						path,
						path.GetLength(),
						buf,
						512,
						type
					);
			if(!WouldBlock(rc) && rc <= 0)
				SetError(rc);
			if(rc > 0) {
				LLOG("Symbolic link operation is successful.");
				if(target)
					target->Set(buf, rc);
				else
					sftp->value = pick(String(buf, rc));
			}
			return rc > 0;
		});
}

bool SFtp::GetAttrs(SFtpHandle handle, SFtpAttrs& attrs)
{
	return Cmd(SFTP_GET_STAT, [=, &attrs]() mutable {
		int rc = FStat(handle, attrs, false);
		if(rc < 0)	SetError(rc);
		if(rc == 0)	LLOG("File attributes successfully retrieved.");
		return rc == 0;
	});
}

bool SFtp::GetAttrs(const String& path, SFtpAttrs& attrs)
{
	return Cmd(SFTP_GET_STAT, [=, &attrs]() mutable {
		int rc = LStat(path, attrs, LIBSSH2_SFTP_STAT);
		if(rc < 0)	SetError(rc);
		if(rc == 0) LLOG(Format("File attributes of '%s' is successfully retrieved.", path));
		return rc == 0;
	});
}

bool SFtp::SetAttrs(SFtpHandle handle, const SFtpAttrs& attrs)
{
	return Cmd(SFTP_SET_STAT, [=, &attrs]() mutable {
		int rc = FStat(handle, const_cast<SFtpAttrs&>(attrs), true);
		if(rc < 0)	SetError(rc);
		if(rc == 0)	LLOG("File attributes successfully modified.");
		return rc == 0;
	});
}

bool SFtp::SetAttrs(const String& path, const SFtpAttrs& attrs)
{
	return Cmd(SFTP_SET_STAT, [=, &attrs]() mutable {
		int rc = LStat(path,  const_cast<SFtpAttrs&>(attrs), LIBSSH2_SFTP_SETSTAT);
		if(rc < 0)	SetError(rc);
		if(rc == 0)	LLOG(Format("File attributes of '%s' is successfully modified.", path));
		return rc == 0;
	});
}

bool SFtp::QueryAttr(const String& path, int attr)
{
	return ComplexCmd(SFTP_QUERY, [=]() mutable {
		sftp->finfo = Null;
		sftp->value = Null;
		GetAttrs(path, *sftp->finfo);
		Cmd(SFTP_QUERY, [=] {
			sftp->finfo.filename = path;
			switch(attr) {
				case SFTP_ATTR_FILE:
					sftp->value = sftp->finfo.IsFile();
					break;
				case SFTP_ATTR_DIRECTORY:
					sftp->value = sftp->finfo.IsDirectory();
					break;
				case SFTP_ATTR_SOCKET:
					sftp->value = sftp->finfo.IsSocket();
					break;
				case SFTP_ATTR_SYMLINK:
					sftp->value = sftp->finfo.IsSymLink();
					break;
				case SFTP_ATTR_PIPE:
					sftp->value = sftp->finfo.IsPipe();
					break;
				case SFTP_ATTR_BLOCK:
					sftp->value = sftp->finfo.IsBlock();
					break;
				case SFTP_ATTR_SPECIAL:
					sftp->value = sftp->finfo.IsSpecial();
					break;
				case SFTP_ATTR_SIZE:
					sftp->value = sftp->finfo.GetSize();
					break;
				case SFTP_ATTR_LAST_MODIFIED:
					sftp->value = sftp->finfo.GetLastModified();
					break;
				case SFTP_ATTR_LAST_ACCESSED:
					sftp->value = sftp->finfo.GetLastAccessed();
					break;
				case SFTP_ATTR_INFO:
					sftp->finfo.valid = true;
					sftp->value = RawPickToValue(pick(sftp->finfo));
					sftp->finfo = Null;
					break;
				default:
					break;
			}
			return true;
		});
	});
}

bool SFtp::ModifyAttr(const String& path, int attr, const Value& v)
{
	return ComplexCmd(SFTP_MODIFY, [=]() mutable {
		sftp->finfo = Null;
		GetAttrs(path, *sftp->finfo);
		Cmd(SFTP_MODIFY, [=]{
			auto& attrs = *sftp->finfo;
			switch(attr) {
				case SFTP_ATTR_SIZE:
					attrs.flags |= LIBSSH2_SFTP_ATTR_SIZE;
					attrs.filesize = v.To<int64>();
					break;
				case SFTP_ATTR_LAST_MODIFIED:
					attrs.flags |= LIBSSH2_SFTP_ATTR_ACMODTIME;
					attrs.mtime = GetUTCSeconds(v);
					break;
				case SFTP_ATTR_LAST_ACCESSED:
					attrs.flags |= LIBSSH2_SFTP_ATTR_ACMODTIME;
					attrs.atime = GetUTCSeconds(v);
					break;
				default:
					break;
			}
			return true;
		});
		SetAttrs(path, ~sftp->finfo);
	});
}

SFtp::DirEntry SFtp::GetInfo(const String& path)
{
	QueryAttr(path, SFTP_ATTR_INFO);
	if(IsBlocking()&& !IsError()) {
		auto& e = const_cast<DirEntry&>(sftp->value.To<DirEntry>());
		return pick(e);
	}
	return Null;
}

void SFtp::StartAsync(int cmd, SshSession& session, const String& path, Stream& io,
						Gate<int64, int64, int64> progress, Event<int64, const void*, int> consumer)
{
	SFtp worker(session);
	worker.NonBlocking();
	
	auto wid = worker.GetId();
	
	if(consumer)
		worker.WhenContent = [=, &consumer](const void* b, int l) {
			consumer(wid, b, l);
		};
	if(progress)
		worker.WhenProgress = [=, &progress](int64 d, int64 t) {
			return progress(wid, d, t);
		};

	switch(cmd) {
	case SFtp::SFTP_GET:
		worker.Get(path, io);
		break;
	case SFtp::SFTP_PUT:
		worker.Put(io, path);
		break;
	case SFtp::SFTP_APPEND:
		worker.Append(io, path);
		break;
	default:
		NEVER();
	}

	bool cancelled = false;
	int  waitstep  = worker.GetWaitStep();

	while(worker.Do()) {
		if(!cancelled && CoWork::IsCanceled()) {
			worker.Cancel();
			cancelled = true;
		}
		Sleep(waitstep);
	}
	if(worker.IsError())
		throw Ssh::Error(worker.GetError(), worker.GetErrorDesc());
}

AsyncWork<String> SFtp::AsyncGet(SshSession& session, const String& path, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, progress = pick(progress)]{
		StringStream data;
		SFtp::StartAsync(SFtp::SFTP_GET, session, path, data, progress);
		return pick(data.GetResult());
	});
}

AsyncWork<void> SFtp::AsyncGet(SshSession& session, const String& path, Stream& out, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, &out, progress = pick(progress)]{
		SFtp::StartAsync(SFtp::SFTP_GET, session, path, out, progress);
	});
}

AsyncWork<void> SFtp::AsyncPut(SshSession& session, String& in, const String& path, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, &in, progress = pick(progress)]{
		StringStream ss(in);
		SFtp::StartAsync(SFtp::SFTP_PUT, session, path, ss, progress);
	});
}

AsyncWork<void> SFtp::AsyncPut(SshSession& session, Stream& in, const String& path, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, &in, progress = pick(progress)]{
		SFtp::StartAsync(SFtp::SFTP_PUT, session, path, in, progress);
	});
}

AsyncWork<void> SFtp::AsyncAppend(SshSession& session, String& in, const String& path, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, &in, progress = pick(progress)]{
		StringStream ss(in);
		SFtp::StartAsync(SFtp::SFTP_APPEND, session, path, ss, progress);
	});
}

AsyncWork<void> SFtp::AsyncAppend(SshSession& session, Stream& in, const String& path, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, &in, progress = pick(progress)]{
		SFtp::StartAsync(SFtp::SFTP_APPEND, session, path, in, progress);
	});
}

AsyncWork<void> SFtp::AsyncGetToFile(SshSession& session, const String& src, const String& dest, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, progress = pick(progress)]{
		FileOut fo(dest);
		if(!fo)
			throw Ssh::Error(Format("Unable to open file '%s' for writing.", dest));
		SFtp::StartAsync(SFtp::SFTP_GET, session, src, fo, progress);
	});
}

AsyncWork<void> SFtp::AsyncPutFromFile(SshSession& session, const String& src, const String& dest, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, progress = pick(progress)]{
		FileIn fi(src);
		if(!fi)
			throw Ssh::Error(Format("Unable to open file '%s' for reading.", src));
		SFtp::StartAsync(SFtp::SFTP_PUT, session, dest, fi, progress);
	});
}

AsyncWork<void> SFtp::AsyncAppendFromFile(SshSession& session, const String& src, const String& dest, Gate<int64, int64, int64> progress)
{
	return Async([=, &session, progress = pick(progress)]{
		FileIn fi(src);
		if(!fi)
			throw Ssh::Error(Format("Unable to open file '%s' to append.", src));
		SFtp::StartAsync(SFtp::SFTP_APPEND, session, dest, fi, progress);
	});
}

AsyncWork<void> SFtp::AsyncConsumerGet(SshSession& session, const String& path, Event<int64, const void*, int> consumer)
{
	return Async([=, &session, consumer = pick(consumer)]{
		SFtp::StartAsync(SFtp::SFTP_GET, session, path, NilStream(), Null, consumer);
	});
}

SFtp::SFtp(SshSession& session)
: Ssh()
{
	sftp.Create();
	ssh->otype		= SFTP;
	sftp->session	= NULL;
	sftp->handle	= NULL;
	sftp->value		= Null;
	sftp->done		= 0;
	ssh->session	= session.GetHandle();
	ssh->socket		= &session.GetSocket();
	ssh->timeout	= session.GetTimeout();
	ssh->waitstep   = session.GetWaitStep();
	ssh->wait       = Proxy(session.WhenWait);
}

SFtp::~SFtp()
{
	if(sftp && sftp->session) { // Picked?
		Ssh::Exit();
		Exit();
	}
}

String SFtp::DirEntry::ToString() const
{
	if(!valid) return "<N/A>";
	static const char *hypen = "-", *r = "r", *w = "w", *x = "x";
	return Format("%c%c%c%c%c%c%c%c%c%c %5<d %5<d %12>d %s %s %s",
				(IsFile()
				? *hypen : (IsDirectory()
				? 'd' : (IsSymLink()
				? 'l' : (IsSocket()
				? 's' : (IsPipe()
				? 'p' : (IsBlock()
				? 'b' : (IsSpecial()
				? 'c' : 'o' ))))))),
				((a->permissions & IRUSR) ? *r : *hypen),
				((a->permissions & IWUSR) ? *w : *hypen),
				((a->permissions & IXUSR) ? *x : *hypen),
				((a->permissions & IRGRP) ? *r : *hypen),
				((a->permissions & IWGRP) ? *w : *hypen),
				((a->permissions & IXGRP) ? *x : *hypen),
				((a->permissions & IROTH) ? *r : *hypen),
				((a->permissions & IWOTH) ? *w : *hypen),
				((a->permissions & IXOTH) ? *x : *hypen),
				GetUid(),
				GetGid(),
				GetSize(),
				AsString(GetLastModified()),
				AsString(GetLastAccessed()),
				GetName());
}

String SFtp::DirEntry::ToXml() const
{
	if(!valid) return XmlTag("<N/A>").Text("<N/A>");
	static const char *hypen = "-", *r = "r", *w = "w", *x = "x";
	return XmlTag("sftp:direntry")
			("type", (IsFile()
				? "file" : (IsDirectory()
				? "directory" : (IsSymLink()
				? "symlink" : (IsSocket()
				? "socket" : (IsPipe()
				? "pipe" : (IsBlock()
				? "block-special" : (IsSpecial()
				? "character-special" : "other")
			)))))))
			("uid", AsString(GetUid()))
			("gid", AsString(GetGid()))
			("size", AsString(GetSize()))
			("modified", AsString(GetLastModified()))
			("accessed", AsString(GetLastAccessed()))
			("permissions", Format("%c%c%c%c%c%c%c%c%c",
				((a->permissions & IRUSR) ? *r : *hypen),
				((a->permissions & IWUSR) ? *w : *hypen),
				((a->permissions & IXUSR) ? *x : *hypen),
				((a->permissions & IRGRP) ? *r : *hypen),
				((a->permissions & IWGRP) ? *w : *hypen),
				((a->permissions & IXGRP) ? *x : *hypen),
				((a->permissions & IROTH) ? *r : *hypen),
				((a->permissions & IWOTH) ? *w : *hypen),
				((a->permissions & IXOTH) ? *x : *hypen)
			))
			.Text(GetName());
}

bool SFtp::DirEntry::CanMode(dword u, dword g, dword o) const
{
	return a->flags & LIBSSH2_SFTP_ATTR_PERMISSIONS &&
		   a->permissions & o ||
		   a->permissions & g ||
		   a->permissions & u;
}

void SFtp::DirEntry::Zero()
{
	a.Create();
	Upp::Zero(*a);
	valid = false;
}

SFtp::DirEntry::DirEntry(const String& path)
: DirEntry()
{
	filename = path;
}

SFtp::DirEntry::DirEntry(const String& path, const SFtpAttrs& attrs)
: DirEntry()
{
	filename = path;
	*a = attrs;
}
}
