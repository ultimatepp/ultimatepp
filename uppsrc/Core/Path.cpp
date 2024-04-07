#include "Core.h"
//#BLITZ_APPROVE

#ifdef PLATFORM_POSIX
#include <sys/types.h>
#include <utime.h>
#endif//PLATFORM_POSIX

#ifdef PLATFORM_WIN32

#define Ptr Ptr_
#define byte byte_
#define CY win32_CY_

#include <winnls.h>
#include <winnetwk.h>

#include <wincon.h>

#ifdef COMPILER_MINGW
#undef CY
#endif

#include <shlobj.h>

#undef Ptr
#undef byte
#undef CY

#endif

namespace Upp {

static int sDirSep(int c) {
	return c == '/' || c == '\\' ? c : 0;
}

static bool strecmp0(const char *p, const char *s) {
	while(*p) {
		if(*p == '*') {
			while(*p == '*') p++;
			do
				if(ToUpper(*p) == ToUpper(*s) && strecmp0(p, s)) return true;
			while(*s++);
			return false;
		}
		if(*p == '?') {
			if(*s == '\0') return false;
		}
		else
			if(ToUpper(*p) != ToUpper(*s)) return false;
		s++;
		p++;
	}
	return *s == '\0';
}

bool PatternMatch(const char *p, const char *s) {
	const char *q;
	q = strchr(p, '.');
	if(q) {
		if(q[1] == '\0') {
			if(strchr(s, '.')) return false;
			String h(p, q);
			return strecmp0(h, s);
		}
		else
		if(q[1] == '*' && q[2] == '\0') {
			String h(p, q);
			return strecmp0(h, s) || strecmp0(p, s);
		}
	}
	return strecmp0(p, s);
}

bool PatternMatchMulti(const char *p, const char *s) {
	String pt;
	while(*p) {
		if(*p == ';' || *p == ',' || *p == ' ') {
			if(PatternMatch(pt, s)) return true;
			p++;
			while(*p == ';' || *p == ',' || *p == ' ') p++;
			pt.Clear();
		}
		else
			pt.Cat(*p++);
	}
	return pt.IsEmpty() ? false : PatternMatch(pt, s);
}

const char *GetFileNamePos(const char *fileName) {
	const char *s = fileName;
	const char *fname = s;
	char c;
	while((c = *s++) != '\0')
	#ifdef PLATFORM_WIN32
		if(c == '\\' || c == ':' || c == '/')
	#else
		if(c == '/')
	#endif
			fname = s;
	return fname;
}

const char *GetFileExtPos(const char *fileName) {
	fileName = GetFileNamePos(fileName);
	const char *ext = strrchr(fileName, '.');
	return ext ? ext : fileName + strlen(fileName);
}

bool HasFileExt(const char *path) {
	return *GetFileExtPos(path);
}

bool HasWildcards(const char *fileName) {
	return strchr(fileName, '*') || strchr(fileName, '?');
}

bool IsFullPath(const char *r) {
#ifdef PLATFORM_WIN32
	return *r && r[1] && (r[1] == ':' || r[0] == '\\' && r[1] == '\\' || r[0] == '/' && r[1] == '/');
#endif
#ifdef PLATFORM_POSIX
	return *r == '/';
#endif
}

String GetFileDirectory(const char *fileName) {
	return String(fileName, (int)(GetFileNamePos(fileName) - fileName));
}

String GetFileFolder(const char *fileName) {
	const char *s = GetFileNamePos(fileName);
#ifdef PLATFORM_WIN32
	if(s - fileName == 3 && fileName[1] == ':')
		return String(fileName, 3);
#endif
#ifdef PLATFORM_POSIX
	if(s - fileName == 1 && s[0] == '/')
		return "/";
#endif
	if(s > fileName)
		return String(fileName, (int)(s - fileName) - 1);
	return Null;
}

String GetFileTitle(const char *fileName) {
	fileName = GetFileNamePos(fileName);
	const char *ext = GetFileExtPos(fileName);
	if(*ext)
		return String(fileName, (int)(ext - fileName));
	else
		return fileName;
}

String GetFileExt(const char *fileName) {
	return GetFileExtPos(fileName);
}

String GetFileName(const char *fileName) {
	return GetFileNamePos(fileName);
}

String AppendFileName(const String& path, const char *fileName) {
	String result = path;
	if(result.GetLength() && *result.Last() != DIR_SEP && *fileName != DIR_SEP)
		result += DIR_SEP;
	result += fileName;
	return result;
}

#ifdef PLATFORM_WIN32
bool   PathIsEqual(const char *p1, const char *p2)
{
	return ToLower(NormalizePath(p1)) == ToLower(NormalizePath(p2));
}
#endif

#ifdef PLATFORM_POSIX
bool   PathIsEqual(const char *p1, const char *p2)
{
	return NormalizePath(p1) == NormalizePath(p2);
}
#endif

#ifndef PLATFORM_WINCE
String GetCurrentDirectory() {
#if defined(PLATFORM_WIN32)
	WCHAR h[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, h);
	return FromSystemCharsetW(h);
#elif defined(PLATFORM_POSIX)
	char h[1024];
	return getcwd(h, 1024) ? FromSystemCharset(h) : String();
#else
#error GetCurrentDirectory not implemented for this platform, comment this line to get Null
	return Null;
#endif//PLATFORM
}
#endif

#ifdef PLATFORM_POSIX

bool SetCurrentDirectory(const char *path)
{
	return chdir(path) == 0;
}

bool ChangeCurrentDirectory(const char *path)
{
	return SetCurrentDirectory(path);
}

#endif

#ifdef PLATFORM_WIN32
bool ChangeCurrentDirectory(const char *path)
{
	return SetCurrentDirectory(ToSystemCharset(path));
}
#endif

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)

