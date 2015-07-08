#ifndef _Browser_Browser_h
#define _Browser_Browser_h

#include <CtrlLib/CtrlLib.h>
#include <CppBase/CppBase.h>
#include <ide/Common/Common.h>
#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

#define LAYOUTFILE <ide/Browser/Browser.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS BrowserImg
#define IMAGEFILE <ide/Browser/Browser.iml>
#include <Draw/iml_header.h>

class Browser;

CppBase&       CodeBase();

struct SourceFileInfo {
	Time                      time;
	String                    dependencies_md5sum; // dependencies from other files - usings, initial namespace, macros
	String                    md5sum; // preprocessing 'fingerprint' to detect changes
	Vector<int>               depends; // indicies of file this files depends on, for time-check
	Time                      depends_time;
	
	void Serialize(Stream& s);

	SourceFileInfo() { time = Null; depends_time = Null; }
};

void           NewCodeBase();
void           ParseSrc(Stream& in, int file, Callback2<int, const String&> error);
void           CodeBaseScanFile(Stream& in, const String& fn);
void           CodeBaseScanFile(const String& fn);
void           ClearCodeBase();
// void           CheckCodeBase();
void           RescanCodeBase();
void           SyncCodeBase();
void           SaveCodeBase();
bool           ExistsBrowserItem(const String& item);
void           FinishCodeBase();

String         PreprocessLayFile(const char *fn);
Vector<String> PreprocessSchFile(const char *fn);
String         PreprocessImlFile(const char *fn);

int            GetSourceFileIndex(const String& path);
String         GetSourceFilePath(int file);

String         MakeCodeRef(const String& scope, const String& item);
void           SplitCodeRef(const String& ref, String& scope, String& item);

const CppItem *GetCodeRefItem(const String& ref, const String& rfile);
const CppItem *GetCodeRefItem(const String& ref);

int            GetMatchLen(const char *s, const char *t);


enum WithBodyEnum { WITHBODY = 33 };

inline Font BrowserFont() { return StdFont(); }

struct CppItemInfo : CppItem {
	String scope;
	bool   over;
	bool   overed;
	int    inherited;
	int    typei;
	
	CppItemInfo() { over = overed = virt = false; inherited = line = 0; }
};

enum {
	ITEM_TEXT,
	ITEM_NAME,
	ITEM_CPP_TYPE,
	ITEM_CPP,
	ITEM_PNAME,
	ITEM_TNAME,
	ITEM_NUMBER,
	ITEM_SIGN,
	ITEM_UPP,
	ITEM_TYPE,
	
	ITEM_PTYPE = ITEM_TYPE + 10000,
};

struct ItemTextPart : Moveable<ItemTextPart> {
	int pos;
	int len;
	int type;
	int ii;
	int pari;
};

Vector<ItemTextPart> ParseItemNatural(const String& name, const String& natural, const String& ptype,
                                      const String& pname, const String& type, const String& tname,
                                      const String& ctname, const char *s);
Vector<ItemTextPart> ParseItemNatural(const String& name, const CppItem& m, const char *natural);
Vector<ItemTextPart> ParseItemNatural(const CppItemInfo& m);
Vector<ItemTextPart> ParseItemNatural(const CppItemInfo& m);

int GetItemHeight(const CppItem& m, int cx);

enum AdditionalKinds {
	KIND_INCLUDEFILE = 100,
	KIND_INCLUDEFILE_ANY,
	KIND_INCLUDEFOLDER,
};

void PaintText(Draw& w, int& x, int y, const char *text,
               const Vector<ItemTextPart>& n, int starti, int count, bool focuscursor,
               Color _ink, bool italic);
void PaintCppItemImage(Draw& w, int& x, int ry, int access, int kind, bool focuscursor);

struct CppItemInfoDisplay : public Display
{
	bool   namestart;
	bool   showtopic;

	int DoPaint(Draw& w, const Rect& r, const Value& q,
		        Color _ink, Color paper, dword style) const;
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color _ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
	
	CppItemInfoDisplay() { namestart = false; showtopic = false; }
};

int SearchItemFilter(int c);

struct CodeBrowser {
	typedef CodeBrowser CLASSNAME;

	CppItemInfoDisplay     display;
	ArrayCtrl              scope;
	ArrayCtrl              item;
	EditString             search_scope;
	EditString             search;
//	FrameRight<Button>     clear;
	int                    range;
	ButtonOption           rangebutton[3];
	ButtonOption           sort;
	Callback               WhenKeyItem;
	Callback               WhenClear;
	
	String             GetPm();
	void               Load();
	void               LoadScope();
	void               Goto(const String& coderef, const String& rfile);
	void               Search();
	void               NameStart()               { display.namestart = true; }
	String             GetCodeRef(int i) const;
	String             GetCodeRef() const;
	const CppItemInfo& GetItemInfo(int i) const;
	const CppItemInfo& GetItemInfo() const;
	bool               Key(dword key, int count);
	bool               IsSearch() const;
	void               ClearSearch();
	void               SetRange(int r);
	void               Sort();
	
	CodeBrowser();
};

struct TopicInfo : Moveable<TopicInfo> {
	Time           time;
	String         path;
	String         title;
	Vector<int>    words;

	TopicInfo() {}

	rval_default(TopicInfo);
};

String          GetTopicPath(const TopicLink& link);
String          GetTopicPath(const String& link);

extern bool SyncRefsFinished;
extern bool SyncRefsShowProgress;

void            SyncRefs();
void            SyncTopicFile(const RichText& text, const String& link, const String& path,
                              const String& title);
