#include "CodeEditor.h"

NAMESPACE_UPP

#define LTIMING(x)  // RTIMING(x)

bool cmps(const wchar *q, const char *s, int& n) {
	const wchar *t = q;
	while(*q)
		if(*q++ != *s++)
			return false;
	n += int(q - t);
	return *q == *s;
}

bool IsUpperString(const char *q)
{
	while(*q)
	{
		if(*q != '_' && (*q < '0' || *q > '9') && !IsUpper(*q))
			return false;
		q++;
    }
	return true;
}

Color CSyntax::BlockColor(int level)
{
	if(hilite_scope == 1)
		return  GetHlStyle(level & 1 ? PAPER_BLOCK1 : PAPER_NORMAL).color;
	if(hilite_scope == 2) {
		int q = level % 5;
		return  GetHlStyle(q ? PAPER_BLOCK1 + q - 1 : PAPER_NORMAL).color;
	}
	return GetHlStyle(PAPER_NORMAL).color;
}

void CSyntax::Bracket(int pos, HighlightOutput& hls, CodeEditor& editor) // TODO:SYNTAX: Cleanup passing bracket info
{
	if(editor.IsCursorBracket(pos)) {
		HlStyle& h = hl_style[PAPER_BRACKET0];
		hls.SetPaper(hls.pos, 1, h.color);
		hls.SetFont(hls.pos, 1, h);
	}
	if(editor.IsMatchingBracket(pos)) {
		HlStyle& h = hl_style[PAPER_BRACKET];
		hls.SetPaper(hls.pos, 1, h.color);
		hls.SetFont(hls.pos, 1, h);
	}
}

