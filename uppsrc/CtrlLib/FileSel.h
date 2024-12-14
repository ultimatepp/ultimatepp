#define FILESELVERSION 2

Image GetDriveImage(char drive_style);
Image GetFileIcon(const char *path, bool dir, bool force = false);

void DrawFileName(Draw& w, int x, int y, int wcx, int cy, const WString& mname, bool isdir, Font font,
                  Color ink, Color extink, const WString& desc = Null, Font descfont = Null,
                  bool justname = false, Color underline = Null);

Image NativePathIcon(const char *path, bool folder);
Image NativePathIcon(const char *path);

Image NativePathLargeIcon(const char *path, bool folder);
Image NativePathLargeIcon(const char *path);

class FileList : public ColumnList, private Display {
public:
	virtual void  LeftDown(Point p, dword);
	virtual bool  Key(dword key, int count);

	virtual void  Paint(Draw& w, const Rect& r, const Value& q,
		                Color ink, Color paper, dword style) const;
	virtual Size  GetStdSize(const Value& q) const;

public:
	struct File {
		bool   isdir;
		bool   unixexe;
		bool   hidden;
		Image  icon;
		String name;
		Font   font;
		Color  ink;
		int64  length;
		Time   time;
		Color  extink;
		String desc;
		Font   descfont;
		Value  data;
		Color  underline;

		operator const String&() const { return name; }
	};

	class Order {
	public:
		virtual bool operator()(const File& a, const File& b) const = 0;
		virtual ~Order() {}
	};

private:
	int            iconwidth;
	EditField      edit;

	bool           renaming;
	bool           justname;
	bool           accelkey;
	bool           selectdir;

	void        UpdateSelect();
	void        Update();
	bool        FindChar(int from, int chr);
	void        KillEdit();

	struct FileOrder;

	using ColumnList::GetStdSize;
	using ColumnList::Paint;

protected:
	enum {
		TIMEID_STARTEDIT = ColumnList::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	Event<const String&, const String&> WhenRename;

	void        StartEdit();
	void        EndEdit();
	bool        IsEdit() const                       { return edit.IsVisible(); }
	void        OkEdit();

	const File& Get(int i) const;
	const File& operator[](int i) const              { return Get(i); }
	void        Insert(int ii,
	                const String& name, const Image& icon = Null,
		            Font font = StdFont(), Color ink = SColorText(),
	                bool isdir = false, int64 length = 0, Time time = Null, Color extink = Null,
	                const String& desc = Null, Font descfont = Null, Value data = Null,
	                Color underline = Null, bool unixexe = false, bool hidden = false);
	void        Set(int ii,
	                const String& name, const Image& icon = Null,
		            Font font = StdFont(), Color ink = SColorText(),
	                bool isdir = false, int64 length = 0, Time time = Null, Color extink = Null,
	                const String& desc = Null, Font descfont = Null, Value data = Null,
	                Color underline = Null, bool unixexe = false, bool hidden = false);
	void        Set(int ii, const File& f);
	void        SetIcon(int ii, const Image& icon);
	void        Add(const File& f);
	void        Add(const String& name, const Image& icon = Null,
		            Font font = StdFont(), Color ink = SColorText(),
	                bool isdir = false, int64 length = 0, Time time = Null, Color extink = Null,
	                const String& desc = Null, Font descfont = Null, Value data = Null,
	                Color underline = Null, bool unixexe = false, bool hidden = false);
	String      GetCurrentName() const;

	int         Find(const char *name);
	bool        FindSetCursor(const char *name);

	void        Sort(const Order& order);

	FileList&   IconWidth(int w)                     { iconwidth = w; Refresh(); return *this; }
	int         GetIconWidth() const                 { return iconwidth; }
	FileList&   Renaming(bool b = true)              { renaming = b; return *this; }
	FileList&   JustName(bool b = true)              { justname = b; Refresh(); return *this; }
	FileList&   AccelKey(bool b = true)              { accelkey = b; return *this; }
	FileList&   SelectDir(bool b = true)             { selectdir = b; return *this; }

	typedef FileList CLASSNAME;

