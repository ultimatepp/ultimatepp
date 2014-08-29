#include "ide.h"

#define LTIMING(x) // RTIMING(x)

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

	if(!navigating) {
		navlines.KillCursor();
		int q = linefo.Find(GetCppFileIndex(theide->editfile));
		if(q < 0)
			return;
		navigating = true;
		SortedVectorMap<int, int>& m = linefo[q];
		q = m.FindUpperBound(GetLine(GetCursor()) + 1) - 1;
		if(q >= 0 && q < m.GetCount())
			list.SetCursor(m[q]);
		navigating = false;
	}
	SyncLines();
}

void AssistEditor::SyncLines()
{
	int ln = GetLine(GetCursor()) + 1;
	int fi = GetCppFileIndex(theide->editfile);
	int q = -1;
	for(int i = 0; i < navlines.GetCount(); i++) {
		const NavLine& l = navlines.Get(i, 0).To<NavLine>();
		if(l.file == fi && l.line <= ln && i < navlines.GetCount())
			q = i;
	}
	if(q >= 0)
		navlines.SetCursor(q);
}

void AssistEditor::SyncNavLines()
{
	int sc = navlines.GetScroll();
	navlines.Clear();
	int ii = list.GetCursor();
	if(ii >= 0 && ii < litem.GetCount()) {
		int ln = GetLine(GetCursor()) + 1;
		Vector<NavLine> l = GetNavLines(*litem[ii]);
		for(int i = 0; i < l.GetCount(); i++) {
			String p = GetCppFile(l[i].file);
			navlines.Add(RawToValue(l[i]));
		}
		navlines.ScrollTo(sc);
		SyncLines();
	}
}

void PaintTeXt(Draw& w, int& x, int y, const String& text, Font font, Color ink)
{
	Size sz = GetTextSize(text, font);
	w.DrawText(x, y, text, font, ink);
	x += sz.cx;
}

int AssistEditor::LineDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	const NavLine& l = q.To<NavLine>();
	int x = r.left;
	String p = GetCppFile(l.file);
	int y = r.top + (r.GetHeight() - StdFont().GetCy()) / 2;
	PaintTeXt(w, x, y, GetFileName(GetFileFolder(p)) + "/", StdFont(), ink);
	PaintTeXt(w, x, y, GetFileName(p), StdFont().Bold(), ink);
	PaintTeXt(w, x, y, " (", StdFont(), ink);
	PaintTeXt(w, x, y, AsString(l.line), StdFont().Bold(), ink);
	PaintTeXt(w, x, y, ")", StdFont(), ink);
	return x;
}

void AssistEditor::LineDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

Size AssistEditor::LineDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return Size(DoPaint(w, Size(999999, 999999), q, White(), White(), 0), StdFont().Bold().GetCy());
}

void AssistEditor::GoToNavLine()
{
	int ii = navlines.GetClickPos().y;
	if(ii >= 0 && ii < navlines.GetCount() && theide) {
		const NavLine& l = navlines.Get(ii, 0).To<NavLine>();
		theide->GotoPos(GetCppFile(l.file), l.line);
	}
}

void AssistEditor::SelectionChanged()
{
	CodeEditor::SelectionChanged();
	SyncCursor();
	SyncParamInfo();
}

bool AssistEditor::NavLine::operator<(const NavLine& b) const
{
	String p1 = GetCppFile(file);
	String p2 = GetCppFile(b.file);
	return CombineCompare(!impl, !b.impl)
	                     (GetFileExt(p1), GetFileExt(p2)) // .h > .c
	                     (GetFileName(p1), GetFileName(p2))
	                     (p1, p2)
	                     (line, b.line) < 0;
}

Vector<AssistEditor::NavLine> AssistEditor::GetNavLines(const NavItem& m)
{
	Vector<NavLine> l;
	int q = CodeBase().Find(m.nest);
	if(q < 0 || IsNull(m.qitem))
		return l;
	const Array<CppItem>& a = CodeBase()[q];
	for(int i = 0; i < a.GetCount(); i++) {
		const CppItem& mm = a[i];
		if(mm.qitem == m.qitem) {
			NavLine& nl = l.Add();
			nl.impl = mm.impl;
			nl.file = mm.file;
			nl.line = mm.line;
		}
	}
	Sort(l);
	return l;
}

