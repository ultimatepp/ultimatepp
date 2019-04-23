class RichText : public RichTxt, public DeepCopyOption<RichText> {
	mutable Mutex mutex; // To cover all those laze evaluation scenarios
	RichStyles    style;
	String        footer_hack; // ugly hack
	bool          nolinks = false; // another ugly hack

	void       Init();

	void       PaintHeaderFooter(PageDraw& pw, const Rect& page, PageY py, const PaintInfo& pi,
	                             int from_page, int to_page) const;

	struct StyleChangeOp;
	struct SetStylesOp;
	struct RemoveStyleOp;
	struct MergeStylesOp;
	friend struct MergeStylesOp;
	struct OverrideStylesOp;

public:
	void                  Clear();

	int                   GetStyleCount() const                { return style.GetCount(); }
	Uuid                  GetStyleId(int i) const              { return style.GetKey(i); }
	Uuid                  GetStyleId(const String& name) const;
	const RichStyle&      GetStyle(int i) const                { return style[i]; }
	const RichStyle&      GetStyle(const Uuid& id) const       { return UPP::GetStyle(style, id); }
	void                  SetStyle(const Uuid& id, const RichStyle& _style);
	bool                  HasStyle(const Uuid& id)             { return style.Find(id) >= 0; }
	void                  RemoveStyle(const Uuid& id);

	const RichStyles&     GetStyles() const                    { return style; }
	void                  SetStyles(const RichStyles& styles);
	void                  OverrideStyles(const RichStyles& masterstyles,
	                                     bool matchname = true, bool addmissing = true);
	void                  MergeStyles(const RichText& text);

	RichPara              Get(int i) const                     { return RichTxt::Get(i, style); }
	void                  Cat(const RichPara& p)               { RichTxt::Cat(p, style); }
	void                  CatPick(RichText&& p);
	using                 RichTxt::CatPick;

	RichContext           Context(const Rect& page, PageY py, RichText *header, RichText *footer) const;
	RichContext           Context(const Rect& page, PageY py) const { return Context(page, py, ~header, ~footer); }
//	RichContext           Context(const Rect& page) const { return Context(page, PageY(0, 0)); }

	RichPos               GetRichPos(int pos, int maxlevel = INT_MAX) const;
	int                   operator[](int pos) const            { return GetRichPos(pos).chr; }

	void                  ApplyZoom(Zoom z);

	PageY                 GetHeight(PageY py, const Rect& page) const;
	PageY                 GetHeight(const Rect& page) const;
	void                  Paint(PageDraw& w, PageY py, const Rect& page, const PaintInfo& pi) const;
	void                  Paint(PageDraw& w, const Rect& page, const PaintInfo& pi) const;
	RichCaret             GetCaret(int pos, const Rect& page) const;
	int                   GetPos(int x, PageY y, const Rect& page) const;
	RichHotPos            GetHotPos(int x, PageY y, int tolerance, const Rect& page) const;
	int                   GetVertMove(int pos, int gx, const Rect& page, int dir) const;
	int                   GetWidth() const;

	int                   GetHeight(int cx) const;
	int                   GetHeight(Zoom zoom, int cx) const;
	void                  Paint(Draw& w, int x, int y, int cx, const PaintInfo& pi) const;
	void                  Paint(Zoom zoom, Draw& w, int x, int y, int cx) const;
	void                  Paint(Draw& w, int x, int y, int cx) const;

	Vector<RichValPos>    GetValPos(const Rect& page, int type) const;

	int                   AdjustCursor(int anchor, int cursor) const;

	void                  Remove(int pos, int count);
	void                  Insert(int pos, const RichText& p);
	RichText              Copy(int pos, int count) const;

	FormatInfo            GetFormatInfo(int pos, int count) const;
	void                  ApplyFormatInfo(int pos, const FormatInfo& fi, int count);
	void                  ReplaceStyle(int pos, const Uuid& id);

	void                  ReStyle(int pos, const Uuid& id);

	Formating             SaveFormat(int pos, int count) const;
	void                  RestoreFormat(int pos, const Formating& info);


	RichTable::Format     GetTableFormat(int table) const;
	void                  SetTableFormat(int table, const RichTable::Format& fmt);
	int                   SetTable(int pos, const RichTable& table);
	void                  DestroyTable(int table);
	RichTable             CopyTable(int table) const;
	void                  ReplaceTable(int table, const RichTable& tab);
	void                  RemoveParaSpecial(int table, bool before);
	bool                  CanRemoveParaSpecial(int table, bool before);
	void                  InsertParaSpecial(int table, bool before, const RichPara::Format& fmt);
	bool                  ShouldInsertParaSpecial(int table, bool before);
	RichTable             CopyTable(int table, const Rect& sel) const;
	void                  ClearTable(int table, const Rect& sel);
	void                  PasteTable(int table, Point pos, const RichTable& tab);
	void                  InsertTableRow(int table, int row);
	void                  RemoveTableRow(int table, int row);
	void                  InsertTableColumn(int table, int column);
	void                  RemoveTableColumn(int table, int column);
	Point                 GetMasterCell(int table, int row, int column);
	Point                 GetMasterCell(int table, Point p)                 { return GetMasterCell(table, p.y, p.x); }
	void                  SplitCell(int table, Point cell, Size sz);
	void                  JoinCell(int table, const Rect& sel);
	RichCell::Format      GetCellFormat(int table, const Rect& sel) const;
	void                  SetCellFormat(int table, const Rect& sel, const RichCell::Format& fmt,
	                                    bool setkeep, bool setround);
	FormatInfo            GetTableFormatInfo(int table, const Rect& sel) const;
	void                  ApplyTableFormatInfo(int table, const Rect& sel, const RichText::FormatInfo& fi);
	void                  AdjustTableSel(int table, Rect& sel) const        { return GetConstTable(table).AdjustSel(sel); }

	bool                  Iterate(Iterator& r) const                        { return RichTxt::Iterate(r, 0, style); }
	bool                  Iterate(UpdateIterator& r)                        { return RichTxt::Iterate(r, 0, style); }

	bool                  EvaluateFields(VectorMap<String, Value>& vars)    { return RichTxt::EvaluateFields(style, vars); }

	void                  InvalidateAll()                                   { r_type = ALL; }
	void                  Validate();
	bool                  GetInvalid(PageY& top, PageY& bottom, const Rect& page,
	                                 int sell, int selh, int osell, int oselh) const;

	struct ClipboardType {
		ClipboardType();
		virtual ~ClipboardType();

		virtual int      Level();
		virtual RichText ReadClipboard(const RichPara::Format& f) = 0;
		virtual void     WriteClipboard(const RichText& text) = 0;
	};

	static void           Register(ClipboardType& type);
	
	//Ugly hacks
	void                  SetFooter(const String& s)                          { footer_hack = s; }
	String                GetFooter() const                                   { return footer_hack; }
	void                  PrintNoLinks(bool b = true)                         { nolinks = b; }
	bool                  IsPrintNoLinks() const                              { return nolinks; }

	RichText()            { Init(); }
	RichText(const RichText& x, int);
	RichText(RichText&& x);
	RichText(RichTxt&& x, RichStyles&& st);
	
	RichText& operator=(RichText&& x);
};
