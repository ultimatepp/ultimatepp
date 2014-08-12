#include "ide.h"

void AssistEditor::SyncNavigator()
{
	if(navigating)
		return;
	if(IsNavigator()) {
		Search();
		SyncCursor();
	}
	navigatorframe.Show(navigator && theide && !theide->IsEditorMode());
}

void AssistEditor::SyncCursor()
{
	String k = "(" + GetKeyDesc(IdeKeys::AK_GOTO().key[0]) + ") ";
	search.NullText("Symbol/lineno " + k);
	search.Tip(IsNull(search) ? String() : "Clear " + k);
	
	if(IsNull(search)) {
		int ii = 0;
		int line = GetLine(GetCursor());
		for(int i = 0; i < item.GetCount(); i++)
			if(item[i].line - 1 <= line)
				ii = i;
		navigating = true;
		list.SetCursor(ii);
		navigating = false;
	}
}

void AssistEditor::SelectionChanged()
{
	CodeEditor::SelectionChanged();
	SyncCursor();
	SyncParamInfo();
}

void AssistEditor::Navigate()
{
	if(navigating)
		return;
	navigating = true;
	int ii = list.GetCursor();
	if(theide && ii >= 0 && ii < item.GetCount()) {
		const NavItem& m = item[ii];
		if(m.kind == KIND_LINE || IsNull(search))
			theide->GotoPos(Null, item[ii].line);
		else
			theide->IdeGotoCodeRef(MakeCodeRef(item[ii].nest, item[ii].qitem));
	}
	navigating = false;
}

void AssistEditor::NavigatorEnter()
{
	if(list.GetCount()) {
		list.GoBegin();
		Navigate();
		if(item[0].kind == KIND_LINE) {
			search.Clear();
			Search();
		}
	}
}

void AssistEditor::Navigator(bool nav)
{
	navigator = nav;
	navigatorframe.Show(navigator && theide && !theide->IsEditorMode());
	if(IsNavigator())
		SetFocus();
	SyncNavigator();
	SyncCursor();
}

void AssistEditor::SerializeNavigator(Stream& s)
{
	int version = 2;
	s / version;
	s % navigatorframe;
	s % navigator;
	if(version >= 1 && version < 2) {
		Splitter scope_item;
		s % scope_item;
	}
	Navigator(navigator);
}

void Ide::ToggleNavigator()
{
	editor.Navigator(!editor.navigator);
}

void Ide::SearchCode()
{
	if(!editor.navigator)
		editor.Navigator(true);
	if(!IsNull(~editor.search)) {
		editor.search.Clear();
		editor.Search();
	}
	else
		editor.search.SetFocus();
}

void Ide::SwitchHeader() {
	int c = filelist.GetCursor();
	if(c < 0) return;
	String currfile = filelist[c];
	const char *ext = GetFileExtPos(currfile);
	if(!stricmp(ext, ".h") || !stricmp(ext, ".hpp")
	|| !stricmp(ext, ".lay") || !stricmp(ext, ".iml")) {
		int f = filelist.Find(ForceExt(currfile, ".cpp"));
		if(f < 0) f = filelist.Find(ForceExt(currfile, ".c"));
		if(f < 0) f = filelist.Find(ForceExt(currfile, ".cc"));
		if(f >= 0) filelist.SetCursor(f);
	}
}

void AssistEditor::NavItem::Set(const CppItem& m)
{
	qitem = m.qitem;
	name = m.name;
	uname = m.uname;
	natural = m.natural;
	type = m.type;
	pname = m.pname;
	ptype = m.ptype;
	tname = m.tname;
	ctname = m.ctname;
	access = m.access;
	kind = m.kind;
	at = m.at;
	line = m.line;
	file = m.file;
	impl = m.impl;
}

