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

enum { WITHBODY = 33 };

inline Font BrowserFont()
#ifdef PLATFORM_WIN32
{ return Arial(Ctrl::VertLayoutZoom(11)); }
#else
{ return Arial(Ctrl::VertLayoutZoom(9)); }
#endif

struct CppNestingInfo {
	String nesting;
	int    namespacel;
};

struct CppItemInfo : CppSimpleItem, CppNestingInfo {
	String key;
	String name;
	bool   over;
	bool   overed;
	int    inherited;
	String fn;
	int    line;
	int    typei;
	int    nesti;

	CppItemInfo() { over = overed = virt = false; inherited = line = namespacel = 0; }
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
	ITEM_RET,
	ITEM_TYPE,
	ITEM_PTYPE = ITEM_TYPE + 10000,
};

struct ItemTextPart : Moveable<ItemTextPart> {
	int pos;
	int len;
	int type;
	int ii;
};

Vector<ItemTextPart> ParseItemNatural(const CppItemInfo& m, const char *natural);
Vector<ItemTextPart> ParseItemNatural(const CppItemInfo& m);

bool SplitNestKey(const String& s, String& nest, String& key);

struct BrowserFileInfo {
	Time     time;
	String   package;
	String   file;

	BrowserFileInfo() { time = Null; }
};

ArrayMap<String, BrowserFileInfo>& FileSet();

int GetItemHeight(const CppSimpleItem& m, int cx);

struct BrowserQuery {
	String name;
	String package;
	String file;
	bool   a_private, a_protected, a_public;
	bool   code, data, type, macro;
	bool   documented, notdocumented;

	void Clear();
	void Serialize(Stream& s);

	BrowserQuery() { Clear(); }
};

struct QueryDlg : public WithQueryLayout<TopWindow> {
	typedef QueryDlg CLASSNAME;

	void Serialize(Stream& s);
	void EnterPackage();
	void Clear();

	int Perform(BrowserQuery& q);

	QueryDlg();
};

struct CppItemInfoDisplay : public Display
{
	String hkey;
	int    htopic;

	int DoPaint(Draw& w, const Rect& r, const Value& q,
		        Color _ink, Color paper, dword style) const;
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color _ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
};

struct CppNestingInfoDisplay : public Display
{
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color _ink, Color paper, dword style) const;
};

class ItemList : public ColumnList {
	CppItemInfoDisplay display;

	friend struct ItemDisplay;

	int    GetTopic(Point p, String& key);
	String Item(int i);

public:
	bool active_topics;

	void Clear();

	ItemList();
};

class Browser : public StaticRect {
public:
	void Serialize(Stream& s);
	void SerializeWspc(Stream& s);

	ArrayCtrl               nesting;
	ItemList                item;
	Splitter                split;
	BrowserQuery            query;
	QueryDlg                querydlg;
	Callback2<String, int>  WhenPos;
	int                     pos;
	Callback                WhenItem;
	Callback                WhenItemDblClk;
	Callback1<String>       WhenShowTopic;
	bool                    clickpos;
	bool                    show_inherited;
	EditString             *s_item, *s_nest;

	void     LoadNest(const String& nest, ArrayMap<String, CppItemInfo>& item, int inherited);
	bool     IsCurrentItem();
	CppItem& CurrentItem();
	void     Reload();
	void     EnterNesting();
	void     EnterItem();
	void     ItemClick();
	void     ItemDblClk();
	void     GotoItem();
	void     GotoPos(int n);
	void     ItemMenu(Bar& bar);
	void     QueryNest();
	bool     FindSet(const String& knesting, const String& kitem, int nestingsc = 0, int itemsc = 0);
	bool     FindSet(const String& item);

	String      GetItem(int i);
	CppItemInfo GetItemInfo(int i);

	String      GetItem()               { return GetItem(item.GetCursor()); }
	CppItemInfo GetItemInfo()           { return GetItemInfo(item.GetCursor()); }

	bool     DoQuery();
	void     DoDoQuery()                { DoQuery(); }
	void     QueryWord(const String& w);
	void     SetId(const String& id, const Vector<String>& nest);

	void     ShowTopic(String w);
	void     ShowHelp();
	
	void     WithSearch(EditString& search_nest, EditString& search_item);

	typedef Browser CLASSNAME;
	Browser();
	~Browser();
};

CppBase&       BrowserBase();
void           StartBrowserBase();
void           BrowserBaseScan(Stream& s, const String& fn);
void           ClearBrowserBase();
void           RescanBrowserBase();
void           SyncBrowserBase();
void           SaveBrowserBase();
void           Register(Browser *b);
void           UnRegister(Browser *b);
bool           ExistsBrowserItem(const String& item);
void           ReQualifyBrowserBase();

void           BrowserBaseScanLay(const String& fn);
void           ScanLayFile(const char *fn);

struct TopicInfo : Moveable<TopicInfo> {
	Time           time;
	String         path;
	String         title;
	Vector<int>    words;
};

void            SyncRefs();
void            SyncTopicFile(const RichText& text, const String& link, const String& path,
                              const String& title);
void            SyncTopicFile(const String& link, const String& path);
String          GetTopicTitle(const String& link);

Vector<String>  GetRefLinks(const String& ref);

int             TopicWordIndex(const String& w);

bool            MatchTopicLink(const String& link, const Vector<int>& query);

#define LAYOUTFILE <ide/Browser/Topic.lay>
#include <CtrlCore/lay.h>

struct ReferenceDlg : WithReferenceDlgLayout<TopWindow> {
	void   EnterItem();
	void   EnterItemOk();
	void   Set(const String& s);
	String Get() const            { return ~reference; }

	void   Serialize(Stream& s)   { browser.SerializeWspc(s); SerializePlacement(s); }

	typedef ReferenceDlg CLASSNAME;

	ReferenceDlg();
};

#define IMAGEFILE <ide/Browser/Topic.iml>
#define IMAGECLASS TopicImg
#include <Draw/iml_header.h>

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

struct StyleDlg;

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

	void   NewTopic();
	void   RenameTopic();
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
	void   CreateQtf(const String& item, const CppItemInfo& m, String& p1, String& p2);
	void   InsertItem();

	void   FindBrokenRef();

	void   Repair();
	void   CompressGroup();

public:
	Callback1<Bar&> WhenTemplatesMenu;

	enum {
		TIMEID_AUTOSAVE = Ctrl::TIMEID_COUNT,
	    TIMEID_COUNT
	};

	static Size TopicPage()                          { return Size(3968, INT_MAX); }

	void   ExportPdf();
	void   Print();

	typedef TopicEditor CLASSNAME;

	void Open(const String& grouppath);
	void GoTo(const String& topic, const String& link);
	
	static int  GetSerial();

	static void SerializeEditPos(Stream& s);
	
	void Serialize(Stream& s);

	TopicEditor();
	virtual ~TopicEditor();
};

#endif
