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
	
	rval_default(PackageTemplate);
	PackageTemplate() {}
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

	Array<Label>   label;
	Array<Ctrl>    ctrl;
	DelayCallback  delay;

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

int FilterPackageName(int c);

struct NestEditorDlg : public WithNestEditorLayout<TopWindow> {
    NestEditorDlg();
    
    bool firstbold = true;
    
	void   Set(const String& nests);
	String Get() const;

    void Sync();
    void RemoveEmptyRows();
    bool IsRowEmpty() const { return IsNull(nests.Get(nests.GetCursor(), 0)); }
};

class BaseSetupDlg : public WithBaseSetupLayout<TopWindow>
{
public:
    BaseSetupDlg();

    bool Run(String& vars);

private:
    void OnUpp();

private:
    bool               new_base;
};


bool BaseSetup(String& vars);
void AddAssemblyPaths(FileSel& dir);

inline bool PackageLess(String a, String b)
{
	int nc = CompareNoCase(a, b);
	if(nc) return nc < 0;
	return a < b;
};

struct SelectPackageDlg : public WithSelectPackageLayout<TopWindow> {
	virtual bool Key(dword key, int count);

	typedef SelectPackageDlg CLASSNAME;

	SelectPackageDlg(const char *title, bool selectvars, bool main);

	String         Run(String& nest, String startwith);

	void           Serialize(Stream& s);

	SplitterFrame     splitter;
	ArrayCtrl         base;
	ParentCtrl        list;
	FileList          clist;
	ArrayCtrl         alist;
	ProgressIndicator progress;
	StatusBar         lists_status;
	bool              alist_external = false;
	
	Vector<String> nest_list;

	bool           selectvars;
	bool           loading;
	int            loadi;
	bool           finished;
	bool           canceled;
	String         selected, selected_nest;

	Vector<Tuple<String, String>> lru;

	struct PkInfo {
		String package;
		String description;
		String nest;
		Image  icon;
		bool   main = false;
		bool   upphub = false;

		bool operator<(const PkInfo& b) const { return PackageLess(package, b.package); }
	};
	
	struct PkData : PkInfo {
		bool   ispackage = true;
		Time   tm = Null;
		Time   itm = Null;
		
		void Serialize(Stream& s)  { s % package % description % nest % icon % main % ispackage % tm % itm % upphub; }
		PkData()                   { tm = itm = Null; ispackage = true; }
	};

	Array<PkInfo>             packages;
	Array< ArrayMap<String, PkData> > data;

	String         GetCurrentNest();
	String         GetCurrentName();
	int            GetCurrentIndex();
	void           SyncBrief();

	void           ToolBase(Bar& bar);

	void           OnBaseAdd();
	void           OnBaseEdit();
	void           OnBaseRemove();
	void           RemoveInvalid();

	void           OnOK();
	void           OnCancel();

	void           OnNew();
	void           OnBase();
	void           OnFilter();

	void           ChangeDescription();
	
	String         LRUFilePath();
	void           LoadLRU();
	void           StoreLRU(const String& p);

	void           SyncFilter();
	void           ScanFolder(const String& path, ArrayMap<String, PkData>& nd,
	                          const String& nest, Index<String>& dir_exists,
	                          const String& prefix);
	String         CachePath(const char *vn) const;
	void           Load(const String& find = Null);
	void           SyncBase(String initvars);
	void           SyncList(const String& find);
	static bool    Pless(const SelectPackageDlg::PkInfo& a, const SelectPackageDlg::PkInfo& b);
	
	Vector<String> GetRepoDirs();
	void           SyncRepoDir(const String& dir);
	void           SyncRepoDirs();

	void           DuplicatePackage();
	void           RenamePackage(bool duplicate);
	void           DeletePackage();
	void           PackageMenu(Bar& bar);
	void           MovePackage(bool copy);

	enum {
		MAIN = 1, NONMAIN = 2, ALL = 0x8000, UPPHUB = 0x4000, NEST_MASK = 0xfff
	};
};

bool RenamePackageFs(const String& upp, const String& newname, bool duplicate = false);

String SelectPackage(String& nest, const char *title, const char *startwith = NULL,
                     bool selectvars = false, bool all = false);
String SelectPackage(const char *title, const char *startwith = NULL,
	                 bool selectvars = false, bool all = false);

int CondFilter(int c);
int FlagFilter(int c);
int FlagFilterM(int c);

struct UsesDlg : public WithUppOptPushDlg<TopWindow> {
	bool New();

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

void SyncPackage(const String& active, Package& actual);
void SyncEmptyPackage(const String& p);

struct WorkspaceWork {
	static    Font ListFont();

	FileList     package;
	UppList      filelist;
	Vector<int>  fileindex;

	String    main;
	String    actualpackage;
	int       actualfileindex;
	Package   actual;
	bool      editormode;
	bool      repo_dirs;

	struct Sepfo : Moveable<Sepfo> {
		String package;
		String separator;

