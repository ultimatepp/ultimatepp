class RichCell : DeepCopyOption<RichCell> {
public:
	struct Format {
		Rect     border;
		Rect     margin;
		int      align;
		int      minheight;
		Color    color;
		Color    bordercolor;
		bool     keep;

		Format();
	};

	Format   format;
	int      vspan, hspan;
	RichTxt  text;

private:
	bool     Reduce(RichContext& rc) const;
	PageY    Align(const RichContext& rc, PageY npy) const;
	void     DrawCell(Draw& w, int l, int r, int y, int yy, const Rect& border) const;

	void        ClearText();
	void        ClearText(const RichPara::Format& format, const RichStyles& style);
	void        Clear();

	PageY       GetTop(RichContext rc) const;
	PageY       GetHeight(RichContext rc) const;
	void        Paint(PageDraw& pw, RichContext rc, PageY npy,
	                  const Rect& xpg, int y, int ny, const PaintInfo& pi,
	                  bool select) const;
	RichCaret   GetCaret(int pos, RichContext rc, PageY pyy) const;
	int         GetPos(int x, PageY y, RichContext rc, PageY pyy) const;
	RichHotPos  GetHotPos(int x, PageY y, int tolerance, RichContext rc, PageY pyy) const;

	void        GatherValPos(Vector<RichValPos>& f, RichContext rc, PageY pyy, int pos, int type) const;

	friend class RichTable;
	friend class RichText;

public:
	RichCell(const RichCell& src, int);
	RichCell();
};

class RichTable : DeepCopyOption<RichTable> {
public:
	struct Format {
		int   before, lm, rm, after;
		int   frame;
		Color framecolor;
		int   grid;
		Color gridcolor;
		WithDeepCopy< Vector<int> > column;
		int   header;
		bool  keep;

		Format();
	};

	struct CellInfo {
		bool  valid;
		Point master;

		operator bool() const { return valid; }

		CellInfo()            { valid = true; }
	};

	Format format;
	Array< Array<RichCell> > cell;

private:
	struct PaintCell : Moveable<PaintCell> {
		int   left;
		int   right;
		Rect  page;
		PageY hy;
		bool  top;
		bool  bottom;

		PaintCell()    { top = true; }
	};

	struct PaintRow : Moveable<PaintRow> {
		PageY             gpy;
		PageY             py, pyy;
		Buffer<PaintCell> cell;
		bool              first;

		PaintCell& operator[](int i)                { return cell[i]; }
		const PaintCell& operator[](int i) const    { return cell[i]; }
	};

	struct Layout {
		Buffer<Rect>      col;
		Buffer<PaintRow>  row;
		int               frame;
		int               grid;
		PageY             pyy;

		PaintRow& operator[](int i)                 { return row[i]; }
		const PaintRow& operator[](int i) const     { return row[i]; }
	};

	struct TabLayout : Layout {
		bool              hasheader;
		Layout            header;
		Rect              page;
		int               page0;
		Size              sz;
	};

	mutable TabLayout clayout;
	mutable Rect      cpage;
	mutable PageY     cpy;

	Buffer< Buffer<CellInfo> > ci;
	int              r_row, r_column;
	Rect             r_page;
	PageY            r_py, r_pyy;

	void             Invalidate();
	void             InvalidateRefresh(int i, int j);
	void             InvalidateRefresh(Point p)            { InvalidateRefresh(p.y, p.x); }

	bool             Reduce(RichContext& rc) const;
	Layout           Realize(RichContext rc, int ny) const;
	bool             RowPaint(PageDraw& pw, const RichStyles& st, const Layout& tab,
	                          int i, int ny, const Rect& pg, VectorMap<int, Rect>& frr,
	                          PaintInfo& pi, int pd, bool sel) const;

	const TabLayout& Realize(RichContext rc) const;

	mutable int  length, tabcount;

