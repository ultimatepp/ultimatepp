#include <RichEdit/RichEdit.h>
#include <docpp/docpp.h>

#define IMAGECLASS DppImg
#define IMAGEFILE  <Docedit/Docedit.iml>
#include <Draw/iml_header.h>

// before placed to IDE...

String SourcePath(const String& package, const String& name);
String PackageDirectory(const String& name);
String CommonPath(const String& filename);

// ------

struct DocBase : public CppBase {
	Vector<String> DocBase::ignore;
	Vector<String> package;

	void RemoveFile(const String& file);
	void ParseFile(const String& file, const String& package) throw(Parser::Error);
	void RefreshFile(const String& file, const String& package) throw(Parser::Error);
	Vector<String> GetHeaders();
	Vector<String> GetPackages();
};

extern DocBase doc_base;

Time   GetFileTime(const char *path);
String DocFile(const String& package, const String& name);

struct DocKey {
	String nameing;
	String nesting;
	String item;
	dword  lang;

	bool operator==(const DocKey& b) const;
	dword GetHashValue(const DocKey& k);
	operator bool() const  { return item.GetLength(); }
	void  Clear()          { item.Clear(); nesting.Clear(); nameing.Clear(); lang = 0; }

	DocKey() {}
	DocKey(const String& m, const String& n, const String& s, dword l)
	   : nameing(m), nesting(n), item(s), lang(l) {}
};

dword GetHashValue(const DocKey& k);

struct DocQuery {
	String name, text, package, header;
	bool   undocumented, normal, external, obsolete, ignored;
	dword  lang;
};

struct DocItem {
	CppItem *cppitem;
	String   item;
	int      status;
	String   package;
};

typedef VectorMap<String, VectorMap<String, ArrayMap<String, DocItem> > > DocSet;

class DocDir {
	struct Entry {
		int    type;
		String text;
	};
	VectorMap<String, ArrayMap<DocKey, Entry> > dir;

	int    ReadDocHeader(const char *filename, DocKey& key);
	String GetAddFileName(const String& package, const DocKey& key, int type);
	bool   GetFileName(const DocKey& key, String& fn, String& package);

	bool   LoadLinks(const String& package);
	void   SaveLinks(const String& package) const;

	void   RemoveOtherKey(const DocKey& k, int t1, int t2 = UNDOCUMENTED);
	Entry *Find(const DocKey& key, String& package) const;
	Entry *Find(const DocKey& key) const;

public:
	enum {
		NORMAL, EXTERNAL, LINK, IGNORED,
		UNDOCUMENTED = -1, OBSOLETE = -2, OBSOLETELINK = -3
	};

	bool   LoadDir(const String& package);
	void   SaveDir(const String& package) const;

	void   RebuildDir(const String& package);

	void   Refresh(const String& package);

	int    GetStatus(const DocKey& key);
	int    GetStatus(const String& nameing, const String& nesting, const String& item, int lang);

	String GetFilePath(const DocKey& key) const;
	String GetPackage(const DocKey& key) const;
	String GetText(const DocKey& key) const;
	String GetLink(const DocKey& key) const;

	void   Remove(const DocKey& key);
	void   SaveText(const String& package, const DocKey& key, const String& text, bool external);
	void   SaveLink(const String& package, const DocKey& key, const String& text);
	void   Ignore(const String& package, const DocKey& key);

	DocSet Select(const DocQuery& query);
};

bool ParseLink(const String& link, DocKey& key, String& label);

extern DocDir doc_dir;

#define LAYOUTFILE "DocEdit.lay"
#include <CtrlCore/lay.h>

struct DocItemDisplay : public Display
{
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color _ink, Color paper, dword style) const;
};

struct DocNestDisplay : public Display
{
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color _ink, Color paper, dword style) const;
};

void InitItemArray(ArrayCtrl& item);
void LoadItems(const DocSet& set, const String& nameing, const String& nesting, ArrayCtrl& item);
int  PosOf(const String& s, const String& subs);
bool IsCppKeyword(const String& id);
int  CompareNests(const Value& v1, const Value& v2);
int  CompareItems(const Vector<Value>& row1, const Vector<Value>& row2);

struct DocLinkDlg : public WithLinkLayout<TopWindow> {
	const DocSet *set;
	bool          sort;
	dword         lang;

	void   Nameing();
	void   Nesting();
	void   Item();
	bool   Perform(const DocSet& set, bool sort, String& link, int lang,
	               const String& nameing = Null, const String& nesting = Null);
	void   Label();
	String Get();

	typedef DocLinkDlg CLASSNAME;

	DocLinkDlg();
};

class DocBrowser : public TopWindow {
public:
	virtual bool Key(dword key, int count);
	virtual void Close();

private:
	struct FileInfo {
		Time               time;
		RichEdit::UndoInfo info;
	};

	ArrayMap<String, FileInfo>          editstate;
	ArrayMap<DocKey, RichEdit::PosInfo> posinfo;

	DropList     nameing;
	StaticRect   larea, rarea;
	ArrayCtrl    nesting;
	ArrayCtrl    item;
	Splitter     ni_split;
	Splitter     vi_split;

	FrameTop<StaticRect> trect;
	Label                title;
	DataPusher           package;
	RichTextView         view;
	RichEdit             edit;

	DropList             lang;

	MenuBar      menu;
	ToolBar      tool;

	ToolBar      editbar;

	WithQueryLayout<TopWindow> query;
	DocSet       set;
	DocKey       current, target;

	DocLinkDlg   linkdlg;

	bool         sort;

	WithExternalLayout<TopWindow> external;

	void   Nameing();
	void   Nesting();
	void   Item();
	void   EnterItem();

	void   ReloadItems();

	DocKey Key();
	void   Flush();
	int    GetCurrentStatus();
	void   SetCurrentStatus(int status);
	String GetItemPackage()      { return item.Get(7); }

	void EditLink(String& s);

	void Edit();
	void View();
	void Sort();
	void Select();
	void Link();
	void Ignore();
	void Delete();
	void New();

	void FollowLink(const String& s);

	void SetMainBar();

	void MainMenu(Bar& bar);
	void ItemMenu(Bar& bar);
	void BrowseMenu(Bar& bar);

	void EditBar(Bar& bar);
	void SetEditBar();

public:
	typedef DocBrowser CLASSNAME;

	void Query();

	DocBrowser();
};