String GetTempPath()
{
	WCHAR h[MAX_PATH];
	GetTempPathW(MAX_PATH, h);
	return FromSystemCharsetW(h);
}

#endif

#ifdef PLATFORM_POSIX

String GetTempPath()
{
	return FromSystemCharset(P_tmpdir);
}

#endif

#ifndef PLATFORM_WINCE
String GetTempFileName(const char *prefix) {
	Uuid id = Uuid::Create();
	return AppendFileName(GetTempPath(), String(prefix) + Format(id) + ".tmp");
}
#endif

String FromUnixName(const char* fn, const char* stop = NULL) {
	String s;
	char c;
	while(fn != stop && (c = *fn++))
		s += (c == '/' ? '\\' : c);
	return s;
}

String ToUnixName(const char* fn, const char* stop = NULL) {
	String s;
	char c;
	while(fn != stop && (c = *fn++))
		s += (c == '\\' ? '/' : c);
	return s;
}

String GetFullPath(const char *file) {
#ifdef PLATFORM_WIN32
	String ufn = FromUnixName(file);
	WCHAR h[MAX_PATH];
	GetFullPathNameW(ToSystemCharsetW(ufn), MAX_PATH, h, 0);
	return FromSystemCharsetW(h);
#else
	return NormalizePath(file);
#endif
}

String GetFileOnPath(const char* file, const char* paths, bool current, const char *curdir) {
	String ufn = NativePath(file);
	if(IsFullPath(ufn) && FileExists(ufn))
		return ufn;
	String fn;
	String cd = curdir;
	if(!curdir)
		cd = GetCurrentDirectory();
	if(current && FileExists(fn = NormalizePath(ufn, cd)))
		;
	else if(paths)
	{
		fn = Null;
		while(*paths) {
			const char* start = paths;
#ifdef PLATFORM_WIN32
			while(*paths && *paths != ';')
				paths++;
#else
			while(*paths && *paths != ';' && *paths != ':')
				paths++;
#endif
			String dir(start, (int)(paths - start));
			if(!dir.IsEmpty()) {
#ifdef PLATFORM_WINCE
				dir = NormalizePath(AppendFileName(NativePath(dir), ufn));
#else
				dir = NormalizePath(AppendFileName(NativePath(dir), ufn), cd);
#endif
				if(FileExists(dir)) {
					fn = dir;
					break;
				}
			}
			if(*paths)
				paths++;
		}
	}
	return fn;
}

