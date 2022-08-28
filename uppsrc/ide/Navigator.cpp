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
	static int maxascent = MaxAscent(StdFont());
	Size sz = GetTextSize(text, font);
	w.DrawText(x, y + maxascent - font.GetAscent(), text, font, ink);
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
	scope_display.navigator = this;
	scope.NoHeader();
	scope.AddColumn().SetDisplay(scope_display);
	scope.NoWantFocus();
	scope.WhenSel = THISBACK(Scope);
	scope.WhenLeftDouble = THISBACK(ScopeDblClk);

	list.NoHeader();
	list.AddRowNumColumn().SetDisplay(navidisplay);
	list.SetLineCy(max(16, GetStdFontCy()));
	list.NoWantFocus();
	list.WhenLeftClick = THISBACK(NavigatorClick);
	list.WhenLineEnabled = THISBACK(ListLineEnabled);
	
	search <<= THISBACK(TriggerSearch);
	search.SetFilter(CharFilterNavigator);
	search.WhenEnter = THISBACK(NavigatorEnter);
	
	sortitems.Image(BrowserImg::Sort());
	sortitems <<= THISBACK(NaviSort);
	sorting = false;
	
	dlgmode = false;
}

AnnotationItem AssistEditor::FindCurrentAnnotation()
{
	Point pos = GetCurrentPos();
	AnnotationItem q;
	bool line1st = true;
	for(const AnnotationItem& m : annotations)
		if(m.pos.y < pos.y)
			q = m;
		else
		if(m.pos.y == pos.y) {
			if(line1st) {
				q = m;
				line1st = false;
			}
			else
			if(m.pos.x < pos.x)
				q = m;
		}
		else
			break;
	return q;
}

void Navigator::SyncCursor()
{
	String k = "(" + GetKeyDesc(IdeKeys::AK_GOTO().key[0]) + ") ";
	search.NullText("Symbol/lineno " + k);
	search.Tip(IsNull(search) ? String() : "Clear " + k);
	
	if(!navigating && !navigator_global) {
		AnnotationItem q = theide->editor.FindCurrentAnnotation();
		navigating = true;
		for(int i = 0; i < list.GetCount(); i++) {
			const NavItem& m = *litem[i];
			if(m.id == q.id && m.pos == q.pos) {
				list.SetCursor(i);
				break;
			}
		}
		navigating = false;
	}

	if(scope.IsCursor())
		scope.RefreshRow(scope.GetCursor());
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
		DLOG("Navigate");
		DDUMP(m.path);
		DDUMP(m.pos);
		if(m.kind == KIND_LINE || IsNull(search)) {
			theide->GotoPos(Null, m.pos);
			if(m.kind == KIND_LINE) { // Go to line - restore file view
				search.Clear();
				Search();
				navigating = false;
			}
			SyncCursor();
		}
		else
		if(m.kind == KIND_SRCFILE) {
			theide->AddHistory();
			theide->EditFile(m.id);
			theide->AddHistory();
		}
		else {
			theide->Cycle(m, GetCurrentLine(), true);
		}
	}
	navigating = false;
}

