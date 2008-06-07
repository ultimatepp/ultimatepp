NAMESPACE_UPP

#define LAYOUTFILE <TCtrlLib/DocMgr.lay>
#include <CtrlCore/lay.h>

class DlgFile;

class DocType;
class DocCtrl;
class DocList;

class DocType : Moveable<DocType>
{
public:
	DocType(DocCtrl *(*create_doc)() = 0);

	int              GetIndex() const;

	DocType&         Icon(Image l, Image s)         { large_icon = l; small_icon = s; return *this; }
	DocType&         SmallIcon(Image s)             { small_icon = s; return *this; }
	DocType&         LargeIcon(Image l)             { large_icon = l; return *this; }
	Image            GetSmallIcon() const           { return small_icon; }
	Image            GetLargeIcon() const           { return large_icon; }
	Image            GetIcon(bool s) const          { return s ? small_icon : large_icon; }

	DocType&         Paper(Color _paper)            { paper = _paper; return *this; }
	Color            GetPaper() const               { return paper; }

	DocType&         DocName(const char *_ndn);
	String           GetDocName() const             { return doc_name; }

	DocType&         EnableNewList(bool en = true)  { new_list = en; return *this; }
	DocType&         DisableNewList()               { return EnableNewList(false); }
	bool             IsNewListEnabled() const       { return new_list; }

	DocType&         Filter(const char *_filter, const char *_defext);
	String           GetFilter() const              { return filter; }
	String           GetExt() const                 { return extensions; }

	DocType&         DefaultName(String dn)         { default_name = dn; return *this; }
	String           GetDefaultName() const         { return default_name; }

	DocType&         StreamName(const char *sn);
	String           GetStreamName() const;
	static const DocType *FindStreamName(String sn);

	DocType&         HelpTopic(const char *_ht);
	String           GetHelpTopic() const           { return help_topic; }

	DocType&         CreateDoc(DocCtrl *(*c)())     { create_doc = c; return *this; }
	DocCtrl       *(*GetCreateDoc() const)()        { return create_doc; }

	DocType&         DetectDoc(bool (*d)(Stream&))  { detect_doc = d; return *this; }
	bool           (*GetDetectDoc() const)(Stream&) { return detect_doc; }

	DocCtrl         *CreateDocument(DocList& list) const;

	static const DocType *GetDocType(const char *filename);

	static DocCtrl  *CreateDefaultDocument(DocList& list);
	static DocCtrl  *CreateAnyDocument(DocList& list);

	static void      SetOpenFileDlg(FileSelector& fsel);
	void             SetSaveFileDlg(FileSelector& fsel) const;

public:
	DocCtrl       *(*create_doc)();
	bool           (*detect_doc)(Stream& stream);

	String           stream_name;
	String           doc_name;
	String           filter;
	String           extensions;
	String           default_name;
	String           help_topic;
	Image            small_icon;
	Image            large_icon;
	bool             new_list;

	Color            paper;

	static Vector<const DocType*> doc_types;

	static int      Add(const DocType& doctype);
};

class DocCtrl : public TopWindow
{
	friend class DocList;

public:
	typedef DocCtrl CLASSNAME;

	DocCtrl();
	DocCtrl(const DocCtrl& another)                       { NEVER(); }
	virtual ~DocCtrl();

	DocCtrl&         SetDocType(const DocType *type)      { doc_type = type; return *this; }
	const DocType   *GetDocType() const                   { return doc_type; }

	DocCtrl&         SetDocList(DocList *list);
	DocList         *GetDocList() const                   { return doc_list; }

	void             SetActive(bool kill_init = false);
	virtual void     Activate();

	virtual void     Serialize(Stream& stream);   // default does nothing

	virtual bool     New();
	virtual bool     Open(const char *doc, bool rdonly = false); // default uses Serialize to load the file
	virtual bool     Save(const char *as); // default uses Serialize to save the file
	virtual bool     CloseDoc();