void AssistEditor::Navigate()
{
	if(navigating)
		return;
	navigating = true;
	int ii = list.GetCursor();
	if(theide && ii >= 0 && ii < litem.GetCount()) {
		int ln = GetLine(GetCursor()) + 1;
		const NavItem& m = *litem[ii];
		if(m.kind == KIND_LINE || IsNull(search))
			theide->GotoPos(Null, m.line);
		else {
			Vector<NavLine> l = GetNavLines(m);
			int q = l.GetCount() - 1;
			for(int i = q; i >= 0; i--) {
				if(GetCppFile(l[i].file) == theide->editfile && l[i].line == ln) {
					q = (i + l.GetCount() - 1) % l.GetCount();
					break;
				}
			}
			if(q >= 0 && q < l.GetCount())
				theide->GotoPos(GetCppFile(l[q].file), l[q].line);
		}
	}
	navigating = false;
}

void AssistEditor::NavigatorClick()
{
	int q = list.GetClickPos().y;
	if(q >= 0 && q < list.GetCount())
		Navigate();
}

void AssistEditor::NavigatorEnter()
{
	if(list.GetCount()) {
		list.GoBegin();
		Navigate();
		if(litem[0]->kind == KIND_LINE) {
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
	int version = 4;
	s / version;
	s % navigatorframe;
	s % navigator;
	if(version == 1 || version == 3) {
		Splitter dummy;
		s % dummy;
	}
	if(version >= 4)
		s % navigator_splitter;
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

void AssistEditor::NavigatorDisplay::PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	int ii = q;
	if(ii < 0 || ii >= item.GetCount())
		return;
	const NavItem& m = *item[ii];
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
	if(findarg(m.kind, KIND_FILE, KIND_NEST) >= 0)
		w.DrawRect(r, focuscursor ? paper : m.kind == KIND_NEST ? Blend(SColorMark, SColorPaper, 220)
		                                    : SColorFace);
	else
		w.DrawRect(r, paper);
}


int AssistEditor::NavigatorDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	int ii = q;
	if(ii < 0 || ii >= item.GetCount())
		return 0;
	const NavItem& m = *item[ii];
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);

	int x = r.left;
	int ry = r.top + r.GetHeight() / 2;
	int y = ry - Draw::GetStdFontCy() / 2;

	if(findarg(m.kind, KIND_FILE, KIND_NEST) >= 0) {
		w.DrawRect(r, focuscursor ? paper : m.kind == KIND_NEST ? Blend(SColorMark, SColorPaper, 220)
		                                    : SColorFace);
		if(m.kind == KIND_FILE)
			return PaintFileName(w, r, m.type, ink);
		w.DrawText(x, y, m.type, StdFont().Bold(), ink);
		return GetTextSize(m.type, StdFont().Bold()).cx;
	}
	
	w.DrawRect(r, paper);
	if(m.kind == KIND_LINE) {
		w.DrawText(x, y, m.type, StdFont().Bold(), ink);
		return GetTextSize(m.type, StdFont().Bold()).cx;
	}

	PaintCppItemImage(w, x, ry, m.access, m.kind, focuscursor);	

	x += 15;
	int x0 = x;
	Vector<ItemTextPart> n = ParseItemNatural(m.name, m.natural, m.ptype, m.pname, m.type,
	                                          m.tname, m.ctname, ~m.natural + m.at);
	int starti = 0;
	for(int i = 0; i < n.GetCount(); i++)
		if(n[i].type == ITEM_NAME) {
			starti = i;
			break;
		}
	PaintText(w, x, y, m.natural, n, starti, n.GetCount(), focuscursor, ink, false);
	if(starti) {
		const char *h = " : ";
		w.DrawText(x, y, h, BrowserFont(), SColorText);
		x += GetTextSize(h, BrowserFont()).cx;
	}
	PaintText(w, x, y, m.natural, n, 0, starti, focuscursor, ink, false);
	return x;
}

void AssistEditor::NavigatorDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

Size AssistEditor::NavigatorDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return Size(DoPaint(w, Size(999999, 999999), q, White(), White(), 0), Draw::GetStdFontCy());
}

