#include "CodeEditor.h"

NAMESPACE_UPP

bool FindReplaceDlg::Key(dword key, int cnt) {
	if(key == K_CTRL_I) {
		if(find.HasFocus()) {
			find <<= itext;
			return true;
		}
		if(replace.HasFocus()) {
			replace <<= itext;
			return true;
		}
	}
	return TopWindow::Key(key, cnt);
}

Size FindReplaceDlg::GetAdjustedSize()
{
	Size sz = GetLayoutSize();
	if(!replacing)
		sz.cy -= replace.GetRect().bottom - find.GetRect().bottom;
	return sz;
}

void FindReplaceDlg::Setup(bool doreplace)
{
	replacing = doreplace;
	replace_lbl.Show(replacing);
	replace.Show(replacing);
	amend.Show(replacing);
//	amendall.Show(false); // not yet implemented
	Rect r = GetRect();
	Size sz = GetAdjustedSize();
	SetMinSize(sz);
	r.SetSize(sz);
	SetRect(r);
	ActiveFocus(find);
}

void CodeEditor::SetFound(int fi, int type, const WString& text)
{
	Found& f = foundwild.At(fi);
	f.type = type;
	f.text = text;
}

int CodeEditor::Match(const wchar *f, const wchar *s, int line, bool we, bool ignorecase, int fi)
{
	const wchar *b = s;
	int n = 0;
	WString ln;
	while(*f) {
		if(*f == WILDANY) {
			f++;
			WString wild;
			for(;;) {
				int nn = Match(f, s, line, we, ignorecase, fi + 1);
				if(nn >= 0) {
					SetFound(fi, WILDANY, wild);
					return int(s - b) + n + nn;
				}
				wild.Cat(*s ? *s : '\n');
				if(!*s++) return -1;
			}
			return -1;
		}
		else
		if(*f == WILDONE) {
			if(!*s) return -1;
			SetFound(fi++, WILDONE, WString(*s, 1));
			s++;
		}
		else
		if(*f == WILDSPACE) {
			const wchar *wb = s;
			if(*s != ' ' && *s != '\t') return -1;
			s++;
			while(*s == ' ' || *s == '\t')
				s++;
			SetFound(fi++, WILDSPACE, WString(wb, s));
		}
		else
		if(*f == WILDNUMBER) {
			const wchar *wb = s;
			if(*s < '0' || *s > '9') return -1;
			s++;
			while(*s >= '0' && *s <= '9')
				s++;
			SetFound(fi++, WILDNUMBER, WString(wb, s));
		}
		else
		if(*f == WILDID) {
			const wchar *wb = s;
			if(!iscib(*s)) return -1;
			s++;
			while(iscidl(*s)) s++;
			SetFound(fi++, WILDID, WString(wb, s));
		}
		else
		if(*f == '\n') {
			if(*s != '\0' || ++line >= GetLineCount()) return -1;
			n += int(s - b) + 1;
			ln = GetWLine(line);
			s = b = ln;
		}
		else {
			if(ignorecase ? ToUpper(*s) != ToUpper(*f) : *s != *f) return -1;
			s++;
		}
		f++;
	}
	return we && iscidl(*s) ? -1 : int(s - b) + n;
}

