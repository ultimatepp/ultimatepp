
bool PatternMatch(const char *p, const char *s);
bool PatternMatchMulti(const char *p, const char *s);

const char  *GetFileNamePos(const char *path);
const char  *GetFileExtPos(const char *path);

bool    HasFileExt(const char *path);
bool    HasWildcards(const char *path);
bool    IsFullPath(const char *path);

String  GetFileDirectory(const char *path); // with DIR_SEP at the end
String  GetFileFolder(const char *path); // without DIR_SEP at the end, if not Win32 root
String  GetFileTitle(const char *path);
String  GetFileExt(const char *path);
String  GetFileName(const char *path);

String  AppendFileName(const String& path, const char *filename);

String WinPath(const char *path);
String UnixPath(const char *path);

#ifdef  PLATFORM_WIN32
inline String  NativePath(const char *path) { return WinPath(path); }
#endif

#ifdef  PLATFORM_POSIX
inline String  NativePath(const char *path) { return UnixPath(path); }
#endif

String  AppendExt(const char *path, const char *ext);
String  ForceExt(const char *path, const char *ext);

String  GetFileOnPath(const char *file, const char *paths, bool current = true, const char *curdir = NULL);

#ifndef PLATFORM_WINCE
String  GetFullPath(const char *path);
String  GetCurrentDirectory();
#endif

struct FileTime;

int Compare_FileTime(const FileTime& fa, const FileTime& fb);

#ifdef PLATFORM_WIN32

struct FileTime : FILETIME, CompareRelOps<const FileTime&, &Compare_FileTime> {
	FileTime()                          {}
	FileTime(const FILETIME& ft)        { dwLowDateTime = ft.dwLowDateTime;
	                                      dwHighDateTime = ft.dwHighDateTime; }
};

class  FindFile : NoCopy {
	WIN32_FIND_DATA  *a;
	WIN32_FIND_DATAW *w;
	HANDLE            handle;
	String            pattern;
	String            path;

	void        Init();
	bool        Next0();
	void        Close();

public:
	bool        Search(const char *path);
	bool        Next();

	dword       GetAttributes() const;
	String      GetName() const;
	String      GetPath() const;
	int64       GetLength() const;
	FileTime    GetCreationTime() const;
	FileTime    GetLastAccessTime() const;
	FileTime    GetLastWriteTime() const;

	bool        IsDirectory() const;
	bool        IsFolder() const;
	bool        IsFile() const           { return !IsDirectory(); }
	bool        IsSymLink() const;
	bool        IsExecutable() const;

	bool        IsArchive() const;
	bool        IsCompressed() const;
	bool        IsHidden() const;
	bool        IsReadOnly() const;
	bool        IsSystem() const;
	bool        IsTemporary() const;

	operator    bool() const             { return handle != INVALID_HANDLE_VALUE; }

	FindFile();
	FindFile(const char *name);
	~FindFile();
};

#endif

#ifdef PLATFORM_POSIX

struct FileTime : CompareRelOps<const FileTime&, &Compare_FileTime>
{
	FileTime() {}
	FileTime(time_t ft) : ft(ft) {}

	operator time_t () const { return ft; }

	time_t ft;
};

inline int Compare_FileTime(const FileTime& f, const FileTime& g) { return f.ft < g.ft ? -1 : f.ft > g.ft ? 1 : 0; }

class FindFile : NoCopy {
	bool           file;
	DIR           *dir;
	mutable bool   statis;
	mutable struct stat statf;
	String         path;
	String         name;
	String         pattern;

	struct stat &Stat() const;
	bool CanMode(dword usr, dword grp, dword oth) const;

public:
	bool        Search(const char *name);
	bool        Next();
	void        Close();

	dword       GetMode() const           { return Stat().st_mode; }
	String      GetName() const           { return name; }
	String      GetPath() const;
	int64       GetLength() const         { return Stat().st_size; }
	FileTime    GetLastChangeTime() const { return Stat().st_ctime; }
	FileTime    GetLastAccessTime() const { return Stat().st_atime; }
	FileTime    GetLastWriteTime() const  { return Stat().st_mtime; }

	uid_t       GetUid()                  { return Stat().st_uid; }
	gid_t       GetGid()                  { return Stat().st_gid; }

	bool        CanRead() const           { return CanMode(S_IRUSR, S_IRGRP, S_IROTH); }
	bool        CanWrite() const          { return CanMode(S_IWUSR, S_IWGRP, S_IWOTH); }
	bool        CanExecute() const        { return CanMode(S_IXUSR, S_IXGRP, S_IXOTH); }

	bool        IsReadOnly() const        { return CanRead() && !CanWrite(); }

	bool        IsHidden() const          { return *name == '.'; }
	bool        IsDirectory() const       { return S_ISDIR(GetMode()); }
	bool        IsFolder() const;
	bool        IsFile() const            { return S_ISREG(GetMode()); }
	bool        IsSymLink() const;
	bool        IsExecutable() const;

