/*
class Ftp {
	HINTERNET hinternet, hftp;

public:
	bool   Save(const char *path, String data, Gate2<int, int> progress = false);
	String Load(const char *path, Gate1<String> progress = false);
	bool   Rename(const char *oldpath, const char *newpath);
	bool   Cd(const char *path);
	bool   MkDir(const char *path);
	bool   Delete(const char *path);
	bool   Exists(const char *path);

	bool   Connect(const char *host, const char *user = NULL, const char *password = NULL);
	void   Close();

	String GetError();

	void   RealizePath(const char *path);

	void   ThrowError() throw(Exc);

	operator bool() const  { return hftp; }

	Ftp()                  { hinternet = hftp = NULL; }
	~Ftp();
};
*/

class UvsDiffEntry
{
public:
	UvsDiffEntry(int index = 0, int count = 0) : index(index), count(count) {}

	static bool    AddLine(Array<UvsDiffEntry>& list, String line);
	static void    AddStream(Array<UvsDiffEntry>& list,	Stream& stream);
	static void    Order(Array<UvsDiffEntry>& list);

public:
	int            index, count; // count < 0 = forced insert
	Vector<String> insert;

private:
	static int     Order(Array<UvsDiffEntry>& list, int blist, int elist, int bline, int eline);
};

class UvsDiffBlock
{
public:
	UvsDiffBlock();
	UvsDiffBlock(const UvsDiffBlock& b, int)
		: lines(b.lines, 0), newest_version(b.newest_version), line_index(b.line_index, 0) {}

	static void    SetInserts(Array<UvsDiffEntry>& elist, const Array<UvsDiffBlock>& file, int version_index);

	static void    AddStream(Array<UvsDiffBlock>& file, Stream& stream, int version_index);
	static void    AddVersion(Array<UvsDiffBlock>& file, Stream& stream, int version_index);
	static void    Merge(Array<UvsDiffBlock>& file);

public:
	Vector<String> lines;
	int            newest_version; // 0 = newest, 1 = one before that
	Vector<int>    line_index; // numbered beginning with 0 (C-style)

private:
	static void    Split(Array<UvsDiffBlock>& blocks, int index, int at_index);
};

bool           SaveFileBackup(String fn, String data, bool keep_backup = false);
bool           SaveFileTime(const char *filename, String data, Time time);
void           SplitPath(String path, String& dir, String& file);

//void           RealizeDirectory(String dir);
//void           RealizePath(String file);

String         NoCr(String in);
String         DoCr(String in, bool usecr);

bool           ParseVer(const char *p, int& filetime, String& author);
String         Diff(String newfile, String oldfile);
String         PrevVersion(String newfile, const char *diff, int *errln0 = NULL);
String         PrevVersion(String newfile, const char *verfile, int verindex, int *errln0 = NULL);
String         ConflictDiff(String org, String my, String rep);
String         AddPrevVersion(String rep, String ver, String add, String text);

Time           FromUvsTime(int time);
inline String  FormatUvsTime(int time)  { return Format(FromUvsTime(time)); }
int            ToUvsTime(Time tm);
inline bool    IsSameTime(int a, int b) { return tabs(b - a) <= 4; }

struct FileInfo {
	int    filetime;
	String author;
	bool   deleted;

	FileInfo(int filetime = Null, String author = Null, bool deleted = false)
	: filetime(filetime), author(author), deleted(deleted) {}
};

typedef ArrayMap<String, FileInfo> DirInfo;

DirInfo        ReadDir(String dir) throw(Exc);
String         AsString(const DirInfo& info);

struct FileVersion
{
	int    filetime;
	String author;
	String data;
};

typedef Array<FileVersion> FileVerInfo;

FileVerInfo    ReadVer(String verfile, const FileInfo& current, String curdata);
String         AsString(const FileVerInfo& verinfo);
bool           MergeVer(FileVerInfo& dest, const FileVerInfo& src);

struct UvsDataBlock
{
	String              Compress(Gate2<int, int> progress) const;
	void                Cleanup();

	static UvsDataBlock Decompress(String data, bool headers_only, Gate2<int, int> progress);
	static int          GetLength(String data, bool headers_only);

	String              temp_filename;
	DirInfo             dir;
	DirInfo             download;
	Vector<String>      data;
	Vector<String>      version;
	Vector<bool>        ignore;
};

struct UvsHostIndex
{
	UvsHostIndex() : last_update(0), last_full(0) {}

	String              Compress() const;
	static UvsHostIndex Decompress(String data);

	int                 last_update;
	int                 last_full;
};

enum SORT_ATTR { SORT_ACTION, SORT_PATH, SORT_NAME, SORT_EXT, SORT_TIME, SORT_SIZE };

struct ComplexFileInfo
{
	ComplexFileInfo();

	String           GetSortKey(int sort_method, bool is_download, int64 sort_size) const;

	bool             IsTree() const           { return !IsNull(tree_time); }
	bool             IsTreeNew() const;
	bool             IsTreeDeleted() const;
	bool             IsTreeEdit() const;
	bool             IsHostNew() const;
	bool             IsHostDeleted() const;
	bool             IsHostEdit() const;
	bool             IsTreeConflict() const   { return treeconflict; }
	bool             IsTimeConflict() const   { return timeconflict; }
	bool             IsNoConflict() const     { return noconflict; }
	bool             IsUpload() const;

	int              GetTimeModified() const  { return max(tree_time, local.filetime); }

//	const FileInfo&  Base() const             { return !IsNull(prev.filetime) ? prev : local; }
	const FileInfo&  Base() const             { return !IsNull(download.filetime) ? download : local; }

	String           path;
	int              tree_time;
	int64            tree_size;
	int64            host_size;
//	bool             prev_loaded;
	bool             treeconflict;
	bool             timeconflict;
	bool             ignorehost;
	bool             noconflict;
	FileInfo         local;
	FileInfo         download;
//	FileInfo         prev;
};

typedef ArrayMap<String, ComplexFileInfo> ComplexDirInfo;

void           SetTree(ComplexDirInfo& cfi, String dir, Gate2<int, int> progress);
void           SetLocal(ComplexDirInfo& cfi, const DirInfo& localinfo);
void           SetHost(ComplexDirInfo& cfi, const DirInfo& download);
//void           SetPrev(ComplexDirInfo& cfi, const DirInfo& hostinfo);

struct UvsJob {
	String dir;
	String excludedirs;
	String binaryfiles;
	String excludefiles;
	String author;
	String localrepository;
	String host;
	String user;
	String password;
	String hostrepository;
	String tree_backup;
	String local_backup;
	bool   usecr;
	bool   anonymous;
	bool   passive;
	bool   fullblock;

	void   Clear();
	bool   Read(String s);
	bool   Edit();
	String AsString() const;

	bool   IsExcluded(String dir, String file) const;
	bool   IsExcluded(String path) const;
	bool   IsBinary(String path) const;

	String NoCr(String data, String filename) const;
	String DoCr(String data, String filename) const;

	String GetTreePath(String relpath) const;
	String GetLocalPath(String relpath) const;

	String GetLocalDirPath() const { return GetLocalPath("uvs.$dir"); }
	String GetLocalIdxPath() const { return GetLocalPath("uvs.$idx"); }
	String GetLocalLogPath() const { return GetLocalPath("uvs.$log"); }

	UvsJob() { Clear(); }
};
