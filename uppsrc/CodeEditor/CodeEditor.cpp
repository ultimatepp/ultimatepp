#include "CodeEditor.h"

NAMESPACE_UPP

#define LLOG(x) // RLOG(x)
#define LTIMING(x) // RTIMING(x)

#define IMAGEVECTOR Vector
#define IMAGECLASS  CodeEditorImg
#define IMAGEFILE   <CodeEditor/CodeEditor.iml>
#include <Draw/iml_source.h>

void CodeEditor::DirtyFrom(int line) {
	for(int i = 0; i < 4; i++)
		if(scache[i].line >= line)
			scache[i].Clear();

	if(check_edited) {
		bar.ClearErrors(line);
		bar.Refresh();
	}
}

inline bool IsComment(int a, int b) {
	return a == '/' && b == '*' || a == '*' && b == '/' || a == '/' && b == '/';
}

void CodeEditor::PreInsert(int pos, const WString& text) {
	DTIMING("PreInsert");
	if(IsFullRefresh()) return;
	rm_ins = ScanSyntax(GetLine(pos) + 1);
}

inline bool RBR(int c) {
	return isbrkt(c);
}

void CodeEditor::CheckBraces(const WString& text)
{
	for(const wchar *s = text; *s; s++)
		if(*s == '{' || *s == '(' || *s == '[' || *s == '/' || *s == '*' ||
		   *s == '}' || *s == ')' || *s == ']') {
			Refresh();
			break;
		}
}

void CodeEditor::PostInsert(int pos, const WString& text) {
	DTIMING("PostInsert");
	if(check_edited)
		bar.SetEdited(GetLine(pos));
	if(IsFullRefresh()) return;
	if(text.GetCount() > 200)
		Refresh();
	else
		CheckBraces(text);
	// Following code is probable not needed anymore...
	if(!ScanSyntax(GetLine(pos + text.GetLength()) + 1).MatchHilite(rm_ins)) {
		if(text.GetLength() == 1 && *text == '(' || *text == '[' || *text == ']' || *text == ')')
			RefreshChars(RBR);
		else
			Refresh();
		bar.Refresh();
	}
}

void CodeEditor::PreRemove(int pos, int size) {
	DTIMING("PreRemove");
	if(IsFullRefresh()) return;
	if(size > 200)
		Refresh();
	else
		CheckBraces(GetW(pos, size));
	// Following code is probable not needed anymore...
	if(size == 1)
		rmb = Get(pos, 1)[0];
	else
		rmb = 0;
	rm_ins = ScanSyntax(GetLine(pos + size) + 1);
}

void CodeEditor::PostRemove(int pos, int size) {
	DTIMING("PostRemove");
	if(check_edited)
		bar.SetEdited(GetLine(pos));
	if(IsFullRefresh()) return;
	// Following code is probable not needed anymore...
	if(!ScanSyntax(GetLine(pos) + 1).MatchHilite(rm_ins)) {
		if(rmb == '(' || rmb == '[' || rmb == ']' || rmb == ')')
			RefreshChars(RBR);
		else
			Refresh();
		bar.Refresh();
	}
	CheckBrackets();
}

void CodeEditor::ClearLines() {
	bar.ClearLines();
}

void CodeEditor::InsertLines(int line, int count) {
	bar.InsertLines(line, count);
	if(line <= line2.GetCount())
		line2.Insert(line, GetLine2(line), count);
}

void CodeEditor::RemoveLines(int line, int count) {
	bar.RemoveLines(line, count);
	if(line + count <= line2.GetCount())
		line2.Remove(line, count);
}

void CodeEditor::Renumber2()
{
	line2.SetCount(GetLineCount());
	for(int i = 0; i < GetLineCount(); i++)
		line2[i] = i;
}

int CodeEditor::GetLine2(int i) const
{
	return line2.GetCount() ? line2[min(line2.GetCount() - 1, i)] : 0;
}

void CodeEditor::NewScrollPos() {
	bar.Scroll();
}

bool NotEscape(int pos, const WString& s)
{
	return pos == 0 || s[pos - 1] != '\\' ? true : !NotEscape(pos - 1, s);
}

