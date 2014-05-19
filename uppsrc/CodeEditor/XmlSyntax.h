class XmlSyntax : public EditorSyntax { // Tag based languages (XML, HTML)
public:
	virtual void            Clear();
	virtual void            ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size);
	virtual void            Serialize(Stream& s);
	virtual void            Highlight(const wchar *s, const wchar *end, HighlightOutput& hls,
	                                  CodeEditor *editor, int line, int pos);

private:

	enum { TEXT, TAG0, TAG, ATTR, COMMENT, DECL, PI, SCRIPT };
	
	int     status;
	int     hl_ink;
	int     hl_paper;
	String  tagname;
	
	CSyntax script;

	HighlightOutput *hout;

	const wchar *Spaces(const wchar *s, const wchar *e);
	void DoScript(const wchar *s, const wchar *e);
	void Do(const wchar *ln, const wchar *e);
	void Put0(int ink, int n = 1, int paper = PAPER_NORMAL);

	void Set(int ink, int paper = PAPER_NORMAL)                 { hl_ink = ink; hl_paper = paper; }
	void Put(int n = 1)                                         { Put0(hl_ink, n, hl_paper); }
	void SetPut(int ink, int n = 1, int paper = PAPER_NORMAL)   { Set(ink, paper); Put(n); }

public:
	XmlSyntax()                             { Clear(); hout = NULL; }
};
