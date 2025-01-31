#include "SSH.h"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool SFtp::Init()
{
	if(!ssh->session)
		ThrowError(-1, "SSH session is invalid.");
	auto session = libssh2_sftp_init(ssh->session);
	if(!session && !WouldBlock())
		ThrowError(-1);
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
	}, false);
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
			ThrowError(-1);
		if(h)
			LLOG(Format("File '%s' is successfully opened.", path));
		return h;
	});
	return h;
}

void SFtp::Close(SFtpHandle handle)
{
	if(!handle)
		return;

	Run([=] () mutable {
		int rc = libssh2_sftp_close_handle(handle);
		if(!rc)	LLOG("File handle freed.");
		return !rc;
	}, false);
}

bool SFtp::Rename(const String& oldpath, const String& newpath)
{
	return Run([=] () mutable {
		int rc = libssh2_sftp_rename(*sftp_session, oldpath, newpath);
		if(!WouldBlock(rc) && rc != 0)
			ThrowError(rc);
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
			ThrowError(rc);
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
			ThrowError(rc);
		if(rc == 0)
			LLOG("File successfully synchronized to disk.");
		return !rc;
	});
}

SFtp& SFtp::Seek(SFtpHandle handle, int64 position)
{
	INTERLOCKED
	{
		// LLOG("Seeking to offset: " << position);
		libssh2_sftp_seek64(handle, position);
	}
	return *this;
}

int64 SFtp::GetPos(SFtpHandle handle)
{
	int64 pos = 0;

	INTERLOCKED
	{
		pos = libssh2_sftp_tell64(handle);
		// LLOG("File position: " << pos);
	};
	return pos;
}

int SFtp::Get(SFtpHandle handle, void *ptr, int size)
{
	done = 0;

	Run([=]() mutable {
		while(done < size && !IsTimeout()) {
			int rc = static_cast<int>(
				libssh2_sftp_read(handle, (char*) ptr + done, min(size - done, ssh->chunk_size))
			);
			if(rc < 0) {
				if(!WouldBlock(rc))
					ThrowError(rc);
				return false;
			}
			else
			if(rc == 0) {
				LLOG("EOF received.");
				break;
			}
			done += rc;
			ssh->start_time = msecs();
			UpdateClient();
		}
		return true;
	});

	return GetDone();
}

int SFtp::Put(SFtpHandle handle, const void *ptr, int size)
{
	done = 0;

	Run([=]() mutable {
		while(done < size && !IsTimeout()) {
			int rc = static_cast<int>(
				libssh2_sftp_write(handle, (const char*) ptr + done, min(size - done, ssh->chunk_size))
			);
			if(rc < 0) {
				if(!WouldBlock(rc))
					ThrowError(rc);
				return false;
			}
			else
			if(rc == 0) {
				LLOG("EOF received.");
				break;
			}
			done += rc;
			ssh->start_time = msecs();
			UpdateClient();
		}
		return true;
	});

	return GetDone();
}

bool SFtp::CopyData(Stream& dest, Stream& src, int64 maxsize)
{
	if(IsError())
		return false;

	int64 size = src.GetSize(), count = 0;
	Buffer<byte> chunk(ssh->chunk_size, 0);

	WhenProgress(0, size);

	while(!src.IsEof()) {
		int n = src.Get(chunk, (int) min<int64>(size - count, ssh->chunk_size));
		if(n > 0) {
			dest.Put(chunk, n);
			if(dest.IsError()) {
				LLOG("Stream write error. " + dest.GetErrorText());
				return false;
			}
			count += n;
			if(WhenProgress(count, size)) {
				return false;
			}
		}
		if(src.IsError()) {
			LLOG("Stream read error. " + src.GetErrorText());
			break;
		}
	}
	return !src.IsError();
}

bool SFtp::SaveFile(const char *path, const String& data)
{
	StringStream in(data);
	SFtpFileOut out(*this, path);
	return CopyData(out, in);
}

