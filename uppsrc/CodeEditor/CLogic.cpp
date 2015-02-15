#include "CodeEditor.h"

NAMESPACE_UPP

void CSyntax::IndentInsert(CodeEditor& e, int chr, int count)
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
			if(syntax->stmtline == cl || syntax->blk.GetCount() && syntax->blk.Top() == cl) {
				if(indent_spaces || (s > pl && s[-1] == ' '))
					e.InsertChar(' ', indent_amount);
				else
					e.InsertChar('\t', 1);
			}
		}
		return;
	}
	// {, } inserted on line alone should be moved left sometimes:
	int cl = e.GetCursorLine();
	WString l = e.GetWLine(cl);
	if(chr != '{' && chr != '}' || count > 1) {
		e.InsertChar(chr, 1, true);
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

bool NotEscape(int pos, const WString& s)
{
	return pos == 0 || s[pos - 1] != '\\' ? true : !NotEscape(pos - 1, s);
}

bool CSyntax::CheckBracket(CodeEditor& e, int li, int pos, int ppos, int pos0, WString ln, int d, int limit,
                               int& bpos0, int& bpos)
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
			c = ln[pos];
			if((c == '\"' || c == '\'') && (pos > 0 && NotEscape(pos, ln) && ln[pos - 1] != '\'')) {
				pos += d;
				ppos += d;
				int lc = c;
				while(pos < ln.GetLength() && pos > 0) {
					if(ln[pos] == lc && NotEscape(pos, ln)) {
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

bool CSyntax::CheckLeftBracket(CodeEditor& e, int pos, int& bpos0, int& bpos)
{
	if(pos < 0 || pos >= e.GetLength())
		return false;
	int ppos = pos;
	int li = e.GetLinePos(pos);
	WString ln = e.GetWLine(li);
	return islbrkt(ln[pos]) &&
	       CheckBracket(e, li, pos, ppos, ppos, ln, 1, min(li + 3000, e.GetLineCount()), bpos0, bpos);
}

bool CSyntax::CheckRightBracket(CodeEditor& e, int pos, int& bpos0, int& bpos)
{
	if(pos < 0 || pos >= e.GetLength())
		return false;
	int ppos = pos;
	int li = e.GetLinePos(pos);
	WString ln = e.GetWLine(li);
	return isrbrkt(ln[pos]) &&
	       CheckBracket(e, li, pos, ppos, ppos, ln, -1, max(li - 3000, 0), bpos0, bpos);
}

bool CSyntax::CheckBrackets(CodeEditor& e, int& bpos0, int& bpos)
{
	int c = e.GetCursor();
	return CheckLeftBracket(e, c, bpos0, bpos) ||
	       CheckRightBracket(e, c, bpos0, bpos) ||
	       CheckLeftBracket(e, c - 1, bpos0, bpos) ||
	       CheckRightBracket(e, c - 1, bpos0, bpos);
}

bool CSyntax::CanAssist() const
{
	return !comment && !string && !linecomment;
}

Vector<IfState> CSyntax::PickIfStack()
{
	return pick(ifstack);
}

void CSyntax::CheckSyntaxRefresh(CodeEditor& e, int pos, const WString& text)
{
	for(const wchar *s = text; *s; s++) {
		if(*s == '{' || *s == '(' || *s == '[' || *s == '/' || *s == '*' ||
		   *s == '}' || *s == ')' || *s == ']' || *s == '\\') {
			e.Refresh();
			break;
		}
	}
	String s = TrimLeft(e.GetUtf8Line(e.GetLine(pos)));
	if(s.StartsWith("#if") || s.StartsWith("#e"))
		e.Refresh();

	WString h = e.GetWLine(e.GetLinePos(pos)); // block highlighting changes if start of line is changed
	for(int i = 0; i < pos; i++)
		if(findarg(h[i], ' ', '\t') < 0)
			return;
	e.Refresh();
}

END_UPP_NAMESPACE