String WinPath(const char *p) {
	String r;
	while(*p) {
		r.Cat(*p == '/' ? '\\' : *p);
		p++;
	}
	return r;
}

String UnixPath(const char *p) {
	String r;
	while(*p) {
		r.Cat(*p == '\\' ? '/' : *p);
		p++;
	}
	return r;
}

String AppendExt(const char* fn, const char* ext) {
	String result = NativePath(fn);
	if(!HasFileExt(fn))
		result += ext;
	return result;
}

String ForceExt(const char* fn, const char* ext) {
	return NativePath(String(fn, GetFileExtPos(fn))) + ext;
}

#ifdef PLATFORM_WIN32

FindFile::~FindFile()
{
	Close();
}

FindFile::FindFile()
{
	handle = INVALID_HANDLE_VALUE;
}

FindFile::FindFile(const char *name) {
	handle = INVALID_HANDLE_VALUE;
	Search(name);
}

bool FindFile::Search(const char *name) {
	pattern = GetFileName(name);
	path = NormalizePath(GetFileDirectory(name));
	Close();
	handle = FindFirstFileW(ToSystemCharsetW(name), data);
	if(handle == INVALID_HANDLE_VALUE)
		return false;
	if(!PatternMatch(pattern, GetName()))
		return Next();
	return true;
}

static bool sGetSymLinkPath0(const char *linkpath, String *path)
{
	bool ret = false;
	HRESULT hres;
	IShellLinkW* psl;
	IPersistFile* ppf;
	CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW,
	                        (PVOID *) &psl);
	if(SUCCEEDED(hres)) {
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);
		if(SUCCEEDED(hres)) {
			hres = ppf->Load(ToSystemCharsetW(linkpath), STGM_READ);
			if(SUCCEEDED(hres)) {
				if(path) {
					WCHAR fileW[_MAX_PATH] = {0};
					psl->GetPath(fileW, _MAX_PATH, NULL, 0);
					*path = fileW;
				}
				else
					ret = true;
			}
			ppf->Release();
		}
		psl->Release();
	}
	CoUninitialize();
	return ret;
}

bool FindFile::IsSymLink() const
{
	String name = GetName();
	if(GetFileExt(name) != ".lnk")
		return false;
	return sGetSymLinkPath0(AppendFileName(path, name), NULL);
}

bool FindFile::IsExecutable() const
{
	return !IsDirectory() && ToLower(GetName()).EndsWith(".exe");
}

void FindFile::Close() {
	if(handle != INVALID_HANDLE_VALUE) FindClose(handle);
	handle = INVALID_HANDLE_VALUE;
}

bool FindFile::Next0() {
	if(!FindNextFileW(handle, data)) {
		Close();
		return false;
	}
	return true;
}

bool FindFile::Next()
{
	for(;;) {
		if(!Next0())
			return false;
		if(PatternMatch(pattern, GetName()))
			return true;
	}
}

String FindFile::GetName() const
{
	return FromSystemCharsetW(data->cFileName);
}

int64 FindFile::GetLength() const
{
	return (int64)data->nFileSizeLow | ((int64)data->nFileSizeHigh << 32);
}

bool FindFile::IsFolder() const {
	auto h = data->cFileName;
	return IsDirectory() && !(h[0] == '.' && h[1] == 0) && !(h[0] == '.' && h[1] == '.' && h[2] == 0);
}

