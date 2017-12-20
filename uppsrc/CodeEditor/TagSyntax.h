class TagSyntax : public EditorSyntax { // Tag based languages (XML, HTML)
public:
	virtual void            Clear();
	virtual void            ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size);
	virtual void            Serialize(Stream& s);
	virtual void            Highlight(const wchar *s, const wchar *end, HighlightOutput& hls,
	                                  CodeEditor *editor, int line, int64 pos);
	virtual void            CheckSyntaxRefresh(CodeEditor& e, int64 pos, const WString& text);
	virtual void            IndentInsert(CodeEditor& editor, int chr, int count);
	virtual bool            CheckBrackets(CodeEditor& e, int64& bpos0, int64& bpos);

private:

	enum { TEXT, TAG0, TAG, ENDTAG, ATTR, COMMENT, DECL, PI, SCRIPT };
	
	bool    html;
	bool    witz;
	int     status;
	int     hl_ink;
	int     hl_paper;
	String  tagname;

	CSyntax script;     // for <style> or <script>
	enum { CSS, JS };
	int     script_type; 

	HighlightOutput *hout;

	const wchar *Spaces(const wchar *s, const wchar *e);
	void DoScript(const wchar *s, const wchar *e, CodeEditor *editor, int line, int tabsize, int64 pos);
	void Do(const wchar *s, const wchar *e, CodeEditor *editor, int line, int tabsize, int64 pos);
	void Put0(int ink, int n = 1, int paper = PAPER_NORMAL);

	void Set(int ink, int paper = PAPER_NORMAL)                 { hl_ink = ink; hl_paper = paper; }
	void Put(int n = 1)                                         { Put0(hl_ink, n, hl_paper); }
	void SetPut(int ink, int n = 1, int paper = PAPER_NORMAL)   { Set(ink, paper); Put(n); }

public:
	TagSyntax&  Html(bool b)                                    { html = b; return *this; }
	TagSyntax&  Witz(bool b)                                    { html = b; return *this; }
	CSyntax *GetScript()                                        { return status == SCRIPT ? &script : NULL; }

	TagSyntax();
};
