class RichTxt : DeepCopyOption<RichTxt> {
public:
	enum {
		BOLD       = 0x00000001,
		ITALIC     = 0x00000002,
		UNDERLINE  = 0x00000004,
		FACE       = 0x00000008,
		HEIGHT     = 0x00000010,
		LANGUAGE   = 0x00000020,
		INK        = 0x00000040,
		PAPER      = 0x00000080,
		LINK       = 0x00000100,
		SSCRIPT    = 0x00000200,
		CAPITALS   = 0x00000400,
		STRIKEOUT  = 0x00000800,
		LANG       = 0x00001000,
		INDEXENTRY = 0x00002000,
		DASHED     = 0x00004000,
		NOAA       = 0x00008000,
	};

	enum {
		ALIGN     = 0x80000000,
		BEFORE    = 0x40000000,
		LM        = 0x20000000,
		INDENT    = 0x10000000,
		RM        = 0x08000000,
		AFTER     = 0x04000000,
		TABSIZE   = 0x02000000,
		BULLET    = 0x01000000,
		NEWPAGE   = 0x00800000,
		KEEP      = 0x00400000,
		TABS      = 0x00200000,
		STYLE     = 0x00100000,
		LABEL     = 0x00080000,
		KEEPNEXT  = 0x00040000,
		ORPHAN    = 0x00020000,
		NUMBERING = 0x00010000,
		SPACING   = 0x00008000,
		RULER     = 0x00004000,
		RULERINK  = 0x00002000,
		RULERSTYLE= 0x00001000,
		NEWHDRFTR = 0x00000800,
		FIRSTONPAGE = 0x00000400,
	};

	struct FormatInfo : RichPara::Format {
		dword charvalid;
		dword paravalid;

		void  Set(const RichPara::Format& fmt);
		void  Set(const RichPara::CharFormat& fmt);
		void  Combine(const RichPara::CharFormat& fmt);
		void  Combine(const RichPara::Format& fmt);
		void  Combine(const FormatInfo& fmt);
		void  ApplyTo(RichPara::CharFormat& fmt) const;
		void  ApplyTo(RichPara::Format& fmt) const;
	};

	class Formating {
		Vector<Uuid>    styleid;
		Vector<String>  format;
		friend class RichText;
		friend class RichTxt;
	};

protected:
	struct Para : DeepCopyOption<Para> {
		Uuid                  styleid;
		int                   length;
		String                content;
		Array<RichObject>     object;
		One<RichPara::NumberFormat> number_fmt;

		mutable int64         updateserial;
		mutable RichPara::Number number;
		mutable Vector<int>   linecy;
		mutable Bits          spellerrors;
		mutable String        header_qtf, footer_qtf;
		mutable One<RichText> header, footer;
		mutable int           ccx;
		mutable int           cy;
		mutable int           ruler;
		mutable int           before;
		mutable int           after;
		mutable bool          newpage;
		mutable bool          firstonpage;
		mutable bool          keep;
		mutable bool          keepnext;
		mutable bool          orphan;
		mutable int           numbering;
		mutable bool          checked;
		mutable bool          haspos;
		mutable bool          newhdrftr;

		void Invalidate() const;

		Para(const Para& src, int);
		Para() { length = 0; Invalidate(); numbering = -1; checked = false; haspos = false; }
	};

	struct Part : MoveableAndDeepCopyOption<Part>, Any {
		Part(const Part& src, int);
		Part();
	};

	Vector<Part>           part;
	String                 header_qtf, footer_qtf;
	mutable One<RichText>  header, footer;
	mutable int            length;
	mutable int            tabcount;

	enum {
		NONE, SPARA, PARA, FROM, ALL
	};

	int  r_type;
	int  r_parti;
	int  r_paraocx;
	int  r_paraocy;
	bool r_keep = false;
	bool r_keepnext = false;
	bool r_newpage = false;
	bool r_firstonpage = false;

	void       Init();

	void       Invalidate();

	void       SetRefresh(int parai);
	void       SetRefreshFrom(int parai);
	void       RefreshAll()                   { SetRefreshFrom(0); }

	void       ParaRemove(int parai, int pos, int count);
	void       ParaInsert(int parai, int pos, const RichPara& p);
	RichPara   ParaCopy(int parai, int pos, int count) const;

	void       Put(int i, const RichPara& p, const RichStyle& s);
	void       Put(int i, const RichPara& p, const RichStyles& s);

	void        Sync0(const Para& pp, int parti, const RichContext& rc) const;
	void        Sync(int parti, const RichContext& rc) const;
	bool        BreaksPage(PageY py, const Para& pp, int i, const Rect& page) const;
	void        Advance(int parti, RichContext& rc, RichContext& begin) const;
	RichContext GetAdvanced(int parti, const RichContext& rc, RichContext& begin) const;
	RichContext GetPartContext(int parti, const RichContext& rc0) const;
	PageY       GetPartPageY(int parti, const RichContext& rc) const     { return GetPartContext(parti, rc).py; }