bool CodeEditor::Find(bool back, const wchar *text, bool wholeword, bool ignorecase,
                     bool wildcards, bool block)
{
	WString ft;
	const wchar *s = text;
	while(*s) {
		int c = *s++;
		if(c == '\\') {
			c = *s++;
			if(c == '\0') break;
			if(c == 'n')
				ft.Cat('\n');
			else
			if(c == 't')
				ft.Cat('\t');
			else
			if(c >= ' ')
				ft.Cat(c);
		}
		else
		if(c >= ' ')
			if(wildcards)
				ft.Cat(c == '*' ? WILDANY :
					   c == '?' ? WILDONE :
					   c == '%' ? WILDSPACE :
					   c == '#' ? WILDNUMBER :
					   c == '$' ? WILDID :
								  c
				);
			else
				ft.Cat(c);
	}
	foundwild.Clear();
	if(ft.IsEmpty()) return false;
	if(notfoundfw) MoveTextBegin();
	if(notfoundbk) MoveTextEnd();
	bool wb = wholeword ? iscidl(*ft) : false;
	bool we = wholeword ? iscidl(*ft.Last()) : false;
	int cursor, pos;
	if(found)
		GetSelection(pos, cursor);
	else
		GetSelection(cursor, pos);
	pos = cursor;
	int line = GetLinePos(pos);
	int linecount = GetLineCount();
	WString ln = GetWLine(line);
	const wchar *l = ln;
	s = l + pos;
	for(;;) {
		for(;;) {
			if(!wb || (s == l || !iscidl(s[-1]))) {
				int n = Match(ft, s, line, we, ignorecase);
				if(n >= 0) {
					int pos = GetPos(line, int(s - l));
					if(!back || pos + n < cursor) {
						foundsel = true;
						SetSelection(pos, pos + n);
						foundsel = false;
						if(!block)
							CenterCursor();
						found = true;
						return true;
					}
				}
			}
			if(back) {
				if(s-- == l) break;
			}
			else
				if(!*s++) break;
		}
		if(back) {
			if(--line < 0) break;
			ln = GetWLine(line);
			l = ln;
			s = ln.End();
		}
		else {
			if(++line >= linecount) break;
			ln = GetWLine(line);
			l = s = ln;
		}
	}
	if(back)
		notfoundbk = true;
	else
		notfoundfw = true;
	ClearSelection();
	return false;
}

void CodeEditor::FindReplaceAddHistory()
{
	if(!IsNull(findreplace.find))
		findreplace.find.AddHistory();
	if(!IsNull(findreplace.replace))
		findreplace.replace.AddHistory();
}

bool CodeEditor::Find(bool back, bool blockreplace, bool replace)
{
	FindReplaceAddHistory();
	if(Find(back, (WString)~findreplace.find, findreplace.wholeword,
		    findreplace.ignorecase, findreplace.wildcards, blockreplace)) {
		if(!blockreplace) {
			if(!findreplace.IsOpen()) {
				findreplace.NoCenter();
				OpenNormalFindReplace(replace);
			}
			Rect lr = GetLineScreenRect(GetLine(GetCursor()));
			Size fsz = findreplace.GetAdjustedSize();
			Rect r = GetTopCtrl()->GetRect();
			int y = r.bottom - fsz.cy;
			if(lr.bottom > y) y = r.top;
			Rect wa = Ctrl::GetWorkArea();
			findreplace.SetRect(RectC(wa.left + (wa.Width() - fsz.cx) / 2, y, fsz.cx, fsz.cy));
			findreplace.amend.Enable();
			SetFocus();
		}
		return true;
	}
	else {
		CloseFindReplace();
		return false;
	}
}

WString CodeEditor::GetWild(int type, int& i)
{
	for(;;) {
		if(i >= foundwild.GetCount()) return WString();
		Found& f = foundwild[i++];
		if(f.type == type) return f.text;
	}
}

WString CodeEditor::GetReplaceText()
{
	return GetReplaceText(findreplace.replace.GetText(), findreplace.wildcards);
}

WString CodeEditor::GetReplaceText(WString rs, bool wildcards)
{
	int anyi = 0, onei = 0, spacei = 0, numberi = 0, idi = 0;
	WString rt;
	const wchar *s = rs;
	while(*s) {
		int c = *s++;
		if(c == '\\') {
			c = *s++;
			if(c == '\0') break;
			if(c == 'n')
				rt.Cat('\n');
			else
			if(c == 't')
				rt.Cat('\t');
			else
			if(c >= ' ')
				rt.Cat(c);
		}
		else
		if(c >= ' ') {
			if(wildcards) {
				if(c == '*')
					rt.Cat(GetWild(WILDANY, anyi));
				else
				if(c == '?')
					rt.Cat(GetWild(WILDONE, onei));
				else
				if(c == '%')
					rt.Cat(GetWild(WILDSPACE, spacei));
				else
				if(c == '#')
					rt.Cat(GetWild(WILDNUMBER, numberi));
				else
				if(c == '$')
					rt.Cat(GetWild(WILDID, idi));
				else
				if(c == '@') {
					c = *s++;
					if(c == '\0') break;
					if(c >= '1' && c <= '9') {
						c -= '1';
						rt.Cat(c < foundwild.GetCount() ? foundwild[c].text : WString());
					}
					else {
						rt.Cat('@');
						if(c >= ' ' && c < 255) rt.Cat(c);
					}
				}
				else
					rt.Cat(c);
			}
			else
				rt.Cat(c);
		}
	}
	return rt;
}