String NormalizePath(const char *path, const char *currdir)
{
	String join_path;
	if(!IsFullPath(path))
		path = join_path = AppendFileName(currdir, path);
	String out;
	if(*path && path[1] == ':') {
		out << path[0] << ":\\";
		path += 3;
	}
	else
	if(path[0] == '\\' && path[1] == '\\') {
		out = "\\\\";
		path += 2;
	}
	else
	if(sDirSep(*path)) {
		if(*currdir)
			out << *currdir << ':';
		out.Cat(DIR_SEP);
		path++;
	}
	int outstart = out.GetLength();
	while(*path) {
		if(sDirSep(*path)) {
			while(sDirSep(*path))
				path++;
			if(*path == '\0')
				break;
			if(out.IsEmpty() || *out.Last() != DIR_SEP)
				out.Cat(DIR_SEP);
		}
		const char *b = path;
		while(*path && !sDirSep(*path))
			path++;
		if(path - b == 1 && *b == '.')
			; //no-op
		else if(path - b == 2 && *b == '.' && b[1] == '.') {
			const char *ob = ~out + outstart, *oe = out.End();
			if(oe - 1 > ob && oe[-1] == DIR_SEP)
				oe--;
			while(oe > ob && oe[-1] != DIR_SEP)
				oe--;
			out.Trim((int)(oe - out.Begin()));
		}
		else
			out.Cat(b, (int)(path - b));
	}
	return out;
}

#endif

#ifdef PLATFORM_POSIX

void FindFile::Close() {
	if(dir) {
		closedir(dir);
		dir = NULL;
	}
}

bool FindFile::IsFolder() const {
	return IsDirectory()
		&& !(name[0] == '.' && name[1] == '\0')
		&& !(name[0] == '.' && name[1] == '.' && name[2] == '\0');
}

struct stat& FindFile::Stat() const {
	if(!statis) {
		statis = true;
		if(file)
			stat(ToSystemCharset(AppendFileName(path, name)), &statf);
	}
	return statf;
}

bool FindFile::CanMode(dword usr, dword grp, dword oth) const
{
	const struct stat& s = Stat();
	dword mode = GetMode();
	static uid_t uid = getuid();
	static gid_t gid = getgid();
	return (mode & oth) ||
	       (mode & grp) && gid == s.st_gid ||
	       (mode & usr) && uid == s.st_uid;
}

bool FindFile::IsSymLink() const
{
	if(file)  {
		struct stat stf;
		lstat(AppendFileName(path, name), &stf);
		return S_ISLNK(stf.st_mode);
	}
	return false;
}

bool FindFile::IsExecutable() const
{
	return !IsDirectory() && ((S_IXUSR|S_IXGRP|S_IXOTH) & GetMode());
}

bool FindFile::Next() {
	file = false;
	if(!dir) return false;
	statis = false;
	for(;;) {
		struct dirent *e = readdir(dir);
		if(!e) {
			name.Clear();
			file = false;
			Close();
			return false;
		}
		name = FromSystemCharset(e->d_name);
		if(PatternMatch(pattern, name)) {
			file = true;
			return true;
		}
	}
}

bool FindFile::Search(const char *fn) {
	Close();
	path = NormalizePath(GetFileDirectory(fn));
	statis = false;
	file = false;
	if(HasWildcards(fn)) {
		pattern = GetFileName(fn);
		dir = opendir(ToSystemCharset(path));
		return Next();
	}
	else {
		name = GetFileName(fn);
		if(stat(ToSystemCharset(fn), &statf)) return false;
		statis = true;
		file = true;
		return true;
	}
}

FindFile::FindFile(const char *fn) {
	dir = NULL;
	Search(fn);
}