void Navigator::ScopeDblClk()
{
	if(!scope.IsCursor())
		return;
	String h = scope.GetKey();
	if((byte)*h == 0xff)
		theide->GotoPos(h.Mid(1), 1);
	else {
		list.GoBegin();
		Navigate();
	}
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

void Ide::SwitchHeader()
{
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

void Navigator::NavigatorDisplay::PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	int ii = q;
	if(ii < 0 || ii >= item.GetCount())
		return;
	const NavItem& m = *item[ii];
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
	if(findarg(m.kind, KIND_NEST) >= 0)
		w.DrawRect(r, focuscursor ? paper : m.pretty.Find('\xff') >= 0 ? SColorFace()
		                                  : Blend(SColorMark, SColorPaper, 220));
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
	int y = r.top + (r.GetHeight() - Draw::GetStdFontCy()) / 2;
	
	if(m.kind == KIND_NEST) {
		bool fn = m.pretty.Find('\xff') >= 0;
		w.DrawRect(r, focuscursor ? paper : fn ? SColorFace()
		                                  : Blend(SColorMark(), SColorPaper(), 220));
		if(fn)
			return PaintFileName(w, r, m.pretty, ink);
		w.DrawText(x, y, m.pretty, StdFont().Bold(), ink);
		return GetTextSize(m.pretty, StdFont().Bold()).cx;
	}

	w.DrawRect(r, paper);

	if(m.kind == KIND_SRCFILE)
		return PaintFileName(w, r, m.pretty, ink);

	if(m.kind == KIND_LINE) {
		w.DrawText(x, y, m.pretty, StdFont().Bold(), ink);
		return GetTextSize(m.pretty, StdFont().Bold()).cx;
	}

	return PaintCpp(w, r, m.kind, m.name, m.pretty, ink, focuscursor, true);
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
		search_name = ~search;
		both = true;
	}
	s = Join(Split(s, '.'), "::") + (s.EndsWith(".") ? "::" : "");
	int lineno = StrInt(s);
	nitem.Clear();
	Index<String> nests;
	auto Nest = [&](const AnnotationItem& m, const String& path) {
		if(m.nspace == m.nest)
			return m.nest + "\xff" + path;
		return m.nest;
	};
	nests.Add(Null);
	if(!IsNull(lineno)) {
		NavItem& m = nitem.Add();
		m.pretty = "Go to line " + AsString(lineno);
		m.kind = KIND_LINE;
		m.pos.y = lineno;
		m.pos.x = 0;
	}
	else
	if(IsNull(s) && !sorting) {
		bool sch = GetFileExt(theide->editfile) == ".sch";
		Ide *theide = TheIde();
		if(theide)
			for(const AnnotationItem& m : theide->editor.annotations) {
				NavItem& n = nitem.Add();
				(AnnotationItem&)n = m;
				n.path = theide->editfile;
				nests.FindAdd(n.nest = Nest(m, theide->editfile));
			}
		SortIndex(nests);
	}
	else {
		navigator_global = true;
		String usearch_nest = ToUpper(search_nest);
		String usearch_name = ToUpper(search_name);
		SortByKey(CodeIndex());
		Index<String> set;
		for(int definition = 0; definition < 2; definition++) // we prefer header order
			for(int pass = 0; pass < 3; pass++)
				for(const auto& f : ~CodeIndex())
					for(const AnnotationItem& m : f.value.items) {
						if(m.definition == definition && set.Find(m.id) < 0 &&
						   (pass == 0 ? m.name == search_name : pass == 1 ? m.name.StartsWith(search_name) : m.uname.Find(usearch_name) >= 0) &&
						   (pass == 2 ? m.unest.Find(usearch_nest) >= 0 : m.nest.Find(search_nest) >= 0)) {
							NavItem& n = nitem.Add();
							(AnnotationItem&)n = m;
							n.path = f.key;
							nests.FindAdd(n.nest = Nest(m, theide->editfile));
							set.Add(m.id);
							if(usearch_name.GetCount()) {
								DDUMP(f.key);
								DDUMP(m.pos);
								DDUMP(m.nest);
								DDUMP(m.id);
							}
						}
					}
		
		SortIndex(nests);
		const Workspace& wspc = GetIdeWorkspace();
		String s = ToUpper(TrimBoth(~search));
		for(int i = 0; i < wspc.GetCount(); i++) {
			const Package& p = wspc.GetPackage(i);
			for(int j = 0; j < p.GetCount(); j++) {
				if(!p[j].separator && ToUpper(p[j]).Find(s) >= 0) {
					NavItem& m = nitem.Add();
					m.kind = KIND_SRCFILE;
					m.pretty = wspc[i] + "/" + p[j];
					m.id = SourcePath(wspc[i], p[j]);
					m.pos = Point(0, 0);
					m.nest = "<files>";
					nests.FindAdd(m.nest);
				}
			}
		}
	}
	scope.Clear();
	for(String n : nests)
		scope.Add(n);
	scope.ScrollTo(sc);
	if(!navigator_global || !scope.FindSetCursor(key))
		scope.GoBegin();
}

int Navigator::ScopeDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	if(IsNull(q)) {
		const char *txt = "* ";
		int x = 0;
		w.DrawText(r.left, r.top, txt, StdFont().Bold().Italic(),
		           style & CURSOR ? ink : HighlightSetup::GetHlStyle(HighlightSetup::INK_KEYWORD).color);
		x += GetTextSize(txt, StdFont().Bold().Italic()).cx;
		int ii = navigator->list.GetCursor();
		if(ii >= 0 && ii < navigator->litem.GetCount()) {
			const NavItem& m = *navigator->litem[ii];
			String txt = m.nest;
			if(IsFunction(m.kind))
				txt << "::" << m.name;
			w.DrawText(r.left + x, r.top, txt, StdFont().Bold(), ink);
			x += GetTextSize(txt, StdFont().Bold()).cx;
		}
		return x;
	}
	String h = q;
	if(h.Find('\xff') >= 0) // TODO
		return PaintFileName(w, r, h, ink);
	else
		h = FormatNest(h);
	w.DrawText(r.left, r.top, h, StdFont(), ink);
	return GetTextSize(h, StdFont()).cx;
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
	String sc = scope.GetKey();
	String nest;
	for(const NavItem& n : nitem)
		if(IsNull(sc) || n.nest == sc) {
			if(!sorting && n.nest != nest) {
				NavItem& m = nest_item.Add();
				m.kind = KIND_NEST;
				nest = m.pretty = n.nest;
				litem.Add(&m);
			}
			litem.Add(&n);
		}
	
	// TODO sorting
		
	int lsc = list.GetScroll();
	list.Clear();
	list.SetVirtualCount(litem.GetCount());
	list.ScrollTo(lsc);
}

void Navigator::ListLineEnabled(int i, bool& b)
{
	if(i >= 0 && i < litem.GetCount()) {
		int kind = litem[i]->kind;
		if(findarg(kind, KIND_NEST) >= 0)
			b = false;
	}
}

void Navigator::NaviSort()
{
	sorting = !sorting;
	Search();
}
