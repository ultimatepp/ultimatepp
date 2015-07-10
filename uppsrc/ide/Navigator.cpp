#include "ide.h"

#define LTIMING(x) // RTIMING(x)

String FormatNest(String nest)
{
	int q = nest.Find("@");
	if(q >= 0) {
		nest.Trim(q);
		nest << "[anonymous]";
	}
	return nest;
}

int CharFilterNavigator(int c)
{
	return c == ':' ? '.' : IsAlNum(c) || c == '_' || c == '.' ? c : 0;
}

void PaintTeXt(Draw& w, int& x, int y, const String& text, Font font, Color ink)
{
	Size sz = GetTextSize(text, font);
	w.DrawText(x, y, text, font, ink);
	x += sz.cx;
}

int DrawFileName0(Draw& w, const Rect& r, const String& h, Color ink, int x)
{
	if(h.GetCount() == 0)
		return 0;
	int q = h.Find("\xff");
	String ns;
	String fn = h;
	if(q >= 0) {
		ns = h.Mid(0, q) + ' ';
		fn = h.Mid(q + 1);
	}
	String s = GetFileName(GetFileFolder(h)) + "/";
	x += r.left;
	if(ns.GetCount()) {
		PaintTeXt(w, x, r.top, ns, StdFont().Bold(), ink);
		PaintTeXt(w, x, r.top, "(", StdFont(), ink);
	}
	PaintTeXt(w, x, r.top, s, StdFont(), ink);
	s = GetFileName(h);
	PaintTeXt(w, x, r.top, s, StdFont().Bold(), ink);
	if(ns.GetCount())
		PaintTeXt(w, x, r.top, ")", StdFont(), ink);
	return x - r.left;
}

Size GetDrawFileNameSize(const String& h)
{
	NilDraw w;
	return Size(DrawFileName0(w, Size(999999, 999999), h, Null, 0), StdFont().Bold().GetCy());
}

void DrawFileName(Draw& w, const Rect& r, const String& h, Color ink)
{
	DrawFileName0(w, r, h, ink, min(r.GetWidth() - GetDrawFileNameSize(h).cx, 0));
}

int PaintFileName(Draw& w, const Rect& r, String h, Color ink)
{
	if(*h == '\xff')
		h.Remove(0, 1);
	return DrawFileName0(w, r, h, ink, 0);
}

Navigator::Navigator()
:	navidisplay(litem)
{
	scope.NoHeader();
	scope.AddColumn().SetDisplay(Single<ScopeDisplay>());
	scope.NoWantFocus();
	scope.WhenSel = THISBACK(Scope);

	list.NoHeader();
	list.AddRowNumColumn().SetDisplay(navidisplay);
	list.SetLineCy(max(16, GetStdFontCy()));
	list.NoWantFocus();
	list.WhenLeftClick = THISBACK(NavigatorClick);
	list.WhenSel = THISBACK(SyncNavLines);
	list.WhenLineEnabled = THISBACK(ListLineEnabled);
	
	navlines.NoHeader().NoWantFocus();
	navlines.WhenLeftClick = THISBACK(GoToNavLine);
	navlines.AddColumn().SetDisplay(Single<LineDisplay>());

	search <<= THISBACK(TriggerSearch);
	search.SetFilter(CharFilterNavigator);
	search.WhenEnter = THISBACK(NavigatorEnter);
	
	sortitems.Image(BrowserImg::Sort());
	sortitems <<= THISBACK(NaviSort);
	sorting = false;
	
	dlgmode = false;
}

void Navigator::SyncCursor()
{
	String k = "(" + GetKeyDesc(IdeKeys::AK_GOTO().key[0]) + ") ";
	search.NullText("Symbol/lineno " + k);
	search.Tip(IsNull(search) ? String() : "Clear " + k);

	if(!navigating && theide->editfile.GetCount()) {
		navlines.KillCursor();
		int q = linefo.Find(GetSourceFileIndex(theide->editfile));
		if(q < 0)
			return;
		navigating = true;
		SortedVectorMap<int, int>& m = linefo[q];
		q = m.FindUpperBound(GetCurrentLine() + 1) - 1;
		if(q >= 0 && q < m.GetCount())
			list.SetCursor(m[q]);
		navigating = false;
	}
	SyncLines();
}

void Navigator::SyncLines()
{
	if(IsNull(theide->editfile) || navigating)
		return;
	int ln = GetCurrentLine() + 1;
	int fi = GetSourceFileIndex(theide->editfile);
	int q = -1;
	for(int i = 0; i < navlines.GetCount(); i++) {
		const NavLine& l = navlines.Get(i, 0).To<NavLine>();
		if(l.file == fi && l.line <= ln && i < navlines.GetCount())
			q = i;
	}
	if(dlgmode)
		navlines.GoBegin();
	else
	if(q >= 0)
		navlines.SetCursor(q);
}

