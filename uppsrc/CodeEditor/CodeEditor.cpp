#include "CodeEditor.h"

namespace Upp {

#define LLOG(x)    // DLOG(x)
#define LTIMING(x) // RTIMING(x)

#define IMAGEVECTOR Vector
#define IMAGECLASS  CodeEditorImg
#define IMAGEFILE   <CodeEditor/CodeEditor.iml>
#include <Draw/iml_source.h>

#define  TFILE <CodeEditor/CodeEditor.t>
#include <Core/t.h>

void RegisterSyntaxModules();

INITBLOCK {
	RegisterSyntaxModules();
}

One<EditorSyntax> CodeEditor::GetSyntax(int line)
{
	LTIMING("GetSyntax");
	One<EditorSyntax> syntax = EditorSyntax::Create(highlight);
	syntax->SpellCheckComments(spellcheck_comments);
	int ln = 0;
	for(int i = 0; i < __countof(syntax_cache); i++)
		if(line >= syntax_cache[i].line && syntax_cache[i].line > 0) {
			syntax->Set(syntax_cache[i].data);
			ln = syntax_cache[i].line;
			LLOG("Found " << line << " " << syntax_cache[i].line);
			break;
		}
	line = min(line, GetLineCount());
	if(line - ln > 10000) { // optimization hack for very huge files
		syntax = EditorSyntax::Create(highlight);
		ln = line - 10000; // we just pray it gets synced
	}
	while(ln < line) {
		WString l = GetWLine(ln);
		CTIMING("ScanSyntax3");
		syntax->ScanSyntax(l, l.End(), ln, GetTabSize());
		ln++;
		static int d[] = { 0, 100, 1000, 3000, 7000 };
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
	SetColor(LineEdit::WHITESPACE, hl_style[HighlightSetup::WHITESPACE].color);
	SetColor(LineEdit::WARN_WHITESPACE, hl_style[HighlightSetup::WARN_WHITESPACE].color);
	Refresh();
	EditorBarLayout();
}

void CodeEditor::HighlightLine(int line, Vector<LineEdit::Highlight>& hl, int64 pos)
{
	CTIMING("HighlightLine");
	HighlightOutput hls(hl);
	WString l = GetWLine(line);
	GetSyntax(line)->Highlight(l.Begin(), l.End(), hls, this, line, pos);
	if(illuminated.GetCount()) {
		int q = 0;
		while(q < l.GetCount() && q < hl.GetCount()) {
			q = l.Find(illuminated, q);
			if(q < 0)
				break;
			int n = illuminated.GetCount();
			if(n > 1 || !iscid(illuminated[0]) ||
			   (q == 0 || !iscid(l[q - 1])) && (q + n >= l.GetCount() || !iscid(l[q + n])))
				while(n-- && q < hl.GetCount()) {
					const HlStyle& st = hl_style[PAPER_SELWORD];
					hl[q].paper = st.color;
					if(st.bold)
						hl[q].font.Bold();
					q++;
				}
			else
				q++;
		}
	}
	for(Point p : errors)
		if(p.y == line && p.x < hl.GetCount()) {
			hl[p.x].paper = hl_style[PAPER_ERROR_FILE].color;
			hl[p.x].flags |= LineEdit::NOENDFILL;
		}
}

void CodeEditor::Paint(Draw& w)
{
	LineEdit::Paint(w);

	if(!blk0_header)
		return;

	int sline = GetColumnLine(GetCursor()).y - GetScrollPos().y;
	if(sline >= 0 && sline < 2)
		return;
	Point start, end; // paint zero level block header
	if(!GetSyntax(GetScrollPos().y + 1)->GetBlockHeader(start, end))
		return;

	if(IsNull(start) || IsNull(end) || end.y > start.y)
		return;

	int line0 = -1;
	for(int i = end.y + 1; i <= start.y; i++) {
		String l = GetUtf8Line(i);
		bool hdr = false;
		bool empty = true;
		try {
			CParser p(l);
			while(!p.IsEof()) {
				if(p.Char('(') || p.Id("class") || p.Id("struct") || p.Id("enum")) {
					hdr = true;
					break;
				}
				p.Skip();
			}
		}
		catch(CParser::Error) {}
		if(hdr) {
			line0 = i;
			break;
		}
		if(!empty && line0 < 0)
			line0 = i;
	}

	if(line0 < 0)
		return;

	int l = GetPos(line0, 0);
	int h = GetPos(start.y, start.x) - 1;
	if(l < 0 || l >= h)
		return;

	Size sz = GetSize();

	bool dark = IsDark(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_NORMAL).color);
	Color paper = GrayColor(dark ? 40 : 245);

	w.DrawRect(0, 0, GetSize().cx, GetFontSize().cy, paper);
	
	Size hsz = CodeEditorImg::HdrSep0().GetSize();
	for(int x = 0; x < sz.cx; x += hsz.cx) {
		w.DrawImage(x, GetFontSize().cy, CodeEditorImg::HdrSep0(), paper);
		w.DrawImage(x, GetFontSize().cy, CodeEditorImg::HdrSep(), dark ? White() : Black());
	}
	
	String ls = TrimBoth(Get(l, h - l));
	WString r;
	for(const char *s = ls; *s;) {
		if(IsSpace(*s)) {
			while(IsSpace(*s))
				s++;
			r.Cat(' ');
		}
		else
			r.Cat(*s++);
	}
	r << " {";
	
	Vector<LineEdit::Highlight> hln;
	hln.SetCount(r.GetCount() + 1);
	for(int i = 0; i < hln.GetCount(); i++) {
		LineEdit::Highlight& h = hln[i];
		h.paper = paper;
		h.ink = SColorText();
		h.chr = r[i];
		h.font = GetFont();
	}
	
	HighlightOutput hls(hln);
	GetSyntax(0)->Highlight(r.Begin(), r.End(), hls, this, 0, 0);

	int ii = 0;
	int x = 0;
	while(ii < hln.GetCount() - 1) {
		int n = 1;
		while(ii + n < hln.GetCount() - 1 && hln[ii + n].ink == hln[ii].ink && hln[ii + n].font == hln[ii].font)
			n++;
		w.DrawText(x, 0, ~r + ii, hln[ii].font, hln[ii].ink, n);
		x += GetTextSize(~r + ii, hln[ii].font, n).cx;
		ii += n;
	}
}

void CodeEditor::RefreshBlkHeader()
{
	if(blk0_header)
		Refresh(0, 0, GetSize().cx, GetFontSize().cy + CodeEditorImg::HdrSep().GetSize().cy);
}

void CodeEditor::NewScrollPos() {
	bar.Scroll();
	RefreshBlkHeader();
}

void CodeEditor::DirtyFrom(int line) {
	for(int i = 0; i < __countof(syntax_cache); i++)
		if(syntax_cache[i].line >= line)
			syntax_cache[i].Clear();

	if(check_edited)
		bar.Refresh();
}

inline bool IsComment(int a, int b) {
	return a == '/' && b == '*' || a == '*' && b == '/' || a == '/' && b == '/';
}

inline bool RBR(int c) {
	return isbrkt(c);
}

String CodeEditor::GetRefreshInfo(int pos)
{
	int ii = GetLine(pos) + 1;
	return ii < GetLineCount() ? GetSyntax(ii)->Get() : String();
}

void CodeEditor::CheckSyntaxRefresh(int64 pos, const WString& text)
{
	GetSyntax(GetLine(pos))->CheckSyntaxRefresh(*this, pos, text);
}

void CodeEditor::PreInsert(int pos, const WString& text)
{
	refresh_info = GetRefreshInfo(pos);
}

void CodeEditor::PostInsert(int pos, const WString& text) {
	if(check_edited)
		bar.SetEdited(GetLine(pos));
	if(!IsFullRefresh()) {
		if(text.GetCount() > 200 || GetRefreshInfo(pos) != refresh_info || text.Find('\n') >= 0)
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
		else {
			CheckSyntaxRefresh(pos, text);
			refresh_info = GetRefreshInfo(pos);
		}
	}
	sbi.Refresh();
}

void CodeEditor::PostRemove(int pos, int size) {
	if(check_edited)
		bar.SetEdited(GetLine(pos));
	WhenUpdate();
	EditorBarLayout();
	if(GetRefreshInfo(pos) != refresh_info)
		Refresh();
	sbi.Refresh();
}

void CodeEditor::ClearLines() {
	bar.ClearLines();
	sbi.Refresh();
}

void CodeEditor::InsertLines(int line, int count) {
	if(IsView())
		return;
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
	if(IsView())
		return;
	line2.SetCount(GetLineCount());
	for(int i = 0; i < GetLineCount(); i++)
		line2[i] = i;
}

int CodeEditor::GetLine2(int i) const
{
	return line2.GetCount() ? line2[min(line2.GetCount() - 1, i)] : 0;
}

String CodeEditor::GetPasteText()
{
	String h;
	WhenPaste(h);
	return h;
}

bool CodeEditor::IsCursorBracket(int64 pos) const
{
	return pos == highlight_bracket_pos0 && hilite_bracket;
}

bool CodeEditor::IsMatchingBracket(int64 pos) const
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
	int64 p = GetCursor64();
	if(iscidl(GetChar(p)) || (p > 0 && iscidl(GetChar(--p)))) {
		int64 e = GetLength64();
		int64 f = p;
		while(--p >= 0 && iscidl(GetChar(p))) {}
		++p;
		while(++f < e && iscidl(GetChar(f)));
		WString txt = GetW(p, LimitSize(f - p));
		WriteClipboardUnicodeText(txt);
		AppendClipboardText(txt.ToString());
	}
}

