#include "CodeEditor.h"

NAMESPACE_UPP

void CodeEditor::InitFindReplace()
{
	findreplace.find.AddButton().SetMonoImage(CtrlImg::smallright()).Tip("Wildcard")
		<<= THISBACK(FindWildcard);
	findreplace.replace.AddButton().SetMonoImage(CtrlImg::smallright()).Tip("Wildcard")
		<<= THISBACK(ReplaceWildcard);
	PutI(findreplace.find);
	PutI(findreplace.replace);
	findreplace.amend <<= THISBACK(Replace);
	findreplace.prev <<= THISBACK(DoFindBack);
	findreplace.replacing = false;
	found = notfoundfw = notfoundbk = foundsel = false;
	persistent_find_replace = false;
	findreplace.find <<= findreplace.wholeword <<= findreplace.wildcards
	                 <<= findreplace.ignorecase <<= THISBACK(IncrementalFind);
}

FindReplaceDlg::FindReplaceDlg()
{
	ignorecase <<= THISBACK(Sync);
	samecase <<= true;
	close.Cancel();
	prev.SetImage(CtrlImg::SmallUp());
	amend.SetImage(CodeEditorImg::Replace());
	incremental <<= true;
	Sync();
}

void FindReplaceDlg::Sync()
{
	samecase.Enable(ignorecase);
}

dword CodeEditor::find_next_key = K_F3;
dword CodeEditor::find_prev_key = K_SHIFT_F3;
dword CodeEditor::replace_key = K_CTRL_R;

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
	if(key == K_ENTER) {
		next.WhenAction();
		return true;
	}
	if(findarg(key, K_TAB, K_SHIFT_TAB, K_UP, K_DOWN) >= 0 && replace.IsShown()) {
		if(find.HasFocus())
			replace.SetFocus();
		else
			find.SetFocus();
		return true;
	}
	if(key == K_ESCAPE) {
		close.WhenAction();
		return true;
	}
	return TopWindow::Key(key, cnt);
}

void FindReplaceDlg::Setup(bool doreplace)
{
	CtrlLayout(*this);
	close.SetImage(CodeEditorImg::Cancel());
	close.Tip("Close (Esc)");
	close.Normal();
	close.SetLabel("");
	next.SetImage(CtrlImg::SmallDown());
	next.Tip("Find next (" + GetKeyDesc(CodeEditor::find_next_key) + ")");
	next.Normal();
	next.SetLabel("");
	prev.Tip("Find prev (" + GetKeyDesc(CodeEditor::find_prev_key) + ")");
	amend.Tip("Replace (" + GetKeyDesc(CodeEditor::replace_key) + ")");
	info.SetLabel("&Find");
	replacing = doreplace;
	replace.Show(replacing);
	amend.Show(replacing);
	Height(doreplace ? GetLayoutSize().cy : replace.GetRect().top);
	SetFrame(TopSeparatorFrame());
	Sync();
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
	if(notfoundfw) MoveTextBegin();
	if(notfoundbk) MoveTextEnd();
	int cursor, pos;
	if(found)
		GetSelection(pos, cursor);
	else
		GetSelection(cursor, pos);
	pos = cursor;
	return FindFrom(pos, back, text, wholeword, ignorecase, wildcards, block);
}

bool CodeEditor::FindFrom(int pos, bool back, const wchar *text, bool wholeword, bool ignorecase,
                          bool wildcards, bool block) {
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
		if(c >= ' ') {
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
	}
	bool wb = wholeword ? iscidl(*ft) : false;
	bool we = wholeword ? iscidl(*ft.Last()) : false;
	if(ft.IsEmpty()) return false;
	foundwild.Clear();
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
					foundtext = GetW(pos, n);
					if(!back || pos + n < cursor) {
						if(!block) {
							foundsel = true;
							SetSelection(pos, pos + n);
							foundsel = false;
							CenterCursor();
						}
						foundpos = pos;
						foundsize = n;
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
	findreplace.find.Error(false);
	findreplace.info.SetLabel("&Find");
	if(Find(back, (WString)~findreplace.find, findreplace.wholeword,
		    findreplace.ignorecase, findreplace.wildcards, blockreplace)) {
		if(!blockreplace) {
			if(!findreplace.IsOpen())
				OpenNormalFindReplace(replace);
			findreplace.amend.Enable();
			if(!findreplace.incremental)
				SetFocus();
		}
		return true;
	}
	else {
		findreplace.find.Error();
		findreplace.info.SetLabel("Not &found");
		if(!findreplace.incremental)
			SetFocus();
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
	return GetReplaceText(~findreplace.replace, findreplace.wildcards,
	                      findreplace.wildcards && findreplace.samecase);
}

WString CodeEditor::GetReplaceText(WString rs, bool wildcards, bool samecase)
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
				WString w;
				if(c == '*')
					w = GetWild(WILDANY, anyi);
				else
				if(c == '?')
					w = GetWild(WILDONE, onei);
				else
				if(c == '%')
					w = GetWild(WILDSPACE, spacei);
				else
				if(c == '#')
					w = GetWild(WILDNUMBER, numberi);
				else
				if(c == '$')
					w = GetWild(WILDID, idi);
				else
				if(c == '@') {
					c = *s++;
					if(c == '\0') break;
					if(c == '@') {
						rt << AsString(replacei).ToWString();
						continue;
					}
					if(c == '#') {
						rt << AsString(replacei + 1).ToWString();
						continue;
					}
					if(c >= '1' && c <= '9') {
						c -= '1';
						w = c < foundwild.GetCount() ? foundwild[c].text : WString();
					}
					else {
						rt.Cat('@');
						if(c >= ' ' && c < 255) rt.Cat(c);
						continue;
					}
				}
				else {
					rt.Cat(c);
					continue;
				}
				if(*s == '+') {
					w = ToUpper(w);
					s++;
				}
				else
				if(*s == '-') {
					w = ToLower(w);
					s++;
				}
				else
				if(*s == '!') {
					w = InitCaps(w);
					s++;
				}
				rt.Cat(w);
			}
			else
				rt.Cat(c);
		}
	}
	if(samecase) {
		if(foundtext.GetCount() && rt.GetCount()) {
			if(IsUpper(foundtext[0]))
				rt.Set(0, ToUpper(rt[0]));
			if(IsLower(foundtext[0]))
				rt.Set(0, ToLower(rt[0]));
		}
		if(foundtext.GetCount() > 1) {
			if(IsUpper(foundtext[1]))
				for(int i = 1; i < rt.GetCount(); i++)
					rt.Set(i, ToUpper(rt[i]));
			if(IsLower(foundtext[1]))
				for(int i = 1; i < rt.GetCount(); i++)
					rt.Set(i, ToLower(rt[i]));
		}
	}
	replacei++;
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

