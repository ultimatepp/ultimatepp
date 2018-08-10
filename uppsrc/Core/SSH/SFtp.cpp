#include "SSH.h"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool SFtp::Init()
{
	if(!ssh->session)
		SetError(-1, "SSH session is invalid.");
	auto session = libssh2_sftp_init(ssh->session);
	if(!session && !WouldBlock())
		SetError(-1);
	if(session) {
		sftp_session = MakeOne<LIBSSH2_SFTP*>(session);
		LLOG("Session successfully initialized.");
	}
	return session;
}

void SFtp::Exit()
{
	if(!sftp_session)
		return;

	Run([=]() mutable {
		if(WouldBlock(libssh2_sftp_shutdown(*sftp_session)))
			return false;
		ssh->init = false;
		sftp_session.Clear();
		LLOG("Session deinitalized.");
		return true;
	});
}

int SFtp::FStat(SFtpHandle handle, SFtpAttrs& a, bool set)
{
	int rc = libssh2_sftp_fstat_ex(handle, &a, set);
	return WouldBlock(rc) ?  1 : rc;
}

int SFtp::LStat(const String& path, SFtpAttrs& a, int type)
{
	int rc = libssh2_sftp_stat_ex(*sftp_session, ~path, path.GetLength(), type, &a);
	return WouldBlock(rc) ?  1 : rc;
}

SFtpHandle SFtp::Open(const String& path, dword flags, long mode)
{
	SFtpHandle h = nullptr;

	Run([=, &h] () mutable {
		h = libssh2_sftp_open(*sftp_session, path, flags, mode);
		if(!h && !WouldBlock())
			SetError(-1);
		if(h)
			LLOG(Format("File '%s' is successfully opened.", path));
		return h;
	});

	return h;
}

bool SFtp::Close(SFtpHandle handle)
{
	return Run([=] () mutable {
		int rc = libssh2_sftp_close_handle(handle);
		if(!WouldBlock(rc) && rc < 0)
			SetError(-1, "Unable to close file handle.");
		if(rc == 0)
			LLOG("File handle freed.");
		return rc == 0;
	});
}

bool SFtp::Rename(const String& oldpath, const String& newpath)
{
	return Run([=] () mutable {
		int rc = libssh2_sftp_rename(*sftp_session, oldpath, newpath);
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(rc == 0)
			LLOG(Format("'%s' is successfully renamed to '%s'", oldpath, newpath));
		return !rc;
	});
}

bool SFtp::Delete(const String& path)
{
	return Run([=] () mutable {
		int rc = libssh2_sftp_unlink(*sftp_session, path);
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(rc == 0)
			LLOG(Format("File '%s' is successfully deleted.", path));
		return !rc;
	});
}

bool SFtp::Sync(SFtpHandle handle)
{
	return Run([=] () mutable {
		int rc = libssh2_sftp_fsync(handle);
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(rc == 0)
			LLOG("File successfully synchronized to disk.");
		return !rc;
	});
}

SFtp& SFtp::Seek(SFtpHandle handle, int64 position)
{
	Run([=] () mutable {
		LLOG("Seeking to offset: " << position);
		libssh2_sftp_seek64(handle, position);
		return true;
	});
	return *this;
}

int64 SFtp::GetPos(SFtpHandle handle)
{
	int64 pos = 0;

	return Run([=, &pos] () mutable {
		pos = libssh2_sftp_tell64(handle);
		LLOG("File position: " << pos);
		return true;
	});
	return pos;
}

bool SFtp::Read(SFtpHandle handle, Event<const void*, int>&& consumer, int size, int& done)
{
	int sz = min(size - done, ssh->chunk_size);
	Buffer<char> buffer(sz);

	int rc = libssh2_sftp_read(handle, buffer, sz);
	if(!WouldBlock(rc) && rc < 0)
		SetError(rc);
	if(rc > 0) {
		consumer(buffer, rc);
		done += rc;
		if(WhenProgress(done, size))
			SetError(-1, "Read aborted.");
		ssh->start_time = msecs();
	}
	
	auto b = !rc || done == size;

	if(b)
		LLOG("EOF received.");
	return b;
}