String NormalizePath(const char *path, const char *currdir) {
	Vector<String> si = Split(path, sDirSep);
	Vector<String> p;
	int i = 0;
	String out;
	if(path[0] == '~') {
		out = GetHomeDirectory();
		i++;
	}
	else
	if(sDirSep(path[0]))
		out = (sDirSep(path[1]) ? "//" : "/");
	else {
		out = (sDirSep(currdir[0]) && sDirSep(currdir[1]) ? "//" : "/");
		p = Split(currdir, sDirSep);
	}
	for(; i < si.GetCount(); i++) {
		String s = si[i];
		if(s != "." && !s.IsEmpty()) {
			if(s[0] == '.' && s[1] == '.') {
				if(!p.IsEmpty()) p.Drop();
			}
			else
				p.Add(s);
		}
	}
	out.Cat(Join(p, DIR_SEPS));
	return out;
}

#endif//PLATFORM_POSIX

String FindFile::GetPath() const
{
	return AppendFileName(path, GetName());
}

String NormalizePath(const char *path) {
#ifdef PLATFORM_WINCE
	return NormalizePath(path, "");
#else
	return NormalizePath(path, GetCurrentDirectory());
#endif
}

bool FileCopy(const char *oldname, const char *newname)
{
#if defined(PLATFORM_WIN32)
	return CopyFileW(ToSystemCharsetW(oldname), ToSystemCharsetW(newname), false);
#elif defined(PLATFORM_POSIX)
	FileIn fi(oldname);
	if(!fi.IsOpen())
		return false;
	FileOut fo(newname);
	if(!fo.IsOpen())
		return false;
	CopyStream(fo, fi, fi.GetLeft());
	fi.Close();
	fo.Close();
	if(fo.IsError())
	{
		unlink(newname);
		return false;
	}
	SetFileTime(newname, GetFileTime(oldname));
	return true;
#else
	#error
#endif//PLATFORM
}

bool FileMove(const char *oldname, const char *newname)
{
#if defined(PLATFORM_WIN32)
	return !!MoveFileW(ToSystemCharsetW(oldname), ToSystemCharsetW(newname));
#elif defined(PLATFORM_POSIX)
	return !rename(ToSystemCharset(oldname), ToSystemCharset(newname));
#else
	#error
#endif//PLATFORM
}

bool FileDelete(const char *filename)
{
#if defined(PLATFORM_WIN32)
	return !!DeleteFileW(ToSystemCharsetW(filename));
#elif defined(PLATFORM_POSIX)
	return !unlink(ToSystemCharset(filename));
#else
	#error
#endif//PLATFORM
	return false;
}

bool DirectoryDelete(const char *dirname)
{
#if defined(PLATFORM_WIN32)
	return !!RemoveDirectoryW(ToSystemCharsetW(dirname));
#elif defined(PLATFORM_POSIX)
	return !rmdir(ToSystemCharset(dirname));
#else
	#error
#endif//PLATFORM
	return false;
}

#ifdef PLATFORM_WIN32

int Compare_FileTime(const FileTime& fa, const FileTime& fb)
{
	return CompareFileTime(&fa, &fb);
}

static bool sGetFileAttrs(const char *path, WIN32_FILE_ATTRIBUTE_DATA& wfad)
{
    return GetFileAttributesExW(ToSystemCharsetW(path), GetFileExInfoStandard, &wfad);
}

#endif

FileTime GetFileTime(const char *filename)
{
#if defined(PLATFORM_WIN32)
    WIN32_FILE_ATTRIBUTE_DATA wfad;
	static FileTime ft0;
    return sGetFileAttrs(filename, wfad) ? wfad.ftLastWriteTime : ft0;
#elif defined(PLATFORM_POSIX)
	struct stat st;
	if(stat(ToSystemCharset(filename), &st))
		return 0;
	return st.st_mtime;
#else
	#error
#endif//PLATFORM
}

Time FileGetTime(const char *path)
{
	return Time(GetFileTime(path));
}

