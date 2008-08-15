#ifndef IDE_H
#define IDE_H

#include <ide/Common/Common.h>

#include <RichEdit/RichEdit.h>

#define LAYOUTFILE <ide/ide.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS IdeImg
#define IMAGEFILE  <ide/ide.iml>
#include <Draw/iml_header.h>

#define KEYGROUPNAME "Ide"
#define KEYNAMESPACE IdeKeys
#define KEYFILE      <ide/ide.key>
#include             <CtrlLib/key_header.h>

#include <Web/Web.h>

#include <ide/Browser/Browser.h>
#include <ide/QuickTabs/QuickTabs.h>
#include <CodeEditor/CodeEditor.h>
#include <usvn/usvn.h>

#include "UppDlg.h"

#include "version.h"

#include <plugin/astyle/astyle.h>

enum { REMOTE_TIMEOUT = 2000 };

const char *FindTag(const char *txt, const char *tag);
const char *FindAfter(const char *txt, const char *tag);
int         IdeLocateLine(String old_file, int old_line, String new_file);

class FileSelButton : public FileSel
{
public:
	typedef FileSelButton CLASSNAME;
	enum MODE { MODE_OPEN, MODE_SAVE, MODE_DIR };
	FileSelButton(MODE mode = MODE_OPEN, const char *title = NULL);

	void               Attach(Ctrl& parent) { parent.AddFrame(button); }
	void               Title(String t)      { title = t; }
	String             GetTitle() const     { return title; }

private:
	void               OnAction();

private:
	FrameRight<Button> button;
	String             title;
	MODE               mode;
};

struct OpenFileButton  : FileSelButton { OpenFileButton(const char *title = NULL) : FileSelButton(MODE_OPEN, title) {} };
struct SaveFileButton  : FileSelButton { SaveFileButton(const char *title = NULL) : FileSelButton(MODE_SAVE, title) {} };
struct SelectDirButton : FileSelButton { SelectDirButton(const char *title = NULL) : FileSelButton(MODE_DIR,  title) {} };

void Puts(const char *s);

class Console : public LineEdit {
public:
	virtual bool Key(dword key, int count);
	virtual void LeftDouble(Point p, dword);
	virtual void RightDown(Point p, dword);
	virtual void Append(const String& s);

protected:
	struct Slot {
		Slot() : outfile(NULL), quiet(true), exitcode(Null) {}

		One<SlaveProcess> process;
		String            cmdline;
		String            output;
		String            key;
		String            group;
		Stream            *outfile;
		bool              quiet;
		int               exitcode;
		int               last_msecs;
	};

	struct Group {
		Group() : count(0), start_time(::msecs()), finished(false), msecs(0), raw_msecs(0) {}

		int               count;
		int               start_time;
		bool              finished;
		int               msecs;
		int               raw_msecs;
	};

	Array<Slot> processes;
	ArrayMap<String, Group> groups;
	Vector<String> error_keys;
	String current_group;
	String spooled_output;
	int console_lock;
	bool wrap_text;
	FrameBottom<EditString> input;

	void CheckEndGroup();
	void FlushConsole();

public:
	Callback WhenSelect;
	Callback1<Bar&> WhenBar;
	bool console;
	bool verbosebuild;

