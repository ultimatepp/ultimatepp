#include "Core.h"
//#BLITZ_APPROVE

#ifdef PLATFORM_POSIX
#include <sys/types.h>
#include <utime.h>
#endif//PLATFORM_POSIX

#ifdef PLATFORM_WIN32
#define DLLFILENAME "Kernel32.dll"
#define DLIMODULE   UnicodeWin32
#define DLIHEADER   <Core/Kernel32W.dli>
#include <Core/dli_source.h>

#define DLLFILENAME "Mpr.dll"
#define DLIMODULE   UnicodeWin32Net
#define DLIHEADER   <Core/Mpr32W.dli>
#include <Core/dli_source.h>
#endif

NAMESPACE_UPP

static int sDirSep(int c) {
	return c == '/' || c == '\\' ? c : 0;
}

static bool strecmp0(const char *p, const char *s) {
	while(*p) {
		if(*p == '*') {
			while(*p == '*') p++;
			do
				if(*p == *s && strecmp0(p, s)) return true;
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

bool HasFileName(const char *fileName) {
	return *GetFileNamePos(fileName);
}

bool HasFileExt(const char *fileName) {
	return *GetFileExtPos(fileName);
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
	if(result.GetLength() && *result.Last() != DIR_SEP)
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

int ComparePath(const char *a, const char *b, int length) {
	ASSERT(length >= 0);
	if(length <= 0)
		return 0;
#if PLATFORM_PATH_HAS_CASE
	return memcmp(a, b, length);
#else
	return MemICmp(a, b, length);
#endif
}

int ComparePath(String fa, String fb) {
	int la = fa.GetLength(), lb = fb.GetLength();
	int r = ComparePath(fa, fb, min(la, lb));
	return r ? r : cmp(la, lb);
}

#ifndef PLATFORM_WINCE
String GetCurrentDirectory() {
#if defined(PLATFORM_WIN32)
	if(IsWinNT()) {
		wchar h[MAX_PATH];
		UnicodeWin32().GetCurrentDirectoryW(MAX_PATH, h);
		return FromSystemCharsetW(h);
	}
	else {
		char h[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH, h);
		return FromSystemCharset(h);
	}
#elif defined(PLATFORM_POSIX)
	char h[1024];
	getcwd(h, 1024);
	return FromSystemCharset(h);
#else
#error GetCurrentDirectory not implemented for this platform, comment this line to get Null
	return Null;
#endif//PLATFORM
}
#endif

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE)

String GetTempPath()
{
	if(IsWinNT()) {
		wchar h[MAX_PATH];
		UnicodeWin32().GetTempPathW(MAX_PATH, h);
		return FromSystemCharsetW(h);
	}
	else {
		char h[MAX_PATH];
		::GetTempPath(MAX_PATH, h);
		return FromSystemCharset(h);
	}
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

#ifndef PLATFORM_WINCE
String GetFullPath(const char *file) {
#ifdef PLATFORM_WIN32
	if(IsWinNT()) {
		String ufn = FromUnixName(file);
		wchar h[MAX_PATH];
		UnicodeWin32().GetFullPathNameW(ToSystemCharsetW(ufn), MAX_PATH, h, 0);
		return FromSystemCharsetW(h);
	}
	else {
		String ufn = FromUnixName(file);
		char h[MAX_PATH];
		GetFullPathName(ToSystemCharset(ufn), MAX_PATH, h, 0);
		return FromSystemCharset(h);
	}
#else
	return NormalizePath(file);
#endif
}
#endif

String GetFileOnPath(const char* file, const char* paths, bool current, const char *curdir) {
	String ufn = NativePath(file);
	if(IsFullPath(ufn))
		return ufn;
	String fn;
#ifdef PLATFORM_WINCE
	if(current && curdir && FileExists(fn = NormalizePath(ufn, curdir)))
		;
#else
	String cd = curdir;
	if(!curdir)
		cd = GetCurrentDirectory();
	if(current && FileExists(fn = NormalizePath(ufn, cd)))
		;
#endif
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

void FindFile::Init()
{
	if(IsWinNT()) {
		w = new WIN32_FIND_DATAW;
		a = NULL;
	}
	else {
		a = new WIN32_FIND_DATA;
		w = NULL;
	}
}

FindFile::~FindFile()
{
	Close();
	delete a;
	delete w;
}

FindFile::FindFile()
{
	Init();
	handle = INVALID_HANDLE_VALUE;
}

FindFile::FindFile(const char *name) {
	Init();
	handle = INVALID_HANDLE_VALUE;
	Search(name);
}

bool FindFile::Search(const char *name) {
	Close();
	if(w)
		handle = UnicodeWin32().FindFirstFileW(ToSystemCharsetW(name), w);
	else
		handle = FindFirstFile(ToSystemCharset(name), a);
	return handle != INVALID_HANDLE_VALUE;
}

void FindFile::Close() {
	if(handle != INVALID_HANDLE_VALUE) FindClose(handle);
	handle = INVALID_HANDLE_VALUE;
}

bool FindFile::Next() {
	if(w) {
		if(!UnicodeWin32().FindNextFileW(handle, w)) {
			Close();
			return false;
		}
	}
	else {
		if(!FindNextFile(handle, a)) {
			Close();
			return false;
		}
	}
	return true;
}

dword FindFile::GetAttributes() const
{
	return w ? w->dwFileAttributes : a->dwFileAttributes;
}

String FindFile::GetName() const
{
	return w ? FromSystemCharsetW(w->cFileName) : FromSystemCharset(a->cFileName);
}

int64 FindFile::GetLength() const
{
	if(w)
		return (int64)w->nFileSizeLow | ((int64)w->nFileSizeHigh << 32);
	else
		return (int64)a->nFileSizeLow | ((int64)a->nFileSizeHigh << 32);
}

FileTime FindFile::GetCreationTime() const
{
	return w ? w->ftCreationTime : a->ftCreationTime;
}

FileTime FindFile::GetLastAccessTime() const
{
	return w ? w->ftLastAccessTime : a->ftLastAccessTime;
}

FileTime FindFile::GetLastWriteTime() const
{
	return w ? w->ftLastWriteTime : a->ftLastWriteTime;
}

bool FindFile::IsDirectory() const
{
	return w ? w->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
	         : a->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}


bool FindFile::IsFolder() const {
	if(w)
		return IsDirectory()
			&& !(w->cFileName[0] == '.' && w->cFileName[1] == 0)
			&& !(w->cFileName[0] == '.' && w->cFileName[1] == '.' && w->cFileName[2] == 0);
	else
		return IsDirectory()
			&& !(a->cFileName[0] == '.' && a->cFileName[1] == 0)
			&& !(a->cFileName[0] == '.' && a->cFileName[1] == '.' && a->cFileName[2] == 0);
}

bool FindFile::IsArchive() const
{
	return w ? w->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE
	         : a->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

bool FindFile::IsCompressed() const
{
	return w ? w->dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED
	         : a->dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED;
}

bool FindFile::IsHidden() const
{
	return w ? w->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN
	         : a->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
}

bool FindFile::IsReadOnly() const
{
	return w ? w->dwFileAttributes & FILE_ATTRIBUTE_READONLY
	         : a->dwFileAttributes & FILE_ATTRIBUTE_READONLY;
}

bool FindFile::IsSystem() const
{
	return w ? w->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM
	         : a->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM;
}

bool FindFile::IsTemporary() const
{
	return w ? w->dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY
	         : a->dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY;
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
			stat(AppendFileName(path, name), &statf);
	}
	return statf;
}

bool FindFile::Next() {
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
	path = GetFileDirectory(fn);
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
	Vector<String> si = Split(path, CharFilterTextTest(sDirSep));
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
		p = Split(currdir, CharFilterTextTest(sDirSep));
	}
	bool quote = false;
	for(; i < si.GetCount(); i++) {
		String s = si[i];
		if(s != "." && !s.IsEmpty())
			if(s[0] == '.' && s[1] == '.') {
				if(!p.IsEmpty()) p.Drop();
			}
			else {
				p.Add(s);
				if(s.Find(' ') >= 0)
					quote = true;
			}
	}
	out.Cat(Join(p, DIR_SEPS));
//	if(quote)
//		return '\"' + out + '\"';
//	else
		return out;
}

#endif//PLATFORM_POSIX

bool FileExists(const char *name) {
	FindFile ff(name);
	return ff && ff.IsFile();
}

int64 GetFileLength(const char *name) {
	FindFile ff(name);
	return ff ? ff.GetLength() : -1;
}

bool DirectoryExists(const char *name) {
	FindFile ff(name);
	return ff && ff.IsDirectory();
}

bool IsFolder(String path)
{
	if(IsNull(path) || *path.Last() == '\\' || *path.Last() == '/' || *path.Last() == ':')
		return true;
	if(path.Find('?') >= 0 || path.Find('*') >= 0)
		return false;
	FindFile ff(path);
	return ff && ff.IsDirectory();
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
	if(IsWinNT())
		return UnicodeWin32().CopyFileW(ToSystemCharsetW(oldname), ToSystemCharsetW(newname), false);
	else
		return CopyFile(ToSystemCharset(oldname), ToSystemCharset(newname), false);
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
	FileSetTime(newname, FileGetTime(oldname));
	return true;
#else
	#error
#endif//PLATFORM
}

bool FileMove(const char *oldname, const char *newname)
{
#if defined(PLATFORM_WIN32)
	if(IsWinNT())
		return !!UnicodeWin32().MoveFileW(ToSystemCharsetW(oldname), ToSystemCharsetW(newname));
	else
		return !!MoveFile(ToSystemCharset(oldname), ToSystemCharset(newname));
#elif defined(PLATFORM_POSIX)
	return !rename(ToSystemCharset(oldname), ToSystemCharset(newname));
#else
	#error
#endif//PLATFORM
}

bool FileDelete(const char *filename)
{
#if defined(PLATFORM_WIN32)
	if(IsWinNT())
		return !!UnicodeWin32().DeleteFileW(ToSystemCharsetW(filename));
	else
		return !!DeleteFile(ToSystemCharset(filename));
#elif defined(PLATFORM_POSIX)
	return !unlink(ToSystemCharset(filename));
#else
	#error
#endif//PLATFORM
}

bool DirectoryDelete(const char *dirname)
{
#if defined(PLATFORM_WIN32)
	if(IsWinNT())
		return !!UnicodeWin32().RemoveDirectoryW(ToSystemCharsetW(dirname));
	else
		return !!RemoveDirectory(ToSystemCharset(dirname));
#elif defined(PLATFORM_POSIX)
	return !rmdir(ToSystemCharset(dirname));
#else
	#error
#endif//PLATFORM
}

#ifdef PLATFORM_WIN32
int Compare_FileTime(const FileTime& fa, const FileTime& fb)
{
	return CompareFileTime(&fa, &fb);
}
#endif

Time FileGetTime(const char *filename)
{
#if defined(PLATFORM_WIN32)
	HANDLE handle;
	if(IsWinNT())
		handle = UnicodeWin32().CreateFileW(ToSystemCharsetW(filename), GENERIC_READ,
		                    FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	else
		handle = CreateFile(ToSystemCharset(filename), GENERIC_READ,
		                    FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(handle == INVALID_HANDLE_VALUE)
		return Null;
	FileTime ft;
	bool res = GetFileTime(handle, 0, 0, &ft);
	CloseHandle(handle);
	return res ? Time(ft) : Time(Null);
#elif defined(PLATFORM_POSIX)
	struct stat st;
	if(stat(ToSystemCharset(filename), &st))
		return Null;
	return Time(st.st_mtime);
#else
	#error
#endif//PLATFORM
}

bool SetFileTime(const char *filename, FileTime ft)
{
#if defined(PLATFORM_WIN32)
	HANDLE handle;
	if(IsWinNT())
		handle = UnicodeWin32().CreateFileW(ToSystemCharsetW(filename), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
	else
		handle = CreateFile(ToSystemCharset(filename), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
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
	t.tm_sec  = time.second;
	t.tm_min  = time.minute;
	t.tm_hour = time.hour;
	t.tm_mday = time.day;
	t.tm_mon  = time.month - 1;
	t.tm_year = time.year - 1900;
	return mktime(&t);
#endif
}

#ifdef PLATFORM_POSIX
bool DirectoryCreate(const char *path, int mode)
{
	return ::mkdir(ToSystemCharset(path), mode) == 0;
}
#else
bool DirectoryCreate(const char *path)
{
	if(IsWinNT())
		return !!UnicodeWin32().CreateDirectoryW(ToSystemCharsetW(path), 0);
	else
		return !!CreateDirectory(ToSystemCharset(path), 0);
}
#endif

#ifdef PLATFORM_WIN32
#define DIR_MIN 3 //!! wrong! what about \a\b\c ?
#endif

#ifdef PLATFORM_POSIX
#define DIR_MIN 1
#endif

void RealizeDirectory(String dir)
{
	if(dir.GetLength() > DIR_MIN) {
		RealizeDirectory(GetFileFolder(dir));
		if(!DirectoryExists(dir))
			DirectoryCreate(dir);
	}
}

void RealizePath(String file)
{
	RealizeDirectory(GetFileFolder(file));
}

bool DeleteFolderDeep(const char *dir)
{
	{
		FindFile ff(AppendFileName(dir, "*.*"));
		while(ff) {
			String name = ff.GetName();
			String p = AppendFileName(dir, name);
			if(ff.IsFile())
				FileDelete(p);
			else
			if(ff.IsFolder())
				DeleteFolderDeep(p);
			ff.Next();
		}
	}
	return DirectoryDelete(dir);
}

FileSystemInfo::FileInfo::FileInfo()
: length(Null), read_only(false), is_directory(false)
	, is_folder(false), is_file(false), is_symlink(false), is_archive(false)
	, is_compressed(false), is_hidden(false), is_read_only(false), is_system(false)
	, is_temporary(false), root_style(ROOT_NO_ROOT_DIR)
{}

GLOBAL_VAR(FileSystemInfo, StdFileSystemInfo)

int FileSystemInfo::GetStyle() const
{
#ifdef PLATFORM_WIN32
	return STYLE_WIN32;
#endif
#ifdef PLATFORM_POSIX
	return STYLE_POSIX;
#endif
}

Array<FileSystemInfo::FileInfo> FileSystemInfo::Find(String mask, int max_count) const
{
	Array<FileInfo> fi;
	if(IsNull(mask))
	{ // root
#ifdef PLATFORM_WINCE
		FileInfo& f = fi.Add();
		f.filename = "\\";
		f.root_style = ROOT_FIXED;
#elif defined(PLATFORM_WIN32)
		char drive[4] = "?:\\";
		for(int c = 'A'; c <= 'Z'; c++) {
			*drive = c;
			int n = GetDriveType(drive);
			if(n == DRIVE_NO_ROOT_DIR/* || IsWin32() && *drive == 'B'*/) continue;
			FileInfo& f = fi.Add();
			f.filename = drive;
			char name[256], system[256];
			DWORD d;
			if(c != 'A' && c != 'B' && n != DRIVE_REMOTE && n != DRIVE_UNKNOWN) {
				bool b = GetVolumeInformation(drive, name, 256, &d, &d, &d, system, 256);
				if(b) {
				   	if(*name) f.root_desc << " " << FromSystemCharset(name);
				   	if(*system) f.root_desc << " [ " << FromSystemCharset(system) << " ]";
				}
				else
				if(n == DRIVE_REMOVABLE) {
					fi.Drop();
					continue;
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

END_UPP_NAMESPACE