void CodeEditor::CheckBracket(int li, int pos, int ppos, int pos0, WString ln, int d, int limit)
{
	int li0 = li;
	int lvl = 1;
	pos += d;
	ppos += d;
	for(;;) {
		int c;
		for(;;) {
			while(pos < 0 || pos >= ln.GetLength()) {
				li += d;
				if(d * li >= d * limit)
					return;
				ln = GetWLine(li);
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
			highlight_bracket_pos0 = pos0;
			highlight_bracket_pos = ppos;
			RefreshLine(li);
			RefreshLine(li0);
			bracket_start = GetTimeClick();
			return;
		}
		pos += d;
		ppos += d;
	}
}

void CodeEditor::CheckLeftBracket(int pos)
{
	LTIMING("CheckLeftBracket");
	if(pos < 0 || pos >= GetLength())
		return;
	int ppos = pos;
	int li = GetLinePos(pos);
	WString ln = GetWLine(li);
	if(islbrkt(ln[pos]))
		CheckBracket(li, pos, ppos, ppos, ln, 1, min(li + 3000, GetLineCount()));
}

void CodeEditor::CheckRightBracket(int pos)
{
	LTIMING("CheckRightBracket");
	if(pos < 0 || pos >= GetLength())
		return;
	int ppos = pos;
	int li = GetLinePos(pos);
	WString ln = GetWLine(li);
	int c = ln[pos];
	if(c == ')' || c == '}' || c == ']')
		CheckBracket(li, pos, ppos, ppos, ln, -1, max(li - 3000, 0));
}

void CodeEditor::CopyWord() {
	int p = GetCursor();
	if(iscidl(GetChar(p)) || (p > 0 && iscidl(GetChar(--p)))) {
		int e = GetLength();
		int f = p;
		while(--p >= 0 && iscidl(GetChar(p)));
			++p;
		while(++f < e && iscidl(GetChar(f)));
		WString txt = GetW(p, f - p);
		WriteClipboardUnicodeText(txt);
		AppendClipboardText(txt.ToString());
	}
}

void CodeEditor::SwapChars() {
	if(IsReadOnly()) return;
	int i = GetLine(cursor);
	int j = GetPos(i);
	if (j < cursor && (cursor-j) < line[i].GetLength()) {
		int p = cursor;
		WString txt(GetChar(p-1),1);
		Remove(p-1,1);
		Insert(p, txt, true);
		PlaceCaret(p);
	}
}

void CodeEditor::CancelBracketHighlight(int& pos)
{
	if(pos >= 0) {
		RefreshLine(GetLine(pos));
		pos = -1;
	}
}

void CodeEditor::Periodic()
{
	bool b = (((GetTimeClick() - bracket_start) >> 8) & 1) == 0;
	if(b != bracket_flash && hilite_bracket == 2) {
		bracket_flash = b;
		if(highlight_bracket_pos0 >= 0)
			RefreshLine(GetLine(highlight_bracket_pos0));
		if(highlight_bracket_pos >= 0)
			RefreshLine(GetLine(highlight_bracket_pos));
	}
}

void CodeEditor::SelectionChanged()
{
	if(!foundsel) {
		CloseFindReplace();
		found = false;
		notfoundfw = notfoundbk = false;
		findreplace.amend.Disable();
	}
	CheckBrackets();
	bar.Refresh();
}

void CodeEditor::CheckBrackets()
{
	CancelBracketHighlight(highlight_bracket_pos0);
	CancelBracketHighlight(highlight_bracket_pos);
	if(!IsSelection()) {
		CheckLeftBracket(GetCursor());
		if(highlight_bracket_pos < 0)
			CheckRightBracket(GetCursor());
		if(highlight_bracket_pos < 0)
			CheckLeftBracket(GetCursor() - 1);
		if(highlight_bracket_pos < 0)
			CheckRightBracket(GetCursor() - 1);
	}
	WhenSelection();
}

bool CodeEditor::InsertRS(int chr, int count) {
	if(IsReadOnly()) return true;
	if(IsSelection()) {
		InsertChar(chr, count);
		return true;
	}
	return false;
}

void CodeEditor::IndentEnter(int count) {
	if(InsertRS('\n', count)) return;
	while(count--) {
		int pp = GetCursor();
		int cl = GetLinePos(pp);
		WString pl = GetWLine(cl);
		InsertChar('\n', 1);
		SyntaxState st;
		int p = GetCursor();
		int l = GetLinePos(p);
		if(highlight >= 0 && GetLineLength(l) == p)
			st = ScanSyntax(GetCursorLine());
		if(st.par.GetCount() && st.par.Top().line == cl && !no_parenthesis_indent) {
			for(int i = 0; i < st.par.Top().pos && i < pl.GetLength(); i++)
				InsertChar(pl[i] == '\t' ? '\t' : ' ', 1);
			return;
		}
		if(st.endstmtline == cl && st.seline >= 0 && st.seline < GetLineCount())
			pl = GetWLine(st.seline);
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
			InsertChar(*s++, 1);
		if(st.stmtline == cl || st.blk.GetCount() && st.blk.Top() == cl)
			if(indent_spaces || (s > pl && s[-1] == ' '))
				InsertChar(' ', indent_amount);
			else
				InsertChar('\t', 1);
	}
}

void CodeEditor::IndentInsert(int chr) {
	if(InsertRS(chr)) return;
	int cl = GetCursorLine();
	WString l = GetWLine(cl);
	if(chr != '{' && chr != '}') {
		InsertChar(chr, 1, true);
		return;
	}
	const wchar *s;
	for(s = l; s < l.End(); s++)
		if(*s != ' ' && *s != '\t') {
			InsertChar(chr, 1);
			return;
		}
	int len = l.GetLength();
	SyntaxState st = ScanSyntax(cl);
	WString tl;
	int pos = GetPos(cl);
	if(chr == '{' && cl > 0 && st.stmtline == cl - 1 && highlight >= 0)
		tl = GetWLine(cl - 1);
	else
	if(chr == '}' && st.blk.GetCount() && highlight >= 0)
		tl = GetWLine(st.blk.Top());
	else {
		InsertChar(chr, 1);
		return;
	}
	SetCursor(pos);
	Remove(pos, len);
	s = tl;
	while(*s == '\t' || *s == ' ')
		InsertChar(*s++, 1);
	InsertChar(chr, 1);
}

void CodeEditor::Make(Callback1<String&> op)
{
	Point cursor = GetColumnLine(GetCursor());
	Point scroll = GetScrollPos();
	int l, h;
	bool is_sel = GetSelection(l, h);
	if(!is_sel) { l = 0; h = GetLength(); }
	if(h <= l)
	{
		BeepExclamation();
		return;
	}
	l = GetPos(GetLine(l));
	h = GetPos(GetLine(h - 1) + 1);
	String substring = Get(l, h - l);
	String out = substring;
	op(out);
	if(out == substring)
	{
		BeepInformation();
		return;
	}
	Remove(l, h - l);
	Insert(l, out.ToWString());
	if(is_sel)
		SetSelection(l, l + out.GetLength());
	SetCursor(GetGPos(cursor.y, cursor.x));
	SetScrollPos(scroll);
}

void CodeEditor::TabsOrSpaces(String& out, bool maketabs)
{
	String substring = out;
	out.Clear();
	int tab = GetTabSize();
	if(tab <= 0) tab = 8;
	for(const char *p = substring.Begin(), *e = substring.End(); p < e;)
	{
		int pos = 0;
		for(; p < e; p++)
			if(*p == '\t')
				pos = (pos / tab + 1) * tab;
			else if(*p == ' ')
				pos++;
			else
				break;
		if(maketabs)
		{
			out.Cat('\t', pos / tab);
			const char *b = p;
			while(p < e && *p++ != '\n')
				;
			out.Cat(b, int(p - b));
		}
		else
		{
			out.Cat(' ', pos);
			for(; p < e && *p != '\n'; p++)
				if(*p == '\t')
				{
					int npos = (pos / tab + 1) * tab;
					out.Cat(' ', npos - pos);
					pos = npos;
				}
				else
				{
					out.Cat(*p);
					pos++;
				}
			if(p < e)
				out.Cat(*p++);
		}
	}
}

void CodeEditor::MakeTabsOrSpaces(bool maketabs)
{
	Make(THISBACK1(TabsOrSpaces, maketabs));
}

void CodeEditor::LineEnds(String& out)
{
	String substring = out;
	out.Clear();
	const char *q = ~substring;
	const char *b = q;
	for(const char *p = b, *e = substring.End(); p < e; p++)
	{
		if(*p == '\n') {
			out.Cat(b, q);
			out.Cat("\r\n");
			b = q = p + 1;
		}
		else
		if(*p != '\t' && *p != ' ' && *p != '\r')
			q = p + 1;
	}
	out.Cat(b, substring.End());
}

void CodeEditor::MakeLineEnds()
{
	Make(THISBACK(LineEnds));
}

void CodeEditor::MoveNextWord(bool sel) {
	int p = GetCursor();
	int e = GetLength();
	if(iscidl(GetChar(p)))
		while(p < e && iscidl(GetChar(p))) p++;
	else
		while(p < e && !iscidl(GetChar(p))) p++;
	PlaceCaret(p, sel);
}

void CodeEditor::MovePrevWord(bool sel) {
	int p = GetCursor();
	if(p == 0) return;
	if(iscidl(GetChar(p - 1)))
		while(p > 0 && iscidl(GetChar(p - 1))) p--;
	else
		while(p > 0 && !iscidl(GetChar(p - 1))) p--;
	PlaceCaret(p, sel);
}

void CodeEditor::MoveNextBrk(bool sel) {
	int p = GetCursor();
	int e = GetLength();
	if(!islbrkt(GetChar(p)))
		while(p < e && !islbrkt(GetChar(p))) p++;
	else {
		int lvl = 1;
		p++;
		for(;;) {
			if(p >= e) break;
			int c = GetChar(p++);
			if(islbrkt(c)) lvl++;
			if(isrbrkt(c) && --lvl == 0) break;
		}
	}
	PlaceCaret(p, sel);
}

void CodeEditor::MovePrevBrk(bool sel) {
	int p = GetCursor();
	if(p < 2) return;
	if(!isrbrkt(GetChar(p - 1)))
		if(p < GetLength() - 1 && isrbrkt(GetChar(p)))
			p++;
		else {
			while(p > 0 && !isrbrkt(GetChar(p - 1))) p--;
			PlaceCaret(p, sel);
			return;
		}
	int lvl = 1;
	p -= 2;
	for(;;) {
		if(p <= 1) break;
		int c = GetChar(p);
		if(isrbrkt(c)) lvl++;
		if(islbrkt(c) && --lvl == 0) break;
		p--;
	}
	PlaceCaret(p, sel);
}

bool isspctab(int c) {
	return c == ' ' || c == '\t';
}

void CodeEditor::DeleteWord() {
	if(IsReadOnly() || RemoveSelection()) return;
	int p = GetCursor();
	int e = GetLength();
	int c = GetChar(p);
	if(iscidl(c))
		while(p < e && iscidl(GetChar(p))) p++;
	else
	if(isspctab(c))
		while(p < e && isspctab(GetChar(p))) p++;
	else {
		DeleteChar();
		return;
	}
	Remove(GetCursor(), p - GetCursor());
}

void CodeEditor::DeleteWordBack() {
	if(IsReadOnly() || RemoveSelection()) return;
	int p = GetCursor();
	if(p < 1) return;
	int c = GetChar(p - 1);
	if(iscidl(GetChar(p - 1)))
		while(p > 1 && iscidl(GetChar(p - 1))) p--;
	else
	if(isspctab(c))
		while(p > 1 && isspctab(GetChar(p - 1))) p--;
	else {
		Backspace();
		return;
	}
	Remove(p, GetCursor() - p);
	PlaceCaret(p);
}

void CodeEditor::SetLineSelection(int l, int h) {
	SetSelection(GetPos(l), GetPos(h));
}

bool CodeEditor::GetLineSelection(int& l, int& h) {
	if(!GetSelection(l, h)) return false;
	l = GetLine(l);
	int pos = h;
	h = GetLinePos(pos);
	if(pos && h < GetLineCount()) h++;
	SetLineSelection(l, h);
	return true;
}

void CodeEditor::TabRight() {
	int l, h;
	if(!GetLineSelection(l, h)) return;
	int ll = l;
	String tab(indent_spaces ? ' ' : '\t', indent_spaces ? GetTabSize() : 1);
	while(l < h)
		Insert(GetPos(l++), tab);
	SetLineSelection(ll, h);
}

void CodeEditor::TabLeft() {
	int l, h;
	if(!GetLineSelection(l, h)) return;
	int ll = l;
	while(l < h) {
		WString ln = GetWLine(l);
		int spc = 0;
		while(spc < tabsize && ln[spc] == ' ') spc++;
		if(spc < tabsize && ln[spc] == '\t') spc++;
		Remove(GetPos(l++), spc);
	}
	SetLineSelection(ll, h);
}

bool CodeEditor::GetWordPos(int pos, int& l, int& h) {
	l = h = pos;
	if(!iscidl(GetChar(pos))) return false;
	while(l > 0 && iscidl(GetChar(l - 1))) l--;
	while(iscidl(GetChar(h))) h++;
	return true;
}

String CodeEditor::GetWord(int pos)
{
	int l, h;
	GetWordPos(pos, l, h);
	return Get(l, h - l);
}

String CodeEditor::GetWord()
{
	return GetWord(cursor);
}

void CodeEditor::LeftDouble(Point p, dword keyflags) {
	int l, h;
	int pos = GetMousePos(p);
	if(GetWordPos(pos, l, h))
		SetSelection(l, h);
	else
		SetSelection(pos, pos + 1);
}

void CodeEditor::LeftDown(Point p, dword keyflags) {
	if((keyflags & K_CTRL) && WhenCtrlClick) {
		WhenCtrlClick(GetMousePos(p));
		return;
	}
	LineEdit::LeftDown(p, keyflags);
	WhenLeftDown();
}

void CodeEditor::Tip::Paint(Draw& w)
{
	Rect r = GetSize();
	w.DrawRect(r, SColorInfo());
	r.left++;
	if(d)
		d->Paint(w, r, v, SColorText(), SColorPaper(), 0);
}

CodeEditor::Tip::Tip()
{
	SetFrame(BlackFrame());
	BackPaint();
}

void CodeEditor::SyncTip()
{
	MouseTip mt;
	mt.pos = tippos;
	if(tippos >= 0 && IsVisible() && WhenTip(mt)) {
		tip.d = mt.display;
		tip.v = mt.value;
		Point p = Upp::GetMousePos();
		Size sz = tip.AddFrameSize(mt.sz);
		tip.SetRect(p.x, p.y + 24, sz.cx, sz.cy);
		if(!tip.IsOpen())
			tip.PopUp(this, false, false, true);
		tip.Refresh();
	}
	else
		CloseTip();
}

void CodeEditor::MouseMove(Point p, dword f) {
	LineEdit::MouseMove(p, f);
	if(IsSelection()) return;
	tippos = GetMousePos(p);
	SyncTip();	
}

Image CodeEditor::CursorImage(Point p, dword keyflags)
{
	if(WhenCtrlClick && (keyflags & K_CTRL))
		return Image::Hand();
	if(tip.IsOpen())
		return Image::Arrow();
	return LineEdit::CursorImage(p, keyflags);
}

void CodeEditor::MouseLeave()
{
	tippos = -1;
	LineEdit::MouseLeave();
}

WString CodeEditor::GetI()
{
	int l, h;
	WString ft;
	if(GetSelection(l, h) || GetWordPos(GetCursor(), l, h)) {
		while(l < h) {
			int c = GetChar(l++);
			if(c == '\n')
				break;
			ft.Cat(c);
		}
	}
	return ft;
}

void CodeEditor::FindWord(bool back)
{
	WString I = GetI();
	if(!IsNull(I))
		Find(back, I, true, false, false, false);
}

void CodeEditor::SetI(Ctrl *edit)
{
	*edit <<= GetI();
}

void CodeEditor::Goto() {
	String line = AsString(GetCursorLine());
	if(EditText(line, "Go to", "Line:"))
		SetCursor(GetPos(atoi(line) - 1));
}

bool CodeEditor::ToggleSimpleComment(int &start_line, int &end_line, bool usestars)
{
	int l, h;
	if(!GetSelection(l, h))
		return true;

	int pos = h;
	start_line = GetLine(l);
	end_line = GetLinePos(pos);

	if(usestars && start_line == end_line) {
		Enclose("/*", "*/", l, h);
		return true;
	}

	if(pos && end_line < GetLineCount()) end_line++;
	SetLineSelection(start_line, end_line);

	return false;
}

void CodeEditor::ToggleLineComments(bool usestars)
{
	int start_line, end_line;
	if(ToggleSimpleComment(start_line, end_line))
		return;

	int us = static_cast<int>(usestars);

	bool is_commented = true;

	if(usestars) {

		is_commented &= GetChar(GetPos(start_line) + 0) == '/' &&
						GetChar(GetPos(start_line) + 1) == '*';

		is_commented &= GetChar(GetPos(end_line - 1) + 1) == '*' &&
						GetChar(GetPos(end_line - 1) + 2) == '/';
	}

	for(int line = start_line + us; is_commented && (line < end_line - us * 2); ++line)
		is_commented &= GetChar(GetPos(line)) == (usestars ? ' ' : '/') &&
						GetChar(GetPos(line)+1) == (usestars ? '*' : '/');

	if(!is_commented) {

		if(usestars) {
			Insert(GetPos(end_line)," */\n");
			Insert(GetPos(start_line),"/*\n");
		}

		for(int line = start_line + us; line < end_line + us; ++line)
			Insert(GetPos(line), usestars ? " * " : "//");
	}
	else
	{
		int line = start_line;
		if(usestars)
			Remove(GetPos(start_line), 3);
		for(; line < end_line - us * 2; ++line)
			Remove(GetPos(line), 2 + us);
		if(usestars)
			Remove(GetPos(line), 4);
	}

	if(usestars)
		SetLineSelection(start_line, end_line + (is_commented ? -2 : 2));
	else
		SetLineSelection(start_line, end_line);
}

void CodeEditor::ToggleStarComments()
{
	int start_line, end_line;
	if(ToggleSimpleComment(start_line, end_line))
		return;

	bool is_commented =
		GetChar(GetPos(start_line)) == '/' &&
		GetChar(GetPos(start_line)+1) == '*' &&
		GetChar(GetPos(start_line)+2) == '\n' &&
		GetChar(GetPos(end_line-1)) == '*' &&
		GetChar(GetPos(end_line-1)+1) == '/' &&
		GetChar(GetPos(end_line-1)+2) == '\n';

	if(!is_commented) {
		// Backwards because inserting changes the end line #
		Insert(GetPos(end_line),"*/\n");
		Insert(GetPos(start_line),"/*\n");
		SetLineSelection(start_line, end_line+2);
	} else {
		// Backwards because inserting changes the end line #
		Remove(GetPos(end_line-1),3);
		Remove(GetPos(start_line),3);
		SetLineSelection(start_line, end_line-2);
	}
}

void CodeEditor::Enclose(const char *c1, const char *c2, int l, int h)
{
	if((l < 0 || h < 0) && !GetSelection(l, h))
		return;
	Insert(l, WString(c1));
	Insert(h + (int)strlen(c1), WString(c2));
	ClearSelection();
	SetCursor(h + (int)strlen(c1) + (int)strlen(c2));
}

bool CodeEditor::Key(dword code, int count) {
	Time key_time = GetSysTime();
	double diff;
	if(!IsNull(last_key_time) && (diff = int(key_time - last_key_time)) <= 3)
		stat_edit_time += diff;
	last_key_time = key_time;

	NextUndo();
	switch(code) {
	case K_CTRL_DELETE:
		DeleteWord();
		return true;
	case K_CTRL_BACKSPACE:
		DeleteWordBack();
		return true;
	case K_BACKSPACE:
		if(!IsReadOnly() && !IsSelection() && indent_spaces) {
			int c = GetCursor();
			Point ixln = GetIndexLine(c);
			WString ln = GetWLine(ixln.y);
			bool white = true;
			int startindex = -1, pos = 0, tabsz = GetTabSize();
			for(int i = 0; i < ixln.x; i++) {
				if(ln[i] == '\t' || ln[i] == ' ') {
					if(pos == 0)
						startindex = i;
					if(ln[i] == '\t' || ++pos >= tabsz)
						pos = 0;
				}
				else {
					white = false;
					break;
				}
			}
			if(white && startindex >= 0) {
				int count = ixln.x - startindex;
				PlaceCaret(c - count);
				Remove(c - count, count);
				Action();
				return true;
			}
		}
		break;
	case K_SHIFT_CTRL_TAB:
		return LineEdit::Key(K_TAB, count);
	case K_ENTER:
		IndentEnter(count);
		return true;
	}
	bool sel = code & K_SHIFT;
	switch(code & ~K_SHIFT) {
	case K_F3:
		Find(sel);
		return true;
	case K_CTRL_F3:
		FindWord(sel);
		return true;
	case K_CTRL_RIGHT:
		MoveNextWord(sel);
		return true;
	case K_CTRL_LEFT:
		MovePrevWord(sel);
		return true;
	case K_CTRL_RBRACKET:
		MoveNextBrk(sel);
		return true;
	case K_CTRL_LBRACKET:
		MovePrevBrk(sel);
		return true;
	case K_TAB:
		if(!IsReadOnly()) {
			if(IsSelection()) {
				if(sel)
					TabLeft();
				else
					TabRight();
				return true;
			}
			if(!sel && indent_spaces) {
				int x = GetColumnLine(GetCursor()).x;
				int add = GetTabSize() - x % GetTabSize();
				InsertChar(' ', add, false);
				return true;
			}
		}
	default:
		if(IsSelection() && auto_enclose) {
			if(code == '(') {
				Enclose("(", ")");
				return true;
			}
			if(code == '{') {
				Enclose("{", "}");
				return true;
			}
			if(code == '\"') {
				Enclose("\"", "\"");
				return true;
			}
			if(code == '[') {
				Enclose("[", "]");
				return true;
			}
			if(code == '/') {
				//Enclose("/*", "*/");
				ToggleLineComments(false);
				return true;
			}
			if(code == K_CTRL_SLASH)
			{
				ToggleLineComments(true);
				return true;
			}
			if(code == '*') {
				//Enclose("/*", "*/");
				ToggleStarComments();
				return true;
			}
		}
		if(code >= 32 && code < 128 && count == 1) {
			IndentInsert(code);
			return true;
		}
		if(code == 9 && IsSelection())
			return true;
	}
	if(GetCharset() != CHARSET_UTF8)
		if(code >= 128 && code < 65536 && FromUnicode((wchar)code, GetCharset()) == DEFAULTCHAR)
			return true;
	return LineEdit::Key(code, count);
}

void CodeEditor::ForwardWhenBreakpoint(int i) {
	WhenBreakpoint(i);
}

void CodeEditor::GotoLine(int line)
{
	SetCursor(GetPos(GetLineNo(line)));
}

void CodeEditor::Serialize(Stream& s) {
	int version = 0;
	s / version;
	SerializeFind(s);
}

void CodeEditor::SetLineInfo(const LineInfo& lf)
{
	bar.SetLineInfo(lf, GetLineCount());
}

void CodeEditor::DefaultHlStyles()
{
	SetHlStyle(INK_COMMENT, Green, false, true);
	SetHlStyle(INK_CONST_STRING, Red);

	SetHlStyle(INK_CONST_STRINGOP, LtBlue);
	SetHlStyle(INK_CONST_INT, Red);
	SetHlStyle(INK_CONST_INT_3, Red, true, false, false);
	SetHlStyle(INK_CONST_FLOAT, Magenta);
	SetHlStyle(INK_CONST_FLOAT_3, Magenta, true, false, false);
	SetHlStyle(INK_CONST_HEX, Blue);
	SetHlStyle(INK_CONST_OCT, Blue);

	SetHlStyle(INK_OPERATOR, LtBlue);
	SetHlStyle(INK_KEYWORD, LtBlue, true);
	SetHlStyle(INK_UPP, Cyan);
	SetHlStyle(PAPER_LNG, Color(255, 255, 224));
	SetHlStyle(INK_ERROR, LtRed);
	SetHlStyle(INK_PAR0, Black);
	SetHlStyle(INK_PAR1, Green);
	SetHlStyle(INK_PAR2, Magenta);
	SetHlStyle(INK_PAR3, Brown);

	SetHlStyle(INK_UPPER, Black);
	SetHlStyle(INK_SQLBASE, Black);
	SetHlStyle(INK_SQLFUNC, Black);
	SetHlStyle(INK_SQLBOOL, Black);
	SetHlStyle(INK_UPPMACROS, Cyan);
	SetHlStyle(INK_UPPLOGS, Green);

	SetHlStyle(PAPER_BLOCK1, Blend(LtBlue, White, 240));
	SetHlStyle(PAPER_BLOCK2, Blend(LtGreen, White, 240));
	SetHlStyle(PAPER_BLOCK3, Blend(LtYellow, White, 240));
	SetHlStyle(PAPER_BLOCK4, Blend(LtMagenta, White, 240));

	SetHlStyle(INK_MACRO, Magenta);
	SetHlStyle(PAPER_MACRO, Color(255, 255, 230));
	SetHlStyle(PAPER_IFDEF, Color(230, 255, 255));
	SetHlStyle(INK_IFDEF, Color(170, 170, 170));

	SetHlStyle(PAPER_BRACKET0, LtYellow);
	SetHlStyle(PAPER_BRACKET, Yellow, true);

	SetHlStyle(INK_NORMAL, SColorText);
	SetHlStyle(INK_DISABLED, SColorDisabled);
	SetHlStyle(INK_SELECTED, SColorHighlightText);
	SetHlStyle(PAPER_NORMAL, SColorPaper);
	SetHlStyle(PAPER_READONLY, SColorFace);
	SetHlStyle(PAPER_SELECTED, SColorHighlight);
}


#define HL_COLOR(x, a, b)    #x,
static const char *s_hl_color[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

#define HL_COLOR(a, x, b)    x,
static const char *s_hl_name[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

#define HL_COLOR(a, b, x)    x,
static bool s_hl_font[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

const CodeEditor::HlStyle& CodeEditor::GetHlStyle(int i)
{
	return hl_style[i];
}

const char *CodeEditor::GetHlName(int i)
{
	return s_hl_name[i];
}

bool CodeEditor::HasHlFont(int i)
{
	return s_hl_font[i];
}

void  CodeEditor::SetHlStyle(int i, Color c, bool bold, bool italic, bool underline)
{
	HlStyle& st = hl_style[i];
	st.color = c;
	st.bold = bold;
	st.italic = italic;
	st.underline = underline;
	SetColor(LineEdit::INK_NORMAL, hl_style[INK_NORMAL].color);
	SetColor(LineEdit::INK_DISABLED, hl_style[INK_DISABLED].color);
	SetColor(LineEdit::INK_SELECTED, hl_style[INK_SELECTED].color);
	SetColor(LineEdit::PAPER_NORMAL, hl_style[PAPER_NORMAL].color);
	SetColor(LineEdit::PAPER_READONLY, hl_style[PAPER_READONLY].color);
	SetColor(LineEdit::PAPER_SELECTED, hl_style[PAPER_SELECTED].color);
}

void CodeEditor::LoadHlStyles(const char *s)
{
	CParser p(s);
	try {
		while(!p.IsEof()) {
			String id = p.ReadId();
			Color c = ReadColor(p);
			bool bold = false;
			bool italic = false;
			bool underline = false;
			for(;;)
				if(p.Id("bold"))
					bold = true;
				else
				if(p.Id("italic"))
					italic = true;
				else
				if(p.Id("underline"))
					underline = true;
				else
					break;
			for(int i = 0; i < HL_COUNT; i++)
				if(id == s_hl_color[i]) {
					SetHlStyle(i, c, bold, italic, underline);
					break;
				}
			p.PassChar(';');
		}
	}
	catch(CParser::Error) {
		DefaultHlStyles();
	}
}

String CodeEditor::StoreHlStyles()
{
	String r;
	for(int i = 0; i < HL_COUNT; i++) {
		const HlStyle& s = GetHlStyle(i);
		r << Format("%-25s", s_hl_color[i]) << ' ' << FormatColor(s.color);
		if(s.bold)
			r << " bold";
		if(s.italic)
			r << " italic";
		if(s.underline)
			r << " underline";
		r << ";\r\n";
	}
	return r;
}

void CodeEditor::PutI(WithDropChoice<EditString>& edit)
{
	edit.AddButton().SetMonoImage(CodeEditorImg::I()).Tip("Set word/selection (Ctrl+I)")
	    <<= THISBACK1(SetI, &edit);
}

CodeEditor::CodeEditor() {
	bracket_flash = false;
	highlight_bracket_pos0 = 0;
	bracket_start = 0;
	stat_edit_time = 0;
	last_key_time = Null;
	CtrlLayout(findreplace);
	AddFrame(bar);
	bar.SetEditor(this);
	UndoSteps(10000);
	SetFont(Courier(16));

	findreplace.find.AddButton().SetMonoImage(CtrlImg::smallright()).Tip("Wildcard")
		<<= THISBACK(FindWildcard);
	findreplace.replace.AddButton().SetMonoImage(CtrlImg::smallright()).Tip("Wildcard")
		<<= THISBACK(ReplaceWildcard);
	PutI(findreplace.find);
	PutI(findreplace.replace);
	findreplace.amend <<= THISBACK(Replace);
	findreplace.cancel.Cancel();
	findreplace.ok.Ok();
	findreplace.findback <<= THISBACK(DoFindBack);
	findreplace.ToolWindow();
	findreplace.replacing = false;
	found = notfoundfw = notfoundbk = foundsel = false;

	bar.WhenBreakpoint = THISBACK(ForwardWhenBreakpoint);
	bar.WhenAnnotationMove = Proxy(WhenAnnotationMove);
	bar.WhenAnnotationClick = Proxy(WhenAnnotationClick);
	bar.WhenAnnotationRightClick = Proxy(WhenAnnotationRightClick);
	highlight = HIGHLIGHT_NONE;
	hilite_scope = 0;
	hilite_bracket = 1;
	hilite_ifdef = 1;
	barline = true;
	indent_spaces = false;
	indent_amount = GetTabSize();
	no_parenthesis_indent = false;
	sb.WithSizeGrip();
	DefaultHlStyles();
	sb.y.NoAutoHide();
	sb.y.AddFrame(topsbbutton);
	sb.y.AddFrame(topsbbutton1);
	topsbbutton.Hide();
	topsbbutton1.Hide();
	highlight_bracket_pos = 10;
	SetTimeCallback(-20, THISBACK(Periodic), TIMEID_PERIODIC);
	auto_enclose = false;
	mark_lines = true;
	check_edited = false;
	tippos = -1;
	persistent_find_replace =  false;
}

CodeEditor::~CodeEditor() {}

END_UPP_NAMESPACE