void            SyncTopicFile(const String& link);
String          GetTopicTitle(const String& link);
void            InvalidateTopicInfoPath(const String& path);

bool LegacyRef(String& ref);

Vector<String>  GetRefLinks(const String& ref);

int             TopicWordIndex(const String& w);
int             MatchWord(const Vector<int>& w, const String& pattern);
bool            MatchTopicLink(const String& link, const Vector<String>& query);

#define LAYOUTFILE <ide/Browser/Topic.lay>
#include <CtrlCore/lay.h>

struct ReferenceDlg : WithReferenceDlgLayout<TopWindow>, CodeBrowser {
	void   EnterItem();
	void   EnterItemOk();
	void   Set(const String& s);
	String Get() const            { return ~reference; }

	void   Serialize(Stream& s)   { SerializePlacement(s); }

	typedef ReferenceDlg CLASSNAME;

	ReferenceDlg();
};

#define IMAGEFILE <ide/Browser/Topic.iml>
#define IMAGECLASS TopicImg
#include <Draw/iml_header.h>

String DecoratedItem(const String& name, const CppItem& m, const char *natural, int pari = INT_MIN);

int  CharFilterID(int c);

bool ParseTopicFileName(const String& fn, String& topic, int& lang);

Topic  ReadTopic(const char *text);
Vector<String> GatherLabels(const RichText& text);
String WriteTopic(const char *title, const RichText& text);
String WriteTopicI(const char *title, const RichText& text);

void LoadTopics(FileList& topic, const String& dir);

TopicLink ParseTopicFilePath(const String& path);
String    TopicFilePath(const TopicLink& tl);

void SaveGroupInc(const String& grouppath);

void SetTopicGroupIncludeable(const char *path, bool set);

Uuid CodeItemUuid();
Uuid StructItemUuid();
Uuid BeginUuid();
Uuid EndUuid();

struct StyleDlg;

template <class T>
struct TopicDlg : T {
	String GetName() const {
		return (String)~T::topic + "$" + ToLower(LNGAsText(~T::lang)) + ".tpp";
	}

	TopicDlg(const char *title) {
		CtrlLayoutOKCancel(*this, title);
		T::topic.NotNull();
		T::topic.MaxLen(30);
		T::topic.SetFilter(CharFilterID);
	}
};

class TopicEditor : public IdeDesigner, public Ctrl {
public:
	virtual String GetFileName() const;
	virtual void   Save();
	virtual void   EditMenu(Bar& menu);
	virtual Ctrl&  DesignerCtrl()                                   { return *this; }
	virtual void   SetFocus();

	virtual bool Key(dword key, int);

protected:
	ToolBar           tool;

	FileList          topic;

	EditString        title;
	RichEdit          editor;

	StaticRect        right;
	Splitter          left_right;

	String            grouppath;
	String            topicpath;

	static String     lasttemplate;
	static int        lastlang;
	static bool       allfonts;
	static int        serial;

	struct FileInfo {
		Time               time;
		RichEdit::UndoInfo undo;
		RichEdit::PosInfo  pos;

		FileInfo() { time = Time(1, 1, 1); }
	};

	static VectorMap<String, String>  grouptopic;
	static ArrayMap<String, FileInfo> editstate;

	void   FormatMenu(Bar& bar);
	void   TableMenu(Bar& bar);
	void   TopicMenu(Bar& bar);
	void   MainTool(Bar& bar);
	void   FileBar(Bar& bar);
	void   SetBar();

	String GetCurrentTopicPath();

	void   InsertNew(const String& coderef);
	void   NewTopic();
	void   MoveTopic();
	void   RemoveTopic();

	bool         autosave;
	ReferenceDlg ref;

	void   ShowTopic(bool b = true);
	void   HideTopic()                   { ShowTopic(false); }

	void   TopicCursor();

	void   ListTemplates(Vector<String>& path, Vector<String>& name);
	String ChooseTemplate(const char *title);

	void   SaveAsTemplate();
	void   ApplyStylesheet();
	void   ApplyStylesheetGroup();

	void   Hyperlink(String&, WString&);

	void   Load(const String& path);
	void   SaveTopic();
	void   SaveInc();
	void   Flush();

	void   SyncFonts();
	void   AllFonts();

	void   Tools(Bar& bar);
	void   Label(String&);
//	String CreateQtf(const String& item, const String& name, const CppItem& m, bool onlyhdr = false);
	void   InsertItem();

	void   FindBrokenRef();
	void   JumpToDefinition();
	String GetLang() const;

	void   FixTopic();

public:
	Callback1<Bar&> WhenTemplatesMenu;

	enum {
		TIMEID_AUTOSAVE = Ctrl::TIMEID_COUNT,
	    TIMEID_COUNT
	};

	static Size TopicPage()                          { return Size(3968, INT_MAX); }

	void   ExportPdf();
	void   ExportGroupPdf();
	void   ExportHTML();
	void   ExportGroupHTML();
	void   Print();

	typedef TopicEditor CLASSNAME;

	void ShowEditor(bool b)                          { editor.Show(b); }
	bool NewTopicEx(const String& name, const String& create);
	void Open(const String& grouppath);
	void GoTo(const String& topic, const String& link, const String& create, bool before);
	void PersistentFindReplace(bool b)               { editor.PersistentFindReplace(b); }
	
	static int  GetSerial();

	static void SerializeEditPos(Stream& s);
	
	void Serialize(Stream& s);

	TopicEditor();
	virtual ~TopicEditor();
};

#endif