int64 GetFileLength(const char *path) {
#if defined(PLATFORM_WIN32)
    WIN32_FILE_ATTRIBUTE_DATA wfad;
	static FileTime ft0;
    return sGetFileAttrs(path, wfad) ? MAKEQWORD(wfad.nFileSizeLow, wfad.nFileSizeHigh) : -1;
#elif defined(PLATFORM_POSIX)
	struct stat st;
	return stat(ToSystemCharset(path), &st) ? -1 : st.st_size;
#else
	#error
#endif//PLATFORM
}

bool FileExists(const char *path)
{
#if defined(PLATFORM_WIN32)
    WIN32_FILE_ATTRIBUTE_DATA wfad;
	static FileTime ft0;
    return sGetFileAttrs(path, wfad) && !(wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
#elif defined(PLATFORM_POSIX)
	struct stat st;
	return stat(ToSystemCharset(path), &st) ? false : S_ISREG(st.st_mode);
#else
	#error
#endif//PLATFORM
}

bool DirectoryExists(const char *path)
{
#if defined(PLATFORM_WIN32)
    WIN32_FILE_ATTRIBUTE_DATA wfad;
	static FileTime ft0;
    return sGetFileAttrs(path, wfad) && (wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
#elif defined(PLATFORM_POSIX)
	struct stat st;
	return stat(ToSystemCharset(path), &st) ? false : S_ISDIR(st.st_mode);
#else
	#error
#endif//PLATFORM
}

bool SetFileTime(const char *filename, FileTime ft)
{
#if defined(PLATFORM_WIN32)
	HANDLE handle;
	handle = CreateFileW(ToSystemCharsetW(filename), GENERIC_READ | GENERIC_WRITE,
		                 FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(handle == INVALID_HANDLE_VALUE)
		return false;
	bool res = SetFileTime(handle, 0, 0, &ft);
	CloseHandle(handle);
	return res;
#elif defined(PLATFORM_POSIX)
	struct utimbuf ub;
	ub.actime = ub.modtime = ft;
	return !utime(ToSystemCharset(filename), &ub);
#else
	#error
#endif//PLATFORM
}

bool FileSetTime(const char *filename, Time time)
{
	return SetFileTime(filename, TimeToFileTime(time));
}

FileTime TimeToFileTime(Time time)
{
#ifdef PLATFORM_WIN32
	SYSTEMTIME tm;
	Zero(tm);
	tm.wYear   = time.year;
	tm.wMonth  = time.month;
	tm.wDay    = time.day;
	tm.wHour   = time.hour;
	tm.wMinute = time.minute;
	tm.wSecond = time.second;
	FileTime ftl, ftg;
	SystemTimeToFileTime(&tm, &ftl);
	LocalFileTimeToFileTime(&ftl, &ftg);
	return ftg;
#endif
#ifdef PLATFORM_POSIX
	struct tm t;
	memset(&t, 0, sizeof(t));
	t.tm_sec  = time.second;
	t.tm_min  = time.minute;
	t.tm_hour = time.hour;
	t.tm_mday = time.day;
	t.tm_mon  = time.month - 1;
	t.tm_year = time.year - 1900;
	t.tm_isdst = -1;
	return mktime(&t);
#endif
}

#ifdef PLATFORM_POSIX
bool DirectoryCreate(const char *path, int mode)
{
	return ::mkdir(ToSystemCharset(path), mode) == 0;
}

bool RealizePath(const String& file, int mode)
{
	return RealizeDirectory(GetFileFolder(file), mode);
}
#else
bool DirectoryCreate(const char *path)
{
	return !!CreateDirectoryW(ToSystemCharsetW(path), 0);
}

bool RealizePath(const String& file)
{
	return RealizeDirectory(GetFileFolder(file));
}
#endif

#ifdef PLATFORM_WIN32
#define DIR_MIN 3 //!! wrong! what about \a\b\c ?
#endif

#ifdef PLATFORM_POSIX
#define DIR_MIN 1
#endif

#ifdef PLATFORM_POSIX
bool RealizeDirectory(const String& d, int mode)
#else
bool RealizeDirectory(const String& d)
#endif
{
	String dir = NormalizePath(d);
	Vector<String> p;
	while(dir.GetLength() > DIR_MIN && !DirectoryExists(dir)) {
		p.Add(dir);
		dir = GetFileFolder(dir);
	}
	for(int i = p.GetCount() - 1; i >= 0; i--)
#ifdef PLATFORM_POSIX
			if(!DirectoryCreate(p[i], mode))
#else
			if(!DirectoryCreate(p[i]))
#endif
				return false;
	return true;
}

void SetWritePermission(const char *path)
{
#ifdef PLATFORM_WIN32
	SetFileAttributes(path, GetFileAttributes(path) & ~FILE_ATTRIBUTE_READONLY);
#endif
#ifdef PLATFORM_POSIX
	chmod(path, S_IRWXU);
#endif
}

bool DeleteFolderDeep(const char *dir, bool rdonly)
{
	bool ok = true;
	{
		FindFile ff(AppendFileName(dir, "*.*"));
		while(ff) {
			String name = ff.GetName();
			String p = AppendFileName(dir, name);
			if(ff.IsFile() || ff.IsSymLink()) {
				if(ff.IsReadOnly() && rdonly)
					SetWritePermission(p);
				ok = ok && FileDelete(p);
			}
			else
			if(ff.IsFolder())
				ok = ok && DeleteFolderDeep(p, rdonly);
			ff.Next();
		}
	}
	SetWritePermission(dir);
	return ok && DirectoryDelete(dir);
}

String GetSymLinkPath(const char *linkpath)
{
#ifdef PLATFORM_WIN32
	String path;
	sGetSymLinkPath0(linkpath, &path);
	return path;
#else
	char buff[_MAX_PATH + 1];
	int len = readlink(linkpath, buff, _MAX_PATH);
	if(len > 0 && len < _MAX_PATH)
		return String(buff, len);
	return Null;
#endif
}

FileSystemInfo::FileInfo::FileInfo()
: length(Null), read_only(false), is_directory(false)
	, is_folder(false), is_file(false), is_symlink(false), is_archive(false)
	, is_compressed(false), is_hidden(false), is_read_only(false), is_system(false)
	, is_temporary(false), root_style(ROOT_NO_ROOT_DIR)
{}

FileSystemInfo& StdFileSystemInfo()
{
	static FileSystemInfo h;
	return h;
}

int FileSystemInfo::GetStyle() const
{
#ifdef PLATFORM_WIN32
	return STYLE_WIN32;
#endif
#ifdef PLATFORM_POSIX
	return STYLE_POSIX;
#endif
}

Array<FileSystemInfo::FileInfo> FileSystemInfo::Find(String mask, int max_count, bool unmounted) const
{
	Array<FileInfo> fi;
	if(IsNull(mask))
	{ // root
#ifdef PLATFORM_WINCE
		FileInfo& f = fi.Add();
		f.filename = "\\";
		f.root_style = ROOT_FIXED;
#elif defined(PLATFORM_WIN32)
		WCHAR drive[4] = L"?:\\";
		for(int c = 'A'; c <= 'Z'; c++) {
			*drive = c;
			int n = GetDriveTypeW(drive);
			if(n == DRIVE_NO_ROOT_DIR)
				continue;
			FileInfo& f = fi.Add();
			f.filename = drive;
			WCHAR name[256], system[256];
			DWORD d;
			if(c != 'A' && c != 'B' && n != DRIVE_UNKNOWN) {
				if(n == DRIVE_REMOTE) // if drive is not connected, GetVolumeInformation takes too long - not worth it
					f.root_desc = t_("Network drive");
				else {
					bool b = GetVolumeInformationW(drive, name, 256, &d, &d, &d, system, 256);
					if(b) {
						if(*name) f.root_desc << " " << FromSystemCharsetW(name);
					}
					else if(n == DRIVE_REMOVABLE || n == DRIVE_CDROM) {
						if(unmounted) {
							f.root_desc = t_("Empty drive");
						} else {
							fi.Drop();
							continue;
						}
					}
				}
			}
			switch(n)
			{
			default:
			case DRIVE_UNKNOWN:     f.root_style = ROOT_UNKNOWN; break;
			case DRIVE_NO_ROOT_DIR: f.root_style = ROOT_NO_ROOT_DIR; break;
			case DRIVE_REMOVABLE:   f.root_style = ROOT_REMOVABLE; break;
			case DRIVE_FIXED:       f.root_style = ROOT_FIXED; break;
			case DRIVE_REMOTE:      f.root_style = ROOT_REMOTE; break;
			case DRIVE_CDROM:       f.root_style = ROOT_CDROM; break;
			case DRIVE_RAMDISK:     f.root_style = ROOT_RAMDISK; break;
			}
		}
		
#elif defined(PLATFORM_POSIX)
		FileInfo& f = fi.Add();
		f.filename = "/";
		f.root_style = ROOT_FIXED;
#endif
	}
	else
	{
		FindFile ff;
		if(ff.Search(mask))
			do
			{
				FileInfo& f = fi.Add();
				f.filename = ff.GetName();
#ifndef PLATFORM_POSIX
				f.is_archive = ff.IsArchive();
				f.is_compressed = ff.IsCompressed();
				f.is_hidden = ff.IsHidden();
				f.is_system = ff.IsSystem();
				f.is_temporary = ff.IsTemporary();
#endif
				f.is_read_only = ff.IsReadOnly();
				f.length = ff.GetLength();
#ifdef PLATFORM_POSIX
				f.creation_time = ff.GetLastChangeTime();
				f.unix_mode = ff.GetMode();
#endif
				f.last_access_time = ff.GetLastAccessTime();
				f.last_write_time = ff.GetLastWriteTime();
#ifdef PLATFORM_WIN32
				f.creation_time = ff.GetCreationTime();
				f.unix_mode = 0;
#endif
				f.read_only = ff.IsReadOnly();
				f.is_directory = ff.IsDirectory();
				f.is_folder = ff.IsFolder();
				f.is_file = ff.IsFile();
#ifdef PLATFORM_POSIX
				f.is_symlink = ff.IsSymLink();
#endif
			}
			while(ff.Next() && fi.GetCount() < max_count);
	}
	return fi;
}

bool FileSystemInfo::CreateFolder(String path, String& error) const
{
	if(UPP::DirectoryCreate(path))
		return true;
	error = GetErrorMessage(GetLastError());
	return false;
}

bool FileSystemInfo::FolderExists(String path) const
{
	if(IsNull(path))
		return true;
	if(path.Find('*') >= 0 || path.Find('?') >= 0)
		return false;
	Array<FileInfo> fi = Find(path, 1);
	return !fi.IsEmpty() && fi[0].is_directory;
}

static void FindAllPaths_(Vector<String>& r, const String& dir, const char *patterns, dword opt)
{
	for(FindFile ff(dir + "/*.*"); ff; ff++) {
		String p = ff.GetPath();
		if(PatternMatchMulti(patterns, ff.GetName()) &&
		   ((opt & FINDALLFILES) && ff.IsFile() || (opt & FINDALLFOLDERS) && ff.IsFolder()))
			r.Add(ff.GetPath());
		if(ff.IsFolder())
			FindAllPaths_(r, ff.GetPath(), patterns, opt);
	}
}

Vector<String> FindAllPaths(const String& dir, const char *patterns, dword opt)
{
	Vector<String> r;
	FindAllPaths_(r, dir, patterns, opt);
	return r;
}

}