	bool             IsOpenDoc() const { return !IsNull(filename); }

	virtual bool     IsTopWindow() const; // tests presence of overlaid modal windows

//	bool             BeginApplModal();
//	void             EndApplModal();
//	bool             RunApplModal(Callback cb);

	bool             SaveAs(const char *fn = NULL);

	enum // change reasons
	{
		NONE,

		OPENWINDOW,

		UNTOUCH,
		TOUCH,

		READ_ONLY,
		WRITEABLE,

		RENAME,
		APPNAME,

		NEW,
		OPEN,
		CLOSE,

		OTHER_REASON,
	};

	virtual void     Change(int reason);
	virtual void     State(int reason);

	DocCtrl&         SetReadOnly(bool _ro = true);
	DocCtrl&         SetWriteable()               { return SetReadOnly(false); }
	bool             IsReadOnly() const           { return is_read_only; }

	void             Touch(bool _touch = true);
	void             DoTouch()                    { Touch(true); } // for use in Callbacks
	void             Untouch()                    { Touch(false); }
	bool             IsDirty() const              { return is_dirty; }

	bool             IsUnnamed() const            { return is_unnamed; }
	bool             IsClosing() const            { return is_closing; }

	virtual bool     FlushDirty();                // prompts to save/discard/cancel

	void             DoClose();

	virtual bool     HotKey(dword key); // checks Ctrl+Tab / Ctrl+Shift+Tab
	virtual void     Paint(Draw& draw);

	virtual void     ClipboardChanged()           { Rescan(); }

	TOOL(FileSave)
	TOOL(FileSaveAs)
	TOOL(FileClose)

	void             ToolFile(Bar& bar);

	TOOL(WindowToolBar)
	TOOL(WindowInfoBar)

	void             ToolWindow(Bar& bar);
	void             AddToolWindow(Bar& bar); // handles anchor prefix

	TOOL(HelpContents)
	TOOL(HelpSearch)
	TOOL(HelpBook)

	void             ToolHelp(Bar& bar);

	virtual void     BarView(Bar& bar); // generates view-specific tools
	virtual void     BarTool(Bar& bar);

	virtual void     Rescan(); // refresh menu & toolbar

	void             OnWindowActivate();
	void             OnWindowClose();

	void             SetName(String name, bool clear_unnamed = true);
	String           GetName() const { return filename; }

	virtual String   GetDirtyName() const;
	virtual String   GetShortName() const;
	virtual String   GetFullName() const;

	virtual String   QueryFileName();

public:
	MenuBar          menu_bar;
	ToolBar          tool_bar;
	StatusBar        info_bar;

/*
	class Modal
	{
	public:
		Modal(DocCtrl *c)     { ctrl = (c -> BeginApplModal() ? c : 0); }
		~Modal()              { if(ctrl) ctrl -> EndApplModal(); }

		operator bool() const { return ctrl; }

	private:
		DocCtrl *ctrl;
	};
*/

protected:
	void             PostClose();

protected:
	const DocType   *doc_type;
	DocList         *doc_list;
	String           filename;
	bool             is_dirty;
	bool             is_unnamed;
	bool             is_read_only;
	bool             is_closing;
};

class DocList : public WithDocListWindowsLayout<TopWindow>
{
	friend class DocCtrl;

public:
	typedef DocList        CLASSNAME;
	typedef Array<DocCtrl> DocVector;

	DocList();

	virtual void       Serialize(Stream& stream); // serializes LRU

	DocCtrl           *OpenDoc(const char *filename, int type_index = -1);
	bool               OpenExistingDoc(const char *filename);

	virtual DocList&   AppName(const char *app_name);
	String             GetAppName() const                 { return app_name; }

	DocList&           StoreDocs(bool _sd = true)         { store_docs = _sd; return *this; }
	DocList&           NoStoreDocs()                      { return StoreDocs(false); }
	bool               IsStoreDocs() const                { return store_docs; }

