#ifndef _Browser_Browser_h
#define _Browser_Browser_h

#include <CtrlLib/CtrlLib.h>
#include <ide/Common/Common.h>
#include <ide/clang/clang.h>
#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

#define IMAGECLASS BrowserImg
#define IMAGEFILE <ide/Browser/Browser.iml>
#include <Draw/iml_header.h>

INITIALIZE(CodeBase)

inline Font BrowserFont() { return StdFont(); }

struct TopicInfo : Moveable<TopicInfo> {
	Time           time;
	String         path;
	String         title;
	Vector<int>    words;
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

String          TppId(const String& ref, bool remove_first_scope = false);
String          CleanupTppId(const String& ref);
Vector<String>  AnnotationCandidates(const String& ref);
Vector<String>  GetRefLinks(const String& ref);

int             TopicWordIndex(const String& w);
int             MatchWord(const Vector<int>& w, const String& pattern);
bool            MatchTopicLink(const String& link, const Vector<String>& query);

#define LAYOUTFILE <ide/Browser/Topic.lay>
#include <CtrlCore/lay.h>


#define IMAGEFILE <ide/Browser/Topic.iml>
#define IMAGECLASS TopicImg
#include <Draw/iml_header.h>

int  CharFilterID(int c);

bool ParseTopicFileName(const String& fn, String& topic, int& lang);

Topic  ReadTopic(const char *text);
Vector<String> GatherLabels(const RichText& text);
String WriteTopic(const char *title, const RichText& text);
String WriteTopicI(const char *title, const RichText& text);

bool LoadTopics(Vector<String>& topics, const String& dir);
void FillTopicsList(FileList& list, const Vector<String>& topics);

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
		return (String)~T::topic + "_" + ToLower(LNGAsText(~T::lang)) + ".tpp";
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
	virtual void   RestoreEditPos()                                 { editor.SetFocus(); }


	virtual bool Key(dword key, int);

protected:
	ToolBar           tool;
	
	Vector<String>    topics;
	ParentCtrl        topics_parent;
	FileList          topics_list;
	EditString        topics_search;

	EditString        title;
	RichEdit          editor;

	StaticRect        right;
	Splitter          left_right;
	
	String            grouppath;
	String            topicpath;
	String            singlefilepath;

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

protected:
	void   FormatMenu(Bar& bar);
	void   TableMenu(Bar& bar);
	void   TopicMenu(Bar& bar);
	void   MainTool(Bar& bar);
	void   FileBar(Bar& bar);
	void   SetBar();

	String GetCurrentTopicPath();

	void   InsertNew(const String& id, const String& pretty);
	void   InsertNew(const AnnotationItem& m);
	void   NewTopic();
	void   MoveTopic();
	void   RemoveTopic();

	bool         autosave;
//	ReferenceDlg ref; // TODO

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

	void   FindBrokenRef();
	void   JumpToDefinition();
	String GetLang() const;

	void   OnSearch();
	void   DoSearch();
	void   InsertTableOfContents();
	
public:
	Event<Bar&> WhenTemplatesMenu;

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
	bool NewTopicEx(const String& iname, const AnnotationItem& create);
	void Open(const String& grouppath);
	void OpenFile(const String& path);
	void GoTo(const String& _topic, const String& link, const AnnotationItem& create, bool before);
	void GoToPart(int ii);
	void PersistentFindReplace(bool b)               { editor.PersistentFindReplace(b); }
	
	static int  GetSerial();

	static void SerializeEditPos(Stream& s);
	
	void Serialize(Stream& s);

	TopicEditor();
	virtual ~TopicEditor();
};

#endif
