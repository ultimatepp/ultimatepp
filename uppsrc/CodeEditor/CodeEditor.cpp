#include "CodeEditor.h"

NAMESPACE_UPP

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

#define IMAGEVECTOR Vector
#define IMAGECLASS  CodeEditorImg
#define IMAGEFILE   <CodeEditor/CodeEditor.iml>
#include <Draw/iml_source.h>

#define  TFILE <CodeEditor/CodeEditor.t>
#include <Core/t.h>

One<EditorSyntax> CodeEditor::GetSyntax(int line)
{
	CTIMING("GetSyntax");
	One<EditorSyntax> syntax = EditorSyntax::Create(highlight);
	int ln = 0;
	for(int i = 0; i < __countof(syntax_cache); i++)
		if(line >= syntax_cache[i].line && syntax_cache[i].line > 0) {
			syntax->Set(syntax_cache[i].data);
			ln = syntax_cache[i].line;
			LLOG("Found " << line << " " << syntax_cache[i].line);
			break;
		}
	line = min(line, GetLineCount());
	if(line - ln > 10000) { // optimization hack for huge files
		syntax = EditorSyntax::Create(highlight);
		ln = line - 10000;
	}
	while(ln < line) {
		WString l = GetWLine(ln);
		CTIMING("ScanSyntax3");
		syntax->ScanSyntax(l, l.End(), ln, GetTabSize());
		ln++;
		static int d[] = { 0, 100, 2000, 10000, 50000 };
		for(int i = 0; i < __countof(d); i++)
			if(ln == cline - d[i]) {
				syntax_cache[i + 1].data = syntax->Get();
				syntax_cache[i + 1].line = ln;
			}
	}
	syntax_cache[0].data = syntax->Get();
	syntax_cache[0].line = ln;
	return pick(syntax);
}

void CodeEditor::Highlight(const String& h)
{
	highlight = h;
	SetColor(LineEdit::INK_NORMAL, hl_style[HighlightSetup::INK_NORMAL].color);
	SetColor(LineEdit::INK_DISABLED, hl_style[HighlightSetup::INK_DISABLED].color);
	SetColor(LineEdit::INK_SELECTED, hl_style[HighlightSetup::INK_SELECTED].color);
	SetColor(LineEdit::PAPER_NORMAL, hl_style[HighlightSetup::PAPER_NORMAL].color);
	SetColor(LineEdit::PAPER_READONLY, hl_style[HighlightSetup::PAPER_READONLY].color);
	SetColor(LineEdit::PAPER_SELECTED, hl_style[HighlightSetup::PAPER_SELECTED].color);
	Refresh();
	EditorBarLayout();
}

void CodeEditor::DirtyFrom(int line) {
	for(int i = 0; i < __countof(syntax_cache); i++)
		if(syntax_cache[i].line >= line)
			syntax_cache[i].Clear();

	if(check_edited) {
		bar.ClearErrors(line);
		bar.Refresh();
	}
}

inline bool IsComment(int a, int b) {
	return a == '/' && b == '*' || a == '*' && b == '/' || a == '/' && b == '/';
}

void CodeEditor::PreInsert(int pos, const WString& text)
{
}

inline bool RBR(int c) {
	return isbrkt(c);
}

void CodeEditor::CheckSyntaxRefresh(int pos, const WString& text)
{
	GetSyntax(GetLine(pos))->CheckSyntaxRefresh(*this, pos, text);
}

void CodeEditor::PostInsert(int pos, const WString& text) {
	if(check_edited)
		bar.SetEdited(GetLine(pos));
	if(!IsFullRefresh()) {
		if(text.GetCount() > 200 || text.Find('\n') >= 0)
			Refresh();
		else
			CheckSyntaxRefresh(pos, text);
	}
	WhenUpdate();
	EditorBarLayout();
}