void AssistEditor::TriggerSearch()
{
	search_trigger.KillSet(100, THISBACK(Search));
}

void AssistEditor::NavGroup(bool local)
{
	for(int i = 0; i < nitem.GetCount(); i++) {
		NavItem& m = nitem[i];
		String g = m.nest;
		if(m.kind == TYPEDEF)
			g.Trim(max(g.ReverseFind("::"), 0));
		if(IsNull(g))
			g = "\xff" + GetCppFile(m.decl_file);
		if(local)
			if(gitem.GetCount() && gitem.TopKey() == g)
				gitem.Top().Add(&m);
			else
				gitem.Add(g).Add(&m);
		else
			gitem.GetAdd(g).Add(&m);
	}
}

force_inline
bool AssistEditor::SortByLines(const NavItem *a, const NavItem *b)
{
	return CombineCompare(a->decl_file, b->decl_file)(a->decl_line, b->decl_line) < 0;
}

force_inline
bool AssistEditor::SortByNames(const NavItem *a, const NavItem *b)
{
	return CombineCompare(a->name, b->name)(a->qitem, b->qitem) < 0;
}

void AssistEditor::Search()
{
	sortitems.Check(sorting);
	int sc = scope.GetScroll();
	String key = scope.GetKey();
	String s = TrimBoth(~search);
	String search_name, search_nest;
	bool wholeclass = false;
	bool both = false;
	navigator_global = false;
	if(s.Find('.') >= 0) {
		Vector<String> h = Split((String)~search, '.');
		if(*s.Last() == '.')
			search_nest = Join(h, "::");
		else {
			search_name = h.Pop();
			if(h.GetCount())
				search_nest = Join(h, "::");
		}
		wholeclass = *s == '.' && search_nest.GetCount();
	}
	else {
		search_name = search_nest = ~search;
		both = true;
	}
	s = Join(Split(s, '.'), "::") + (s.EndsWith(".") ? "::" : "");
	int lineno = StrInt(s);
	gitem.Clear();
	nitem.Clear();
	int fileii = GetCppFileIndex(theide->editfile);
	if(!IsNull(lineno)) {
		NavItem& m = nitem.Add();
		m.type = "Go to line " + AsString(lineno);
		m.kind = KIND_LINE;
		m.line = lineno;
		gitem.Add(Null).Add(&m);
	}
	else
	if(IsNull(s) && !sorting) {
		const CppBase& b = CodeBase();
		for(int i = 0; i < b.GetCount(); i++) {
			String nest = b.GetKey(i);
			const Array<CppItem>& ci = b[i];
			for(int j = 0; j < ci.GetCount(); j++) {
				const CppItem& m = ci[j];
				if(m.file == fileii) {
					NavItem& n = nitem.Add();
					n.Set(m);
					n.nest = nest;
					n.decl_line = m.line;
					n.decl_file = m.file;
					n.decl = !m.impl;
					NavLine& l = n.linefo.Add();
					l.impl = m.impl;
					l.file = m.file;
					l.line = m.line;
				}
			}
		}
		Sort(nitem, FieldRelation(&NavItem::line, StdLess<int>()));
		NavGroup(true);
	}
	else {
		navigator_global = true;
		const CppBase& b = CodeBase();
		ArrayMap<String, NavItem> imap;
		bool local = sorting && IsNull(s);
		for(int i = 0; i < b.GetCount(); i++) {
			String nest = b.GetKey(i);
			bool foundnest = wholeclass ? ToUpper(nest) == search_nest
			                            : ToUpper(nest).Find(search_nest) >= 0;
			if(local || foundnest || both) {
				const Array<CppItem>& ci = b[i];
				for(int j = 0; j < ci.GetCount(); j++) {
					const CppItem& m = ci[j];
					if(local ? m.file == fileii : m.uname.Find(search_name) >= 0 || both && foundnest) {
						String key = nest + '\1' + m.qitem;
						int q = imap.Find(key);
						if(q < 0) {
							NavItem& ni = imap.Add(key);
							ni.Set(m);
							ni.nest = nest;
							ni.decl_line = ni.line;
							ni.decl_file = ni.file;
							ni.decl = !ni.impl;
							NavLine& l = ni.linefo.Add();
							l.impl = m.impl;
							l.file = m.file;
							l.line = m.line;
						}
						else {
							NavItem& mm = imap[q];
							String n = mm.natural;
							if(m.natural.GetCount() > mm.natural.GetCount())
								mm.natural = m.natural;
							if(!m.impl &&
							  (!mm.decl
							    || CombineCompare(mm.decl_file, m.file)(mm.decl_line, m.line) < 0)) {
									mm.decl = true;
									mm.decl_line = m.line;
									mm.decl_file = m.file;
							}
							NavLine& l = mm.linefo.Add();
							l.impl = m.impl;
							l.file = m.file;
							l.line = m.line;
						}
					}
				}
			}
		}
		nitem = imap.PickValues();
		NavGroup(false);
		SortByKey(gitem);
		Index<String> sc;
		for(int i = 0; i < gitem.GetCount(); i++)
			Sort(gitem[i], sorting ? SortByNames : SortByLines);
	}
	scope.Clear();
	scope.Add(Null);
	Index<String> done;
	for(int i = 0; i < gitem.GetCount(); i++) {
		String s = gitem.GetKey(i);
		if(done.Find(s) < 0) {
			done.Add(s);
			scope.Add(gitem.GetKey(i));
		}
	}
	scope.ScrollTo(sc);
	if(!navigator_global || !scope.FindSetCursor(key))
		scope.GoBegin();
}

