struct Navigator {
	virtual int GetCurrentLine() = 0;

	struct NavLine : Moveable<NavLine> {
		bool           impl:1;
		int            file:31;
		int            line;

		bool operator<(const NavLine& b) const;
	};

	enum KindEnum { KIND_LINE = -4000, KIND_NEST, KIND_FILE, KIND_SRCFILE };

	struct NavItem : AnnotationItem {
		String path;
	};

	struct NavigatorDisplay : Display {
		const Vector<const NavItem *>& item;

		int DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual void PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual Size GetStdSize(const Value& q) const;

		NavigatorDisplay(const Vector<const NavItem *>& item) : item(item) {}
	};

	struct ScopeDisplay : Display {
		int DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual Size GetStdSize(const Value& q) const;
	};

	Ide             *theide = nullptr;

	Array<NavItem>                             nitem;
	Vector<const NavItem *>                    litem;
	Array<NavItem>                             nest_item; // list separators with nest (scope) or file
	NavigatorDisplay navidisplay;
	bool             navigating; // click on item
	TimeCallback     search_trigger;
	TimeCallback     annotate_trigger;
	bool             navigator_global;
	ToolButton       sortitems;
	bool             sorting;
	bool             dlgmode;

	ArrayCtrl                   scope;
	ArrayCtrl                   list;
	WithDropChoice<EditString>  search;

	void TriggerSearch();
	void Search();
	void SetList();
	void NaviSort();

	void           Navigate();
	void           NavigatorClick();
	void           NavigatorEnter();
	void           SyncCursor();

	typedef Navigator CLASSNAME;

	Navigator();
};

struct AssistEditor : CodeEditor, Navigator {
	virtual bool Key(dword key, int count);
	virtual void LostFocus();
	virtual void ChildLostFocus();
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void SelectionChanged();
	virtual void DirtyFrom(int line);
	virtual void State(int reason);
	virtual void PostInsert(int pos, const WString& s);
	virtual void PostRemove(int pos, int size);

	virtual int  GetCurrentLine();

	struct IndexItem : Moveable<IndexItem> {
		String text;
		Color  ink;
		int    line;
	};
	bool           navigator;
	SplitterFrame  navigatorframe;
	Splitter       scope_list;
	StaticRect     navigatorpane;

	Splitter       popup;
	ArrayCtrl      assist;
	ArrayCtrl      type;

	struct AssistItem : AutoCompleteItem {
		int    typei = -1;
		String uname;
	};

	Index<String>     assist_type;
	Array<AssistItem> assist_item;
	Vector<int>       assist_item_ndx;

	struct AssistDisplay : Display {
		AssistEditor *editor;

		void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const override;
	} assist_display;

	RichTextCtrl           annotation_popup;
	bool                   is_source_file = false;
	bool                   annotating = false;
	Vector<AnnotationItem> annotations;
	Vector<ReferenceItem>  references;
	Vector<AnnotationItem> locals;

	int            assist_cursor;
	bool           auto_assist;
	bool           commentdp;
	bool           include_assist;

	WString        cachedline; // for Ch method
	int            cachedpos; // for Ch method
	int            cachedln; // for Ch method

	bool           include_local;
	int            include_back;
	String         include_path;

	RichTextCtrl   param_info;
	String         param_qtf;
	struct ParamInfo {
		int            line;
		int            pos;
		WString        test;
		AssistItem     item;
		String         editfile;

		ParamInfo()    { line = -1; }
	};
	enum { PARAMN = 16 };
	ParamInfo param[PARAMN];
	int       parami;

	static Ptr<Ctrl> assist_ptr;

	bool      navigator_right = false;
	bool      show_errors = true;
	bool      show_errors_status = true;
	bool      no_empty_autocomplete = false;

	PPInfo             ppi;
	String             master_source;
	Vector<String>     master_chain; // current include -> source file

	Vector<Diagnostic> errors;

	VectorMap<String, MasterSourceCacheRecord> ms_cache;

	int                ToUtf8x(int line, int pos);
	int                FromUtf8x(int line, int pos);
	
	void               ToUtf8x(Point& p)                   { p.x = ToUtf8x(p.y, p.x); }
	void               FromUtf8x(Point& p)                 { p.x = FromUtf8x(p.y, p.x); }

	CurrentFileContext CurrentContext(int pos = INT_MAX);
	void               SetAnnotations(const CppFileInfo& f);
	void               SyncCurrentFile(const CurrentFileContext& ctx);
	void               SyncCurrentFile();
	void               SyncMaster();
	void               NewFile(bool reloading);
	bool               DoIncludeTrick(Index<String>& visited, int level, StringBuffer& out, String path, const String& target_path, int& line_delta);
	void               MakeIncludeTrick(CurrentFileContext& cfx);
	void               ClearErrors();
	void               TriggerSyncFile(int delay_ms);

	void           PopUpAssist(bool auto_insert = false);
	void           CloseAssist();
	static bool    WheelHook(Ctrl *, bool inframe, int event, Point p, int zdelta, dword keyflags);
	void           Assist(bool macros);
	bool           IncludeAssist();
	String         ReadIdBackPos(int& pos, bool include);
	String         ReadIdBack(int q, bool include = false);
	void           SyncAssist();
	void           AssistInsert();
	bool           InCode();

	void           SyncParamInfo();
	void           StartParamInfo(const AssistItem& m, int pos);

	bool           AssistTip(CodeEditor::MouseTip& mt);
	bool           DelayedTip(CodeEditor::MouseTip& mt);
	void           SetQTF(CodeEditor::MouseTip& mt, const String& qtf);

	void           Complete();
	void           Abbr();

	Point          GetCurrentPos() const;
	AnnotationItem FindCurrentAnnotation(bool allow_define = false);

	void           DCopy();
	String         FindCurrentNest(String *local_bases = nullptr);
	void           Virtuals();
	void           Events();

	void           SelParam();
	int            Ch(int q);
	int            ParsBack(int q);
	void           SkipSpcBack(int& q);
	String         IdBack(int& qq);
	String         CompleteIdBack(int& q, const Index<String>& locals);

	bool           WaitCurrentFile();

	AnnotationItem GetCodeAnnotation(const String& id);

	bool           GetAnnotationRefs(Vector<String>& tl, String& coderef, int q = -1);
	String         BestTopic(const Vector<String>& tl);
	bool           GetAnnotationRef(String& t, String& coderef, int q = -1);
	RichText       GetCodeTopic(const String& tl, const String& coderef);
	void           SyncAnnotationPopup();
	void           EditAnnotation(bool fastedit);
	void           OpenTopic(String topic, String create, bool before);
	void           NewTopic(String group, String coderef);

	bool           Esc();

	bool           IsNavigator() const                             { return navigator; }
	void           Navigator(bool navigator);
	void           SyncNavigatorShow();
	void           SyncNavigator();
	void           SerializeNavigator(Stream& s);
	void           SerializeNavigatorWorkspace(Stream& s);
	void           SyncNavigatorPlacement();

	Event<int>     WhenFontScroll;
	Event<>        WhenSelectionChanged;

	typedef AssistEditor CLASSNAME;

	AssistEditor();
	~AssistEditor();
};