void CodeEditor::Replace()
{
	if(!findreplace.replacing)
		return;
	NextUndo();
	FindReplaceAddHistory();
	if(!found) return;
	if(RemoveSelection()) {
		Paste(GetReplaceText());
		Find(false, false, true);
	}
}

int CodeEditor::BlockReplace(WString find, WString replace, bool wholeword, bool ignorecase, bool wildcards)
{
	NextUndo();
	int l, h;
	if(!GetSelection(l, h)) return 0;
	PlaceCaret(l);
	int count = 0;
	for(;;) {
		if(!Find(false, find, wholeword, ignorecase, wildcards, true) || cursor > h) break;
		int hh, ll;
		GetSelection(ll, hh);
		CachePos(ll);
		h = h - (hh - ll) + Paste(GetReplaceText(replace, wildcards));
		count++;
	}
	SetSelection(l, h);
	Refresh();
	return count;
}

void CodeEditor::BlockReplace()
{
	BlockReplace(~findreplace.find, ~findreplace.replace, findreplace.wholeword,
		findreplace.ignorecase, findreplace.wildcards);
/*
	NextUndo();
	int l, h;
	if(!GetSelection(l, h)) return;
	PlaceCaret(l);
	for(;;) {
		if(!Find(false, true) || cursor >= h) break;
		Refresh();
		int hh, ll;
		GetSelection(ll, hh);
		CachePos(ll);
		h = h - (hh - ll) + Paste(GetReplaceText());
	}
	SetSelection(l, h);
*/
}

void CodeEditor::OpenNormalFindReplace(bool replace)
{
	findreplace.Setup(replace);

	findreplace.itext = GetI();
	findreplace.Title(replace ? "Find and Replace" : "Find");
	findreplace.findback.Show();
	findreplace.ok.SetLabel("Find Next");
	findreplace.ok <<= THISBACK(DoFind);
	findreplace.cancel <<= findreplace.WhenClose = THISBACK(CloseFindReplace);
	findreplace.Open();
}

void CodeEditor::FindReplace(bool pick_selection, bool pick_text, bool replace)
{
	CloseFindReplace();
	findreplace.CenterOwner();
	WString find_text;
	int find_pos = -1;
	
	findreplace.Setup(replace);
	
	if(pick_text || pick_selection)
	{
		WString s = GetSelection().ToWString();
		if(IsSelection()) {
			if(s.Find('\n') < 0)
				find_text = s;
		}
		else
		if(pick_text) {
			int l, h;
			l = h = GetCursor();
			while(l > 0 && CharFilterCIdent(GetChar(l - 1)))
				l--;
			while(h < GetLength() && CharFilterCIdent(GetChar(h)))
				h++;
			find_text = Get(l, h - l).ToWString();
			find_pos = h;
		}
		if(find_text.GetCount())
			findreplace.find <<= find_text;
	}
	if(IsSelection() && replace) {
		findreplace.itext = GetI();
		findreplace.Title("Replace in selection");
		findreplace.amend.Hide();
		findreplace.findback.Hide();
		findreplace.ok.SetLabel("Replace");
		findreplace.ok <<= findreplace.Breaker(IDOK);
		findreplace.cancel <<= findreplace.Breaker(IDCANCEL);
		if(findreplace.Execute() == IDOK)
			BlockReplace();
	}
	else {
		if(find_pos >= 0)
			SetCursor(find_pos);
		OpenNormalFindReplace(replace);
		findreplace.find.SetWantFocus();
	}
}