int AssistEditor::PaintFileName(Draw& w, const Rect& r, String h, Color ink)
{
	h.Remove(0, 1);
	int x = 0;
	String s = GetFileName(GetFileFolder(h)) + "/";
	w.DrawText(r.left, r.top, s, StdFont(), ink);
	x += GetTextSize(s, StdFont()).cx;
	s = GetFileName(h);
	w.DrawText(r.left + x, r.top, s, StdFont().Bold(), ink);
	return x + GetTextSize(s, StdFont().Bold()).cx;
}

int AssistEditor::ScopeDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	if(IsNull(q)) {
		const char *txt = "All";
		w.DrawText(r.left, r.top, txt, StdFont().Bold().Italic(),
		           style & CURSOR ? ink : HighlightSetup::GetHlStyle(INK_KEYWORD).color);
		return GetTextSize(txt, StdFont().Bold().Italic()).cx;
	}
	String h = q;
	if(*h == '\xff')
		return PaintFileName(w, r, h, ink);
	w.DrawText(r.left, r.top, h, StdFont().Bold(), ink);
	return GetTextSize(h, StdFont().Bold()).cx;
}

void AssistEditor::ScopeDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

Size AssistEditor::ScopeDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return Size(DoPaint(w, Size(999999, 999999), q, White(), White(), 0), StdFont().Bold().GetCy());
}

void AssistEditor::Scope()
{
	LTIMING("FINALIZE");
	litem.Clear();
	nest_item.Clear();
	linefo.Clear();
	bool all = scope.GetCursor() <= 0;
	String sc = scope.GetKey();
	for(int i = 0; i < gitem.GetCount(); i++) {
		String grp = gitem.GetKey(i);
		int    kind = KIND_NEST;
		if(*grp == '\xff')
			kind = KIND_FILE;
		if(all) {
			NavItem& m = nest_item.Add();
			m.kind = kind;
			m.type = grp;
			litem.Add(&m);
		}
		else
		if(grp != sc)
			continue;
		const Vector<NavItem *>& ia = gitem[i];
		for(int i = 0; i < ia.GetCount(); i++) {
			NavItem *m = ia[i];
			for(int j = 0; j < m->linefo.GetCount(); j++)
				linefo.GetAdd(m->linefo[j].file).Add(m->linefo[j].line, litem.GetCount());
			litem.Add(m);
		}
	}
	list.SetVirtualCount(litem.GetCount());
}

void AssistEditor::ListLineEnabled(int i, bool& b)
{
	if(i >= 0 && i < litem.GetCount()) {
		int kind = litem[i]->kind;
		if(findarg(kind, KIND_FILE, KIND_NEST) >= 0)
			b = false;
	}
}

void AssistEditor::NaviSort()
{
	sorting = !sorting;
	Search();
}