	int  Execute(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false);
	int  Execute(One<SlaveProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false);
	int  GetSlotCount() const { return processes.GetCount(); }
	int  AllocSlot();
	bool Run(const char *cmdline, Stream *out = NULL, const char *endptr = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
	bool Run(One<SlaveProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
	void BeginGroup(String group);
	void EndGroup();

	Console& operator<<(const String& s)      { Append(s); return *this; }

	bool IsRunning();
	bool IsRunning(int slot);
	int  Flush();
	void Kill(int slot);
	void Kill();
	void ClearError()                         { error_keys.Clear(); }
	Vector<String> PickErrors()               { Vector<String> e = error_keys; error_keys.Clear(); return e; }
	void Wait(int slot);
	bool Wait();

	void WrapText(bool w)                     { wrap_text = w; }

	void SetSlots(int s);

	void Input(bool b);

	Console();
};

extern String LinuxHostConsole;

struct LocalHost : Host {
	Vector<String> exedirs;
	String         environment;

	String        *cmdout;
	void    DoDir(const String& s);

	virtual String             GetEnvironment();
	virtual String             GetHostPath(const String& path);
	virtual String             GetLocalPath(const String& path);
	virtual String             NormalizePath(const String& path);
	virtual Vector<FileInfo>   GetFileInfo(const Vector<String>& path);
	virtual void               DeleteFile(const Vector<String>& path);
	virtual void               DeleteFolderDeep(const String& dir);
	virtual void               ChDir(const String& path);
	virtual void               RealizeDir(const String& path);
	virtual void               SaveFile(const String& path, const String& data);
	virtual String             LoadFile(const String& path);
	virtual int                Execute(const char *cmdline);
	virtual int                ExecuteWithInput(const char *cmdline);
	virtual int                Execute(const char *cmdline, Stream& out);
	virtual int                AllocSlot();
	virtual bool               Run(const char *cmdline, int slot, String key, int blitz_count);
	virtual bool               Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count);
	virtual bool               Wait();
	virtual One<SlaveProcess>  StartProcess(const char *cmdline);
	virtual void               Launch(const char *cmdline, bool console);
	virtual void               AddFlags(Index<String>& cfg);
};

struct RemoteHost : Host {
	String         host;
	int            port;
	String         os_type;
//	bool           transfer_files;
	Vector<String> path_map_local;
	Vector<String> path_map_remote;
	String         chdir_path;
	String         environment;

	static Time                TimeBase() { return Time(2000, 1, 1); }

	virtual String             GetEnvironment();
	virtual String             GetHostPath(const String& path);
	virtual String             GetLocalPath(const String& path);
	virtual String             NormalizePath(const String& path);
	virtual Vector<FileInfo>   GetFileInfo(const Vector<String>& path);
	virtual void               DeleteFile(const Vector<String>& path);
	virtual void               DeleteFolderDeep(const String& dir);
	virtual void               ChDir(const String& path);
	virtual void               RealizeDir(const String& path);
	virtual void               SaveFile(const String& path, const String& data);
	virtual String             LoadFile(const String& path);
	virtual int                Execute(const char *cmdline);
	virtual int                ExecuteWithInput(const char *cmdline);
	virtual int                Execute(const char *cmdline, Stream& out);
	virtual int                AllocSlot();
	virtual bool               Run(const char *cmdline, int slot, String key, int blitz_count);
	virtual bool               Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count);
	virtual bool               Wait();
	virtual One<SlaveProcess>  StartProcess(const char *cmdline);
	virtual void               Launch(const char *cmdline, bool console);
	virtual void               AddFlags(Index<String>& cfg);

	String                     RemoteExec(String cmd);
};

VectorMap<String, Builder *(*)()>& BuilderMap();

String SelectAnyPackage();

String FindInDirs(const Vector<String>& dir, const String& file);
String FindCommand(const Vector<String>& exedir, const String& cmdline);

void DlCharset(DropList& d);
void DlCharsetD(DropList& d);

void                      SetupBuildMethods();
VectorMap<String, String> GetMethodVars(const String& method);
String                    GetDefaultMethod();

struct PackageMode : Moveable<PackageMode> {
	int debug;
	int blitz;

	void Serialize(Stream& s) { s / debug / blitz / blitz; }

	PackageMode() { debug = blitz = Null; }
};

struct TargetMode {
	bool        target_override;
	String      target;
	int         linkmode;
	bool        createmap;
	String      version;
	PackageMode def;
	VectorMap<String, PackageMode> package;

	void        Serialize(Stream& s);

	TargetMode() { def.debug = def.blitz = 0; target_override = false; linkmode = 0; createmap = false; }
};

bool CheckLicense();
bool Install();

#define HELPNAME    "Help Topics"
#define METAPACKAGE "<meta>"

class TopicCtrl : public HelpWindow {
public:
	virtual Topic AcquireTopic(const String& topic);
	virtual void  BarEx(Bar& bar);
	virtual bool  Key(dword key, int count);
	virtual void  FinishText(RichText& text);

private:
	void OpenTopic();
	void Search();
	void ShowWords();
	void All();
	void Lang();
	void SShow();

