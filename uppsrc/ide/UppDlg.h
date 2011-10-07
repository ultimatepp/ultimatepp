#ifdef PLATFORM_POSIX
inline bool MoveFile(const char *oldpath, const char *newpath)
{
	return !rename(oldpath, newpath);
}
#endif

struct PackageInfo : Moveable<PackageInfo> {
	Time   stamp;
	String path;
	Color  ink;
	bool   bold, italic;
};

void        InvalidatePackageInfo(const String& name);
PackageInfo GetPackageInfo(const String& name);

struct FileTemplate {
	String filename;
	String condition;
	String text;
};

enum {
	TEMPLATEITEM_ID, TEMPLATEITEM_FILENAME, TEMPLATEITEM_OPTION, TEMPLATEITEM_SELECT,
	TEMPLATEITEM_TEXT
};

struct TemplateItem {
	String         label;
	int            type;
	String         id;
	Vector<String> value;
	String         init;
};

struct PackageTemplate {
	String              name;
	bool                main, sub;
	Array<TemplateItem> item;
	Array<FileTemplate> file;
};

struct AppPreview : Ctrl {
	virtual void Paint(Draw& w);
	virtual void Layout();
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);

	ScrollBar      sb;
	struct Line : Moveable<Line> {
		String text;
		bool   header;
	};
	Vector<Line> line;

	void Scroll() { Refresh(); }

	void Clear() { line.Clear(); Refresh(); sb.Set(0); }
	void Add(const String& data, bool header = false);

	typedef AppPreview CLASSNAME;

	AppPreview();
	virtual ~AppPreview();
};

struct TemplateDlg : public WithNewPackageLayout<TopWindow> {
	AppPreview      preview;

	Array<PackageTemplate> pt;

	Array<Label>  label;
	Array<Ctrl>   ctrl;
	DelayCallback delay;

	const PackageTemplate& ActualTemplate();

	String GetPackageFile();
	void Preview();
	void EnterTemplate();
	void LoadNest(const char *dir, bool main, bool recurse = true);
	void Load(const Vector<String>& path, bool main);
	void EnableCreate() { ok.Enable(!IsNull(package)); Inits(); delay.Invoke(); }
	void Inits();

	ArrayMap<String, EscValue> MakeVars0();
	ArrayMap<String, EscValue> MakeVars();
	void Create();

	void Serialize(Stream& s) { SerializePlacement(s); }

	typedef TemplateDlg CLASSNAME;
	TemplateDlg();
	virtual ~TemplateDlg();
};

String SelectPackage(const char *title, const char *startwith = NULL,
	bool selectvars = false, bool all = false);

int CondFilter(int c);
int FlagFilter(int c);
int FlagFilterM(int c);

struct UsesDlg : public WithUppOptPushDlg<TopWindow> {
	void New();

	typedef UsesDlg CLASSNAME;

	UsesDlg();
};

struct DependsDlg : public WithUppOptPushDlg<TopWindow> {
	String package;

	void New();

	typedef DependsDlg CLASSNAME;

	DependsDlg();
};

struct UppList : FileList {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper,
	                   dword style) const;
};

struct WorkspaceWork {
	static    Font ListFont();

	FileList     package;
	Vector<bool> speed;
	UppList      filelist;
	Vector<int>  fileindex;

	String    main;
	String    actualpackage;
	int       actualfileindex;
	Package   actual;

	struct Sepfo : Moveable<Sepfo> {
		String package;
		String separator;

		bool operator==(const Sepfo& s) const { return package == s.package && separator == s.separator; }
		unsigned GetHashValue() const         { return CombineHash(Upp::GetHashValue(package),
		                                                           Upp::GetHashValue(separator)); }
		void Serialize(Stream& s)             { s % package % separator; }
		Sepfo(const String& package, const String& separator) : package(package), separator(separator) {}
		Sepfo() {}
	};

	Index<Sepfo> closed;

	struct Backup {
		FileTime time;
		String   data;
	};

	ArrayMap<String, Backup> backup;

	bool         organizer;
	bool         showtime;
	bool         sort;
	
	Index<String> errorfiles;

	virtual void   PackageCursor();
	virtual void   FileCursor();
	virtual void   BuildPackageMenu(Bar& bar)         {}
	virtual void   BuildFileMenu(Bar& bar)            {}
	virtual void   FilePropertiesMenu(Bar& bar)       {}
	virtual String GetOutputDir()                     { return Null; }
	virtual void   SyncWorkspace()                    {}
	virtual void   FileSelected()                     {}
	virtual void   FileRename(const String& nm)       {}
	virtual void   SyncSvnDir(const String& working)  {}