void CodeEditor::InsertWildcard(int c)
{
	iwc = c;
}

void FindWildcardMenu(Callback1<int> cb, Point p, bool tablf, Ctrl *owner)
{
	MenuBar menu;
	menu.Add("One or more spaces", callback1(cb, '%'));
	menu.Add("One or more any characters", callback1(cb, '*'));
	menu.Add("C++ identifier", callback1(cb, '$'));
	menu.Add("Number", callback1(cb, '#'));
	menu.Add("Any character", callback1(cb, '?'));
	if(tablf) {
		menu.Separator();
		menu.Add("Tab", callback1(cb, '\t'));
		menu.Add("Line feed", callback1(cb, '\n'));
	}
	menu.Execute(owner, p);
}

void CodeEditor::FindWildcard()
{
	int l, h;
	findreplace.find.GetSelection(l, h);
	iwc = 0;
	FindWildcardMenu(THISBACK(InsertWildcard), findreplace.find.GetPushScreenRect().TopRight(), true,
	                 &findreplace);
	if(iwc) {
		findreplace.wildcards = true;
		findreplace.find.SetFocus();
		findreplace.find.SetSelection(l, h);
		findreplace.find.RemoveSelection();
		if(iwc == '\t') {
			findreplace.find.Insert('\\');
			findreplace.find.Insert('t');
		}
		else
		if(iwc == '\n') {
			findreplace.find.Insert('\\');
			findreplace.find.Insert('n');
		}
		else
			findreplace.find.Insert(iwc);
	}
}

void CodeEditor::ReplaceWildcard()
{
	MenuBar menu;
	menu.Add("Matched spaces", THISBACK1(InsertWildcard, '%'));
	menu.Add("Matched one or more any characters", THISBACK1(InsertWildcard, '*'));
	menu.Add("Matched C++ identifier", THISBACK1(InsertWildcard, '$'));
	menu.Add("Matched number", THISBACK1(InsertWildcard, '#'));
	menu.Add("Matched any character", THISBACK1(InsertWildcard, '?'));
	menu.Separator();
	for(int i = 1; i <= 9; i++)
		menu.Add(Format("Matched wildcard %d", i), THISBACK1(InsertWildcard, i));
	menu.Separator();
	menu.Add("Tab", THISBACK1(InsertWildcard, 20));
	menu.Add("Line feed", THISBACK1(InsertWildcard, 21));
	int l, h;
	findreplace.replace.GetSelection(l, h);
	iwc = 0;
	menu.Execute(&findreplace, findreplace.replace.GetPushScreenRect().TopRight());
	if(iwc) {
		findreplace.wildcards = true;
		findreplace.replace.SetFocus();
		findreplace.replace.SetSelection(l, h);
		findreplace.replace.RemoveSelection();
		if(iwc == 20) {
			findreplace.replace.Insert('\\');
			findreplace.replace.Insert('t');
		}
		else
		if(iwc == 21) {
			findreplace.replace.Insert('\\');
			findreplace.replace.Insert('n');
		}
		else
		if(iwc >= 1 && iwc <= 9) {
			findreplace.replace.Insert('@');
			findreplace.replace.Insert(iwc + '0');
		}
		else
			findreplace.replace.Insert(iwc);
	}
}

void CodeEditor::CloseFindReplace()
{
	findreplace.Close();
}

void CodeEditor::DoFind()
{
	Find();
}

void CodeEditor::DoFindBack()
{
	Find(true);
}

void CodeEditor::SerializeFind(Stream& s)
{
	s % findreplace.find;
	findreplace.find.SerializeList(s);
	s % findreplace.wholeword % findreplace.ignorecase % findreplace.wildcards;
	s % findreplace.replace;
	findreplace.replace.SerializeList(s);
}

END_UPP_NAMESPACE
