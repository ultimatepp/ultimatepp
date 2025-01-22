struct RichCaret;
struct RichPos;

struct RichPara {
	enum Code {
		NONE,

		INDEXENTRY  = 2,
		FIELD       = 3,
		FACE        = 4,
		HEIGHT      = 5,
		LANGUAGE    = 6,
		INK         = 7,
		PAPER       = 8,

		TAB         = 9, // placeholder

		LINK        = 10,
		SSCRIPT     = 11,

		BOLD0       = 12,
		BOLD1       = 13,
		BOLDS       = 14,
		ITALIC0     = 15,
		ITALIC1     = 16,
		ITALICS     = 17,
		UNDERLINE0  = 18,
		UNDERLINE1  = 19,
		UNDERLINES  = 20,
		CAPITALS0   = 21,
		CAPITALS1   = 22,
		CAPITALSS   = 23,
		STRIKEOUT0  = 24,
		STRIKEOUT1  = 25,
		STRIKEOUTS  = 26,
		DASHED0     = 27,
		DASHED1     = 28,
		DASHEDS     = 29,

		EXT         = 30,

		OBJECT      = 31
	};
	
	enum ExtCode {
		NONAA0      = 1,
		NONAA1      = 2,
		NONAAS      = 3,
	};

	enum BULLET_STYLE {
		BULLET_NONE,
		BULLET_ROUND,
		BULLET_ROUNDWHITE,
		BULLET_BOX,
		BULLET_BOXWHITE,
		BULLET_TEXT
	};

	enum NUMBER_STYLE {
		NUMBER_NONE,
		NUMBER_1,
		NUMBER_0,
		NUMBER_a,
		NUMBER_A,
		NUMBER_i,
		NUMBER_I,
	};

	enum LINESPACING {
		LSP10 = 0,
		LSP15 = -1,
		LSP20 = -2,
		LSP115 = -3,
	};
	
	enum TabSpecial {
		TAB_RIGHTPOS = 0x8000000
	};
	
	enum RulerStyle {
		RULER_SOLID,
		RULER_DOT,
		RULER_DASH,
	};

	struct CharFormat : public Font {
		int     language;
		Color   ink, paper;
		String  link;
		WString indexentry;
		dword   sscript:2; // 1 superscript, 2 subscript
		bool    capitals;
		bool    dashed;

#ifdef _DEBUG
		String  ToString() const;
#endif

		CharFormat();
	};

	struct Tab : Moveable<Tab> {
		int   pos;
		byte  align;
		byte  fillchar;

		Tab() { align = ALIGN_LEFT; pos = 0; fillchar = 0; }
	};

	struct NumberFormat {
		String      before_number, after_number;
		bool        reset_number;
		byte        number[8];

		bool        IsNumbered() const;
		int         GetNumberLevel() const;
	};

	struct Format : NumberFormat, CharFormat {
		Uuid        styleid;
		String      label;
		WithDeepCopy< Vector<Tab> > tab;
		int         align;
		int         ruler, before, lm, indent, rm, after;
		Color       rulerink;
		int         rulerstyle;
		int         tabsize;
		int         bullet;
		int         linespacing;
		bool        newpage, firstonpage, keep, keepnext, orphan, newhdrftr;
		String      header_qtf, footer_qtf;

		void        SortTabs();

#ifdef _DEBUG
		String      ToString() const;
#endif

		Format();
	};

	struct Number {
		int     n[8];

		String  AsText(const NumberFormat& format) const;
		void    Next(const NumberFormat& format);
		void    Reset()                               { memset(n, 0, sizeof(n)); }

		bool        operator!=(const Number& b) const { return !memeq_t(n, b.n, 8); }

		Number()                                      { Reset(); }
	};

	struct Part {
		RichObject                  object;
		WString                     text;
		CharFormat                  format;
		Id                          field;
		String                      fieldparam;
		WithDeepCopy< Array<Part> > fieldpart;

		bool        NonText() const    { return object || field; }
		bool        IsText() const     { return !NonText(); }
		int         GetLength() const  { return object || field ? 1 : text.GetLength(); }
	};

	struct FieldType {
		virtual Array<Part> Evaluate(const String& param, VectorMap<String, Value>& vars,
		                             const CharFormat& fmt) = 0;
		virtual void        Menu(Bar& bar, String *param) const {}
		virtual void        DefaultAction(String *param) const {}
		virtual ~FieldType() {}
	};

	struct HeightInfo {
		int                  ascent;
		int                  descent;
		int                  external;
		int                  ydelta;
		const RichObject    *object;

		int                  Sum() const { return ascent + descent + external; }
	};

	struct Line : public HeightInfo {
		int  pos; // offset in Lines arrays (e.g. text or pos)
		int  len; // number of characters in lines array
		int  ppos; // position of the first character
		int  plen; // length in source characters (e.g. fields are single)
		int  xpos;
		int  cx;
		int  objecti;
		bool withtabs;
	};

