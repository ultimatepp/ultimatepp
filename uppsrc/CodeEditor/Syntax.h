#ifndef _CodeEditor_Highlight_h_
#define _CodeEditor_Highlight_h_

#define CTIMING(x) // RTIMING(x)

struct HlStyle {
	Color color;
	bool  bold;
	bool  italic;
	bool  underline;
};

struct Isx : Moveable<Isx> { // '(', '[' position
	int    line;
	int    pos;
	
	void Serialize(Stream& s)    { s % line % pos; }

	friend bool operator==(Isx a, Isx b) { return a.line == b.line && a.pos == b.pos; }
	friend bool operator!=(Isx a, Isx b) { return !(a == b); }
};

struct IfState : Moveable<IfState> {
	enum        { IF = '0', ELIF, ELSE, ELSE_ERROR, ENDIF_ERROR };

	WString iftext;
	short   ifline;
	char    state;

	void Serialize(Stream& s)         { s % iftext % ifline % state; }

	bool operator==(const IfState& b) const {
		return iftext == b.iftext && state == b.state && ifline == b.ifline;
	}

	IfState()                         { ifline = state = 0; }
};

struct HighlightSetup { // Global highlighting settings
public:
#define HL_COLOR(x, a, b)      x,
	enum {
#include "hl_color.i"
		HL_COUNT
	};
#undef HL_COLOR

	static HlStyle hl_style[HL_COUNT];
	static byte    hilite_scope;
	static byte    hilite_ifdef;
	static byte    hilite_bracket;
	static bool    thousands_separator;
	static bool    indent_spaces;
	static int     indent_amount;
	static bool    no_parenthesis_indent;

	static const HlStyle& GetHlStyle(int i);
	static void           SetHlStyle(int i, Color c, bool bold = false, bool italic = false, bool underline = false);
	static void           LoadHlStyles(const char *s);
	static String         StoreHlStyles();
	static void           HostColors();
	static void           DarkTheme(bool host_colors = true);
	static void           WhiteTheme(bool host_colors = true);
	static void           DefaultHlStyles();
	static void           InitOnce();

	static const char    *GetHlName(int i);
	static bool           HasHlFont(int i);
};

struct HighlightOutput : HighlightSetup {
	Vector<LineEdit::Highlight>& v;
	LineEdit::Highlight          def;
	int                          pos;

public:
	void SetChar(int pos, int chr)                    { v[pos].chr = chr; }
	void Set(int pos, int count, const HlStyle& ink);
	void SetFlags(int pos, int count, word flags);
	void SetFont(int pos, int count, const HlStyle& f);
	void SetPaper(int pos, int count, Color paper);
	void SetInk(int pos, int count, Color ink);
	void SetFlags(int count, word flags)              { SetFlags(pos, count, flags); }
	void Put(int count, const HlStyle& ink)           { Set(pos, count, ink); pos += count; }
	void Put(int count, const HlStyle& ink, Color paper);
	void Put(int count, const HlStyle& ink, const HlStyle& paper);
	void Put(const HlStyle& ink)                      { Put(1, ink); }
	void Put(const HlStyle& ink, word flags)          { Put(1, ink); v[pos - 1].flags = flags; }
	void Flags(word flags)                            { v[pos - 1].flags = flags; }
	int  GetCount() const                             { return v.GetCount(); }

	const wchar *CString(const wchar *p);
	
	HighlightOutput(Vector<LineEdit::Highlight>& v);
	~HighlightOutput();
};

class EditorSyntax : public HighlightSetup { // Inheriting to make static members available
	struct SyntaxDef {
		Event<One<EditorSyntax>&> factory;
		String                    patterns;
		String                    description;
	};
	
	static ArrayMap<String, SyntaxDef>& defs();

protected:
	bool                    ignore_errors;
	int                     comments_lang;
	
public:
	virtual void            Clear();
	virtual void            ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size);
	virtual void            Serialize(Stream& s);
	virtual void            IndentInsert(CodeEditor& editor, int chr, int count);
	virtual bool            CheckBrackets(CodeEditor& e, int64& bpos0, int64& bpos); // TODO: Replace with generic mechanism
	virtual void            CheckSyntaxRefresh(CodeEditor& e, int64 pos, const WString& text);
	virtual bool            CanAssist() const;
	virtual void            Highlight(const wchar *s, const wchar *end, HighlightOutput& hls,
	                                  CodeEditor *editor, int line, int64 pos);
	virtual Vector<IfState> PickIfStack();
	virtual void            ReformatComment(CodeEditor& e);
	virtual bool            GetBlockHeader(Point& blk_start, Point& blk_end);
	virtual int             GetBlockLevel();
	virtual ~EditorSyntax();

	static Color IfColor(char ifstate);

	void    Set(const String& s)           { CTIMING("Set"); if(s.GetCount() == 0) Clear(); else LoadFromString(*this, s); }
	String  Get()                          { CTIMING("Get"); return StoreAsString(*this); }
	
	void    IgnoreErrors()                 { ignore_errors = true; }
	void    SpellCheckComments(int lang)   { comments_lang = lang; }

	EditorSyntax()                         { Clear(); ignore_errors = false; }

	static void Register(const char *id, Event<One<EditorSyntax>&> factory,
	                     const char *exts, const char *description);
	static One<EditorSyntax> Create(const char *id);
	static String            GetSyntaxForFilename(const char *fn);
	static int               GetSyntaxCount()             { return defs().GetCount(); }
	static String            GetSyntax(int i)             { return defs().GetKey(i); }
	static String            GetSyntaxDescription(int i);
	static String            GetSyntaxPatterns(int i)     { return defs()[i].patterns; }
};

#endif
