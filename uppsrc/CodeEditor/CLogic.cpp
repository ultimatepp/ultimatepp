#include "CodeEditor.h"

namespace Upp {
	
void CSyntax::IndentInsert(CodeEditor& e, int chr, int count)
{
	IndentInsert0(e, chr, count, false);
}

static void sSpaces(int& cp, const WString& ln)
{
	while(cp < ln.GetCount() && findarg(ln[cp], '\t', ' ') >= 0)
		cp++;
}

int CSyntax::GetCommentPos(CodeEditor& e, int l, WString& ch) const
{
	WString ln = e.GetWLine(l);
	int cp = 0;
	sSpaces(cp, ln);
	One<EditorSyntax> esyntax = e.GetSyntax(e.GetCursorLine());
	CSyntax *syntax = dynamic_cast<CSyntax *>(~esyntax);
	if(syntax && syntax->comment && ln.Find("*/") < 0 ||
	   cp < ln.GetCount() - 2 && ln[cp] == '/' && ln[cp + 1] == '/') {
	    while(cp < ln.GetLength() && findarg(ln[cp], '/', '*', '!', '<', '>', '%', '#', '@', '|') >= 0)
	        cp++;
	    ch = ln.Mid(0, cp);
        sSpaces(cp, ln);
        if(cp < ln.GetCount())
	        return cp;
	}
	ch = "0";
	return -1;
}

void CSyntax::IndentInsert0(CodeEditor& e, int chr, int count, bool reformat)
{
	if(chr == '\n') {
		while(count--) {
			int pp = e.GetCursor();
			int cl = e.GetLinePos(pp);
			WString pl = e.GetWLine(cl);
			e.InsertChar('\n', 1);
			int p = e.GetCursor();
			int l = e.GetLinePos(p);
			One<EditorSyntax> esyntax;
			CSyntax *syntax = NULL;
			if(e.GetLineLength(l) == p) { // At the last char of line
				esyntax = e.GetSyntax(e.GetCursorLine());
				syntax = dynamic_cast<CSyntax *>(~esyntax);
				if(!syntax) {
					TagSyntax *h = dynamic_cast<TagSyntax *>(~esyntax);
					if(h)
						syntax = h->GetScript();
				}
			}
			if(syntax && syntax->par.GetCount() && syntax->par.Top().line == cl && no_parenthesis_indent) {
				for(int i = 0; i < syntax->par.Top().pos && i < pl.GetLength(); i++) // Indent e.g. next line of if(
					e.InsertChar(pl[i] == '\t' ? '\t' : ' ', 1);
				return;
			}
			if(syntax && syntax->endstmtline == cl && syntax->seline >= 0 && syntax->seline < e.GetLineCount())
				pl = e.GetWLine(syntax->seline);
			else {
				int i;
				for(i = 0; i < pl.GetLength(); i++) {
					if(pl[i] != ' ' && pl[i] != '\t')
						break;
				}
				if(pp < i)
					return;
			}
			const wchar *s = pl;
			while(*s == '\t' || *s == ' ')
				e.InsertChar(*s++, 1);
			if(!syntax)
				return;
			if(syntax->stmtline == cl || syntax->blk.GetCount() && syntax->blk.Top() == cl // statement (if, while..) || first line of block
			   || *pl.Last() == ':') { // label
				if(indent_spaces || (s > pl && s[-1] == ' '))
					e.InsertChar(' ', indent_amount);
				else
					e.InsertChar('\t', 1);
			}
		}
		return;
	}

	if(e.IsWordwrapComments()) {
		int limit = e.GetBorderColumn();
		int pos = e.GetCursor();
		int lp = pos;
		int l = e.GetLinePos(lp);
		if(limit > 10 && e.GetColumnLine(pos).x >= limit && lp == e.GetLineLength(l)) {
			int lp0 = e.GetPos(l);
			WString ln = e.GetWLine(l);
			WString dummy;
			int cp = GetCommentPos(e, l, dummy);
			if(cp >= 0) {
				int wl = (int)e.GetGPos(l, limit) - lp0;
				while(wl > cp && ln[wl - 1] != '\n' && ln[wl - 1] != ' ')
					wl--;
				int sl = wl - 1;
				while(sl > cp && ln[wl - 1] != '\n' && ln[sl - 1] == ' ')
					sl--;
				if(sl > cp) {
					e.Remove(lp0 + sl, pos - (lp0 + sl));
					e.SetCursor(lp0 + sl);
					e.Put('\n');
					for(int i = 0; i < cp; i++)
						e.Put(ln[i]);
					for(int i = wl; i < ln.GetCount(); i++)
						e.Put(ln[i]);
					e.Put(chr);
					if(!reformat)
						e.FinishPut();
					return;
				}
			}
		}
	}
	
	if(reformat) {
		e.Put(chr);
		return;
	}

	// {, } inserted on line alone should be moved left sometimes:
	int cl = e.GetCursorLine();
	WString l = e.GetWLine(cl);
	if(chr != '{' && chr != '}' || count > 1) {
		e.InsertChar(chr, count, true);
		return;
	}
	const wchar *s;
	for(s = l; s < l.End(); s++)
		if(*s != ' ' && *s != '\t') {
			e.InsertChar(chr, 1);
			return;
		}
	int len = l.GetLength();
	WString tl;
	int pos = e.GetPos(cl);
	if(chr == '{' && cl > 0 && stmtline == cl - 1)
		tl = e.GetWLine(cl - 1);
	else
	if(chr == '}' && blk.GetCount())
		tl = e.GetWLine(blk.Top());
	else {
		e.InsertChar(chr, 1);
		return;
	}
	e.SetCursor(pos);
	e.Remove(pos, len);
	s = tl;
	while(*s == '\t' || *s == ' ')
		e.InsertChar(*s++, 1);
	e.InsertChar(chr, 1);
}

void CSyntax::ReformatComment(CodeEditor& e)
{
	if(!e.IsWordwrapComments())
		return;
	int first_line = e.GetLine(e.GetCursor64());
	WString ch;
	if(GetCommentPos(e, first_line, ch) < 0)
		return;
	int last_line = first_line;
	while(first_line > 0 && GetCommentHdr(e, first_line - 1) == ch)
		first_line--;
	while(last_line < e.GetLineCount() - 1 && GetCommentHdr(e, last_line + 1) == ch)
		last_line++;
	WString p;
	for(int i = first_line; i <= last_line; i++) {
		WString ln = e.GetWLine(i);
		p << ln.Mid(GetCommentPos(e, i, ch)) << ' ';
	}
	p = Join(Split(p, CharFilterWhitespace), " ");
	WString ln = e.GetWLine(first_line);
	int q = GetCommentPos(e, first_line, ch);
	p = ln.Mid(0, q) + p;
	int p0 = e.GetPos(first_line);
	e.SetCursor(p0);
	e.Remove(p0, e.GetPos(last_line + 1) - 1 - p0);
	for(wchar chr : p)
		IndentInsert0(e, chr, 1, true);
	e.FinishPut();
}

bool NotEscape(int64 pos, const WString& s)
{
	return pos == 0 || s[(int)pos - 1] != '\\' ? true : !NotEscape(pos - 1, s);
}

bool CSyntax::CheckBracket(CodeEditor& e, int li, int64 pos, int64 ppos, int64 pos0, WString ln,
                           int d, int limit, int64& bpos0, int64& bpos)
{
	int lvl = 1;
	pos += d;
	ppos += d;
	for(;;) {
		int c;
		for(;;) {
			while(pos < 0 || pos >= ln.GetLength()) {
				li += d;
				if(d * li >= d * limit)
					return false;
				ln = e.GetWLine(li);
				int q = ln.Find("//");
				if(q >= 0)
					ln = ln.Mid(0, q) + WString(' ', ln.GetCount() - q);
				pos = d < 0 ? ln.GetLength() - 1 : 0;
				ppos += d;
			}
			c = ln[(int)pos];
			if((c == '\"' || c == '\'') && (pos > 0 && NotEscape(pos, ln) && ln[(int)pos - 1] != '\'')) {
				pos += d;
				ppos += d;
				int lc = c;
				while(pos < ln.GetLength() && pos > 0) {
					if(ln[(int)pos] == lc && NotEscape(pos, ln)) {
						pos += d;
						ppos += d;
						break;
					}
					pos += d;
					ppos += d;
				}
			}
			else
				break;
		}
		if(islbrkt(c))
			lvl += d;
		if(isrbrkt(c))
			lvl -= d;
		if(lvl <= 0) {
			bpos0 = pos0;
			bpos = ppos;
			return true;
		}
		pos += d;
		ppos += d;
	}
	return false;
}

bool CSyntax::CheckLeftBracket(CodeEditor& e, int64 pos, int64& bpos0, int64& bpos)
{
	if(pos < 0 || pos >= e.GetLength64())
		return false;
	int64 ppos = pos;
	int li = e.GetLinePos64(pos);
	WString ln = e.GetWLine(li);
	return islbrkt(ln[(int)pos]) &&
	       CheckBracket(e, li, pos, ppos, ppos, ln, 1, min(li + 3000, e.GetLineCount()), bpos0, bpos);
}

bool CSyntax::CheckRightBracket(CodeEditor& e, int64 pos, int64& bpos0, int64& bpos)
{
	if(pos < 0 || pos >= e.GetLength64())
		return false;
	int64 ppos = pos;
	int li = e.GetLinePos64(pos);
	WString ln = e.GetWLine(li);
	return isrbrkt(ln[(int)pos]) &&
	       CheckBracket(e, li, pos, ppos, ppos, ln, -1, max(li - 3000, 0), bpos0, bpos);
}

bool CSyntax::CheckBrackets(CodeEditor& e, int64& bpos0, int64& bpos)
{
	int64 c = e.GetCursor64();
	return CheckLeftBracket(e, c, bpos0, bpos) ||
	       CheckRightBracket(e, c, bpos0, bpos) ||
	       CheckLeftBracket(e, c - 1, bpos0, bpos) ||
	       CheckRightBracket(e, c - 1, bpos0, bpos);
}

bool CSyntax::CanAssist() const
{
	return !comment && !string && !linecomment && IsNull(raw_string);
}

Vector<IfState> CSyntax::PickIfStack()
{
	return pick(ifstack);
}

void CSyntax::CheckSyntaxRefresh(CodeEditor& e, int64 pos, const WString& text)
{
	for(const wchar *s = text; *s; s++) {
		if(*s == '{' || *s == '(' || *s == '[' || *s == '/' || *s == '*' ||
		   *s == '}' || *s == ')' || *s == ']' || *s == '\\' || *s == ':') {
			e.Refresh();
			break;
		}
	}
	String s = TrimLeft(e.GetUtf8Line(e.GetLine(pos)));
	if(s.StartsWith("#if") || s.StartsWith("#e"))
		e.Refresh();

	WString h = e.GetWLine(e.GetLinePos64(pos)); // block highlighting changes if start of line is changed
	for(int i = 0; i < pos; i++)
		if(findarg(h[i], ' ', '\t') < 0)
			return;
	e.Refresh();
}

bool CSyntax::GetBlockHeader(Point& blk_start, Point& blk_end)
{
	blk_start = this->blk_start;
	blk_end = this->blk_end;
	return true;
}

}