void Navigator::SyncNavLines()
{
	int sc = navlines.GetScroll();
	navlines.Clear();
	int ii = list.GetCursor();
	if(ii >= 0 && ii < litem.GetCount()) {
		Vector<NavLine> l = GetNavLines(*litem[ii]);
		for(int i = 0; i < l.GetCount(); i++) {
			String p = GetSourceFilePath(l[i].file);
			navlines.Add(RawToValue(l[i]));
		}
		navlines.ScrollTo(sc);
		SyncLines();
	}
}

int Navigator::LineDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style, int x) const
{
	w.DrawRect(r, paper);
	const NavLine& l = q.To<NavLine>();
	x += r.left;
	String p = GetSourceFilePath(l.file);
	int y = r.top + (r.GetHeight() - StdFont().GetCy()) / 2;
	PaintTeXt(w, x, y, GetFileName(GetFileFolder(p)) + "/", StdFont(), ink);
	PaintTeXt(w, x, y, GetFileName(p), StdFont().Bold(), ink);
	PaintTeXt(w, x, y, " (", StdFont(), ink);
	PaintTeXt(w, x, y, AsString(l.line), StdFont().Bold(), ink);
	PaintTeXt(w, x, y, ")", StdFont(), ink);
	return x - r.left;
}

void Navigator::LineDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style, min(r.GetWidth() - GetStdSize(q).cx, 0));
}

Size Navigator::LineDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return Size(DoPaint(w, Size(999999, 999999), q, White(), White(), 0, 0), StdFont().Bold().GetCy());
}

void Navigator::GoToNavLine()
{
	if(dlgmode)
		return;
	int ii = navlines.GetClickPos().y;
	if(ii >= 0 && ii < navlines.GetCount() && theide) {
		const NavLine& l = navlines.Get(ii, 0).To<NavLine>();
		theide->GotoPos(GetSourceFilePath(l.file), l.line);
	}
}

bool Navigator::NavLine::operator<(const NavLine& b) const
{
	String p1 = GetSourceFilePath(file); // 
	String p2 = GetSourceFilePath(b.file);
	return CombineCompare/*(!impl, !b.impl)*/
	                     (GetFileExt(p2), GetFileExt(p1)) // .h > .c
	                     (GetFileName(p1), GetFileName(p2))
	                     (p1, p2)
	                     (line, b.line) < 0;
}

Vector<Navigator::NavLine> Navigator::GetNavLines(const NavItem& m)
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

void Navigator::Navigate()
{
	if(navigating)
		return;
	navigating = true;
	int ii = list.GetCursor();
	if(theide && ii >= 0 && ii < litem.GetCount()) {
		int ln = GetCurrentLine() + 1;
		const NavItem& m = *litem[ii];
		if(m.kind == KIND_LINE || IsNull(search)) {
			theide->GotoPos(Null, m.line);
			if(m.kind == KIND_LINE) { // Go to line - restore file view
				search.Clear();
				Search();
				navigating = false;
			}
			SyncCursor();
		}
		else {
			Vector<NavLine> l = GetNavLines(m);
			int q = l.GetCount() - 1;
			for(int i = 0; i < l.GetCount(); i++)
				if(GetSourceFilePath(l[i].file) == NormalizeSourcePath(theide->editfile) && l[i].line == ln) {
					q = (i + l.GetCount() + 1) % l.GetCount();
					break;
				}
			if(q >= 0 && q < l.GetCount()) {
				String path = GetSourceFilePath(l[q].file);
				if(!theide->GotoDesignerFile(path, m.nest, m.name, l[q].line))
					theide->GotoPos(path, l[q].line);
			}
		}
	}
	navigating = false;
}

void Navigator::NavigatorClick()
{
	if(dlgmode)
		return;
	int q = list.GetClickPos().y;
	if(q >= 0 && q < list.GetCount())
		Navigate();
}

void Navigator::NavigatorEnter()
{
	if(list.GetCount()) {
		list.GoBegin();
		Navigate();
	}
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
		editor.SetFocus();
	}
	else {
		String h = editor.GetWord();
		if(h.GetCount()) {
			editor.search <<= h;
			editor.search.SelectAll();
			editor.Search();
		}
		editor.search.SetFocus();
	}
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

void Navigator::NavItem::Set(const CppItem& m)
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
	pass = false;
}

void Navigator::NavigatorDisplay::PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
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


int Navigator::NavigatorDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
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
		String h = FormatNest(m.type);
		w.DrawText(x, y, h, StdFont().Bold(), ink);
		return GetTextSize(h, StdFont().Bold()).cx;
	}
	
	w.DrawRect(r, paper);
	if(m.kind == KIND_LINE) {
		w.DrawText(x, y, m.type, StdFont().Bold(), ink);
		return GetTextSize(m.type, StdFont().Bold()).cx;
	}

	PaintCppItemImage(w, x, ry, m.access, m.kind, focuscursor);	

	x += 15;
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

void Navigator::NavigatorDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

Size Navigator::NavigatorDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return Size(DoPaint(w, Size(999999, 999999), q, White(), White(), 0), Draw::GetStdFontCy());
}