	void   ScanWorkspace();
	void   SavePackage();
	void   RestoreBackup();
	void   SyncErrorPackages();

	void SerializeFileSetup(Stream& s)                { s % filelist % package; }

	Sepfo  GetActiveSepfo();
	void   Group();
	void   OpenAllGroups();
	void   CloseAllGroups();
	void   GroupOrFile(Point pos);

	void   SetMain(const String& m)                           { main = m; }
	void   FindSetPackage(const String& s)                    { package.FindSetCursor(s); }

	void   ShowFile(int pi);

	String         GetActivePackage() const                   { return package.GetCurrentName(); }
	String         GetActivePackagePath() const               { return PackagePath(package.GetCurrentName()); }
	String         GetActiveFileName() const;
	String         GetActiveFilePath() const;
	void           OpenFileFolder();
	void           OpenPackageFolder();
	bool           IsActiveFile() const;
	Package::File& ActiveFile();
	String         FileName(int i) const;
	bool           IsSeparator(int i) const;

	void LoadActualPackage();
	void SaveLoadPackage();
	void SaveLoadPackageNS();
	void TouchFile(const String& path);

	void MoveFile(int d);
	void Move(Vector<String>& v, FileList& ta, int d);

	enum ADDFILE { PACKAGE_FILE, OUTPUT_FILE, HOME_FILE, LOCAL_FILE, ANY_FILE };
	void AddFile(ADDFILE type);
	void AddItem(const String& name, bool separator, bool readonly);
	void AddTopicGroup();
	void AddSeparator();
	void Import();
	void DoImport(const String& dir, const String& mask, bool sep, Progress& pi);
	void RemoveFile();
	void DelFile();
	void RenameFile();
	void ToggleFileSpeed();
	void ToggleIncludeable();

	void AddNormalUses();
	void AddAnyUses();
	void TogglePackageSpeed();

	void RemovePackageMenu(Bar& bar);
	void RemovePackage(String from_package);

	static bool IsAux(const String& p);
	bool IsAux();

	void PackageMenu(Bar& bar);
	void FileMenu(Bar& bar);
	void SpecialFileMenu(Bar& bar);
	void InsertSpecialMenu(Bar& menu);

	String PackagePathA(const String& pn);
	
	void SetErrorFiles(const Vector<String>& files);

	void SerializeClosed(Stream& s);

	typedef WorkspaceWork CLASSNAME;

	WorkspaceWork();
	virtual ~WorkspaceWork() {}
};

struct PackageEditor : WorkspaceWork, WithUppLayout<TopWindow> {
	virtual bool Key(dword key, int);
	virtual void FileCursor();
	virtual void PackageCursor();

	enum OptionType {
		FLAG = 0, USES, TARGET, LIBRARY, LINK, COMPILER, INCLUDE,
		FILEOPTION, FILEDEPENDS
	};

	Vector<Array<OptItem> *> opt;
	Vector<String>           opt_name;

	void Add(const char *name, Array<OptItem>& m);

	void Serialize(Stream& s);
	void SaveOptions();
	void SaveOptionsLoad();

	void Empty();
	void FileEmpty();
	void OptionAdd(ArrayCtrl& option, int type, const char *title, const Array<OptItem>& o);
	void FindOpt(ArrayCtrl& option, int type, const String& when, const String& text);
	void FindOpt(ArrayCtrl& option, int type, int ii);
	void AdjustOptionCursor(ArrayCtrl& option);
	void Init(ArrayCtrl& option);
	void SetOpt(ArrayCtrl& opt, int type, OptItem& m, const String& when, const String& text);

	void OptionAdd(int type, const char *title, const Array<OptItem>& o);
	void AdjustPackageOptionCursor();
	void AddOption(int type);
	void OptionMenu(Bar& bar);
	void RemoveOption();
	void EditOption();
	void MoveOption(int d);

	void AdjustFileOptionCursor();
	void FindFileOpt(int type, const String& when, const String& text);
	void FileOptionMenu(Bar& bar);
	void MoveFileOption(int d);
	void AddDepends(bool external_deps);
	void AddFileOption();
	void EditFileOption();
	void RemoveFileOption();
	void Description();

	typedef PackageEditor CLASSNAME;

	PackageEditor();
};

void EditPackages(const char *main, const char *startwith, String& cfg);