int AssistEditor::NavigatorDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	int ii = q;
	if(ii < 0 || ii >= item.GetCount())
		return 0;
	const NavItem& m = item[(int)q];
	w.DrawRect(r, paper);
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
	if(IsNull(q))
		return 0;
	int x = r.left;
	int fcy = Draw::GetStdFontCy();
	int y = r.top + r.GetHeight() / 2 - fcy;
	if(m.kind == KIND_LINE) {
		w.DrawText(x, y, m.type, StdFont().Bold(), ink);
		return GetTextSize(m.type, StdFont().Bold()).cx;
	}
	Vector<ItemTextPart> n = ParseItemNatural(m.name, m.natural, m.ptype, m.pname, m.type,
	                                          m.tname, m.ctname, ~m.natural + m.at);
	int starti = 0;
	for(int i = 0; i < n.GetCount(); i++)
		if(n[i].type == ITEM_NAME) {
			starti = i;
			break;
		}
	PaintText(w, x, y, ~m.natural, n, 0, starti, focuscursor, ink, false);
	if(m.nest.GetCount()) {
		String h =  m.nest + "::";
		w.DrawText(x, y, h, BrowserFont().Bold(), Magenta());
		x += GetTextSize(h, BrowserFont()).cx;
	}
	int x1 = r.left;
	PaintText(w, x1, y + fcy, ~m.natural, n, starti, n.GetCount(), focuscursor, ink, false);
	return max(x, x1) - r.left;
}

void AssistEditor::NavigatorDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

Size AssistEditor::NavigatorDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return Size(DoPaint(w, Size(999999, 999999), q, White(), White(), 0), 2 * Draw::GetStdFontCy());
}

void AssistEditor::Search()
{
	list.Clear();
	item.Clear();
	String s = TrimBoth(~search);
	String search_name, search_nest;
	bool wholeclass = false;
	if(s.Find(':') >= 0) {
		Vector<String> h = Split((String)~search, ':');
		if(*s.Last() == ':')
			search_nest = Join(h, "::");
		else {
			search_name = h.Pop();
			if(h.GetCount())
				search_nest = Join(h, "::");
		}
		wholeclass = *s == ':';
	}
	else
		search_name = search_nest = ~search;
	s = Join(Split(s, ':'), "::") + (s.EndsWith(":") ? "::" : "");
	int lineno = StrInt(s);
	if(!IsNull(lineno)) {
		NavItem& m = item.Add();
		m.type = "Go to line " + AsString(lineno);
		m.kind = KIND_LINE;
		m.line = lineno;
	}
	else
	if(IsNull(s)) {
		int fileii = GetCppFileIndex(theide->editfile);
		const CppBase& b = CodeBase();
		ArrayMap<String, NavItem> imap;
		for(int i = 0; i < b.GetCount(); i++) {
			String nest = b.GetKey(i);
			const Array<CppItem>& ci = b[i];
			for(int j = 0; j < ci.GetCount(); j++) {
				const CppItem& m = ci[j];
				if(m.file == fileii) {
					NavItem& n = item.Add();
					n.Set(m);
					n.nest = nest;
				}
			}
		}
		Sort(item, FieldRelation(&NavItem::line, StdLess<int>()));
	}
	else {
		const CppBase& b = CodeBase();
		ArrayMap<String, NavItem> imap;
		for(int i = 0; i < b.GetCount(); i++) {
			String nest = b.GetKey(i);
			if(wholeclass ? ToUpper(nest) == search_nest : ToUpper(nest).Find(search_nest) >= 0) {
				const Array<CppItem>& ci = b[i];
				for(int j = 0; j < ci.GetCount(); j++) {
					const CppItem& m = ci[j];
					if(m.uname.Find(search_name) >= 0) {
						String key = nest + '\1' + m.qitem;
						int q = imap.Find(key);
						if(q < 0) {
							NavItem& ni = imap.Add(key);
							ni.Set(m);
							ni.nest = nest;
						}
						else {
							NavItem& mm = imap[q];
							String n = mm.natural;
							if(m.natural.GetCount() > mm.natural.GetCount())
								mm.natural = m.natural;
							if(CombineCompare(mm.impl, m.impl)(m.file, mm.file)(m.line, mm.line) < 0)
								mm.Set(m);
						}
					}
				}
			}
		}
		item = imap.PickValues();
	}
	Sort(item);
	list.SetVirtualCount(item.GetCount());
}