		bool operator==(const Sepfo& s) const { return package == s.package && separator == s.separator; }
		hash_t GetHashValue() const           { return CombineHash(Upp::GetHashValue(package),
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
	bool         noemptyload = false; // in external mode, do not load files if package is empty (todo: refactor whole thing)
	
	Index<String> errorfiles;

	virtual void   PackageCursor();
	virtual void   FileCursor();
	virtual void   BuildPackageMenu(Bar& bar)         {}
	virtual void   BuildFileMenu(Bar& bar)            {}
	virtual void   FilePropertiesMenu(Bar& bar)       {}
	virtual String GetOutputDir()                     { return Null; }
	virtual String GetConfigDir()                     { return Null; }
	virtual void   SyncWorkspace()                    {}
	virtual void   FileSelected()                     {}
	virtual void   FileRename(const String& nm)       {}
	virtual bool   FileRemove()                       { return true; }
	virtual void   SyncSvnDir(const String& working)  {}
	virtual void   LaunchTerminal(const char *dir)    {}
	virtual void   InvalidateIncludes()               {}

	void   ScanWorkspace();
	void   SavePackage();
	void   RestoreBackup();
	void   SyncErrorPackages();
	void   Fetch(Package& p, const String& pkg);

	Vector<String> RepoDirs(bool actual = false);

	void SerializeFileSetup(Stream& s)                { s % filelist % package; }

	Sepfo  GetActiveSepfo();
	void   Group();
	void   OpenAllGroups();
	void   CloseAllGroups();
	void   GroupOrFile(Point pos);

	void   SetMain(const String& pkg)                 { main = pkg; }
	void   FindSetPackage(const String& s)            { package.FindSetCursor(s); }

	void   ShowFile(int pi);

	String         GetActivePackage() const           { return package.GetCurrentName(); }
	String         GetActivePackageDir() const        { return PackageDirectory(package.GetCurrentName()); }
	String         GetActivePackageFile() const       { return PackageFile(package.GetCurrentName()); }
	String         GetActiveFileName() const;
	String         GetActiveFilePath() const;
	void           OpenFileFolder();
	void           OpenPackageFolder();
	bool           IsActiveFile() const;
	Package::File& ActiveFile();
	String         FileName(int i) const;
	bool           IsSeparator(int i) const;
	bool           IsActiveSeparator() const;

	void LoadActualPackage();
	void SaveLoadPackage(bool sel = true);
	void SaveLoadPackageNS(bool sel = true);
	void TouchFile(const String& path);

	void DoMove(int b, bool drag);
	void MoveFile(int d);

	void DnDInsert(int line, PasteClip& d);
	void Drag();
	
	void NewPackageFile();
	
	enum ADDFILE { PACKAGE_FILE, OUTPUT_FILE, HOME_FILE, LOCAL_FILE, CONFIG_FILE, ANY_FILE };
	void AddFile(ADDFILE type);
	void AddItem(const String& name, bool separator, bool readonly);
	void AddTopicGroup();
	void AddSeparator();
	void DoImportTree(const String& dir, const String& mask, bool sep, Progress& pi, int from);
	void DoImportTree(const String& dir, const String& mask, bool sep, Progress& pi);
	void DoImport(const String& dir, const String& mask, bool sep, Progress& pi);
	void DoImport(const String& dir, const String& mask, bool sep, Progress& pi, bool tree);
	void Import();
	void RemoveFile();
	void DelFile();
	void RenameFile();
	void TogglePCH();
	void ToggleIncludeable();

	void AddNormalUses();
	void AddFolderUses();
	void AddAnyUses();
	void TogglePackageSpeed();

	void PackageOp(String active, String from_package, String rename);

	void RemovePackageMenu(Bar& bar);
	void RemovePackage(String from_package);

	void RenamePackage();
	void DeletePackage();

	static bool IsAux(const String& p);
	bool IsAux();
	bool IsMeta();

	void PackageMenu(Bar& bar);
	void FileMenu(Bar& bar);
	void InsertSpecialMenu(Bar& menu);

	String PackageDirA(const String& pn);
	String PackageFileA(const String& pn);
	
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
		FLAG = 0, USES, TARGET, LIBRARY, STATIC_LIBRARY, LINK, COMPILER, INCLUDE, PKG_CONFIG,
		PKG_LAST = PKG_CONFIG,
		FILEOPTION, FILEDEPENDS
	};

	Vector<Array<OptItem> *> opt;
	Vector<String>           opt_name;

	void Add(const char *name, Array<OptItem>& m);

	void Serialize(Stream& s);
	void SaveOptions();
	void SaveOptionsLoad();
	
	void MergeNests();

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
	void Prepare(WithUppOptDlg<TopWindow>& dlg, int type);
	void AddOption(int type);
	void OptionMenu(Bar& bar);
	void RemoveOption();
	void EditOption(bool duplicate);
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

String SelectExternalPackage(const String& from = Null);