	Label    search_label;
	WithDropChoice<EditString> search;
	DropList lang;
	bool     internal;
	bool     showwords, issearch, all;


public:
	Callback WhenTopic;

	void SyncDocTree();
	void SearchWord(const String& s);
	void Serialize(Stream& s);

	typedef TopicCtrl CLASSNAME;

	TopicCtrl();
};

struct IdeCalc : CodeEditor {
	virtual bool Key(dword key, int count);
	virtual void LeftDouble(Point p, dword flags);

	ArrayMap<String, EscValue> vars;

	void    Execute();

	IdeCalc();
};

void AutoSetup();

extern bool splash_screen;
void HideSplash();
void ShowSplash();
bool IsSplashOpen();

class RightTabs : public FrameCtrl<Ctrl> {
public:
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword);
	virtual void MouseMove(Point p, dword);
	virtual void MouseLeave();
	virtual void CancelMode();
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameLayout(Rect& r);
	virtual void FrameAddSize(Size& sz);

private:
	enum       { SPACE = 7 };

	struct Tab {
		int    y;
		Image  img;
		String tip;

		int    GetHeight() const { return img.GetSize().cy + SPACE; }
		int    GetRight() const  { return y + GetHeight(); }
	};

	Array<Tab> tab;
	int        cx;
	int        hl;
	int        cursor;

	void Repos();
	void PaintTab(Draw& w, int x, int y, int cx, int cy, Color paper, const Image& img, Color hl);
	int  GetPos(Point p);

public:
	void Clear();
	void Add(const Image& img, const String& tip);
	void SetCursor(int i);
	int  GetCursor() const                                       { return cursor; }

	RightTabs();
};

int  memcmp_i(const char *s, const char *t, int n);
void SubstituteTpars(Vector<String>& type, const String& tname);
int  CharFilterMacro(int c);

struct AssistEditor : CodeEditor {
	virtual bool Key(dword key, int count);
	virtual void LostFocus();
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void SelectionChanged();

	SplitterFrame  indexframe;
	EditString     searchindex;
	ArrayCtrl      index;
	StaticRect     indexpane;
	bool           showindex;
	struct IndexItem : Moveable<IndexItem> {
		String text;
		Color  ink;
		int    line;
	};
	Vector<IndexItem> indexitem;

	Splitter       popup;
	ArrayCtrl      assist;
	ArrayCtrl      type;
	Index<String>                 assist_type;
	ArrayMap<String, CppItemInfo> assist_item;
	int            assist_cursor;
	bool           auto_assist;
	bool           assist_active;
	bool           commentdp;
	bool           inbody;
	Ide           *theide;

	void           PopUpAssist(bool auto_insert = false);
	void           CloseAssist();
	void           Assist();
	String         ReadIdBack(int q);
	void           SyncAssist();
	void           AssistInsert();
	bool           InCode();

	void           Complete();

	void           Context(Parser& parser, int pos);
	bool           NestId(const CppNest& n, const String& id, Vector<String>& type, bool& code, String& t);
	void           TypeOf(const String& id, Vector<String>& r, bool& code);
	Vector<String> Operator(const char *oper, const Vector<String>& type);
	Vector<String> TypeOf(const Vector<String>& xp, const String& tp);
	String         RemoveDefPar(const char *s);
	String         MakeDefinition(const String& cls, const String& _n);
	void           DCopy();
	void           Virtuals();
	void           Thisbacks();
	void           GatherItems(const String& type, bool nom, Index<String>& in_types, bool tp);

	void           SelParam();
	int            Ch(int q);
	int            ParsBack(int q);
	Vector<String> ReadBack(int q, String& type);
	void           SkipSpcBack(int& q);
	String         IdBack(int& qq);

	void           SwapSContext(Parser& p);

	void           CreateIndex();

	void           SyncIndex();
	void           IndexSync();
	void           SearchIndex();
	void           IndexClick();
	void           SyncIndexCursor();
	bool           IndexKey(dword key);
	bool           IsIndex();
	void           ShowIndex(bool b);

	void           SerializeIndex(Stream& s);