	FileList();
	virtual ~FileList();
};

bool Load(FileList& list, const String& dir, const char *patterns, bool dirs = false,
          Event<bool, const String&, Image&> WhenIcon = Null,
          FileSystemInfo& filesystem = StdFileSystemInfo(), const String& search = String(),
          bool hidden = true, bool hiddenfiles = true, bool lazyicons = false);


enum {
	FILELISTSORT_NAME,
	FILELISTSORT_EXT,
	FILELISTSORT_TIME,
	FILELISTSORT_SIZE,
	FILELISTSORT_DESCENDING = 0x8000,
};

void SortBy(FileList& list, int kind);

// deprecated, use Sort above
inline void SortByName(FileList& list) { SortBy(list, FILELISTSORT_NAME); }
inline void SortByExt(FileList& list) { SortBy(list, FILELISTSORT_EXT); }
inline void SortByTime(FileList& list) { SortBy(list, FILELISTSORT_TIME); }

#ifdef GUI_WIN
// Helper class for lazy (using aux thread) evaluation of .exe icons in Win32
class LazyExeFileIcons {
	TimeCallback tm;
	String       dir;
	FileList    *list;
	int          pos;
	Vector<int>  ndx;
	Event<bool, const String&, Image&> WhenIcon;

	Mutex  mutex;

	void   Do();
	void   Restart(int delay)                 { tm.KillSet(delay, callback(this, &LazyExeFileIcons::Do)); }
	WString Path();
	void   Done(Image img);

public:
	void ReOrder();
	void Start(FileList& list_, const String& dir_, Event<bool, const String&, Image&> WhenIcon_);
};
#endif

String DirectoryUp(String& dir, bool basedir = false);

String FormatFileSize(int64 n);

class FileSel : public WithFileSelectorLayout<TopWindow> {
public:
	virtual bool Key(dword key, int count);
	virtual void Activate();
	virtual void Skin();

private:
	SizeGrip    sizegrip;

	Button      dirup, mkdir;

protected:
	Vector<String> mask;
	Vector<String> path;
	Vector<String> lru;
	Vector<String> fw;
	int            lastsby;
	FileSystemInfo *filesystem;

	String         olddir;
	String         basedir;
	String         activetype;
	String         defext;
	String         preselect;
	String         default_name;
	Vector<String> fn;

#ifdef PLATFORM_WIN32
	static bool netroot_loaded;
	static Array<NetNode> netroot;
	Array<NetNode> netstack;
	Array<NetNode> netnode;
#endif

#ifdef GUI_WIN
	LazyExeFileIcons  lazyicons;
#endif

	DisplayCtrl    preview_display;
	Ctrl          *preview;
	FileList       list;
	ArrayCtrl      places;

	enum {
		OPEN, SAVEAS, SELECTDIR
	};

	int         mode;
	bool        asking;
	bool        multi;
	bool        rdonly;
	bool        bidname;
	bool        appmodal;
	bool        loaded;
	bool        noexeicons;

	Ctrl       *file_ctrl = NULL;
	int         file_ctrl_cx;

	bool          force_ext = true; // -> false if there is wildcard mask allowing set of exts
	Index<String> allowed_ext; // allowed extensions typed by user if force_ext

	static StaticMutex li_mutex;
	static void      (*li_current)(const String& path, Image& result);
	static String      li_path;
	static Image       li_result;
	static bool        li_running;
	static int         li_pos;
	TimeCallback       li_tm;

	bool        loading_network = false;
	
	static void LIThread();
	String      LIPath();
	void        StartLI();
	void        DoLI();
	void        ScheduleLI()                                 { li_tm.KillSet(0, THISBACK(DoLI)); }

	void        LoadNet();
	void        SelectNet();
	bool        PatternMatch(const char *fn);
	bool        OpenItem();
	void        OpenItem2()                                  { OpenItem(); }
	void        Open();
	void        DirUp();
	void        MkDir();
	void        Reload();
	void        Update();
	void        FileUpdate();
	void        Rename(const String& on, const String& nn);
	void        Choice();
	void        SearchLoad();
	void        Load();
	String      FilePath(const String& fn);
	void        SetDir(const String& dir);
	String      GetDir() const;
	void        AddName(Vector<String>& fn, String& o);
	String      ResolveLnk(const String& name) const;
	String      ResolveLnkDir(const String& name) const;
	String      ResolveLnkFile(const String& name) const;
	void        Finish();
	bool        Execute(int mode);
	bool        IsMulti()                                     { return multi && mode == OPEN; }
	void        SyncSplitter();
	void        InitSplitter();
	String      GetMask();
	void        GoToPlace();
	void        AddPlaceRaw(const String& path, const Image& m, const String& name, const char* group = NULL, int row = -1);
	void        AddSystemPlaces(int row = -1);
#ifdef PLATFORM_WIN32
	bool        ScanNetwork(Function<Array<NetNode> ()> fn);
	void        ScanNetworkRoot();
#endif