void CodeEditor::DuplicateLine()
{
	if(IsReadOnly()) return;
	int i = GetLine(cursor);
	int pos = GetPos32(i);
	int len = GetLineLength(i);
	Insert(pos + len, "\n" + GetW(pos, len));
}

void CodeEditor::SwapChars() {
	if(IsReadOnly()) return;
	int i = GetLine(cursor);
	int j = GetPos32(i);
	if (j < cursor && cursor - j < GetLineLength(i)) {
		int p = (int)cursor;
		WString txt(GetChar(p-1),1);
		Remove(p-1,1);
		Insert(p, txt, true);
		PlaceCaret(p);
	}
}

void CodeEditor::Put(int chr)
{
	Insert((int)cursor++, WString(chr, 1), true);
}

void CodeEditor::FinishPut()
{
	PlaceCaret(cursor);
	Action();
}

void CodeEditor::ReformatComment()
{
	if(IsReadOnly()) return;
	NextUndo();
	GetSyntax(GetLine(cursor))->ReformatComment(*this);
}

void CodeEditor::CancelBracketHighlight(int64& pos)
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
	int64 l, h;
	WString nilluminated;
	bool sel = GetSelection(l, h);
	bool ill = false;
	if(sel && h - l < 128) {
		for(int64 i = l; i < h; i++) {
			int c = GetChar(i);
			if(c == '\n') {
				nilluminated.Clear();
				break;
			}
			if(!IsSpace(c))
				ill = true;
			nilluminated.Cat(c);
		}
	}
	if(!ill)
		nilluminated.Clear();
	if(illuminated != nilluminated) {
		illuminated = nilluminated;
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
	RefreshBlkHeader();
}