bool SFtp::Write(SFtpHandle handle, const void* buffer, int size, int& done)
{
	int sz = min(size - done, ssh->chunk_size);

	int rc = libssh2_sftp_write(handle, (const char*) buffer + done, sz);
	if(!WouldBlock(rc) && rc < 0)
		SetError(rc);
	if(rc > 0) {
		done += rc;
		if(WhenProgress(done, size))
			SetError(-1, "Write aborted.");
		ssh->start_time = msecs();
	}
	auto b = !rc || done == size;

	if(b)
		LLOG("EOF received.");
	return b;
}

int SFtp::Get(SFtpHandle handle, void *ptr, int size)
{
	int done = 0;

	Run([=, &done]() mutable {
		return Read(
			handle,
			[=, &done](const void* p, int sz) {
				memcpy((char*) ptr + done, (char*) p, sz);
			},
			size,
			done
		);
	});

	return done;
}

bool SFtp::Put(SFtpHandle handle, const void *ptr, int size)
{
	int done = 0;

	Run([=, &done]() mutable {
		return Write(handle, (char*) ptr, size, done);
	});

	return done;
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

SFtpHandle	 SFtp::OpenDir(const String& path)
{
	SFtpHandle h = nullptr;

	Run([=, &h] () mutable {
		h = libssh2_sftp_opendir(*sftp_session, path);
		if(!h && !WouldBlock())
			SetError(-1);
		if(h)
			LLOG(Format("Directory '%s' is successfully opened.", path));
		return h;
	});

	return h;
}

bool SFtp::MakeDir(const String& path, long mode)
{
	return Run([=] () mutable {
		int rc = libssh2_sftp_mkdir(*sftp_session, path, mode);
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(!rc)
			LLOG(Format("Directory '%s' is succesfully created.", path));
		return !rc;
	});
}

bool SFtp::RemoveDir(const String& path)
{
	return Run([=] () mutable {
		int rc = libssh2_sftp_rmdir(*sftp_session, path);
		if(!WouldBlock(rc) && rc != 0)
			SetError(rc);
		if(!rc)
			LOG(Format("Directory '%s' is succesfully deleted.", path));
		return !rc;
	});
}

bool SFtp::ListDir(SFtpHandle handle, DirList& list)
{
	SFtpAttrs attrs;
	return Run([=, &attrs, &list] () mutable {
		char label[512];
		char longentry[512];

		int rc = 0;

		do {
			Zero(attrs);

			rc = libssh2_sftp_readdir_ex(
						handle,
						label, sizeof(label),
						longentry, sizeof(longentry),
						&attrs
			);
			if(rc < 0) {
				if(!WouldBlock(rc))
					SetError(rc);
				return false;
			}
			else
			if(rc > 0) {
				DirEntry& entry	= list.Add();
				entry.filename	= label;
				*entry.a		= attrs;
				entry.valid		= true;
//				DUMP(entry);
			}
		}
		while(rc > 0);
		LLOG(Format("Directory listing is successful. (%d entries)", list.GetCount()));
		return true;

	});
}

bool SFtp::ListDir(const String& path, DirList& list)
{
	SFtpHandle h = OpenDir(path);
	return h && ListDir(h, list) && Close(h);
}

bool SFtp::SymLink(const String& path, String* target, int type)
{
	Buffer<char> buffer(512);

	if(type == LIBSSH2_SFTP_SYMLINK)
		return Run([=, &buffer] () mutable {
			int rc = libssh2_sftp_symlink_ex(
						*sftp_session,
						path,
						path.GetLength(),
						buffer,
						512,
						type
					);
			if(!WouldBlock(rc) && rc != 0)
				SetError(rc);
			if(!rc) {
				target->Set((const char*) buffer, rc);
				LLOG(Format("Symbolic link '%s' for path '%s' is successfult created.", target, path));
			}
			return !rc;
		});
	else
		return Run([=, &buffer] () mutable {
			int rc = libssh2_sftp_symlink_ex(
						*sftp_session,
						path,
						path.GetLength(),
						buffer,
						512,
						type
					);
			if(!WouldBlock(rc) && rc <= 0)
				SetError(rc);
			if(rc > 0) {
				LLOG("Symbolic link operation is successful.");
				target->Set(buffer, rc);
			}
			return rc > 0;
		});
}

SFtp::DirEntry SFtp::GetInfo(const String& path)
{
	DirEntry finfo;
	
	if(!GetAttrs(path, *finfo))
		return pick(Null);

	finfo.filename = path;
	finfo.valid = true;
	
	return pick(finfo);
}

bool SFtp::GetAttrs(SFtpHandle handle, SFtpAttrs& attrs)
{
	return Run([=, &attrs] () mutable {
		int rc = FStat(handle, attrs, false);
		if(rc < 0)	SetError(rc);
		if(!rc)	LLOG("File attributes successfully retrieved.");
		return !rc;
	});
}

bool SFtp::GetAttrs(const String& path, SFtpAttrs& attrs)
{
	return Run([=, &attrs] () mutable {
		int rc = LStat(path, attrs, LIBSSH2_SFTP_STAT);
		if(rc < 0)	SetError(rc);
		if(!rc) LLOG(Format("File attributes of '%s' is successfully retrieved.", path));
		return !rc;
	});
}

bool SFtp::SetAttrs(SFtpHandle handle, const SFtpAttrs& attrs)
{
	return Run([=, &attrs] () mutable {
		int rc = FStat(handle, const_cast<SFtpAttrs&>(attrs), true);
		if(rc < 0)	SetError(rc);
		if(!rc)	LLOG("File attributes successfully modified.");
		return !rc;
	});
}

bool SFtp::SetAttrs(const String& path, const SFtpAttrs& attrs)
{
	return Run([=, &attrs] () mutable {
		int rc = LStat(path,  const_cast<SFtpAttrs&>(attrs), LIBSSH2_SFTP_SETSTAT);
		if(rc < 0)	SetError(rc);
		if(!rc)	LLOG(Format("File attributes of '%s' is successfully modified.", path));
		return !rc;
	});
}

Value SFtp::QueryAttr(const String& path, int attr)
{
	DirEntry finfo;
	Value v;

	if(!GetAttrs(path, *finfo))
		return Null;

	switch(attr) {
		case SFTP_ATTR_FILE:
			v = finfo.IsFile();
			break;
		case SFTP_ATTR_DIRECTORY:
			v = finfo.IsDirectory();
			break;
		case SFTP_ATTR_SOCKET:
			v = finfo.IsSocket();
			break;
		case SFTP_ATTR_SYMLINK:
			v = finfo.IsSymLink();
			break;
		case SFTP_ATTR_PIPE:
			v = finfo.IsPipe();
			break;
		case SFTP_ATTR_BLOCK:
			v = finfo.IsBlock();
			break;
		case SFTP_ATTR_SPECIAL:
			v = finfo.IsSpecial();
			break;
		case SFTP_ATTR_SIZE:
			v = finfo.GetSize();
			break;
		case SFTP_ATTR_LAST_MODIFIED:
			v = finfo.GetLastModified();
			break;
		case SFTP_ATTR_LAST_ACCESSED:
			v = finfo.GetLastAccessed();
			break;
		default:
			break;
	}
	return pick(v);
}

bool SFtp::ModifyAttr(const String& path, int attr, const Value& v)
{
	DirEntry finfo;

	if(!GetAttrs(path, *finfo))
		return false;

	SFtpAttrs& attrs = *finfo;

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

	return SetAttrs(path, ~finfo);;
}

SFtp::SFtp(SshSession& session)
: Ssh()
{
	ssh->otype		= SFTP;
	ssh->session	= session.GetHandle();
	ssh->socket		= &session.GetSocket();
	ssh->timeout	= session.GetTimeout();
	ssh->waitstep   = session.GetWaitStep();
	ssh->wait       = Proxy(session.WhenWait);
}

SFtp::~SFtp()
{
	Exit();
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