	static void  ExpandFrr(VectorMap<int, Rect>& frr, int pi, int l, int r, int t, int b);

	friend class RichTxt;
	friend class RichText;
public:

	Array<RichCell>&       operator[](int i)             { return cell[i]; }
	const Array<RichCell>& operator[](int i) const       { return cell[i]; }
	RichCell&       operator[](Point p)                  { return cell[p.y][p.x]; }
	const RichCell& operator[](Point p) const            { return cell[p.y][p.x]; }
	int             GetCellPos(int i, int j) const;
	int             GetCellPos(Point p) const            { return GetCellPos(p.y, p.x); }
	int             GetTableCount(int i, int j) const;
	int             GetTableCount(Point p) const         { return GetTableCount(p.y, p.x); }
	Point           GetMasterCell(int i, int j) const;
	Point           GetMasterCell(Point p) const         { return GetMasterCell(p.y, p.x); }
	const RichCell& GetMaster(int i, int j) const;

	int           GetLength() const;
	int           GetTableCount() const;

private:
	void          Normalize0();

	RichTable     Copy(const Rect& sel) const;
	void          Paste(Point pos, const RichTable& tab);
	void          RemoveRow0(int rowi);
	void          RemoveRow(int rowi);
	void          InsertRow(int rowi, const RichStyles& style);
	void          RemoveColumn0(int column);
	void          RemoveColumn(int column);
	void          InsertColumn(int column, const RichStyles& style);
	bool          IsRowEmpty(int row);
	bool          IsColumnEmpty(int column);
	void          SplitCell(Point cl, Size sz, const RichStyles& style);
	RichCell::Format GetCellFormat(const Rect& sel) const;
	void             SetCellFormat(const Rect& sel, const RichCell::Format& fmt, bool setkeep);

	PageY         GetHeight(RichContext rc) const;
	PageY         GetTop(RichContext rc) const;
	void          Paint(PageDraw& pw, RichContext rc, const PaintInfo& pi) const;
	RichCaret     GetCaret(int pos, RichContext rc) const;
	int           GetPos(int x, PageY y, RichContext rc) const;
	int           GetVertMove(int pos, int gx, RichContext rc, int dir) const;
	RichHotPos    GetHotPos(int x, PageY y, int tolerance, RichContext rc) const;
	void          AdjustSel(Rect& sel) const;

	void          GatherValPos(Vector<RichValPos>& f, RichContext rc, int pos, int type) const;
	void          ClearSpelling();

	Point         FindCell(int& pos) const;
	RichPos       GetRichPos(int pos, const RichStyles& st) const;

	CellInfo      GetCellInfo(int i, int j)              { return ci[i][j]; }

	int           GetInvalid(PageY& top, PageY& bottom, RichContext rc) const;
	void          Validate();

	void          ApplyZoom(Zoom z, const RichStyles& ostyle, const RichStyles& zstyle);

public:
	void                    AddColumn(int cx);
	void                    SetPick(int i, int j, pick_ RichTxt& text);
	RichTxt                 GetPick(int i, int j);
	const RichTxt&          Get(int i, int j) const                { return cell[i][j].text; }
	void                    SetQTF(int i, int j, const char *qtf);
	void                    SetFormat(int i, int j, const RichCell::Format& fmt);
	const RichCell::Format& GetFormat(int i, int j)                { return cell[i][j].format; }
	bool                    operator()(int i, int j) const         { return ci[i][j].valid; }
	void                    SetSpan(int i, int j, int vspan, int hspan);
	Size                    GetSpan(int i, int j) const;
	void                    SetFormat(const Format& fmt);
	const Format&           GetFormat() const                      { return format; }

	int                     GetColumns() const                     { return format.column.GetCount(); }
	int                     GetRows() const                        { return cell.GetCount(); }
	Size                    GetSize() const                        { return Size(GetColumns(), GetRows()); }

	void          Normalize();

	RichTable(const RichTable& src, int);
	RichTable();
};