int CodeEditor::BlockReplace(WString find, WString replace, bool wholeword, bool ignorecase, bool wildcards, bool samecase)
{
	NextUndo();
	Refresh(); // Setting full-refresh here avoids Pre/Post Remove/Insert costs
	int l, h;
	if(!GetSelection(l, h)) return 0;
	PlaceCaret(l);
	int count = 0;
	foundpos = l;
	while(FindFrom(foundpos, false, find, wholeword, ignorecase, wildcards, true) && foundpos + foundsize <= h) {
		CachePos(foundpos);
		Remove(foundpos, foundsize);
		WString rt = GetReplaceText(replace, wildcards, ignorecase && samecase);
		Insert(foundpos, rt);
		foundpos += rt.GetCount();
		h = h - foundsize + rt.GetCount();
		count++;
	}
	SetSelection(l, h);
	return count;
}

void CodeEditor::BlockReplace()
{
	BlockReplace(~findreplace.find, ~findreplace.replace, findreplace.wholeword,
		         findreplace.ignorecase, findreplace.wildcards, findreplace.samecase);
}

void CodeEditor::OpenNormalFindReplace(bool replace)
{
	findreplace.Setup(replace);
	findreplace.find.Error(false);
	findreplace.find.SetFocus();
	findreplace.itext = GetI();
	findreplace.prev.Show();
	findreplace.next <<= THISBACK(DoFind);
	findreplace.close <<= THISBACK(CloseFindReplace);
	if(!findreplace.IsOpen())
		AddFrame(findreplace);
}

void CodeEditor::FindReplace(bool pick_selection, bool pick_text, bool replace)
{
	if(findreplace.IsOpen())
		CloseFindReplace();

	replacei = 0;
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
		SetLayout_BlockReplaceLayout(findreplace);
		findreplace.SetRect(WithBlockReplaceLayout<EmptyClass>::GetLayoutSize());
		findreplace.Title("Replace in selection");
		findreplace.amend.Hide();
		findreplace.prev.Hide();
		findreplace.next.Ok() <<= findreplace.Breaker(IDOK);
		findreplace.close.Cancel() <<= findreplace.Breaker(IDCANCEL);
		findreplace.close.SetImage(Null);
		findreplace.close.Tip("");
		findreplace.next.SetImage(Null);
		findreplace.next.Tip("");
		if(findreplace.Execute() == IDOK)
			BlockReplace();
	}
	else {
		if(find_pos >= 0)
			SetCursor(find_pos);
		OpenNormalFindReplace(replace);
		findreplace.find.SetFocus();
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
	menu.Add("0-based replace index", THISBACK1(InsertWildcard, '0'));
	menu.Add("1-based replace index", THISBACK1(InsertWildcard, '1'));
	menu.Separator();
	for(int i = 1; i <= 9; i++)
		menu.Add(Format("Matched wildcard %d", i), THISBACK1(InsertWildcard, i));
	menu.Separator();
	menu.Add("To upper", THISBACK1(InsertWildcard, '+'));
	menu.Add("To lower", THISBACK1(InsertWildcard, '-'));
	menu.Add("InitCaps", THISBACK1(InsertWildcard, '!'));
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
		if(iwc == '0') {
			findreplace.replace.Insert('@');
			findreplace.replace.Insert('@');
		}
		else
		if(iwc == '1') {
			findreplace.replace.Insert('@');
			findreplace.replace.Insert('#');
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
	if(findreplace.IsOpen())
		RemoveFrame(findreplace);
}

void CodeEditor::IncrementalFind()
{
	if(!findreplace.incremental || findreplace.GetTopCtrl() == &findreplace) // || we are block replace
		return;
	int l, h;
	GetSelection(l, h);
	ClearSelection();
	SetCursor(min(l, h));
	Find();
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
	int version = 1;
	s / version;
	s % findreplace.find;
	findreplace.find.SerializeList(s);
	s % findreplace.wholeword % findreplace.ignorecase % findreplace.wildcards;
	if(version >= 0)
		s % findreplace.samecase;
	s % findreplace.replace;
	if(version >= 1)
		s % findreplace.incremental;
	findreplace.replace.SerializeList(s);
}

END_UPP_NAMESPACE
