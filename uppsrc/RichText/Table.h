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
		bool     round;

		Format();
	};

	Format   format;
	int      vspan, hspan;
	RichTxt  text;

private:
	bool     Reduce(RichContext& rc) const;
	PageY    Align(const RichContext& rc, PageY npy) const;
	void     DrawCell(Draw& w, int l, int r, int y, int yy, const Rect& border,
	                  const Rect& margin, const PaintInfo& pi) const;

	void        ClearText();
	void        ClearText(const RichPara::Format& format, const RichStyles& style);
	void        Clear();

	PageY       GetTop(RichContext rc) const;
	PageY       GetHeight(RichContext rc) const;
	void        Paint(PageDraw& pw, RichContext rc, PageY npy,
	                  const Rect& xpg, const Rect& nxpg,
	                  int y, int ny, const PaintInfo& pi,
	                  bool select) const;
	RichCaret   GetCaret(int pos, RichContext rc, PageY pyy) const;
	int         GetPos(int x, PageY y, RichContext rc, PageY pyy) const;
	RichHotPos  GetHotPos(int x, PageY y, int tolerance, RichContext rc, PageY pyy) const;

	void        GatherValPos(Vector<RichValPos>& f, RichContext rc, PageY pyy, int pos, int type) const;

	friend class RichTable;
	friend class RichText;

public:
	rval_default(RichCell);

	RichCell(const RichCell& src, int);
	RichCell();
};

class RichTable : DeepCopyOption<RichTable> {
public:
	struct Format {
		int    before, lm, rm, after;
		int    frame;
		Color  framecolor;
		int    grid;
		Color  gridcolor;
		WithDeepCopy< Vector<int> > column;
		int    header;
		bool   keep, newpage, newhdrftr;
		String header_qtf, footer_qtf;

		Format();
	};

	struct CellInfo {
		bool  valid;
		Point master;

		operator bool() const { return valid; }

		CellInfo()            { valid = true; }
	};

private:
	Format format;
	Array< Array<RichCell> > cell;
	mutable One<RichText> header, footer;

	struct PaintCell : Moveable<PaintCell> {
		int   left; // left pos with grid
		int   right; // right pos with grid
		int   page_left; // left pos without grid (if any)
		int   page_right; // right pos without grid (if any)
		PageY hy; // end of cell
		bool  top; // this is top cell of vspan (or single cell without vspan)
		bool  bottom; // this is bottom cell of vspan (or single cell without vspan)
		
		RichContext MakeRichContext(RichContext rc) const;

		PaintCell()    { top = true; }
	};

	struct PaintRow : Moveable<PaintRow> {
		PageY             gpy; // position of grid line (if not first)
		PageY             py, pyy; //start, end of line
		Buffer<PaintCell> cell;
		bool              first; // first row on the page

		PaintCell& operator[](int i)                { return cell[i]; }
		const PaintCell& operator[](int i) const    { return cell[i]; }
	};

	struct Layout {
		Buffer<PaintRow>  row;
		int               frame;
		int               grid;
		PageY             pyy;

		PaintRow& operator[](int i)                 { return row[i]; }
		const PaintRow& operator[](int i) const     { return row[i]; }
		
		rval_default(Layout); // Old CLANG requires this
		Layout() {}
	};

	struct TabLayout : Layout {
		bool              hasheader = false;
		Layout            header;
		Size              sz;
		PageY             py;
		Rect              first_page;
		Rect              next_page;
		Rect              header_page;
		
		rval_default(TabLayout); // Old CLANG requires this
		TabLayout() {}
	};

	mutable TabLayout clayout;

	Buffer< Buffer<CellInfo> > ci;
	int              r_row, r_column; // r_ - refresh info
	Rect             r_first_page, r_next_page, r_header_page;
	PageY            r_py, r_pyy;

	void             Invalidate();
	void             InvalidateRefresh(int i, int j);
	void             InvalidateRefresh(Point p)            { InvalidateRefresh(p.y, p.x); }

	bool             Reduce(Rect& r) const;
	bool             Reduce(RichContext rc, Rect& first_page, Rect& next_page) const;
	Rect             GetPageRect(PageY py) const;
	void             NextPage(PageY& py, bool header = false) const;
	RichContext      MakeRichContext(RichContext rc, PageY py, bool header = false) const;
	Layout           Realize(PageY py, RichContext arc, int ny, bool header) const;
	bool             RowPaint(PageDraw& pw, RichContext rc, const Layout& tab, bool header,
                              int i, int ny, Rect pg, Rect npg, VectorMap<int, Rect>& frr,
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

	void          RemoveRow(int rowi);
	void          InsertRow(int rowi, const RichStyles& style);

private:
	void          Normalize0();

	RichTable     Copy(const Rect& sel) const;
	void          Paste(Point pos, const RichTable& tab);
	void          RemoveRow0(int rowi);
	void          RemoveColumn0(int column);
	void          RemoveColumn(int column);
	void          InsertColumn(int column, const RichStyles& style);
	bool          IsRowEmpty(int row);
	bool          IsColumnEmpty(int column);
	void          SplitCell(Point cl, Size sz, const RichStyles& style);
	RichCell::Format GetCellFormat(const Rect& sel) const;
	void             SetCellFormat(const Rect& sel, const RichCell::Format& fmt,
	                               bool setkeep, bool setround);

	PageY         GetHeight(RichContext rc) const;
	PageY         GetTop(RichContext rc) const;
	void          Paint(PageDraw& pw, RichContext rc, const PaintInfo& pi, bool baselevel) const;
	RichCaret     GetCaret(int pos, RichContext rc) const;
	int           GetPos(int x, PageY y, RichContext rc) const;
	int           GetVertMove(int pos, int gx, RichContext rc, int dir) const;
	RichHotPos    GetHotPos(int x, PageY y, int tolerance, RichContext rc) const;
	void          AdjustSel(Rect& sel) const;
	
	int           GetWidth(const RichStyles& st) const;

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
	void                    SetPick(int i, int j, RichTxt&& text);
	RichTxt                 GetPick(int i, int j);
	const RichTxt&          Get(int i, int j) const                { return cell[i][j].text; }
	void                    SetQTF(int i, int j, const char *qtf);
	void                    SetFormat(int i, int j, const RichCell::Format& fmt);
	const RichCell::Format& GetFormat(int i, int j) const          { return cell[i][j].format; }
	bool                    operator()(int i, int j) const         { return ci[i][j].valid; }
	void                    SetSpan(int i, int j, int vspan, int hspan);
	Size                    GetSpan(int i, int j) const;
	void                    SetFormat(const Format& fmt);
	const Format&           GetFormat() const                      { return format; }

	int                     GetColumns() const                     { return format.column.GetCount(); }
	int                     GetRows() const                        { return cell.GetCount(); }
	Size                    GetSize() const                        { return Size(GetColumns(), GetRows()); }

	void          Normalize();

	rval_default(RichTable);

	RichTable(const RichTable& src, int);
	RichTable();
};