void CSyntax::Highlight(CodeEditor& editor, int line, Vector<LineEdit::Highlight>& hl, int pos)
{
	ONCELOCK {
		InitKeywords();
	}
	
	LTIMING("HighlightLine");
	if(highlight < 0 || highlight >= keyword.GetCount())
		return;
	WString text = editor.GetWLine(line);
	One<EditorSyntax> ess = editor.GetSyntax(line);
	CSyntax *ss = dynamic_cast<CSyntax *>(~ess);
	
	CSyntax next;
	next.Set(ss->Get());
	next.ScanSyntax(~text, text.End(), line + 1, editor.GetTabSize());
	bool macro = next.macro != MACRO_OFF;
	
	ss->Grounding(text.Begin(), text.End());
	HighlightOutput hls(hl);
	const wchar *p = text;
	const wchar *e = text.End();
	if(highlight == HIGHLIGHT_CALC) {
		if(line == editor.GetLineCount() - 1 || *p == '$')
			hls.SetPaper(0, text.GetLength() + 1, hl_style[PAPER_BLOCK1].color);
	}
	else
		hls.SetPaper(0, text.GetLength() + 1,
		             macro ? hl_style[PAPER_MACRO].color : hl_style[PAPER_NORMAL].color);
	int block_level = ss->bid.GetCount() - 1;
	String cppid;
	if(!ss->comment && highlight != HIGHLIGHT_CALC) {
		if(!macro) {
			int i = 0, bid = 0, pos = 0, tabsz = editor.GetTabSize();
			while(bid < ss->bid.GetCount() - 1
			&& (i >= text.GetLength() || text[i] == ' ' || text[i] == '\t')) {
				hls.SetPaper(i, 1, BlockColor(bid));
				if(i < text.GetLength() && text[i] == '\t' || ++pos >= tabsz) {
					pos = 0;
					bid++;
				}
				i++;
			}
			hls.SetPaper(i, 1 + max(0, text.GetLength() - i), BlockColor(ss->bid.GetCount() - 1));
		}
		while(*p == ' ' || *p == '\t') {
			p++;
			hls.Put(hl_style[INK_NORMAL]);
		}
		if(*p == '#' && findarg(highlight, HIGHLIGHT_JAVASCRIPT, HIGHLIGHT_CSS, HIGHLIGHT_JSON) < 0) {
			static const char *pd[] = {
				"define", "error", "if", "elif", "else", "endif",
				"ifdef", "ifndef", "include", "line", "undef", "pragma",
				// CLR
				"using"
			};
			static Index<String> macro;
			if(macro.GetCount() == 0)
				for(int i = 0; i < __countof(pd); i++)
					macro.Add(pd[i]);
			const wchar *q = p + 1;
			while(*q == ' ' || *q == '\t')
				q++;
			StringBuffer id;
			while(IsAlpha(*q))
				id.Cat(*q++);
			cppid = id;
			hls.Put(macro.Find(cppid) < 0 ? 1 : int(q - p), hl_style[INK_MACRO]);
			p = q;
		}
	}
	int lindent = int(p - ~text);
	int lbrace = -1;
	int lbclose = -1;
	Color lbcolor = Null;
	bool is_comment = false;
	while(p < e) {
		int pair = MAKELONG(p[0], p[1]);
		if(ss->linecomment && ss->linecont || pair == MAKELONG('/', '/') && highlight != HIGHLIGHT_CSS && highlight != HIGHLIGHT_JSON) {
			hls.Put(text.GetLength() + 1 - hls.pos, hl_style[INK_COMMENT]);
			is_comment = true;
			break;
		}
		else
		if(ss->comment && highlight != HIGHLIGHT_JSON)
			if(pair == MAKELONG('*', '/')) {
				hls.Put(2, hl_style[INK_COMMENT]);
				p += 2;
				ss->comment = false;
			}
			else {
				hls.Put(hl_style[INK_COMMENT]);
				p++;
			}
		else
		if((*p == '\"' || *p == '\'') || ss->linecont && ss->string)
			p = hls.CString(p);
		else
		if(pair == MAKELONG('/', '*') && highlight != HIGHLIGHT_JSON) {
			hls.Put(2, hl_style[INK_COMMENT]);
			p += 2;
			ss->comment = true;
			is_comment = true;
		}
		else
		if(*p == '(') {
			ss->brk.Add(')');
			Bracket(int(p - text) + pos, hls, editor);
			hls.Put(hl_style[INK_PAR0 + max(ss->pl++, 0) % 4]);
			p++;
		}
		else
		if(*p == '{') {
			ss->brk.Add(ss->was_namespace ? 0 : '}');
			Bracket(int(p - text) + pos, hls, editor);
			hls.Put(hl_style[INK_PAR0 + max(ss->cl, 0) % 4]);
			if(ss->was_namespace)
				ss->was_namespace = false;
			else {
				++block_level;
				++ss->cl;
			}
			if(hls.pos < text.GetCount())
				hls.SetPaper(hls.pos, text.GetCount() - hls.pos + 1, BlockColor(block_level));
			p++;
		}
		else
		if(*p == '[') {
			ss->brk.Add(']');
			Bracket(int(p - text) + pos, hls, editor);
			hls.Put(hl_style[INK_PAR0 + max(ss->bl++, 0) % 4]);
			p++;
		}
		else
		if(*p == ')' || *p == '}' || *p == ']') {
			int bl = ss->brk.GetCount();
			int bc = bl ? ss->brk.Pop() : 0;
			if(*p == '}' && hilite_scope && block_level > 0 && bc)
				hls.SetPaper(hls.pos, text.GetLength() + 1 - hls.pos, BlockColor(--block_level));
			Bracket(int(p - text) + pos, hls, editor);
			int& l = *p == ')' ? ss->pl : *p == '}' ? ss->cl : ss->bl;
			if(bc && (bc != *p || l <= 0) || bc == 0 && *p != '}') {
				hls.Put(p == ~text ? hl_style[INK_PAR0] : hl_style[INK_ERROR]);
				ss->brk.Clear();
				ss->cl = ss->bl = ss->pl = 0;
			}
			else {
				if(bc) --l;
				hls.Put(1, hl_style[INK_PAR0 + l % 4]);
			}
			p++;
		}
		else
		if(highlight == HIGHLIGHT_CSS ? *p == '#' : pair == MAKELONG('0', 'x') || pair == MAKELONG('0', 'X')) {
			hls.Put(2, hl_style[INK_CONST_HEX]);
			p += 2;
			const wchar *t = p;
			while(IsXDigit(*p))
				p++;
			hls.Put(int(p - t), hl_style[INK_CONST_HEX]);
		}
		else
		if(IsDigit(*p)) {
			const wchar *t = p;
			int c = INK_CONST_INT;
			bool isDot, isFloat = false;
			if(*p == '0') c = INK_CONST_OCT;
			while(IsDigit(*p)) p++;
			int fixdigits = p - t;
			if(*p == '.' || (*p == 'e' || *p == 'E') && highlight != HIGHLIGHT_CSS) {
				if(*p == '.')
					isDot = true;
				c = INK_CONST_FLOAT;
				p++;
				if(*p == '-')
					p++;
			}
			while((IsDigit(*p) || (isDot && *p == 'f')) && (isFloat == false)) {
				if(*p == 'f')
					isFloat = true;
				p++;
			}
			if(c == INK_CONST_OCT && p - t == 1)
				c = INK_CONST_INT;
			int n = int(p - t);
			for(int i = 0; i < n; i++)
				hls.Put(hl_style[c],
				        c == INK_CONST_OCT || (fixdigits < 4 && n - fixdigits < 5) || i == fixdigits || !thousands_separator ? 0 :
				        i < fixdigits ? decode((fixdigits - i) % 3, 1, LineEdit::SHIFT_L, 0, LineEdit::SHIFT_R, 0) :
				                        decode((i - fixdigits) % 3, 1, LineEdit::SHIFT_R, 0, LineEdit::SHIFT_L, 0));

//				hls.Put(q > 0 && ((q / 3) & 1) == 1 && c != INK_CONST_OCT ?
//				        hl_style[c == INK_CONST_INT ? INK_CONST_INT_3 : INK_CONST_FLOAT_3]
//				        : hl_style[c]);
		}
		else
		if(pair == MAKELONG('t', '_') && p[2] == '(' && p[3] == '\"') {
			int pos0 = hls.pos;
			hls.Put(3, hl_style[INK_UPP]);
			p = hls.CString(p + 3);
			if(*p == ')') {
				hls.Put(hl_style[INK_UPP]);
				p++;
			}
			hls.SetPaper(pos0, hls.pos - pos0, hl_style[PAPER_LNG].color);
		}
		else
		if(pair == MAKELONG('t', 't') && p[3] == '(' && p[4] == '\"') {
			int pos0 = hls.pos;
			hls.Put(4, hl_style[INK_UPP]);
			p = hls.CString(p + 4);
			if(*p == ')') {
				hls.Put(hl_style[INK_UPP]);
				p++;
			}
			hls.SetPaper(pos0, hls.pos - pos0, hl_style[PAPER_LNG].color);
		}
		else
		if(iscib(*p)) {
			const wchar *q = p;
			StringBuffer id;
			while((iscidl(*q) || highlight == HIGHLIGHT_CSS && *q == '-') && q < e)
				id.Cat(*q++);
			String iid = id;
			if(highlight == HIGHLIGHT_SQL)
				iid = ToUpper(iid);
			int uq = kw_upp.Find(iid);
			int nq;
			hls.Put(int(q - p), (nq = keyword[highlight].Find(iid)) >= 0 ? hl_style[INK_KEYWORD] :
			                    name[highlight].Find(iid) >= 0 ? hl_style[INK_UPP] :
			                    uq >= 0 ? uq < kw_macros ? hl_style[INK_UPPMACROS] :
			                              uq < kw_logs ? hl_style[INK_UPPLOGS] :
			                              uq < kw_sql_base ? hl_style[INK_SQLBASE] : 
			                              uq < kw_sql_func ? hl_style[INK_SQLFUNC] :
			                              hl_style[INK_SQLBOOL] :
			                    IsUpperString(iid) && !macro ? hl_style[INK_UPPER] :
			                    hl_style[INK_NORMAL]);
			p = q;
			if(nq == 0)
				ss->was_namespace = true;
		}
		else {
			if(*p == ';')
				ss->was_namespace = false;
			hls.Put(strchr("!+-*^/%~&|=[]:?<>.", *p) ? hl_style[INK_OPERATOR] : hl_style[INK_NORMAL]);
			p++;
		}
	}
	if(hilite_ifdef && !IsNull(cppid) && !is_comment) {
		if((cppid == "else" || cppid == "elif" || cppid == "endif") && !ss->ifstack.IsEmpty()) {
			WStringBuffer ifln;
			ifln.Cat(" // ");
			ifln.Cat(ss->ifstack.Top().iftext);
			if(ss->ifstack.Top().ifline && hilite_ifdef == 2) {
				ifln.Cat(", line ");
				ifln.Cat(FormatInt(ss->ifstack.Top().ifline));
			}
			ifln.Cat('\t');
			int start = text.GetLength();
			WString ifs(ifln);
			hls.Set(start, ifs.GetLength(), hl_style[INK_IFDEF]);
			for(int i = 0; i < ifs.GetCount(); i++)
				hl[start + i].chr = ifs[i];
		}
	}
	if(hilite_scope) {
		if(lbrace >= 0 && lbclose < 0 && lbrace > lindent)
			hls.SetPaper(lindent, lbrace - lindent, lbcolor);
		if(lbrace < 0 && lbclose >= 0)
			hls.SetPaper(lbclose, text.GetLength() + 1 - lbclose, lbcolor);
	}
	if(!IsNull(cppid) && (cppid == "else" || cppid == "elif" || cppid == "endif" || cppid == "if"
	                      || cppid == "ifdef" || cppid == "ifndef"))
	   hls.SetPaper(0, hls.v.GetCount(), hl_style[PAPER_IFDEF].color);
}

END_UPP_NAMESPACE
