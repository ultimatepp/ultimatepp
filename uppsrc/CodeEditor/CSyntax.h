class CSyntax : public EditorSyntax { // Curly braces languages (C++, Java, C#, Javascript...) common support
public:
	virtual void            Clear();
	virtual void            ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size);
	virtual void            Serialize(Stream& s);
	virtual void            IndentInsert(CodeEditor& editor, int chr, int count);
	virtual bool            CheckBrackets(CodeEditor& e, int64& bpos0, int64& bpos);
	virtual bool            CanAssist() const;
	virtual void            Highlight(const wchar *s, const wchar *end, HighlightOutput& hls,
	                                  CodeEditor *editor, int line, int64 pos);
	virtual void            CheckSyntaxRefresh(CodeEditor& e, int64 pos, const WString& text);
	virtual Vector<IfState> PickIfStack();
	virtual void            ReformatComment(CodeEditor& e);
	virtual bool            GetBlockHeader(Point& blk_start, Point& blk_end);

protected:
	bool        comment;       // we are in /* */ block comment
	bool        linecomment;   // we are in // line comment (because it can be continued by '\')
	bool        string;        // we are in string (becase it can be continued by '\')
	bool        linecont;      // line ended with '\'
	bool        was_namespace; // true if there was 'namespace', until '{' or ';' (not in ( [ brackets)
	WString     raw_string;    // we are in C++11 raw string literal, this is end delimiter, e.g. )"
	char        macro;         // can be one of:
	enum        {
		MACRO_OFF = 0,  // last line was not #define
	    MACRO_CONT, // last line was #define and ended with '\'
	    MACRO_END   // last line was a macro, but ended
	};

	int         cl, bl, pl; // levels of { [ (

	Vector<int>     brk; // { ( [ stack (contains '{', ')', ']')
	Vector<int>     blk; // { line stack
	Vector<int>     bid; // { indentation stack
	Vector<Isx>     par; // ( [ position stack
	Vector<IfState> ifstack;

	Point       blk_start = Null; // last zero block level { (so it is actually end of header)
	Point       blk_end = Null; // last zero block level } or ; or prepro line

	int         stmtline;     // line of latest "if", "else", "while", "do", "for" or -1
	int         endstmtline;  // line of latest ';' (not in ( [ brackets)
	int         seline;       // stmtline stored here on ';' (not in ( [ brackets)
	int         spar;         // ( [ level, reset on "if", "else", "while", "do", "for"
	
	int         highlight;    // subtype, 0 = C++

	static int  InitUpp(const char **q);
	static void InitKeywords();
	const wchar *DoComment(HighlightOutput& hls, const wchar *p, const wchar *e);

	static Vector<Index<String>> keyword;
	static Vector<int> breakers;
	static Vector<Index<String>> name;
	static Index<String> kw_upp;
	static int kw_macros, kw_logs, kw_sql_base, kw_sql_func;

	
	static Color BlockColor(int level);

	int     GetCommentPos(CodeEditor& e, int l, WString& ch) const;
	WString GetCommentHdr(CodeEditor& e, int l) const { WString h; GetCommentPos(e, l, h); return h; }
	void    IndentInsert0(CodeEditor& e, int chr, int count, bool reformat);

	void Bracket(int64 pos, HighlightOutput& hls, CodeEditor *editor);

	void  ClearBraces();

	void  Grounding(const wchar *ln, const wchar *e);

	bool CheckBracket(CodeEditor& e, int li, int64 pos, int64 ppos, int64 pos0, WString ln, int d, int limit, int64& bpos0, int64& bpos);
	bool CheckLeftBracket(CodeEditor& e, int64 pos, int64& bpos0, int64& bpos);
	bool CheckRightBracket(CodeEditor& e, int64 pos, int64& bpos0, int64& bpos);

	bool RawString(const wchar *p, int& n);
	
public:
	static int  LoadSyntax(const char *keywords[], const char *names[]);

	enum HighlightType {
		HIGHLIGHT_NONE = -1, HIGHLIGHT_CPP = 0, HIGHLIGHT_USC, HIGHLIGHT_JAVA, HIGHLIGHT_T,
		HIGHLIGHT_CALC, HIGHLIGHT_LAY, HIGHLIGHT_SCH, HIGHLIGHT_SQL, HIGHLIGHT_CS,
		HIGHLIGHT_JAVASCRIPT, HIGHLIGHT_CSS, HIGHLIGHT_JSON, HIGHLIGHT_PHP, HIGHLIGHT_DBG,
		HIGHLIGHT_GLSL, HIGHLIGHT_CUDA,
		HIGHLIGHT_COUNT
	};
	
	void    SetHighlight(int h)           { highlight = h; }

	CSyntax()                             { Clear(); }
};

const wchar *HighlightNumber(HighlightOutput& hls, const wchar *p, bool ts, bool octal, bool css);
const wchar *HighlightHexBin(HighlightOutput& hls, const wchar *p, int plen, bool thousands_separator);
