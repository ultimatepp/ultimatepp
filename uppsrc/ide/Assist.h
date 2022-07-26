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

	struct ScopeDisplay : Display {
		Navigator *navigator;

		int DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual Size GetStdSize(const Value& q) const;
	};

	struct LineDisplay : Display {
		int DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style, int x) const;
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual Size GetStdSize(const Value& q) const;
	};

	struct NavigatorDisplay : Display {
		const Vector<const NavItem *>& item;

		int DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual void PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
		virtual Size GetStdSize(const Value& q) const;

		NavigatorDisplay(const Vector<const NavItem *>& item) : item(item) {}
	};

	Ide             *theide;

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

	ArrayCtrl         scope;
	ArrayCtrl         list;
	EditString        search;

	ScopeDisplay      scope_display;

	void TriggerSearch();
	void Search();
	void Scope();
	void ListLineEnabled(int i, bool& b);
	void NaviSort();

	void           Navigate();
	void           ScopeDblClk();
	void           NavigatorClick();
	void           NavigatorEnter();
	void           SyncCursor();

	typedef Navigator CLASSNAME;

	Navigator();
};

struct AssistEditor : CodeEditor, Navigator {
	virtual bool Key(dword key, int count);
	virtual void LostFocus();
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void SelectionChanged();
	virtual void DirtyFrom(int line);
	virtual void State(int reason);

	virtual int  GetCurrentLine();

	struct IndexItem : Moveable<IndexItem> {
		String text;
		Color  ink;
		int    line;
	};
	bool           navigator;
	SplitterFrame  navigatorframe;
	StaticRect     navigatorpane;
	Splitter       navigator_splitter;

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

	RichTextCtrl   annotation_popup;
	bool                   annotating = false;
	Vector<AnnotationItem> annotations;

	int            assist_cursor;
	bool           auto_assist;
	bool           auto_check;
	bool           assist_active;
	bool           commentdp;
	bool           inbody;
	bool           include_assist;
	WString        cachedline;
	int            cachedpos;
	int            cachedln;
	bool           include_local;
	int            include_back;
	String         include_path;
	int            scan_counter;

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

	bool      navigator_right = true;
	
	Hdepend   hdepend;
	String    master_source;

	CurrentFileContext CurrentContext(int& line_delta);
	void               SyncCurrentFile(CurrentFileContext& ctx, int line_delta);
	void               SyncCurrentFile();
	void               SyncHeaders();
	void               NewFile();
	bool               DoIncludeTrick(Index<String>& visited, int level, StringBuffer& out, String path, const String& target_path, int& line_delta);
	void               MakeIncludeTrick(CurrentFileContext& cfx, int& line_delta);

	void           PopUpAssist(bool auto_insert = false);
	void           CloseAssist();
	static bool    WheelHook(Ctrl *, bool inframe, int event, Point p, int zdelta, dword keyflags);
	void           Assist(bool macros);
	bool           IncludeAssist();
	String         ReadIdBackPos(int& pos, bool include);
	String         ReadIdBack(int q, bool include = false, bool *destructor = NULL);
	void           SyncAssist();
	void           AssistInsert();
	bool           InCode();

	void           SyncParamInfo();
	void           StartParamInfo(const AssistItem& m, int pos);

	void           Complete();
	void           Abbr();

	void           Context(ParserContext& parser, int pos);
	void           ExpressionType(const String& type,
	                              const String& context_type,
	                              const String& usings,
	                              const Vector<String>& xp, int ii,
	                              Index<String>& typeset, bool variable,
	                              bool can_shortcut_operator, Index<String>& visited_bases,
	                              int lvl);
	void           ExpressionType(const String& type,
	                              const String& context_type,
	                              const String& usings,
	                              const Vector<String>& xp, int ii,
	                              Index<String>& typeset, bool variable, int lvl);
//	void           ExpressionType(const String& type, const Vector<String>& xp, int ii,
//	                              Index<String>& typeset);
	Index<String>  ExpressionType(const ParserContext& parser, const Vector<String>& xp);

	Index<String>  EvaluateExpressionType(const ParserContext& parser, const Vector<String>& xp);

	String         RemoveDefPar(const char *s);
	String         MakeDefinition(const String& cls, const String& _n);
	void           DCopy();
	void           Virtuals();
	void           Thisbacks();

	void           SelParam();
	int            Ch(int q);
	int            ParsBack(int q);
	Vector<String> ReadBack(int q, const Index<String>& locals);
	void           SkipSpcBack(int& q);
	String         IdBack(int& qq);
	String         CompleteIdBack(int& q, const Index<String>& locals);

	void           SwapSContext(ParserContext& p);

	const AnnotationItem *GetAnnotationPtr(const String& id);

	bool           GetAnnotationRefs(Vector<String>& tl, String& coderef, int q = -1);
	bool           GetAnnotationRef(String& t, String& coderef, int q = -1);
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
	void           SyncNavigatorPlacement();

	Event<int>     WhenFontScroll;
	Event<>        WhenSelectionChanged;

	typedef AssistEditor CLASSNAME;

	AssistEditor();
	~AssistEditor();
};