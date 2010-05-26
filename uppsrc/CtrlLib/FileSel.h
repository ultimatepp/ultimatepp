Image GetDriveImage(char drive_style);

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

	void        UpdateSelect();
	void        Update();
	bool        FindChar(int from, int chr);
	void        KillEdit();

	struct FileOrder;

protected:
	enum {
		TIMEID_STARTEDIT = ColumnList::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	Callback2<const String&, const String&> WhenRename;

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
	                Color underline = Null);
	void        Set(int ii,
	                const String& name, const Image& icon = Null,
		            Font font = StdFont(), Color ink = SColorText(),
	                bool isdir = false, int64 length = 0, Time time = Null, Color extink = Null,
	                const String& desc = Null, Font descfont = Null, Value data = Null,
	                Color underline = Null);
	void        Add(const String& name, const Image& icon = Null,
		            Font font = StdFont(), Color ink = SColorText(),
	                bool isdir = false, int64 length = 0, Time time = Null, Color extink = Null,
	                const String& desc = Null, Font descfont = Null, Value data = Null,
	                Color underline = Null);
	String      GetCurrentName() const;

	int         Find(const char *name);
	bool        FindSetCursor(const char *name);

	void        Sort(const Order& order);

	FileList&   IconWidth(int w)                     { iconwidth = w; Refresh(); return *this; }
	int         GetIconWidth() const                 { return iconwidth; }
	FileList&   Renaming(bool b = true)              { renaming = b; return *this; }
	FileList&   JustName(bool b = true)              { justname = b; Refresh(); return *this; }
	FileList&   AccelKey(bool b = true)              { accelkey = b; return *this; }

	typedef FileList CLASSNAME;

	FileList();
	virtual ~FileList();
};

bool Load(FileList& list, const String& dir, const char *patterns, bool dirs = false,
          Callback3<bool, const String&, Image&> WhenIcon = CNULL,
          FileSystemInfo& filesystem = StdFileSystemInfo(), const String& search = String(),
          bool hidden = true, bool hiddenfiles = true);
void SortByName(FileList& list);
void SortByExt(FileList& list);

String DirectoryUp(String& dir, bool basedir = false);

class FileSel : public WithFileSelectorLayout<TopWindow> {
public:
	virtual bool Key(dword key, int count);

private:
	SizeGrip    sizegrip;

	Button      dirup, mkdir, plus, minus, toggle;

protected:
	Vector<String> mask;
	Vector<String> path;
	Vector<String> lru;
	int            lastsby;
	FileSystemInfo *filesystem;

	String         olddir;
	String         basedir;
	String         activetype;
	String         defext;
	String         preselect;
	Vector<String> fn;

#ifdef PLATFORM_WIN32
	Array<NetNode> netstack;
	Array<NetNode> netnode;
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

	void        LoadNet();
	void        SelectNet();
	bool        PatternMatch(const char *fn);
	bool        OpenItem();
	void        OpenItem2()                                  { OpenItem(); }
	void        Open();
	void        DirUp();
	void        MkDir();
	void        Plus();
	void        Minus();
	void        Toggle();
	void        PlusMinus(const char *title, bool sel);
	void        Update();
	void        FileUpdate();
	void        Rename(const String& on, const String& nn);
	void        Choice();
	void        SearchLoad();
	void        Load();
	String      FilePath(const String& fn);
	void        SetDir(const String& dir);
	String      GetDir();
	void        AddName(Vector<String>& fn, String& o);
	void        Finish();
	bool        Execute(int mode);
	bool        IsMulti()                                     { return multi && mode == OPEN; }
	void        SyncSplitter();
	void        InitSplitter();
	String      GetMask();
	void        GoToPlace();
	void        AddPlaceRaw(const String& path, const Image& m, const String& name);


	using       WithFileSelectorLayout<TopWindow>::Title;

	typedef FileSel CLASSNAME;
public:

	Callback3<bool, const String&, Image&> WhenIcon;

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
	FileSel& AddPlace(const String& path, const Image& m, const String& name);
	FileSel& AddPlace(const String& path, const String& name);
	FileSel& AddPlace(const String& path);
	FileSel& AddPlaceSeparator();
	FileSel& AddStandardPlaces();

	FileSel();
	virtual ~FileSel();
};

String SelectFileOpen(const char *types);
String SelectFileSaveAs(const char *types);
String SelectLoadFile(const char *types);
bool   SelectSaveFile(const char *types, const String& data);

struct SelectFileIn : FileIn {
	SelectFileIn(const char *types);
};

struct SelectFileOut : FileOut {
	SelectFileOut(const char *types);
};