void CodeEditor::Illuminate(const WString& text)
{
	if(illuminated != text) {
		illuminated = text;
		Refresh();
	}
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
		InsertChar(chr, count, true);
}

void CodeEditor::Make(Event<String&> op)
{
	if(IsReadOnly()) return;
	Point cursor = GetColumnLine(GetCursor32());
	Point scroll = GetScrollPos();
	int l, h;
	bool is_sel = GetSelection32(l, h);
	if(!is_sel) { l = 0; h = GetLength32(); }
	if(h <= l)
	{
		BeepExclamation();
		return;
	}
	l = GetPos32(GetLine(l));
	h = GetPos32(GetLine(h - 1) + 1);
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

void CodeEditor::ConvertToOverrides()
{
	Make([](String& out) {
		out.Insert(0, " ");
		out.Replace("\tvirtual\t", "\t");
		out.Replace("\tvirtual ", "\t");
		out.Replace(" virtual\t", " ");
		out.Replace(" virtual ", " ");
		out.Replace(";", " override;");
		out.Remove(0, 1);
	});
}

void CodeEditor::MoveNextWord(bool sel) {
	int64 p = GetCursor64();
	int64 e = GetLength64();
	if(iscidl(GetChar(p)))
		while(p < e && iscidl(GetChar(p))) p++;
	else
		while(p < e && !iscidl(GetChar(p))) p++;
	PlaceCaret(p, sel);
}

void CodeEditor::MovePrevWord(bool sel) {
	int64 p = GetCursor64();
	if(p == 0) return;
	if(iscidl(GetChar(p - 1)))
		while(p > 0 && iscidl(GetChar(p - 1))) p--;
	else
		while(p > 0 && !iscidl(GetChar(p - 1))) p--;
	PlaceCaret(p, sel);
}

void CodeEditor::MoveNextBrk(bool sel) {
	int64 p = GetCursor64();
	int64 e = GetLength64();
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
	int64 p = GetCursor64();
	if(p < 2) return;
	if(!isrbrkt(GetChar(p - 1))) {
		if(p < GetLength64() - 1 && isrbrkt(GetChar(p)))
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
	int p = GetCursor32();
	int e = GetLength32();
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
	Remove(GetCursor32(), p - GetCursor32());
}

void CodeEditor::DeleteWordBack() {
	if(IsReadOnly() || RemoveSelection()) return;
	int p = GetCursor32();
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
	Remove(p, GetCursor32() - p);
	PlaceCaret(p);
}

void CodeEditor::SetLineSelection(int l, int h) {
	SetSelection(GetPos64(l), GetPos64(h));
}

bool CodeEditor::GetLineSelection(int& l, int& h) {
	int64 ll, hh;
	if(!GetSelection(ll, hh)) return false;
	l = GetLine(ll);
	h = GetLinePos64(hh);
	if(hh && h < GetLineCount()) h++;
	SetLineSelection(l, h);
	return true;
}

void CodeEditor::SwapUpDown(bool up)
{
	int l, h;
	if(GetSelection(l, h)) {
		l = GetLine(l);
		int hh = h;
		h = GetLinePos(hh);
		if(hh && h < GetLineCount()) h++;
	}
	else {
		l = GetLine(cursor);
		h = l + 1;
	}
	if(up) {
		if(l == 0)
			return;
		Insert(GetPos(h), GetWLine(l - 1) + "\n");
		Remove(GetPos(l - 1), GetLineLength(l - 1) + 1);
		SetSelection(GetPos(l - 1), GetPos(h - 1));
	}
	else {
		if(h >= GetLineCount() - 1)
			return;
		WString line = GetWLine(h) + "\n";
		Remove(GetPos(h), GetLineLength(h) + 1);
		Insert(GetPos(l), line);
		SetSelection(GetPos(l + 1), GetPos(h + 1));
	}
}

void CodeEditor::TabRight() {
	if(IsReadOnly()) return;
	int l, h;
	if(!GetLineSelection(l, h)) return;
	int ll = l;
	String tab(indent_spaces ? ' ' : '\t', indent_spaces ? GetTabSize() : 1);
	while(l < h)
		Insert(GetPos32(l++), tab);
	SetLineSelection(ll, h);
}

void CodeEditor::TabLeft() {
	if(IsReadOnly()) return;
	int l, h;
	if(!GetLineSelection(l, h)) return;
	int ll = l;
	while(l < h) {
		WString ln = GetWLine(l);
		int spc = 0;
		while(spc < tabsize && ln[spc] == ' ') spc++;
		if(spc < tabsize && ln[spc] == '\t') spc++;
		Remove(GetPos32(l++), spc);
	}
	SetLineSelection(ll, h);
}

bool CodeEditor::GetWordPos(int64 pos, int64& l, int64& h) {
	l = h = pos;
	if(!iscidl(GetChar(pos))) return false;
	while(l > 0 && iscidl(GetChar(l - 1))) l--;
	while(iscidl(GetChar(h))) h++;
	return true;
}

String CodeEditor::GetWord(int64 pos)
{
	int64 l, h;
	GetWordPos(pos, l, h);
	return Get(l, LimitSize(h - l));
}

String CodeEditor::GetWord()
{
	return GetWord(cursor);
}

void CodeEditor::LeftDouble(Point p, dword keyflags) {
	int64 l, h;
	int64 pos = GetMousePos(p);
	if(GetWordPos(pos, l, h))
		SetSelection(l, h);
	else
		SetSelection(pos, pos + 1);
	selkind = SEL_WORDS;
	SetFocus();
	SetCapture();
}

void CodeEditor::LeftTriple(Point p, dword keyflags)
{
	LineEdit::LeftTriple(p, keyflags);
	selkind = SEL_LINES;
	SetFocus();
	SetCapture();
}

void CodeEditor::LeftDown(Point p, dword keyflags) {
	if((keyflags & K_CTRL) && WhenCtrlClick) {
		WhenCtrlClick(GetMousePos(p));
		return;
	}
	LineEdit::LeftDown(p, keyflags);
	WhenLeftDown();
	CloseFindReplace();
	selkind = SEL_CHARS;
}

void CodeEditor::Tip::Paint(Draw& w)
{
	Rect r = GetSize();
	w.DrawRect(r, background);
	r.left++;
	if(d)
		d->Paint(w, r, v, SColorText(), background, 0);
}

CodeEditor::Tip::Tip()
{
	SetFrame(BlackFrame());
	BackPaint();
}

bool CodeEditor::SyncCloseTip()
{
	if(HasMouse() && IsVisible() && GetTopCtrl()->HasFocusDeep())
		return false;
	CloseTip();
	return true;
}

void CodeEditor::SyncTip()
{
	if(SyncCloseTip())
		return;
	Rect wa = GetWorkArea();
	Point p = Upp::GetMousePos();
	MouseTip mt;
	mt.background = Blend(SWhite(), SLtYellow());
	mt.pos = tippos;
	mt.sz.cx = min(DPI(1000), 2 * wa.GetWidth() / 3);
	if(tippos >= 0 &&
	   (WhenTip(mt) || delayed_tip && DelayedTip(mt) && p == delayed_pos)) {
		mt.sz.cy = min(wa.GetHeight() / 2 - DPI(20), mt.sz.cy);
		tip.d = mt.display;
		tip.v = mt.value;
		tip.background = mt.background;
		Size sz = tip.AddFrameSize(mt.sz);
		int y = p.y + DPI(24);
		if(y + sz.cy > wa.bottom)
			y = max(0, p.y - sz.cy);
		int x = p.x;
		if(x + sz.cx > wa.right)
			x = max(0, wa.right - sz.cx);
		tip.SetRect(RectC(x, y, sz.cx, sz.cy) & wa);
		if(!tip.IsOpen())
			tip.PopUp(this, false, false, true);
		tip.Refresh();
	}
	else
		CloseTip();
}

bool CodeEditor::MouseSelSpecial(Point p, dword flags) {
	if((flags & K_MOUSELEFT) && HasFocus() && HasCapture() && !(flags & K_ALT) && selkind != SEL_CHARS) {
		int64 c = GetMousePos(p);
		int64 l, h;
		
		if(selkind == SEL_LINES) {
			l = c;
			int i = GetLinePos64(l);
			l = c - l;
			h = min(l + GetLineLength(i) + 1, GetLength64() - 1);
			c = c < anchor ? l : h;
		}
		else
			c = iscidl(GetChar(c - 1)) && GetWordPos(c, l, h) ? c < anchor ? l : h : c;
		PlaceCaret(c, mpos != c);
		return true;
	}
	return false;
}

void CodeEditor::LeftRepeat(Point p, dword flags)
{
	if(!MouseSelSpecial(p, flags))
		LineEdit::LeftRepeat(p, flags);
}

void CodeEditor::MouseMove(Point p, dword flags) {
	if(!MouseSelSpecial(p, flags))
		LineEdit::MouseMove(p, flags);

	tippos = Null;

	if(IsSelection()) return;

	if(p.x > 0) { // ignore calls from EditorBar::MouseMove
		Size fsz = GetFontSize();
		p = (p + fsz * (Size)sb.Get()) / fsz;
		int64 h = GetGPos(p.y, p.x);
		tippos = h < INT_MAX ? (int)h : -1;
	}
	
	SyncTip();
	delayed_tip = false;
	delayed_pos = Upp::GetMousePos();
	delayed.KillSet(1000, [=] {
		delayed_tip = true;
		SyncTip();
	});
}

bool CodeEditor::DelayedTip(MouseTip& tip) { return false; }

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
	delayed_tip = false;
	delayed_pos = Null;
	tippos = -1;
	LineEdit::MouseLeave();
	CloseTip();
	delayed.Kill();
}

WString CodeEditor::GetI()
{
	int64 l, h;
	WString ft;
	if((GetSelection(l, h) || GetWordPos(GetCursor64(), l, h)) && h - l < 60)
		while(l < h) {
			int c = GetChar(l++);
			if(c == '\n')
				break;
			ft.Cat(c);
		}
	return ft;
}

void CodeEditor::SetI(Ctrl *edit)
{
	*edit <<= GetI();
}

void CodeEditor::Goto() {
	String line = AsString(GetCursorLine());
	if(EditText(line, t_("Go to"), t_("Line:")))
		SetCursor(GetPos64(atoi(line) - 1));
}

bool CodeEditor::ToggleSimpleComment(int &start_line, int &end_line, bool usestars)
{
	if(IsReadOnly()) return true;

	int l, h;
	if(!GetSelection32(l, h))
		return true;

	int pos = h;
	start_line = GetLine(l);
	end_line = GetLinePos32(pos);

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
	if(IsReadOnly()) return;

	int start_line, end_line;
	if(ToggleSimpleComment(start_line, end_line))
		return;

	int us = static_cast<int>(usestars);

	bool is_commented = true;

	if(usestars) {
		is_commented &= GetChar(GetPos64(start_line) + 0) == '/' &&
						GetChar(GetPos64(start_line) + 1) == '*';

		is_commented &= GetChar(GetPos64(end_line - 1) + 1) == '*' &&
						GetChar(GetPos64(end_line - 1) + 2) == '/';
	}

	for(int line = start_line + us; is_commented && (line < end_line - us * 2); ++line)
		is_commented &= GetChar(GetPos64(line)) == (usestars ? ' ' : '/') &&
						GetChar(GetPos64(line)+1) == (usestars ? '*' : '/');

	if(!is_commented) {

		if(usestars) {
			Insert(GetPos32(end_line)," */\n");
			Insert(GetPos32(start_line),"/*\n");
		}

		for(int line = start_line + us; line < end_line + us; ++line)
			Insert(GetPos32(line), usestars ? " * " : "//");
	}
	else
	{
		int line = start_line;
		if(usestars)
			Remove(GetPos32(start_line), 3);
		for(; line < end_line - us * 2; ++line)
			Remove(GetPos32(line), 2 + us);
		if(usestars)
			Remove(GetPos32(line), 4);
	}

	if(usestars)
		SetLineSelection(start_line, end_line + (is_commented ? -2 : 2));
	else
		SetLineSelection(start_line, end_line);
}

void CodeEditor::ToggleStarComments()
{
	if(IsReadOnly()) return;

	int start_line, end_line;
	if(ToggleSimpleComment(start_line, end_line))
		return;

	bool is_commented =
		GetChar(GetPos64(start_line)) == '/' &&
		GetChar(GetPos64(start_line)+1) == '*' &&
		GetChar(GetPos64(start_line)+2) == '\n' &&
		GetChar(GetPos64(end_line-1)) == '*' &&
		GetChar(GetPos64(end_line-1)+1) == '/' &&
		GetChar(GetPos64(end_line-1)+2) == '\n';

	if(!is_commented) {
		// Backwards because inserting changes the end line #
		Insert(GetPos32(end_line),"*/\n");
		Insert(GetPos32(start_line),"/*\n");
		SetLineSelection(start_line, end_line+2);
	} else {
		// Backwards because inserting changes the end line #
		Remove(GetPos32(end_line-1),3);
		Remove(GetPos32(start_line),3);
		SetLineSelection(start_line, end_line-2);
	}
}

void CodeEditor::Enclose(const char *c1, const char *c2, int l, int h)
{
	if(IsReadOnly()) return;

	if((l < 0 || h < 0) && !GetSelection32(l, h))
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
	if(IsRectSelection())
		return LineEdit::Key(code, count);
	switch(code) {
	case K_SHIFT_CTRL_UP:
		SwapUpDown(true);
		return true;
	case K_SHIFT_CTRL_DOWN:
		SwapUpDown(false);
		return true;
	case K_CTRL_DELETE:
		DeleteWord();
		return true;
	case K_CTRL_BACKSPACE:
		DeleteWordBack();
		return true;
	case K_BACKSPACE:
		if(!IsReadOnly() && !IsAnySelection() && indent_spaces) {
			int c = GetCursor32();
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
		if(!IsRectSelection())
			IndentInsert('\n', count);
		return true;
	}
	bool sel = code & K_SHIFT;
	switch(code & ~K_SHIFT) {
	case K_CTRL_F:
		if(withfindreplace) {
			FindReplace(sel, true, false);
			return true;
		}
		break;
	case K_CTRL_H:
		if(withfindreplace) {
			FindReplace(sel, true, true);
			return true;
		}
		break;
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
	case K_CTRL_ADD:
		Zoom(1);
		return true;
	case K_CTRL_SUBTRACT:
		Zoom(-1);
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
				int x = GetColumnLine(GetCursor64()).x;
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
		if(wordwrap && code > 0 && code < 65535) {
			int limit = GetBorderColumn();
			int pos = GetCursor32();
			int lp = pos;
			int l = GetLinePos32(lp);
			if(limit > 10 && GetColumnLine(pos).x >= limit && lp == GetLineLength(l)) {
				int lp0 = GetPos32(l);
				WString ln = GetWLine(l);
				int wl = (int)GetGPos(l, limit) - lp0;
				while(wl > 0 && ln[wl - 1] != ' ')
					wl--;
				int sl = wl - 1;
				while(sl > 0 && ln[wl - 1] != '\n' && ln[sl - 1] == ' ')
					sl--;
				wordwrap = false;
				Remove(lp0 + sl, pos - (lp0 + sl));
				SetCursor(lp0 + sl);
				Put('\n');
				for(int i = 0; i < wl && findarg(ln[i], ' ', '\t') >= 0; i++)
					Put(ln[i]);
				for(int i = wl; i < ln.GetCount(); i++)
					Put(ln[i]);
				while(count--)
					Put(code);
				FinishPut();
				wordwrap = true;
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
	SetCursor(GetPos64(GetLineNo(line)));
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

void CodeEditor::PutI(WithDropChoice<EditString>& edit)
{
	edit.AddButton().SetMonoImage(CodeEditorImg::I()).Tip(t_("Set word/selection (Ctrl+I)"))
	    <<= THISBACK1(SetI, &edit);
}

void CodeEditor::Zoom(int d)
{
	Font f = GetFont();
	int h = f.GetCy();
	int q = f.GetHeight();
	while(f.GetCy() == h && (d < 0 ? f.GetCy() > 5 : f.GetCy() < 80))
		f.Height(q += d);
	SetFont(f);
	EditorBarLayout();
}

void CodeEditor::MouseWheel(Point p, int zdelta, dword keyFlags) {
	if(keyFlags & K_CTRL)
		Zoom(sgn(zdelta));
	else
		LineEdit::MouseWheel(p, zdelta, keyFlags);
}

void CodeEditor::Clear()
{
	for(SyntaxPos& p : syntax_cache)
		p.Clear();
	LineEdit::Clear();
	found = notfoundfw = notfoundbk = false;
}

void CodeEditor::ScrollBarItems::Paint(Draw& w)
{
	Rect sr = sb.GetSliderRect();
	for(const Tuple<int, Image, Color>& x : pos) {
		int y = sb.GetSliderPos(x.Get<int>());
		if(!IsNull(y)) {
			Image m = x.Get<Image>();
			Size isz = m.GetSize();
			if(y + isz.cy >= sr.top && y < sr.bottom)
				w.DrawImage(sr.CenterPoint().x - isz.cx / 2, sr.top + y - isz.cy / 2, m, x.Get<Color>());
		}
	}
	Color bg = IsDarkTheme() ? GrayColor(70) : SColorLtFace();

	if(editor.bar.li.GetCount() > 200000) // otherwise it gets too slow
		return;

	int py = -1; // accelerate long files
	int h = max(sr.GetHeight() / max(sb.GetTotal(), 1) + DPI(1), DPI(4));
	for(int i = 0; i < editor.bar.li.GetCount(); i++) {
		int edit = editor.bar.li[i].edited;
		if(edit) {
			int y = sb.GetSliderPos(i);
			if(y > sr.bottom)
				break;
			if(!IsNull(y) && y + h >= sr.top && y != py) {
				int age = (int)(log((double)(editor.GetUndoCount() + 1 - edit)) * 30);
				w.DrawRect(sr.left + DPI(2), sr.top + y, DPI(2), h,
				           Blend(SLtBlue(), bg, min(220, age)));
				py = y;
			}
		}
	}
}

CodeEditor::ScrollBarItems::ScrollBarItems(ScrollBar& sb, CodeEditor& e)
:	sb(sb), editor(e) {
	sb.Add(SizePos());
	Transparent();
	IgnoreMouse();
}

void CodeEditor::Errors(Vector<Point>&& errs)
{
	errors = pick(errs);
	Refresh();
	sbi.pos.Clear();
	for(Point p : errors)
		sbi.pos.Add({ p.y, CodeEditorImg::dot(), LtRed() });
	sbi.Refresh();
}

void CodeEditor::Layout()
{
	sbi.Refresh();
	LineEdit::Layout();
}

CodeEditor::CodeEditor()
:	sbi(sb.y, *this) {
	bracket_flash = false;
	highlight_bracket_pos0 = 0;
	bracket_start = 0;
	stat_edit_time = 0;
	last_key_time = Null;
	SetFont(CourierZ(12));
	AddFrame(bar);
	bar.SetEditor(this);
	UndoSteps(10000);
	InitFindReplace();
	bar.WhenBreakpoint = THISBACK(ForwardWhenBreakpoint);
	bar.WhenAnnotationMove = WhenAnnotationMove.Proxy();
	bar.WhenAnnotationClick = WhenAnnotationClick.Proxy();
	bar.WhenAnnotationRightClick = WhenAnnotationRightClick.Proxy();
	barline = true;
	sb.WithSizeGrip();
	HighlightSetup::InitOnce();
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
	selkind = SEL_CHARS;
	withfindreplace = true;
	wordwrap = false;
	blk0_header = false;
	closetip.Set(-200, [=] { SyncCloseTip(); });
}

CodeEditor::~CodeEditor() {}

}