	int                GetIndex(const DocCtrl *doc) const { return doc_list.GetIndex(*doc); }
	int                GetDocCount() const                { return doc_list.GetCount(); }
	DocCtrl&           GetDoc(int i)                      { return doc_list[i]; }
	const DocCtrl&     GetDoc(int i) const                { return doc_list[i]; }
	const DocVector&   GetDocVector() const               { return doc_list; }

	Vector<DocCtrl *>  FindDoc(const DocType& type) const;
	bool               IsDocAvail(const DocType& type) const;

	int                GetNextDocIndex()                  { return ++doc_counter; }

	void               SetActive(int index, bool kill_init = false);
	DocCtrl           *GetActive();
	const DocCtrl     *GetActive() const;
	int                GetActiveIndex() const;

	int                FindDoc(const char *filename) const;

	bool               Close(int index);

	void               AddLRU(const DocCtrl *doc);
	void               AddLRU(const char *doc_name, const DocType *type);
	void               RemoveLRU(const char *doc_name);

	DocList&           SetRecentPath(String path)         { recent_path = path; return *this; }
	String             GetRecentPath() const              { return recent_path; }

	virtual void       Activate();
	virtual void       Change(DocCtrl& doc, int change);

	void               Maximize(bool _maximize)           { maximize = _maximize; }
	bool               IsMaximize() const                 { return maximize; }

	TOOL(FileNew)
	TOOL(FileNewDefault)
	TOOL(FileOpen)
	TOOL(FileSaveAll)
	bool               ToolFileLru(Bar& bar);
	void               OnFileLru(int index);
	TOOL(FileQuit)

	void               ToolFileIo(Bar& bar, DocCtrl *doc);

	TOOL(FileMisc)

	TOOL(WindowActivate)
	TOOL(WindowList)
	TOOL(WindowPrev)
	TOOL(WindowNext)

	TOOL(HelpContents)
	TOOL(HelpSearch)
	TOOL(HelpBook)

	void               OnRemove();
	void               OnActivate();
	void               OnClose();

	void               StdBar(Bar& bar);

	virtual int        SaveAll();
	virtual bool       CloseAll(bool save = true);

	void               Run();

protected:
	virtual void       Insert(DocCtrl *document);
	virtual void       Remove(DocCtrl *document);
	virtual void       Detach(DocCtrl *document);
	virtual void       Raise(DocCtrl *document);

	void               RefreshTable(); // refreshes if open
	void               UpdateTable(int new_ix = Null);

	void               Destroy();
	void               SetActiveTop(DocCtrl *ignore = NULL);

protected:
	struct DocInfo
	{
		DocInfo() {}

		DocInfo& Name(String _name) { name = _name; return *this; }
		DocInfo& Type(String _type) { type = _type; return *this; }

		String name;
		String type;

		void Serialize(Stream& stream);
	};

	bool            store_docs;
	bool            lock;
	bool            maximize;
	String          app_name;
	int             doc_counter;
	DocVector       doc_list;
	Array<DocInfo>  lru;
	Array<DocInfo>  old_docs;
	String          recent_path;
};

template <class T>
T *CreateDocument(DocList& doclist, T * = 0)
{
	DocCtrl *doc = T::GetType().CreateDocument(doclist);
	T *t = NULL;
	if(doc && !(t = dynamic_cast<T *>(doc)))
		doc -> DoClose();
	return t;
}

template <class T>
Vector<T *> FindAllDocuments(DocList& doclist, T * = 0)
{
	Vector<T *> sum;
	for(int i = 0; i < doclist.GetDocCount(); i++)
	{
		T *doc = dynamic_cast<T *>(&doclist.GetDoc(i));
		if(doc && !doc -> IsClosing())
			sum.Add(doc);
	}
	return sum;
}

#if 0
class DocPokus : public DocCtrl
{
public:
	DocPokus();
	virtual ~DocPokus();

public:
	static void     RegisterType();
	static DocCtrl *CreateDoc();
};
#endif

END_UPP_NAMESPACE
