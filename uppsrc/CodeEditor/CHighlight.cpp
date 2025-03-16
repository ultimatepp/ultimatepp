#include "CodeEditor.h"

namespace Upp {

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
		return GetHlStyle(level & 1 ? PAPER_BLOCK1 : PAPER_NORMAL).color;
	if(hilite_scope == 2) {
		int q = level % 5;
		return GetHlStyle(q ? PAPER_BLOCK1 + q - 1 : PAPER_NORMAL).color;
	}
	return GetHlStyle(PAPER_NORMAL).color;
}

void CSyntax::Bracket(int64 pos, HighlightOutput& hls, CodeEditor *editor) // TODO:SYNTAX: Cleanup passing bracket info
{
	if(!editor)
		return;
	if(editor->IsCursorBracket(pos)) {
		HlStyle& h = hl_style[PAPER_BRACKET0];
		hls.SetPaper(hls.pos, 1, h.color);
		hls.SetFont(hls.pos, 1, h);
	}
	if(editor->IsMatchingBracket(pos)) {
		HlStyle& h = hl_style[PAPER_BRACKET];
		hls.SetPaper(hls.pos, 1, h.color);
		hls.SetFont(hls.pos, 1, h);
	}
}

const wchar *HighlightNumber(HighlightOutput& hls, const wchar *p, bool ts, bool octal, bool css)
{
	int c = octal ? HighlightSetup::INK_CONST_OCT : HighlightSetup::INK_CONST_INT;
	auto SkipDigits = [&] {
		while(IsDigit(*p) || *p == '\'')
			if(*p++ == '\'')
				ts = false;
	};
	const wchar *t = p;
	SkipDigits();
	int fixdigits = int(p - t);
	bool u = false;
	if(*p == '.') {
		c = HighlightSetup::INK_CONST_FLOAT;
		p++;
	}
	SkipDigits();
	if(*p == 'e' || *p == 'E') {
		c = HighlightSetup::INK_CONST_FLOAT;
		p++;
		p += *p == '-' || *p == '+';
		while(IsDigit(*p)) p++;
		if(*p == 'f' || *p == 'F')
			p++;
	}
	else
	if(*p == 'u' && c != HighlightSetup::INK_CONST_FLOAT) {
		u = true;
		p++;
	}
	if(c == HighlightSetup::INK_CONST_OCT && p - t == (u ? 2 : 1))
		c = HighlightSetup::INK_CONST_INT;
	int n = int(p - t);
	for(int i = 0; i < n; i++) {
		if(t[i] == 'e')
			ts = false;
		hls.Put(HighlightSetup::hl_style[c],
		        c == HighlightSetup::INK_CONST_OCT || (fixdigits < 5 && n - fixdigits < 5)
		             || i == fixdigits || !ts ? 0 :
		        i < fixdigits ? decode((fixdigits - i) % 3, 1, LineEdit::SHIFT_L, 0, LineEdit::SHIFT_R, 0) :
		                        decode((i - fixdigits) % 3, 1, LineEdit::SHIFT_R, 0, LineEdit::SHIFT_L, 0));
	}
	return p;
}

const wchar *HighlightHexBin(HighlightOutput& hls, const wchar *p, int plen, bool thousands_separator)
{
	hls.Put(plen, HighlightSetup::hl_style[HighlightSetup::INK_CONST_HEX]);
	p += plen;
	const wchar *t = p;
	while(IsXDigit(*p))
		p++;
	int n = int(p - t);
	for(int i = 0; i < n; i++) {
		hls.Put(HighlightSetup::hl_style[HighlightSetup::INK_CONST_HEX],
		        thousands_separator && ((n - i) & 1) ? LineEdit::SHIFT_L : 0);
	}
	return p;
}

const wchar *CSyntax::DoComment(HighlightOutput& hls, const wchar *p, const wchar *e)
{
	WString w;
	for(const wchar *s = p; s < e && IsLetter(*s); s++)
		w.Cat(ToUpper(*s));
	int n = w.GetCount();
	word flags = 0;
	if(n) {
		if(comments_lang && !SpellWord(w, comments_lang))
			flags = LineEdit::SPELLERROR;
	}
	else
		for(const wchar *s = p; s < e && !IsLetter(*s); s++)
			n++;
	hls.SetFlags(n, flags);
	static const Index<WString> key_words = {"TODO", "NOTE", "FIXME"};
	if(w.GetCount() >= 4 && w.GetCount() <= 5 && key_words.Find(w) >= 0)
		hls.Put(n, hl_style[INK_COMMENT_WORD], hl_style[PAPER_COMMENT_WORD]);
	else
		hls.Put(n, hl_style[INK_COMMENT]);
	return p + n;
}