void CodeEditor::PreRemove(int pos, int size) {
	if(IsFullRefresh()) return;
	if(size > 200)
		Refresh();
	else {
		WString text = GetW(pos, size);
		if(text.Find('\n') >= 0)
			Refresh();
		else
			CheckSyntaxRefresh(pos, text);
	}
}

void CodeEditor::PostRemove(int pos, int size) {
	if(check_edited)
		bar.SetEdited(GetLine(pos));
	WhenUpdate();
	EditorBarLayout();
}

void CodeEditor::ClearLines() {
	bar.ClearLines();
}

void CodeEditor::InsertLines(int line, int count) {
	bar.InsertLines(line, count);
	if(line <= line2.GetCount())
		line2.Insert(line, GetLine2(line), count);
	EditorBarLayout();
}

void CodeEditor::RemoveLines(int line, int count) {
	bar.RemoveLines(line, count);
	if(line + count <= line2.GetCount())
		line2.Remove(line, count);
	EditorBarLayout();
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

String CodeEditor::GetPasteText()
{
	String h;
	WhenPaste(h);
	return h;
}


bool CodeEditor::IsCursorBracket(int pos) const
{
	return pos == highlight_bracket_pos0 && hilite_bracket;
}

bool CodeEditor::IsMatchingBracket(int pos) const
{
	return pos == highlight_bracket_pos && (hilite_bracket == 1 || hilite_bracket == 2 && bracket_flash);
}

void CodeEditor::CheckBrackets()
{
	CancelBracketHighlight(highlight_bracket_pos0);
	CancelBracketHighlight(highlight_bracket_pos);
	if(!IsSelection()) {
		if(GetSyntax(GetCursorLine())->CheckBrackets(*this, highlight_bracket_pos0, highlight_bracket_pos)) {
			RefreshLine(GetLine(highlight_bracket_pos0));
			RefreshLine(GetLine(highlight_bracket_pos));
			bracket_start = GetTimeClick();
		}
	}
	WhenSelection();
}

void CodeEditor::CopyWord() {
	int p = GetCursor();
	if(iscidl(GetChar(p)) || (p > 0 && iscidl(GetChar(--p)))) {
		int e = GetLength();
		int f = p;
		while(--p >= 0 && iscidl(GetChar(p))) {}
		++p;
		while(++f < e && iscidl(GetChar(f)));
		WString txt = GetW(p, f - p);
		WriteClipboardUnicodeText(txt);
		AppendClipboardText(txt.ToString());
	}
}

void CodeEditor::DuplicateLine()
{
	int i = GetLine(cursor);
	int pos = GetPos(i);
	int len = GetLineLength(i);
	Insert(pos + len, "\n" + GetW(pos, len));
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
	if(b != bracket_flash && EditorSyntax::hilite_bracket == 2) {
		bracket_flash = b;
		if(highlight_bracket_pos0 >= 0)
			RefreshLine(GetLine(highlight_bracket_pos0));
		if(highlight_bracket_pos >= 0)
			RefreshLine(GetLine(highlight_bracket_pos));
	}
}

void CodeEditor::SelectionChanged()
{
	int l, h;
	WString nselword;
	bool sel = GetSelection(l, h);
	if(sel && h - l < 128 &&
	   (l == 0 || !iscid(GetChar(l - 1))) &&
	   (h >= GetLength() || !iscid(GetChar(h)))) {
		for(int i = l; i < h; i++) {
			int c = GetChar(i);
			if(!iscid(c)) {
				nselword.Clear();
				break;
			}
			nselword.Cat(c);
		}
	}
	if(selword != nselword) {
		selword = nselword;
		Refresh();
	}
	if(!foundsel) {
		if(!persistent_find_replace)
			CloseFindReplace();
		found = false;
		notfoundfw = notfoundbk = false;
		findreplace.amend.Disable();
	}
	CheckBrackets();
	bar.Refresh();
}

bool CodeEditor::InsertRS(int chr, int count) {
	if(IsReadOnly()) return true;
	if(IsSelection()) {
		InsertChar(chr, count);
		return true;
	}
	return false;
}

void CodeEditor::IndentInsert(int chr, int count) {
	if(InsertRS(chr)) return;
	One<EditorSyntax> s = GetSyntax(GetCursorLine());
	if(s)
		s->IndentInsert(*this, chr, count);
	else
		InsertChar(chr, count);
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
	if(!isrbrkt(GetChar(p - 1))) {
		if(p < GetLength() - 1 && isrbrkt(GetChar(p)))
			p++;
		else {
			while(p > 0 && !isrbrkt(GetChar(p - 1))) p--;
			PlaceCaret(p, sel);
			return;
		}
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
	CloseFindReplace();
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

//void CodeEditor::FindWord(bool back)
//{
//	WString I = GetI();
//	if(!IsNull(I))
//		Find(back, I, true, false, false, false, false);
//}

void CodeEditor::SetI(Ctrl *edit)
{
	*edit <<= GetI();
}

void CodeEditor::Goto() {
	String line = AsString(GetCursorLine());
	if(EditText(line, t_("Go to"), t_("Line:")))
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
	if(code == replace_key) {
		Replace();
		return true;
	}
	switch(code) {
	case K_CTRL_DELETE:
		DeleteWord();
		return true;
	case K_CTRL_BACKSPACE:
		DeleteWordBack();
		return true;
	case K_BACKSPACE:
		if(!IsReadOnly() && !IsAnySelection() && indent_spaces) {
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
		IndentInsert('\n', count);
		return true;
	}
	bool sel = code & K_SHIFT;
	switch(code & ~K_SHIFT) {
	case K_F3:
		if(sel)
			FindPrev();
		else
			FindNext();
		return true;
//	case K_CTRL_F3:
//		FindWord(sel);
//		return true;
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
			IndentInsert(code, 1);
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

void CodeEditor::HighlightLine(int line, Vector<LineEdit::Highlight>& hl, int pos)
{
	CTIMING("HighlightLine");
	HighlightOutput hls(hl);
	WString l = GetWLine(line);
	GetSyntax(line)->Highlight(l.Begin(), l.End(), hls, this, line, pos);
	if(selword.GetCount()) {
		int q = 0;
		for(;;) {
			q = l.Find(selword, q);
			if(q < 0)
				break;
			int h = q + selword.GetCount();
			if((q == 0 || !iscid(l[q - 1])) && (h >= l.GetCount() || !iscid(l[h])))
				for(int i = 0; i < selword.GetCount() && i + q < hl.GetCount(); i++) {
					const HlStyle& st = hl_style[PAPER_SELWORD];
					hl[i + q].paper = st.color;
					if(st.bold)
						hl[i + q].font.Bold();
				}
			q = h;
		}
	}
}

void CodeEditor::PutI(WithDropChoice<EditString>& edit)
{
	edit.AddButton().SetMonoImage(CodeEditorImg::I()).Tip(t_("Set word/selection (Ctrl+I)"))
	    <<= THISBACK1(SetI, &edit);
}

CodeEditor::CodeEditor() {
	bracket_flash = false;
	highlight_bracket_pos0 = 0;
	bracket_start = 0;
	stat_edit_time = 0;
	last_key_time = Null;
	SetFont(Courier(16));
	AddFrame(bar);
	bar.SetEditor(this);
	UndoSteps(10000);
	InitFindReplace();
	bar.WhenBreakpoint = THISBACK(ForwardWhenBreakpoint);
	bar.WhenAnnotationMove = Proxy(WhenAnnotationMove);
	bar.WhenAnnotationClick = Proxy(WhenAnnotationClick);
	bar.WhenAnnotationRightClick = Proxy(WhenAnnotationRightClick);
	barline = true;
	sb.WithSizeGrip();
	DefaultHlStyles();
	Highlight(Null);
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
}

CodeEditor::~CodeEditor() {}

END_UPP_NAMESPACE