String SFtp::LoadFile(const char *path)
{
	StringStream out;
	SFtpFileIn in(*this, path);
	CopyData(out, in, INT_MAX);
	return pick(out.GetResult());
}

bool SFtp::SaveFile(const char *path, Stream& in)
{
	SFtpFileOut out(*this, path);
	return CopyData(out, in);
}

bool SFtp::LoadFile(Stream& out, const char *path)
{
	SFtpFileIn in(*this, path);
	return CopyData(out, in);
}

SFtpHandle SFtp::OpenDir(const String& path)
{
	SFtpHandle h = nullptr;

	Run([=, &h] () mutable {
		h = libssh2_sftp_opendir(*sftp_session, path);
		if(!h && !WouldBlock())
			ThrowError(-1);
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
			ThrowError(rc);
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
			ThrowError(rc);
		if(!rc)
			LLOG(Format("Directory '%s' is succesfully deleted.", path));
		return !rc;
	});
}

bool SFtp::ListDir(SFtpHandle handle, DirList& list)
{
	SFtpAttrs attrs;
	return Run([=, &attrs, &list] () mutable {
		char label[512];
		char longentry[512];

		while(InProgress()) {
			Zero(attrs);
			int rc = libssh2_sftp_readdir_ex(
						handle,
						label, sizeof(label),
						longentry, sizeof(longentry),
						&attrs
			);
			if(rc < 0) {
				if(!WouldBlock(rc))
					ThrowError(rc);
				break;
			}
			else
			if(rc > 0) {
				DirEntry& entry	= list.Add();
				entry.filename	= label;
				*entry.a		= attrs;
				entry.valid		= true;
//				DUMP(entry);
			}
			if(rc == 0) {
				LLOG(Format("Directory listing is successful. (%d entries)", list.GetCount()));
				return true;
			}
		}
		return false;
	});
}

bool SFtp::ListDir(const String& path, DirList& list)
{
	SFtpHandle h = OpenDir(path);
	if(h) {
		bool b = ListDir(h, list);
		Close(h);
		if(!b) ssh->status = FAILED;
	}
	return !IsError();
}

bool SFtp::SymLink(const String& path, String& target, int type)
{
	Buffer<char> buffer(512, 0);

	if(type == LIBSSH2_SFTP_SYMLINK)
		return Run([=, &path, &buffer, &target] () mutable {
			int rc = libssh2_sftp_symlink_ex(
						*sftp_session,
						path,
						path.GetLength(),
						buffer,
						512,
						type
					);
			if(!WouldBlock(rc) && rc != 0)
				ThrowError(rc);
			if(!rc) {
				target.Set(buffer, rc);
				LLOG(Format("Symbolic link '%s' for path '%s' is successfult created.", target, path));
			}
			return !rc;
		});
	else
		return Run([=, &path, &buffer, &target] () mutable {
			Buffer<char> sbuf(512, 0);
			int rc = libssh2_sftp_symlink_ex(
						*sftp_session,
						path,
						path.GetLength(),
						buffer,
						512,
						type
					);
			if(!WouldBlock(rc) && rc < 0)
				ThrowError(rc);
			if(rc > 0) {
				target.Set(buffer, rc);
				LLOG("Symbolic link operation is successful. Target: " << target);
			}
			return rc > 0;
		});
}

SFtp::DirEntry SFtp::GetInfo(const String& path)
{
	DirEntry finfo;

	if(!GetAttrs(path, *finfo))
		return Null;

	finfo.filename = path;
	finfo.valid = true;

	return pick(finfo);
}

bool SFtp::GetAttrs(SFtpHandle handle, SFtpAttrs& attrs)
{
	return Run([=, &attrs] () mutable {
		int rc = FStat(handle, attrs, false);
		if(rc < 0)	ThrowError(rc);
		if(!rc)	LLOG("File attributes successfully retrieved.");
		return !rc;
	});
}