	struct Lines {
		int                        len;
		int                        clen;
		int                        cx;
		Buffer<wchar>              text;
		Buffer<int>                pos;
		Buffer<int>                width;
		Buffer<HeightInfo>         height;
		Buffer<const CharFormat *> format;
		Array<Line>                line;
		Array<CharFormat>          hformat;
		Array<RichObject>          object;
		int                        first_indent;
		int                        next_indent;
		int                        number_chars; // if paragraph starts with numbering, otherwise 0


		int                  GetCount() const        { return line.GetCount(); }
		Line&                operator[](int i)       { return line[i]; }
		const Line&          operator[](int i) const { return line[i]; }
		void                 Justify(const Format& format);
		int                  BodyHeight() const;

		rval_default(Lines);
	
		Lines();
		~Lines();

	private:
		static Array<Lines>&       Cache();
		bool                       incache;
		bool                       justified;
		int64                      cacheid;
		
		friend struct RichPara;
	};

	static const VectorMap<Id, FieldType *>& fieldtype() { return fieldtype0(); }
	static void  Register(Id id, FieldType& ft);
	template <class T> static void Register(Id id)       { static T x; Register(id, x); }

	int64       cacheid;
	bool        incache;
	Array<Part> part;
	Format      format;

	static void Charformat(Stream& out, const CharFormat& o, const CharFormat& n,
	                       const CharFormat& s);

	static void DrawRuler(Draw& w, int x, int y, int cx, int cy, Color ink, int style);

	void        Cat(const WString& s, const CharFormat& f);
	void        Cat(const char *s, const CharFormat& f);
	void        Cat(const RichObject& o, const CharFormat& f);
	void        Cat(Id fieldtype, const String& param, const CharFormat& f);

	int         GetLength() const;
	int         GetCount() const                          { return part.GetCount(); }
	bool        IsEmpty() const;
	String      Pack(const Format& style, Array<RichObject>& obj) const;
	void        Unpack(const String& s, const Array<RichObject>& obj, const Format& style);
	
	void        ApplyStyle(const Format& newstyle);

	Part&       operator[](int i)                         { return part[i]; }
	const Part& operator[](int i) const                   { return part[i]; }
	int         FindPart(int& pos) const;
	void        Trim(int pos);
	void        Mid(int pos);
	void        Append(const RichPara& p)                 { part.Append(p.part); }

	void        GetRichPos(RichPos& rp, int pos) const;

	Lines       FormatLines(int cx, const Number& n) const;
	void        Paint(PageDraw& pw, RichContext rc, const PaintInfo& pi, const Number& n, const Bits& spellerror, bool baselevel) const;
	RichCaret   GetCaret(int pos, RichContext rc) const;
	int         GetPos(int x, PageY y, RichContext rc) const;
	void        GatherLabels(Vector<RichValPos>& info, RichContext rc, int pos) const;
	void        GatherIndexes(Vector<RichValPos>& info, RichContext rc, int pos) const;
	int         GetVertMove(int pos, int gx, const Rect& page, int dir, const RichContext& rc) const;

	WString     GetText() const;

	bool        HasPos() const;

	bool        EvaluateFields(VectorMap<String, Value>& vars);

	void        operator<<=(const RichPara& p)            { format = p.format; part <<= p.part; }

#ifdef _DEBUG
	void        Dump();
#endif

	void        ApplyZoom(Zoom z);
	
	void        CacheId(int64 id);

	rval_default(RichPara);

	RichPara();
	~RichPara();

private:
	Tab         GetNextTab(int pos, int cx) const;
	void        Smh(Lines& lines, HeightInfo *th, int cx) const;
	Lines       Begin(RichContext& rc) const;
	bool        BreaksPage(const RichContext& rc, const Lines& pl, int i) const;
	void        PackParts(Stream& out, const CharFormat& chrstyle,
	                      const Array<Part>& part, CharFormat& cf,
	                      Array<RichObject>& obj) const;
	void        UnpackParts(Stream& in, const CharFormat& chrstyle,
	                        Array<Part>& part, const Array<RichObject>& obj, int& oi);
	static void Flush(Draw& draw, const PaintInfo& pi, wchar *text, const CharFormat **i0,
                      int *wd, int pos, int len, int x0, int x, int y0, int y, int linecy,
                      int lineascent, Zoom z, bool highlight);
	int         PosInLine(int x, const Rect& page, const Lines& pl, int lni) const;

	static StaticMutex      cache_lock;
	static Array<RichPara>& Cache();

	static VectorMap<Id, FieldType *>& fieldtype0();

	struct StorePart;
};

inline bool operator==(const RichPara::Tab& a, const RichPara::Tab& b) {
	return a.pos == b.pos && a.align == b.align && a.fillchar == b.fillchar;
}

bool NumberingDiffers(const RichPara::Format& fmt1, const RichPara::Format& fmt2);

void Sort(Vector<RichPara::Tab>& tab);

void operator*=(RichPara::Format& fmt, Zoom z);
