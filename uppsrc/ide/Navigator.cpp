#include "ide.h"

void AssistEditor::MakeIndex()
{
	searchindex.Clear();
	Renumber2();
	indexitem.Clear();
	const Index<String>& cppkeywords = CppKeywords();
	String sop = "operator";
	String klass;
	for(int i = 0; i < min(GetLineCount(), 10000); i++) {
		String l = GetEncodedLine(i, CHARSET_UTF8);
		int plevel = 0;
		int tlevel = 0;
		Color ink = Gray;
		bool fn = false;
		bool td = false;
		if(l[0] > 32) {
			String nklass, id;
			try {
				CParser p(l);
				if(p.Char('#')) {
					if(p.Id("define") && p.IsId()) {
						id = p.ReadId();
						ink = Magenta;
					}
				}
				else {
					while(!p.IsEof())
						if(p.Id("typedef")) {
							td = true;
							ink = LtBlue;
						}
						else
						if((p.Id("class") || p.Id("struct") || p.Id("union") || p.Id("enum")) && tlevel == 0 && p.IsId()) {
							id = p.ReadId();
							if(p.Char(';'))
								id.Clear();
							ink = LtBlue;
							break;
						}
						else
						if(p.Char('~') && p.IsId())
							id = '~' + p.ReadId();
						else
						if(p.Char2(':', ':') && tlevel == 0)
							nklass = id;
						else
						if(p.IsId() && tlevel == 0) {
							id = p.ReadId();
							if(id == sop) {
								static char oc[] = "<>!^&*+=-%";
								while(!p.IsEof() && strchr(oc, *p.GetPtr())) {
									id.Cat(*p.GetPtr());
									p.SkipTerm();
								}
							}
						}
						else
						if(p.Char('(')) {
							if(plevel == 0) {
								fn = true;
								ink = Black;
								int q = i;
								for(;;) {
									if(p.IsEof()) {
										if(++q >= GetLineCount() || q > i + 6) {
											id.Clear();
											break;
										}
										l = GetEncodedLine(q, CHARSET_UTF8);
										p.Set(l);
									}
									if(p.Char('{'))
										break;
									if(p.Char(';')) {
										id.Clear();
										break;
									}
									p.SkipTerm();
								}
								break;
							}
							plevel++;
						}
						else
						if(p.Char(')'))
							plevel--;
						else
						if(p.Char('>')) {
							tlevel--;
							id.Clear();
						}
						else
						if(p.Char('<'))
							tlevel++;
						else
						if(p.Char(';') || p.Char('=') && plevel == 0) {
							if(!td) ink = Green;
							break;
						}
						else
							p.SkipTerm();
				}
			}
			catch(CParser::Error) {}
			if(id.GetCount() && CppKeywords().Find(id) < 0) {
				if(nklass != klass) {
					klass = nklass;
					if(klass.GetCount()) {
						IndexItem& m = indexitem.Add();
						m.text = klass + "::";
						m.ink = LtBlue;
						m.line = i;
					}
				}
				if(klass.GetCount())
					id = "  " + id;
				IndexItem& m = indexitem.Add();
				m.text = id;
				m.ink = ink;
				m.line = i;
			}
		}
	}
	SyncIndex();
	SyncCursor();
}

void AssistEditor::SyncNavigator()
{
	if(navigator == NAV_INDEX)
		MakeIndex();
	if(navigator == NAV_BROWSER)
		browser.Load();
	navigatorframe.Show(navigator);
}

void AssistEditor::SyncIndex()
{
	index.Clear();
	if(indexitem.GetCount() && navigator == NAV_INDEX && assist_active) {
		String s = ~searchindex;
		for(int i = 0; i < indexitem.GetCount(); i++) {
			const IndexItem& m = indexitem[i];
			if(s.GetCount() == 0 || ToUpper(m.text).Find(s) >= 0)
				index.Add(AttrText(m.text).Ink(m.ink), m.line);
		}
	}
}

void AssistEditor::IndexSync()
{
	if(index.IsCursor()) {
		int q = index.Get(1);
		for(int i = 0; i < GetLineCount(); i++)
			if(GetLine2(i) >= q) {
				SetCursor(GetPos(i));
				TopCursor();
				return;
			}
	}
}

void AssistEditor::IndexClick()
{
	IndexSync();
	SetFocus();
	SyncCursor();
}

void AssistEditor::SyncCursor()
{
	if(navigator == NAV_INDEX) {
		if(!index.IsCursor())
			index.SetCursor(0);
		if(!index.GetCount() || searchindex.HasFocus())
			return;
		int ln = GetLine2(GetCursorLine());
		int ii = index.GetCursor();
		while(ii > 0 && (int)index.Get(ii, 1) > ln)
			ii--;
		while(ii < index.GetCount() - 1 && (int)index.Get(ii + 1, 1) <= ln)
			ii++;
		index.SetCursor(ii);
	}
	if(navigator == NAV_BROWSER) {
		int ii = GetCursorLine();
		String coderef;
		while(ii >= 0 && IsNull(coderef))
			coderef = GetAnnotation(ii--);
		browser.Goto(coderef);
	}
}

void AssistEditor::SelectionChanged()
{
	CodeEditor::SelectionChanged();
	SyncCursor();
}

void AssistEditor::SearchIndex()
{
	SyncIndex();
	index.GoBegin();
	IndexSync();
}

void AssistEditor::BrowserGoto()
{
	IdeGotoCodeRef(browser.GetCodeRef());
	SetFocus();
}

bool AssistEditor::NavigatorKey(dword key)
{
	if(navigator == NAV_INDEX)
		switch(key) {
		case K_ENTER:
		case K_ESCAPE:
			searchindex.Clear();
			SetFocus();
			SyncIndex();
			SyncCursor();
			return true;
		case K_UP:
		case K_DOWN:
			index.Key(key, 1);
			IndexSync();
			return true;
		}
	return false;
}

void AssistEditor::Navigator(int nav)
{
	navigator = nav;
	navigatorframe.Show(navigator);
	switch(navigator) {
	case NAV_INDEX:
		scope_item.Hide();
		indexpane.Show();
		break;
	case NAV_BROWSER:
		indexpane.Hide();
		scope_item.Show();
		break;
	}
	if(navigator)
		SetFocus();
	SyncNavigator();
	if(navigator == NAV_INDEX)
		SyncIndex();
	SyncCursor();
}

void AssistEditor::SerializeNavigator(Stream& s)
{
	int version = 1;
	s / version;
	s % navigatorframe;
	s % navigator;
	if(version >= 1)
		s % scope_item;
	if(searchindex.HasFocus())
		SetFocus();
	Navigator(navigator);
}

void Ide::ToggleNavigator(int nav)
{
	editor.Navigator(editor.GetNavigator() == nav ? 0 : nav);
}

void Ide::SearchIndex()
{
	editor.Navigator(AssistEditor::NAV_INDEX);
	if(editor.index.IsVisible())
		editor.searchindex.SetFocus();
}