	using       WithFileSelectorLayout<TopWindow>::Title;

	typedef FileSel CLASSNAME;

public:
	static bool IsLnkFile(const String& p);

	Event<bool, const String&, Image&> WhenIcon;
	void (*WhenIconLazy)(const String& path, Image& result);

	void        Serialize(Stream& s);

	bool        ExecuteOpen(const char *title = NULL);
	bool        ExecuteSaveAs(const char *title = NULL);

	bool        ExecuteSelectDir(const char *title = NULL);

	String Get() const                           { return GetFile(0); }
	void   Set(const String& s);
	void   Set(const Vector<String>& s)          { fn <<= s; bidname = true; }

	operator String() const                      { return Get(); }
	void operator=(const String& s)              { Set(s); }

	String operator~() const                     { return Get(); }
	void operator<<=(const String& s)            { Set(s); }

	int    GetCount() const                      { return fn.GetCount(); }
	String GetFile(int i) const;
	String operator[](int i) const               { return GetFile(i); }
	void   ClearFiles()                          { fn.Clear(); }

	bool   GetReadOnly() const                   { return readonly; }
	String GetActiveDir() const                  { return dir.GetData(); }
	int    GetActiveType() const                 { return type.GetIndex(); }

	void   Filesystem(FileSystemInfo& fsys)      { filesystem = &fsys;}
	FileSystemInfo& GetFilesystem() const        { return *filesystem; }

	FileSel& Type(const char *name, const char *ext);
	FileSel& Types(const char *d);
	FileSel& AllFilesType();
	FileSel& ClearTypes();
	FileSel& ActiveDir(const String& d)          { dir <<= d; return *this; }
	FileSel& ActiveType(int i);
	FileSel& PreSelect(const String& path);
	FileSel& DefaultExt(const char *ext)         { defext = ext; return *this; }
	FileSel& Sort(int kind)						 { sortby.SetData(kind); return *this; }
	FileSel& Columns(int n)						 { list.Columns(n); return *this; }
	FileSel& Multi(bool b = true)                { multi = b; return *this; }
	FileSel& ReadOnlyOption(bool b = true)       { rdonly = b; return *this; }
	FileSel& MkDirOption(bool b = true)          { mkdir.Show(b); return *this; }
	FileSel& NoMkDirOption()                     { return MkDirOption(false); }
	FileSel& BaseDir(const char *dir)            { basedir = dir; return *this; }
	FileSel& Asking(bool b = true)               { asking = b; return *this; }
	FileSel& NoAsking()                          { return Asking(false); }
	FileSel& EditFileName(bool b)                { file.SetEditable(b); return *this; }
	FileSel& NoEditFileName()                    { return EditFileName(false); }
	FileSel& AppModal(bool b = true)             { appmodal = b; return *this; }
	FileSel& NoAppModal()                        { return AppModal(false); }
	FileSel& Preview(Ctrl& ctrl);
	FileSel& Preview(const Display& d);
	FileSel& ClearPlaces();
	FileSel& AddPlace(const String& path, const Image& m, const String& name, const char* group = NULL, int row = -1);
	FileSel& AddPlace(const String& path, const String& name, const char* group = NULL, int row = -1);
	FileSel& AddPlace(const String& path, const char* group = NULL, int row = -1);
	FileSel& AddPlaceSeparator();
	FileSel& AddStandardPlaces();
	FileSel& FileCtrl(Ctrl& ext, int cx)         { file_ctrl = &ext; file_ctrl_cx = cx; return *this; }
	FileSel& FileCtrl(Ctrl& ext)                 { return FileCtrl(ext, ext.GetMinSize().cx); }
	FileSel& DefaultName(const String& s)        { default_name = s; return *this; }
	FileSel& NoExeIcons(bool b = true)           { noexeicons = b; return *this; }

	FileSel();
	virtual ~FileSel();
};

String SelectFileOpen(const char *types);
String SelectFileSaveAs(const char *types);
String SelectDirectory();
String SelectLoadFile(const char *types);
bool   SelectSaveFile(const char *types, const String& data);

struct SelectFileIn : FileIn {
	SelectFileIn(const char *types);
};

struct SelectFileOut : FileOut {
	SelectFileOut(const char *types);
};