void Navigator::TriggerSearch()
{
	search_trigger.KillSet(100, THISBACK(Search));
}

void Navigator::NavGroup(bool local)
{
	for(int i = 0; i < nitem.GetCount(); i++) {
		NavItem& m = nitem[i];
		String g = m.nest;
		if(m.kind == TYPEDEF)
			g.Trim(max(g.ReverseFind("::"), 0));
		if(IsNull(m.nest) || CodeBase().namespaces.Find(m.nest) >= 0) {
			if(g.GetCount()) // We want to show the namespace
				g << '\xff';
			else
				g.Clear();
			g << GetSourceFilePath(m.decl_file);
			g = '\xff' + g;
		}
		if(!local)
			g = (char)(m.pass + 10) + g;
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
bool Navigator::SortByLines(const NavItem *a, const NavItem *b)
{
	return CombineCompare(a->decl_file, b->decl_file)(a->decl_line, b->decl_line) < 0;
}

force_inline
bool Navigator::SortByNames(const NavItem *a, const NavItem *b)
{
	return CombineCompare(a->name, b->name)(a->qitem, b->qitem) < 0;
}

void Navigator::Search()
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
	if(IsNull(theide->editfile))
		return;
	int fileii = GetSourceFileIndex(theide->editfile);
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
		String usearch_nest = ToUpper(search_nest);
		String usearch_name = ToUpper(search_name);
		ArrayMap<String, NavItem> imap;
		bool local = sorting && IsNull(s);
		for(int pass = -1; pass < 2; pass++) {
			for(int i = 0; i < b.GetCount(); i++) {
				String nest = b.GetKey(i);
				bool foundnest = (wholeclass ? pass < 0 ? false :
				                               pass ? ToUpper(nest) == usearch_nest 
				                                    : nest == search_nest
				                             : pass < 0 ? nest == search_nest :
				                               (pass ? ToUpper(nest).Find(usearch_nest) >= 0
				                                     : nest.StartsWith(search_nest)))
				                 && *nest != '@';
				if(local || foundnest || both) {
					const Array<CppItem>& ci = b[i];
					for(int j = 0; j < ci.GetCount(); j++) {
						const CppItem& m = ci[j];
						if(local ? m.file == fileii
						         : *m.uname != '@' && (pass < 0 ? m.name == search_name :
						                               pass ? m.uname.Find(usearch_name) >= 0
						                                    : m.name.StartsWith(search_name))
						           || both && foundnest) {
							String key = nest + '\1' + m.qitem;
							int q = imap.Find(key);
							if(q < 0) {
								NavItem& ni = imap.Add(key);
								ni.Set(m);
								ni.nest = nest;
								ni.decl_line = ni.line;
								ni.decl_file = ni.file;
								ni.decl = !ni.impl;
								ni.pass = pass;
								NavLine& l = ni.linefo.Add();
								l.impl = m.impl;
								l.file = m.file;
								l.line = m.line;
							}
							else {
								NavItem& mm = imap[q];
								String n = mm.natural;
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
		}
		nitem = imap.PickValues();
		NavGroup(false);
		SortByKey(gitem);
		Vector<String> keys = gitem.PickKeys();
		Vector<Vector<NavItem *> > values = gitem.PickValues();
		IndexSort(keys, values);
		for(int i = 0; i < keys.GetCount(); i++)
			keys[i].Remove(0);
		VectorMap<String, Vector<NavItem *> > h(pick(keys), pick(values));
		gitem = pick(h);
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

int Navigator::ScopeDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	if(IsNull(q)) {
		const char *txt = "All";
		w.DrawText(r.left, r.top, txt, StdFont().Bold().Italic(),
		           style & CURSOR ? ink : HighlightSetup::GetHlStyle(HighlightSetup::INK_KEYWORD).color);
		return GetTextSize(txt, StdFont().Bold().Italic()).cx;
	}
	String h = q;
	if(*h == '\xff')
		return PaintFileName(w, r, h, ink);
	else
		h = FormatNest(h);
	w.DrawText(r.left, r.top, h, StdFont().Bold(), ink);
	return GetTextSize(h, StdFont().Bold()).cx;
}

void Navigator::ScopeDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	DoPaint(w, r, q, ink, paper, style);
}

Size Navigator::ScopeDisplay::GetStdSize(const Value& q) const
{
	NilDraw w;
	return Size(DoPaint(w, Size(999999, 999999), q, White(), White(), 0), StdFont().Bold().GetCy());
}

void Navigator::Scope()
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
			m.type = FormatNest(grp);
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
	list.Clear();
	list.SetVirtualCount(litem.GetCount());
}

void Navigator::ListLineEnabled(int i, bool& b)
{
	if(i >= 0 && i < litem.GetCount()) {
		int kind = litem[i]->kind;
		if(findarg(kind, KIND_FILE, KIND_NEST) >= 0)
			b = false;
	}
}

void Navigator::NaviSort()
{
	sorting = !sorting;
	Search();
}