	operator    bool() const              { return file; }

	FindFile()                            { file = false; dir = NULL; }
	FindFile(const char *name);
	~FindFile()                           { Close(); }
};

// POSIX FileTime is unfortunately long int and clashes with Date::operator int()
inline bool operator==(Time a, FileTime b) { return a == Time(b); }
inline bool operator!=(Time a, FileTime b) { return a != Time(b); }

inline bool operator==(FileTime a, Time b) { return Time(a) == b; }
inline bool operator!=(FileTime a, Time b) { return Time(a) != b; }

#endif

int64       GetFileLength(const char *path);
bool        FileExists(const char *path);
bool        DirectoryExists(const char *path);

struct Time;
FileTime    GetFileTime(const char *path);
Time        FileGetTime(const char *path);
bool        SetFileTime(const char *path, FileTime ft);
bool        FileSetTime(const char *path, Time time);
FileTime    TimeToFileTime(Time time);

bool        FileCopy(const char *oldpath, const char *newpath);
bool        FileMove(const char *oldpath, const char *newpath);
bool        FileDelete(const char *path);

#ifdef PLATFORM_POSIX
bool        DirectoryCreate(const char *path, int mode = 0755);
bool        RealizeDirectory(const String& path, int mode = 0755);
bool        RealizePath(const String& path, int mode = 0755);
#else
bool        DirectoryCreate(const char *path);
bool        RealizeDirectory(const String& path);
bool        RealizePath(const String& path);
#endif

bool        DirectoryDelete(const char *path);

String      NormalizePath(const char *path, const char *currdir);
String      NormalizePath(const char *path);

bool        PathIsEqual(const char *p1, const char *p2);

#ifdef PLATFORM_POSIX
inline bool DeleteFile(const char *fn)      { return unlink(fn) == 0; }
#endif

bool    DeleteFolderDeep(const char *dir);

#ifndef PLATFORM_WINCE
String  GetTempPath();
String  GetTempFileName(const char *prefix = NULL);
#endif

String GetSymLinkPath(const char *linkpath);

template <class T>
class Array;

class FileSystemInfo {
public:
	enum
	{
		ROOT_UNKNOWN     = 0,
		ROOT_NO_ROOT_DIR = 1,
		ROOT_REMOVABLE   = 2,
		ROOT_FIXED       = 3,
		ROOT_REMOTE      = 4,
		ROOT_CDROM       = 5,
		ROOT_RAMDISK     = 6,
		ROOT_NETWORK     = 7,
		ROOT_COMPUTER    = 8,
	};

	enum
	{
		STYLE_WIN32      = 0x0001,
		STYLE_POSIX      = 0x0002,
	};

	struct FileInfo
	{
		FileInfo();

		operator bool () const { return !IsNull(filename); }

		String filename;
		String msdos_name;
		String root_desc;
		int64  length;
		Time   last_access_time;
		Time   last_write_time;
		Time   creation_time;
		bool   read_only;
		bool   is_directory;
		bool   is_folder;
		bool   is_file;
		bool   is_symlink;
		bool   is_archive;
		bool   is_compressed;
		bool   is_hidden;
		bool   is_read_only;
		bool   is_system;
		bool   is_temporary;
		char   root_style;
		dword  unix_mode;
	};

	virtual int             GetStyle() const;
	bool                    IsWin32() const { return GetStyle() & STYLE_WIN32; }
	bool                    IsPosix() const { return GetStyle() & STYLE_POSIX; }

	virtual Array<FileInfo> Find(String mask, int max_count = 1000000, bool unmounted = false) const; // mask = Null -> root
	virtual bool            CreateFolder(String path, String& error) const;

	bool                    FolderExists(String path) const;

	virtual ~FileSystemInfo() {}
};

FileSystemInfo& StdFileSystemInfo();

#ifdef PLATFORM_WIN32

class NetNode : Moveable<NetNode> {
	NETRESOURCE net;
	String      local, remote, comment, provider;

	String      name;
	String      path;

	static void           Copy(String& t, char *s);
	static Array<NetNode> Enum0(HANDLE hEnum);
	static void           SetPtr(String& s, char *& ptr);

	void SetPtrs();

public:
	enum {
		UNKNOWN, NETWORK, GROUP, SERVER, SHARE
	};
	String         GetName() const     { return name; }
	String         GetPath() const     { return path; }
	int            GetDisplayType() const;
	String         GetRemote() const   { return remote; }
	String         GetLocal() const    { return local; }
	String         GetProvider() const { return provider; }
	String         GetComment() const  { return comment; }
	Array<NetNode> Enum() const;

	void           Serialize(Stream& s);

	static Array<NetNode> EnumRoot();
	static Array<NetNode> EnumRemembered();

	NetNode();
	NetNode(const NetNode& s)         { *this = s; }

	NetNode& operator=(const NetNode& s);
};

#endif