bool CSyntax::RawString(const wchar *p, int& n) {
	if(highlight != HIGHLIGHT_CPP)
		return false;
	const wchar *s = p;
	if(*s++ != 'R')
		return false;
	while(*s == ' ' || *s == '\t')
		s++;
	if(*s++ != '\"')
		return false;
	WString rs;
	while(*s != '(') {
		if(*s == '\0')
			return false;
		rs.Cat(*s++);
	}
	raw_string = ")";
	raw_string.Cat(rs);
	raw_string.Cat('\"');
	n = int(s + 1 - p);
	return true;
};

void CSyntax::Highlight(const wchar *ltext, const wchar *e, HighlightOutput& hls, CodeEditor *editor, int line, int64 pos)
{
	ONCELOCK {
		InitKeywords();
	}
	
	bool include = false;
	
	int tabsize = editor ? editor->GetTabSize() : 4;
	
	LTIMING("HighlightLine");
	if(highlight < 0 || highlight >= keyword.GetCount())
		return;
	bool macro;
	{
		CSyntax next;
		next.Set(Get());
		next.ScanSyntax(ltext, e, line + 1, tabsize);
		macro = next.macro != MACRO_OFF;
	}
	
	int linelen = int(e - ltext);
	const wchar *p = ltext;
	
	for(const wchar *ms = p; ms < e; ms++)
		if(*ms != ' ' && *ms != '\t') {
			macro = macro || *ms == '#';
			break;
		}
	
	Grounding(p, e);
	if(highlight == HIGHLIGHT_CALC) {
		if(editor && line == editor->GetLineCount() - 1 || *p == '$')
			hls.SetPaper(0, linelen + 1, hl_style[PAPER_BLOCK1].color);
	}
	else
		hls.SetPaper(0, linelen + 1,
		             macro ? hl_style[PAPER_MACRO].color : hl_style[PAPER_NORMAL].color);
	int block_level = bid.GetCount() - 1;
	String cppid;
	if(!comment && highlight != HIGHLIGHT_CALC) {
		if(!macro) {
			int i = 0, bid = 0, pos = 0;
			while(bid < this->bid.GetCount() - 1
			&& (i >= linelen || p[i] == ' ' || p[i] == '\t')) {
				hls.SetPaper(i, 1, BlockColor(bid));
				if(i < linelen && p[i] == '\t' || ++pos >= tabsize) {
					pos = 0;
					bid++;
				}
				i++;
			}
			hls.SetPaper(i, 1 + max(0, linelen - i), BlockColor(this->bid.GetCount() - 1));
		}
		while(*p == ' ' || *p == '\t') {
			p++;
			hls.Put(hl_style[INK_NORMAL]);
		}
		if(*p == '#' && findarg(highlight, HIGHLIGHT_CPP, HIGHLIGHT_CS) >= 0) {
			static Index<String> macro;
			ONCELOCK {
				static const char *pd[] = {
					"include", "define", "error", "if", "elif", "else", "endif",
					"ifdef", "ifndef", "line", "undef", "pragma",
					// CLR
					"using"
				};
				for(int i = 0; i < __countof(pd); i++)
					macro.Add(pd[i]);
			}
			const wchar *q = p + 1;
			while(*q == ' ' || *q == '\t')
				q++;
			StringBuffer id;
			while(IsAlpha(*q))
				id.Cat(*q++);
			cppid = id;
			int mq = macro.Find(cppid);
			hls.Put(mq < 0 ? 1 : int(q - p), hl_style[INK_MACRO]);
			if(mq == 0)
				include = true;
			p = q;
		}
	}
	int lindent = int(p - ltext);
	int lbrace = -1;
	int lbclose = -1;
	Color lbcolor = Null;
	bool is_comment = false;
	while(p < e) {
		int  raw_n = 0;
		dword pair = MAKELONG(p[0], p[1]);
		if(pair == MAKELONG('/', '*') && highlight != HIGHLIGHT_JSON || comment) {
			if(!comment) {
				hls.Put(2, hl_style[INK_COMMENT]);
				p += 2;
			}
			for(const wchar *ce = p; ce < e - 1; ce++)
				if(ce[0] == '*' && ce[1] == '/') {
					while(p < ce)
						p = DoComment(hls, p, ce);
					hls.Put(2, hl_style[INK_COMMENT]);
					p += 2;
					comment = false;
					goto comment_ended;
				}
			while(p < e)
				p = DoComment(hls, p, e);
			comment = is_comment = true;
			break;
		comment_ended:;
		}
		else
		if(raw_string.GetCount() || RawString(p, raw_n)) {
			hls.Put(raw_n, hl_style[INK_CONST_STRING]);
			p += raw_n;
			const wchar *b = p;
			while(p < e) {
				const wchar *s = p;
				const wchar *r = raw_string;
				while(*s && *r) {
					if(*s != *r)
						break;
					s++;
					r++;
				}
				if(*r == '\0') {
					hls.Put(int(p - b), hl_style[INK_RAW_STRING]);
					hls.Put(int(s - p), hl_style[INK_CONST_STRING]);
					b = p = s;
					raw_string.Clear();
					break;
				}
				p++;
			}
			if(p != b)
				hls.Put(int(p - b), hl_style[INK_RAW_STRING]);
		}
		else
		if(linecomment && linecont || pair == MAKELONG('/', '/') &&
		   highlight != HIGHLIGHT_CSS && highlight != HIGHLIGHT_JSON ||
		   highlight == HIGHLIGHT_PHP && *p == '#') {
			while(p < e)
				p = DoComment(hls, p, e);
			is_comment = true;
			break;
		}
		else
		if((*p == '\"' || *p == '\'') || linecont && string)
			p = hls.CString(p);
		else
		if(*p == '(' && !ignore_errors) {
			brk.Add(')');
			Bracket(int(p - ltext) + pos, hls, editor);
			hls.Put(hl_style[INK_PAR0 + max(pl++, 0) % 4]);
			p++;
		}
		else
		if(*p == '{' && !ignore_errors) {
			brk.Add(was_namespace ? 0 : '}');
			Bracket(int(p - ltext) + pos, hls, editor);
			hls.Put(hl_style[INK_PAR0 + max(cl, 0) % 4]);
			if(was_namespace)
				was_namespace = false;
			else {
				++block_level;
				++cl;
			}
			if(hls.pos < linelen)
				hls.SetPaper(hls.pos, linelen - hls.pos + 1, BlockColor(block_level));
			p++;
		}
		else
		if(*p == '[' && !ignore_errors) {
			brk.Add(']');
			Bracket(int(p - ltext) + pos, hls, editor);
			hls.Put(hl_style[INK_PAR0 + max(bl++, 0) % 4]);
			p++;
		}
		else
		if((*p == ')' || *p == '}' || *p == ']') && !ignore_errors) {
			int bc = brk.GetCount() ? brk.Pop() : 0;
			if(*p == '}' && hilite_scope && block_level > 0 && bc)
				hls.SetPaper(hls.pos, linelen + 1 - hls.pos, BlockColor(--block_level));
			Bracket(int(p - ltext) + pos, hls, editor);
			int& l = *p == ')' ? pl : *p == '}' ? cl : bl;
			if(bc && (bc != *p || l <= 0) || bc == 0 && *p != '}') {
				hls.Put(p == ltext ? hl_style[INK_PAR0] : hl_style[INK_ERROR]);
				brk.Clear();
				cl = bl = pl = 0;
			}
			else {
				if(bc) --l;
				hls.Put(1, hl_style[INK_PAR0 + l % 4]);
			}
			p++;
		}
		else
		if(highlight == HIGHLIGHT_CSS ? *p == '#' : findarg(pair, MAKELONG('0', 'x'), MAKELONG('0', 'X'), MAKELONG('0', 'b'), MAKELONG('0', 'B')) >= 0)
			p = HighlightHexBin(hls, p, 1 + (highlight != HIGHLIGHT_CSS), thousands_separator);
		else
		if(IsDigit(*p))
			p = HighlightNumber(hls, p, thousands_separator, *p == '0', highlight == HIGHLIGHT_CSS);
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
			int uq = highlight == 0 ? kw_upp.Find(iid) : -1;
			int nq = keyword[highlight].Find(iid);
			hls.Put(int(q - p), !include && nq >= 0 ? hl_style[nq >= breakers[highlight] ? INK_BREAK_KEYWORD : INK_KEYWORD] :
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
				was_namespace = true;
		}
		else {
			if(*p == ';')
				was_namespace = false;
			hls.Put(strchr("!+-*^/%~&|=[]:?<>.", *p) ? hl_style[INK_OPERATOR] : hl_style[INK_NORMAL]);
			p++;
		}
	}
	if(hilite_ifdef && !IsNull(cppid) && !is_comment) {
		if((cppid == "else" || cppid == "elif" || cppid == "endif") && !ifstack.IsEmpty()) {
			WStringBuffer ifln;
			ifln.Cat(" // ");
			ifln.Cat(ifstack.Top().iftext);
			if(ifstack.Top().ifline && hilite_ifdef == 2) {
				ifln.Cat(", line ");
				ifln.Cat(FormatInt(ifstack.Top().ifline));
			}
			ifln.Cat('\t');
			int start = linelen;
			WString ifs(ifln);
			hls.Set(start, ifs.GetLength(), hl_style[INK_IFDEF]);
			for(int i = 0; i < ifs.GetCount(); i++)
				hls.SetChar(start + i, ifs[i]);
		}
	}
	if(hilite_scope) {
		if(lbrace >= 0 && lbclose < 0 && lbrace > lindent)
			hls.SetPaper(lindent, lbrace - lindent, lbcolor);
		if(lbrace < 0 && lbclose >= 0)
			hls.SetPaper(lbclose, linelen + 1 - lbclose, lbcolor);
	}
	if(findarg(cppid, "else", "elif", "endif", "if", "ifdef", "ifndef") >= 0)
	   hls.SetPaper(0, hls.v.GetCount(), hl_style[PAPER_IFDEF].color);
}

}