	struct ParaOp {
		virtual bool operator()(RichTxt::Para& p) = 0;
		virtual ~ParaOp() {}
	};
	bool             Update(ParaOp& op);
	RichTxt&         GetText0(int& pos, bool update);
	RichTxt&         GetUpdateText(int& pos);
	const RichTxt&   GetConstText(int& pos) const;
	RichTable&       GetTable0(int table, bool update);
	RichTable&       GetUpdateTable(int table);
	RichTxt&         GetTableUpdateText(int table, const RichStyles& style, int& pi);

	RichPara         Get(int parti, const RichStyles& s, bool usecache) const;
	RichPara         Get(int parai, const Uuid& styleid, const RichStyles& s, bool usecache) const;
	RichPara         Get(int parai, const RichStyle& style, bool usecache) const;

	void        CombineFormat(FormatInfo& f, int pi, int pi2, bool& first, const RichStyles& style) const;
	static void ApplyStyle(const FormatInfo& fi, RichPara& pa, const RichStyles& style);
	static void Apply(const FormatInfo& fi, RichPara& pa, const RichStyles& style);
	void        ApplyFormat(const FormatInfo& f, int pi, int pi2, const RichStyles& style);
	void        SaveFormat(Formating& f, int p1, int p2, const RichStyles& style) const;
	void        RestoreFormat(int pi, const Formating& info, int& ii, const RichStyles& style);

	void        GetAllLanguages(Index<int>& all) const;

	friend class RichTable;
	friend class RichText;
	friend class RichCell;

	int                   ComputeLength() const;

	void                  GetRichPos(int pos, RichPos& rp, int ti, int maxlevel, const RichStyles& st) const;
	RichPara::Format      GetFirstFormat(const RichStyles& st) const;

	PageY                 GetTop(RichContext rc) const;
	PageY                 GetHeight(RichContext rc) const;
	int                   GetWidth(const RichStyles& st) const;
	void                  Paint(PageDraw& w, RichContext& rc, const PaintInfo& pi) const;
	RichCaret             GetCaret(int pos, RichContext rc) const;
	int                   GetPos(int x, PageY y, RichContext rc) const;
	int                   GetVertMove(int pos, int gx, RichContext rc, int dir) const;
	RichHotPos            GetHotPos(int x, PageY y, int tolerance, RichContext rc) const;

	void                  GatherValPos(Vector<RichValPos>& f, RichContext rc, int pos, int type) const;

	bool                  EvaluateFields(const RichStyles& s, VectorMap<String, Value>& vars);

	void                  ApplyZoom(Zoom z, const RichStyles& ostyle, const RichStyles& zstyle);

public:
	enum ValPosType { LABELS, INDEXENTRIES };

	int                   GetPartCount() const                 { return part.GetCount(); }
	bool                  IsPara(int i) const                  { return part[i].Is<Para>(); }
	bool                  IsTable(int i) const;
	int                   GetPartLength(int pi) const;
	int                   FindPart(int& pos) const;
	int                   GetPartPos(int pi) const;

	RichPara              Get(int i, const RichStyles& s) const;
	RichPara              Get(int i, const Uuid& styleid, const RichStyles& s) const;
	RichPara              Get(int i, const RichStyle& style) const;
	Uuid                  GetParaStyle(int i) const            { return part[i].Get<Para>().styleid; }
	void                  SetParaStyle(int i, const Uuid& si);

	const RichTable&      GetTable(int i) const;

	int                   GetLength() const;

	int                   GetTableCount() const;
	const RichTable&      GetConstTable(int table) const;

	bool                  IsEmpty() const;

	RichCellPos           GetCellPos(int table, int row, int column) const;
	RichCellPos           GetCellPos(int table, Point p) const;

	void                  Clear();

	void                  ClearSpelling();

	void                  SetPick(int parti, RichTable&& table);
	void                  CatPick(RichTable&& table)                  { SetPick(GetPartCount(), pick(table)); }
	void                  Set(int parai, const RichPara& p, const RichStyles& s);
	void                  Cat(const RichPara& p, const RichStyles& s) { Set(GetPartCount(), p, s); }

	void                  RemovePart(int parti);

	void                  Normalize();

	Vector<int>           GetAllLanguages() const;
	WString               GetPlainText(bool withcr = true, bool allow_tabs = false) const;

	void                  SetHeaderQtf(const char *qtf);
	void                  SetFooterQtf(const char *qtf);
	String                GetHeaderQtf() const                        { return header_qtf; }
	String                GetFooterQtf() const                        { return footer_qtf; }
	void                  ClearHeader()                               { SetHeaderQtf(NULL); }
	void                  ClearFooter()                               { SetFooterQtf(NULL); }

	struct UpdateIterator {
		enum { CONTINUE = 0, STOP = 1, UPDATE = 2 };
		virtual int operator()(int pos, RichPara& para) = 0;
		virtual ~UpdateIterator() {}
	};
	bool                  Iterate(UpdateIterator& r, int gpos, const RichStyles& s);

	struct Iterator {
		virtual bool operator()(int pos, const RichPara& para) = 0;
		virtual ~Iterator() {}
	};
	bool                  Iterate(Iterator& r, int gpos, const RichStyles& s) const;

	RichTxt(const RichTxt& src, int);
	RichTxt();

	rval_default(RichTxt);

#ifdef _DEBUG
	void                  Dump();
#endif
};