	typedef AssistEditor CLASSNAME;

	AssistEditor();
};

void DirSel(EditField& f, FrameRight<Button>& b);
bool CopyFolder(const char *dst, const char *src, Progress *pi);

struct Ide : public TopWindow, public WorkspaceWork, public IdeContext {
public:
	virtual   void   Paint(Draw& w);
	virtual   bool   Key(dword key, int count);
	virtual   bool   HotKey(dword key);
	virtual   void   FileCursor();
	virtual   void   PackageCursor();
	virtual   void   SyncWorkspace();
	virtual   void   BuildFileMenu(Bar& menu);
	virtual   void   FilePropertiesMenu(Bar& menu);
	virtual   String GetOutputDir();
	virtual   void   FileSelected();
	virtual   void   Serialize(Stream& s);
	virtual   void   FileRename(const String& nm);
	virtual   void   DragAndDrop(Point p, PasteClip& d);

	virtual   void      PutConsole(const char *s);
	virtual   void      PutVerbose(const char *s);

	virtual   const Workspace& IdeWorkspace() const;
	virtual   bool             IdeIsBuilding() const;
	virtual   String           IdeGetOneFile() const;
	virtual   int              IdeConsoleExecute(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false);
	virtual   int              IdeConsoleExecute(One<SlaveProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false);
	virtual   int              IdeConsoleExecuteWithInput(const char *cmdline, Stream *out, const char *envptr, bool quiet);
	virtual   int              IdeConsoleAllocSlot();
	virtual   bool             IdeConsoleRun(const char *cmdline, Stream *out = NULL, const char *envptr = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
	virtual   bool             IdeConsoleRun(One<SlaveProcess> process, const char *cmdline, Stream *out = NULL, bool quiet = false, int slot = 0, String key = Null, int blitz_count = 1);
	virtual   void             IdeConsoleFlush();
	virtual   void             IdeConsoleBeginGroup(String group);
	virtual   void             IdeConsoleEndGroup();
	virtual   bool             IdeConsoleWait();

	virtual   bool      IdeIsDebug() const;
	virtual   void      IdeEndDebug();
	virtual   void      IdeSetBottom(Ctrl& ctrl);
	virtual   void      IdeActivateBottom();
	virtual   void      IdeRemoveBottom(Ctrl& ctrl);
	virtual   void      IdeSetRight(Ctrl& ctrl);
	virtual   void      IdeRemoveRight(Ctrl& ctrl);

	virtual   String    IdeGetFileName() const;
	virtual   int       IdeGetFileLine();
	virtual   String    IdeGetLine(int i) const;

	virtual   void      IdeSetDebugPos(const String& fn, int line, const Image& img, int i);
	virtual   void      IdeHidePtr();
	virtual   bool      IdeDebugLock();
	virtual   bool      IdeDebugUnLock();
	virtual   bool      IdeIsDebugLock() const;

	virtual   void      IdeSetBar();

	enum {
		EDITING, BUILDING, RUNNING, DEBUGGING,
	};

	String     pocfg;

	MenuBar    menubar;
	ToolBar    toolbar;
	StatusBar  statusbar;
	StaticRect editpane;

	int        idestate;
	int        debuglock;
	int        hydra1_threads;

	int        chstyle;

	One<IdeDesigner> designer;
	AssistEditor     editor;
	CodeEditor       editor2;
	QuickTabs        tabs, tabs2;
	EscValue         macro_api;

	RightTabs   btabs;
	StaticRect  bottom;
	Splitter    editor_bottom;
	Console     console;
	IdeCalc     calc;
	Browser     browser;
	Ptr<Ctrl>   bottomctrl;

	enum Bottoms { BCLOSE, BCONSOLE, BBROWSER, BCALC, BDEBUG };

	FileOut    stdout_fout;

	Splitter   editorsplit;
	Splitter   pfsplit;
	Splitter   wesplit;

	Splitter   right_split;
	StaticRect right;

	String    editfile;
	FileTime  edittime;

	String    editfile2;

	Vector<String> tablru;
	int            tabi;
	bool           blocktabs;

	struct Bookmark : Moveable<Bookmark> {
		String            file;
		LineEdit::EditPos pos;

		void Serialize(Stream& s)        { s % file % pos; }
	}
	bookmark[10];

	struct FileData {
		Time               filetime;
		LineEdit::EditPos  editpos;
		Point              columnline;
		LineEdit::UndoData undodata;
		LineInfo           lineinfo;
		LineInfoRem        lineinforem;

		void Clear()  {
			filetime = Null; editpos.Clear(); undodata.Clear();
			columnline = Null; lineinfo.Clear(); lineinforem.Clear();
		}
	};

	struct TransferFileInfo
	{
		String sourcepath;
		int    filetime;
		int    filesize;
	};

	String posfile[2];
	int    posline[2];
	Image  posimg[2];

	Vector<Bookmark> history;
	int              histi;

	ArrayMap<String, FileData> filedata;
	ArrayMap<String, TransferFileInfo> transferfilecache;
	Index<String> editastext;

	DropList   mainconfiglist;
	String     mainconfigname;
	String     mainconfigparam;

	String       method;
	int          targetmode;
	TargetMode   debug;
	TargetMode   release;

	String       onefile;
	int          build_time;
	String       target;
	String       cmdout;

	bool         stoponerrors;

	MultiButton               buildmode;
	PopUpTable                methodlist;
	PopUpTable                modelist;
	VectorMap<String, String> recent_buildmode;

	Vector<String> recentoutput;
	Vector<String> recentflags;

	String    runarg;
	String    recent_runarg;
	String    rundir;
	enum      { RUN_WINDOW, RUN_CONSOLE, RUN_FILE, RUN_FILE_CONSOLE };
	int       runmode;
	bool      runexternal;
	String    stdout_file;
	String    recent_stdout_file;

// ------------------------------------

	One<Debugger> debugger;

	Time      start_time;
	Time      build_start_time;
	int       stat_build_time;

	Font      editorfont;
	Font      veditorfont;
	Font      consolefont;
	Font      font1;
	Font      font2;
	Font      tfont;
	int       editortabsize;
	int       indent_amount;
	byte      default_charset;
	bool      indent_spaces;
	bool      show_status_bar;
	bool      toolbar_in_row;
	bool      show_tabs;
	bool      tabs_icons;
	bool      tabs_crosses;
	bool      tabs_grouping;
	bool      no_parenthesis_indent;
	bool      hilite_if_endif;
	bool      wrap_console_text;
	bool      mute_sounds;
	bool      line_numbers;
	bool      header_guards;
	bool      filetabs;
	bool      auto_enclose;
	bool      mark_lines;
	int       insert_include;
	int       bordercolumn;
	Color     bordercolor;

	/*
		astyle code formatter control vars
		added 2008.01.27 by Massimo Del Fedele
	*/
	bool	astyle_BracketIndent;
	bool	astyle_NamespaceIndent;
	bool	astyle_BlockIndent;
	bool	astyle_CaseIndent;
	bool	astyle_ClassIndent;
	bool	astyle_LabelIndent;
	bool	astyle_SwitchIndent;
	bool	astyle_PreprocessorIndent;
	int		astyle_MinInStatementIndentLength;
	int		astyle_MaxInStatementIndentLength;
	bool	astyle_BreakClosingHeaderBracketsMode;
	bool	astyle_BreakElseIfsMode;
	bool	astyle_BreakOneLineBlocksMode;
	bool	astyle_SingleStatementsMode;
	bool	astyle_BreakBlocksMode;
	bool	astyle_BreakClosingHeaderBlocksMode;
	int		astyle_BracketFormatMode;
	int		astyle_ParensPaddingMode;
	bool	astyle_ParensUnPaddingMode;
	bool	astyle_OperatorPaddingMode;
	bool	astyle_EmptyLineFill;
	bool	astyle_TabSpaceConversionMode;
	WString	astyle_TestBox;

	// Formats a string of code with a given formatter
	WString FormatCodeString(WString const &Src, astyle::ASFormatter &Formatter);

	// Formats editor's code with Ide format parameters
	void FormatCode();

	bool      use_target;

	bool      browser_closeesc;
	bool      bookmark_pos;

	FrameTop<StaticBarArea> bararea;
	Label                   display;


	byte      hilite_scope;
	int       hilite_bracket;
	int       hilite_ifdef;

	enum { STYLE_NO_REPLACE, STYLE_CONFIRM_REPLACE, STYLE_AUTO_REPLACE };

	struct FindInFiles : WithFindInFilesLayout<TopWindow> {
		WString itext;
		virtual bool Key(dword key, int count);
	} ff;
	int                              iwc;

	int           doc_serial;
	TopicCtrl     doc;

	int             state_icon;

	String        export_name;
	String        export_outdir;
	bool          export_usedonly;

// ------------------------------------

	void      BeepMuteExclamation() { if(!mute_sounds) BeepExclamation(); }
	void      BeepMuteInformation() { if(!mute_sounds) BeepInformation(); }

	void      BookKey(int key);
	void      AddHistory();
	void      GotoBookmark(const Bookmark& b);
	void      HistoryFw();
	void      HistoryBk();
	bool      IsHistDiff(int i);

	bool      HasFileData(const String& file);
	FileData& Filedata(const String& file);

	void      BTabs();
	void      SyncBottom();
	void      HideBottom();
	void      SetBottom(int i);
	void      ToggleBottom(int i);
	void      ShowConsole()                      { SetBottom(1); }
	void      ToggleConsole()                    { ToggleBottom(1); }
	void      SwapBottom();
	bool      IsBottomShown() const;

	void      SetHdependDirs();
	void      MakeTitle();
	void      MakeIcon();
	void      AdjustMainConfig();
	String    GetFirstFile();
	void      SetMain(const String& package);
	void      ChangeFileCharset(const String& name, Package& p, byte charset);
	void      ChangeCharset();
	void      FlushFile();
	void      EditFile0(const String& path, byte charset = CHARSET_DEFAULT, bool astext = false,
	                    const String& header = Null);
	void      EditFile(const String& path);
	void      ReloadFile();
	void      AddLru();
	bool      CanToggleReadOnly();
	void      ToggleReadOnly();
	void      PosSync();

	void      PackageMenu(Bar& menu);

	void      UscFile(const String& file);
	void      UscProcessDir(const String& dir);
	void      SyncUsc();

	void      RefreshBrowser();

	void      SetupOutputMode();
	void      SyncBuildMode();
	void      SetupDefaultMethod();
	void      SetupBuildMethods();
	void      DropMethodList();
	void      SelectMethod();
	void      DropModeList();
	void      SelectMode();
	void      SerializeOutputMode(Stream& s);

	void      GotoPos(String path, int line);
	void      GotoCpp(const CppPos& pos);

	Vector<String> SvnDirs();

	void      File(Bar& menu);
		void   EditWorkspace();
		void   EditAnyFile();
		bool   IsProjectFile(const String& f) const;
		void   SaveFile(bool always = false);
		void   DoSaveFile()    { SaveFile(); }
		void   FileBookmark	(Bar& menu);
		void   SwapPackagesFiles();
		void   Times();
		void   Statistics();
		void   Exit();
		String GetOpposite();
		void   GoOpposite();


	void      Edit(Bar& menu);
		void  EditAsText();
		void  EditUsingDesigner();
		void  EditFindReplace()  { editor.FindReplace(false); }
		void  EditFindReplacePickText() { editor.FindReplace(true); }
		void  EditFindNext()     { editor.Find(false); }
		void  EditFindPrevious() { editor.Find(true); }
		void  EditPaste()        { editor.Paste(); }
		void  EditMakeTabs()     { editor.MakeTabsOrSpaces(true); }
		void  EditMakeSpaces()   { editor.MakeTabsOrSpaces(false); }
		void  FindInFiles();
		void  FindFileName();
		void  FindString(bool back);
		void  FindNextError();
		void  FindPrevError();
		void  ClearEditedFile();
		void  ClearEditedAll();

	void      EditSpecial(Bar& menu);
		void  TranslateString();
		void  SwapChars()        { editor.SwapChars(); }
		void  CopyWord()         { editor.CopyWord(); }

	void      MacroMenu(Bar& menu);
		bool  HasMacros();
		void  EditMacroMenu(Bar& menu, const Vector<int>& items);
		void  EditMacro(int i);

	void      Project(Bar& menu);
		void  SyncT(int kind);
		void  ConvertST();
		void  ExportProject();
		void  SyncSvn();

	void      BuildMenu(Bar& menu);
		void BuildPackageMenu(Bar& menu);

		Index<String> PackageConfig(const ::Workspace& wspc, int package,
		                            const VectorMap<String, String>& bm, String mainparam,
		                            Host& host, Builder& b, String *target = NULL);
		const TargetMode& GetTargetMode();
		One<Host>     CreateHost(bool sync_files);
		bool          SyncHostFiles(RemoteHost& host);
		String        OutDir(const Index<String>& cfg, const String& package, const VectorMap<String, String>& bm,
		                     bool use_target = false);
		One<Builder>  CreateBuilder(Host *host);
		bool  BuildPackage(const ::Workspace& wspc, int pkindex, int pknumber, int pktotal,
		                   String mainparam, String outfile, Vector<String>& linkfile, String& linkopt,
		                   bool link = false);
		static Vector<String> GetAllUses(const ::Workspace& wspc, int index);
		Vector<String> GetAllLibraries(const ::Workspace& wspc, int index,
			                           const VectorMap<String, String>& bm, String mainparam,
			                           Host& host, Builder& builder);
		bool  Build(const Workspace& workspace, String mainparam, String outfile = Null, bool clear_console = true);
		bool  Build();
		void  DoBuild();
		void  PackageBuild();
		void  BeginBuilding(bool sync_files = true, bool clear_console = true);
		void  EndBuilding(bool ok);
		void  StopBuild();
		void  Clean();
		void  CleanPackage(const ::Workspace& wspc, int package);
		void  PackageClean();
		void  RebuildAll();
		void  CreateMakefile();
		void  SaveMakeFile(const String& file, bool exporting = false);
		void  CleanUppOut();
		void  SwitchHeader();
		void  FileCompile();
		void  Preprocess();
		void  ToggleStopOnErrors();
		One<Host> CreateHostRunDir();
		void  OpenOutputFolder();

	void      DebugMenu(Bar& menu);
		void  RunArgs();
		void  BuildAndExecute();
		void  BuildAndDebug0(const String& srcfile);
		void  BuildAndDebug(bool runto);
		void  BuildAndExtDebug();
		void  BuildAndExtDebugFile();
		bool  IsValgrind();
		void  Valgrind();

		void  StartDebug();
		void  StopDebug();
		void  DebugToggleBreak();
		void  ConditionalBreak();
		void  DebugClearBreakpoints();
		void  OnBreakpoint(int i);

		String GetLogPath();
		void  OpenLog();

	void      Setup(Bar& menu);
		void  SetupFormat();
		void  ToggleVerboseBuild();
		void  AutoSetup();

	void      BrowseMenu(Bar& menu);
		void  Query();
		void  QueryWord();
		void  DppIgnoreList();
		void  RescanCode();
		void  QueryId();
		void  About();
		void  OpenATopic();
		void  ToggleIndex();
		void  SearchIndex();
		void  Goto();
		void  GotoGlobal();
		void  ScanFile();
		void  SwapS();
		void  JumpS();
		void  InsertColor();


	void      ConsoleMenu(Bar& menu);
		void  ConsoleClear();
		void  ConsoleCopy();
		void  ConsolePaste();

	void      MainMenu(Bar& menu);
	void      MainTool(Bar& bar);

	void      SaveWorkspace();
	String    WorkspaceFile();
	void      SerializeWorkspace(Stream& s);

//	void      PrintTime(dword time);
	void      SetIdeState(int newstate);

	void      MainConfig();
	void      SyncMainConfigList();
	void      OnMainConfigList();
	void      SetMainConfigList();

	void      FileProperties();

	void      CustomSteps();

	void      CycleFiles();

	void      Renumber();

	bool      FindLineError(int l, Host& host);
	bool      FindLineError(String ln, Host& host, String& file, int& lineno, int& error);
	void      FindError();
	void	  ClearErrorEditor();
	void	  ClearErrorEditor(String file);
	void	  SetErrorEditor();

	void      FindWildcard();
	void      FindFolder();
	void      FindSetStdDir(String n);
	void      FindStdDir();
	void      InsertWildcard(int c);
	void      SerializeFf(Stream& s);
	bool      SearchInFile(const String& fn, const String& pattern,
		                   bool wholeword, bool ignorecase, int& n);
	void      ConstructFindInFiles();
	void      SerializeFindInFiles(Stream& s);

	void      SetupEditor(int font, String highlight, String fn);
	void      SetupEditor();

	void      Display();

	void      SetIcon();
	void      CheckFileUpdate();
	void      Periodic();

	void      PassEditor();
	void      SyncEditorSplit();
	void      SplitEditor(bool horz);
	void      CloseSplit();
	void      KeySplit(bool horz);
	void      SwapEditors();
	void      TabFile();
	void      TabFile2();
	void      ClearTab();
	void      ClearTabs();
	void      CloseRest(QuickTabs *tabs);
	void      TabsLR(int d);

	void      RefreshFrame(bool auto_disasm);
	void      RefreshLine(int frame, bool auto_disasm);

	void      ResolveUvsConflict();

	void      SetBar();

	void      UpdateFormat(CodeEditor& editor, QuickTabs& tabs);
	void      UpdateFormat();
	void      ReadHlStyles(ArrayCtrl& hlstyle);

	bool      OpenMainPackage();
	void      NewMainPackage();

	void      SearchTopics();
	void      ShowTopics();
	void      ShowTopic(String topic);

	void      TopicBack();

	void      SetupBars();

	void      SyncCh();

	EscValue  MacroEditor();
		void  MacroGetLength(EscEscape& e);
		void  MacroGetLineCount(EscEscape& e);
		void  MacroGetLinePos(EscEscape& e);
		void  MacroGetCursor(EscEscape& e);
		void  MacroGetLine(EscEscape& e);
		void  MacroGetColumn(EscEscape& e);
		void  MacroGetSelBegin(EscEscape& e);
		void  MacroGetSelEnd(EscEscape& e);
		void  MacroGetSelCount(EscEscape& e);
		void  MacroSetCursor(EscEscape& e);
		void  MacroSetSelection(EscEscape& e);
		void  MacroClearSelection(EscEscape& e);
		void  MacroGet(EscEscape& e);
		void  MacroRemove(EscEscape& e);
		void  MacroInsert(EscEscape& e);
		void  MacroFind(EscEscape& e);
		void  MacroReplace(EscEscape& e);
		void  MacroFindMatchingBrace(EscEscape& e);
		void  MacroFindClosingBrace(EscEscape& e);
		void  MacroFindOpeningBrace(EscEscape& e);
		void  MacroMoveLeft(EscEscape& e);
		void  MacroMoveRight(EscEscape& e);
		void  MacroMoveWordLeft(EscEscape& e);
		void  MacroMoveWordRight(EscEscape& e);
		void  MacroMoveUp(EscEscape& e);
		void  MacroMoveDown(EscEscape& e);
		void  MacroMoveHome(EscEscape& e);
		void  MacroMoveEnd(EscEscape& e);
		void  MacroMovePageUp(EscEscape& e);
		void  MacroMovePageDown(EscEscape& e);
		void  MacroMoveTextBegin(EscEscape& e);
		void  MacroMoveTextEnd(EscEscape& e);
		void  MacroInput(EscEscape& e);
		void  MacroBuild(EscEscape& e);
		void  MacroBuildProject(EscEscape& e);
		void  MacroExecute(EscEscape& e);
		void  MacroLaunch(EscEscape& e);
		void  MacroClearConsole(EscEscape& e);
		void  MacroEditFile(EscEscape& e);
		void  MacroSaveCurrentFile(EscEscape& e);
		void  MacroProjectDir(EscEscape& e);
		void  MacroFileName(EscEscape& e);

	typedef   Ide CLASSNAME;

	enum {
		TIMEID_PERIODIC = TopWindow::TIMEID_COUNT,
		TIMEID_COUNT,
	};

	Ide();
	~Ide();
};

#endif