bool SFtp::GetAttrs(const String& path, SFtpAttrs& attrs)
{
	return Run([=, &path, &attrs] () mutable {
		int rc = LStat(path, attrs, LIBSSH2_SFTP_STAT);
		if(rc < 0)	ThrowError(rc);
		if(!rc) LLOG(Format("File attributes of '%s' is successfully retrieved.", path));
		return !rc;
	});
}

bool SFtp::SetAttrs(SFtpHandle handle, const SFtpAttrs& attrs)
{
	return Run([=, &attrs] () mutable {
		int rc = FStat(handle, const_cast<SFtpAttrs&>(attrs), true);
		if(rc < 0)	ThrowError(rc);
		if(!rc)	LLOG("File attributes successfully modified.");
		return !rc;
	});
}

bool SFtp::SetAttrs(const String& path, const SFtpAttrs& attrs)
{
	return Run([=, &attrs] () mutable {
		int rc = LStat(path,  const_cast<SFtpAttrs&>(attrs), LIBSSH2_SFTP_SETSTAT);
		if(rc < 0)	ThrowError(rc);
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
, done(0)
{
	ssh->otype		= SFTP;
	ssh->session	= session.GetHandle();
	ssh->socket		= &session.GetSocket();
	ssh->timeout	= session.GetTimeout();
	ssh->waitstep   = session.GetWaitStep();
	ssh->whenwait   = Proxy(session.WhenWait);
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

FileSystemInfo::FileInfo SFtp::DirEntry::ToFileInfo() const
{
	FileSystemInfo::FileInfo fi;
	fi.filename         = GetName();
	fi.length           = GetSize();
	fi.unix_mode        = GetPermissions();
	fi.creation_time    = GetLastModified();
	fi.last_access_time = GetLastAccessed();
	fi.last_write_time  = GetLastModified();
	fi.is_file          = IsFile();
	fi.is_directory     = IsDirectory();
	fi.is_symlink       = IsSymLink();
	fi.is_read_only     = IsReadOnly();
	fi.root_style       = FileSystemInfo::ROOT_FIXED;
	return pick(fi);
}

void SFtp::DirEntry::Serialize(Stream& s)
{
	ASSERT(a);

	s % a->flags;
	s % a->permissions;
	s % a->uid;
	s % a->gid;
	s % a->filesize;
	s % a->mtime;
	s % a->atime;
	s % filename;
}

bool SFtp::DirEntry::CanMode(dword u, dword g, dword o) const
{
	return (a->flags & LIBSSH2_SFTP_ATTR_PERMISSIONS)
		&& (a->permissions & o || a->permissions & g || a->permissions & u);
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

Array<FileSystemInfo::FileInfo> SFtpFileSystemInfo::Find(String mask, int max_count, bool unmounted) const
{
	ASSERT(browser);
	Array<FileInfo> fi;
	String dir;
	if(!browser->InProgress()) {
		if(IsNull(mask))
			dir = browser->GetDefaultDir();
		else {
			String s;
			dir = GetFileDirectory(browser->RealizePath(mask, s) ? s : mask);
		}
		bool haswc = HasWildcards(mask);
		if(!haswc && max_count == 1) { // A small optimization.
			SFtp::DirEntry e = browser->GetInfo(mask);
			if(e)
				fi.Add(e.ToFileInfo());
		}
		else {
			String pattern;
			SFtp::DirList ls;
			if(browser->ListDir(dir, ls)) {
				if(haswc)
					pattern = GetFileName(mask);
				for(int i = 0, j = 0; i < ls.GetCount() && j < max_count; i++) {
					const SFtp::DirEntry& e = ls[i];
					if(!haswc || PatternMatch(pattern, e.GetName())) {
						fi.Add(e.ToFileInfo());
						j++;
					}
				}
			}
		}
	}
	return pick(fi);
}

bool SFtpFileSystemInfo::CreateFolder(String path, String& error) const
{
	ASSERT(browser);
	if(!browser->MakeDir(path, 0))
		error = browser->GetErrorDesc();
	return !browser->IsError();
}
}
