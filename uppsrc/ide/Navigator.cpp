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
	list.NoHeader();
	list.AddRowNumColumn().SetDisplay(navidisplay);
	list.SetLineCy(max(16, GetStdFontCy()));
	list.NoWantFocus();
	list.WhenLeftClick = THISBACK(NavigatorClick);
	
	scope.NoHeader();
	scope.AddColumn().SetDisplay(Single<ScopeDisplay>());
	scope.SetLineCy(max(16, GetStdFontCy()));
	scope.NoWantFocus();
	scope.WhenSel = [=] { SetList(); };
	
	search <<= THISBACK(TriggerSearch);
	search.SetFilter(CharFilterNavigator);
	search.WhenEnter = THISBACK(NavigatorEnter);
	
	sortitems.Image(BrowserImg::Sort());
	sortitems <<= THISBACK(NaviSort);
	sorting = false;
	
	dlgmode = false;
}

AnnotationItem AssistEditor::FindCurrentAnnotation(bool allow_define)
{
	Point pos = GetCurrentPos();
	AnnotationItem q;
	bool line1st = true;
	for(const AnnotationItem& m : annotations)
		if(allow_define || m.kind != CXCursor_MacroDefinition) {
			if(m.pos.y < pos.y)
				q = m;
			else
			if(m.pos.y == pos.y) {
				if(line1st) {
					q = m;
					line1st = false;
				}
				else
				if(m.pos.x <= pos.x)
					q = m;
			}
			else
				break;
		}
	return q;
}

void Navigator::SyncCursor()
{
	String k = "(" + GetKeyDesc(IdeKeys::AK_GOTO().key[0]) + ") ";
	search.NullText("Symbol/lineno/file " + k);
	search.Tip(IsNull(search) ? String() : "Clear " + k);
	
	if(!navigating && theide && !theide->replace_in_files) {
		AnnotationItem q = theide->editor.FindCurrentAnnotation(true);
		navigating = true;
		for(int pass = 0; pass < 2; pass++)
			for(int i = 0; i < list.GetCount(); i++) {
				const NavItem& m = *litem[i];
				if(m.id == q.id && (pass || m.pos == q.pos)) {
					list.SetCursor(i);
					navigating = false;
					return;
				}
			}
		list.KillCursor();
		navigating = false;
	}
}

void Navigator::Navigate()
{
	if(navigating)
		return;
	search.AddHistory();
	navigating = true;
	int ii = list.GetCursor();
	if(theide && ii >= 0 && ii < litem.GetCount()) {
		const NavItem& m = *litem[ii];
		if(m.kind == KIND_NEST) {
			String h = m.pretty;
			int q = h.Find("\xff");
			if(q >= 0) {
				search.Clear();
				theide->AddHistory();
				theide->EditFile(h.Mid(q + 1));
				theide->AddHistory();
			}
			else {
				String id = h;
				h.Replace("::", ".");
				h << ".";
				search <<= h;
				search.AddHistory();
				Search();
				PostCallback([=] {
					for(int i = 0; i < litem.GetCount(); i++)
						if(IsStruct(litem[i]->kind) && litem[i]->id == id) {
							list.SetCursor(i);
							Navigate();
							return;
						}
				});
			}
		}
		else
		if(m.kind == KIND_SRCFILE) {
			theide->AddHistory();
			theide->EditFile(m.id);
			theide->AddHistory();
		}
		else
		if(m.kind == KIND_LINE) {
			theide->GotoPos(Null, m.pos);
			if(m.kind == KIND_LINE) { // Go to line - restore file view
				search.Clear();
				Search();
				navigating = false;
			}
		}
		else
		if(GetAlt())
			theide->Usage(m.id, m.name, m.pos);
		else
		if(IsNull(search)) // current file - do not cycle
			theide->GotoPos(Null, m.pos);
		else
			theide->Cycle(m, GetCurrentLine(), true);
	}
	navigating = false;
	SyncCursor();
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
	editor.search.AddHistory();
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

void Navigator::NavigatorDisplay::PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	int ii = q;
	if(ii < 0 || ii >= item.GetCount())
		return;
	const NavItem& m = *item[ii];
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
	if(m.kind == KIND_NEST && !focuscursor)
		w.DrawRect(r, Blend(SColorMark, SColorPaper, 220));
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
	
	PaintBackground(w, r, q, ink, paper, style);

	if(m.kind == KIND_NEST) {
		if(m.pretty.Find('\xff') >= 0)
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

int Navigator::ScopeDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	if(IsNull(q) || q == "*") {
		const char *txt = "*";
		int x = 0;
		w.DrawText(r.left, r.top, txt, StdFont().Bold().Italic(),
		           style & CURSOR ? ink : HighlightSetup::GetHlStyle(HighlightSetup::INK_KEYWORD).color);
		x += GetTextSize(txt, StdFont().Bold().Italic()).cx;
		return x;
	}
	String h = q;
	if(h.Find('\xff') >= 0)
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

void Navigator::TriggerSearch()
{
	search_trigger.KillSet(100, THISBACK(Search));
}

void Navigator::Search()
{
	sortitems.Check(sorting);
	String s = TrimBoth(~search);
	String search_name, search_nest;
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
	}
	else
		search_name = ~search;
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
	String usearch_nest = ToUpper(search_nest);
	String usearch_name = ToUpper(search_name);
	if(!IsNull(lineno)) {
		NavItem& m = nitem.Add();
		m.pretty = "Go to line " + AsString(lineno);
		m.kind = KIND_LINE;
		m.pos.y = lineno;
		m.pos.x = 0;
	}
	else
	if(IsNull(s)) {
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
		SortByKey(CodeIndex());
		Index<String> set;
		for(int nest_pass = 0; nest_pass < 3; nest_pass++) {
			for(int name_pass = 0; name_pass < 4; name_pass++)
				for(int source_pass = 0; source_pass < 2; source_pass++) // we prefer header order
					for(const auto& f : ~CodeIndex()) {
						if(source_pass == (findarg(ToLower(GetFileExt(f.key)), ".h", ".hh") < 0))
							for(const AnnotationItem& m : f.value.items) {
								if(set.Find(m.id) < 0 &&
								   (name_pass == 3 ? m.uname.Find(usearch_name) >= 0 :
								    name_pass == 2 ? m.name.Find(search_name) >= 0 :
								    name_pass == 1 ? m.name.StartsWith(search_name) :
								                     m.name == search_name) &&
								   (search_nest.GetCount() == 0 ||
									   (nest_pass == 2 ? m.unest.Find(usearch_nest) >= 0 :
									    nest_pass == 1 ? m.nest.Find(search_nest) >= 0 :
									                     m.nest == search_nest))) {
									NavItem& n = nitem.Add();
									(AnnotationItem&)n = m;
									n.path = f.key;
									nests.FindAdd(n.nest = Nest(m, f.key));
									set.Add(m.id);
								}
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
					m.uname = ToUpper(p[j]);
					nests.FindAdd(m.nest);
				}
			}
		}
	}
	
	String k = scope.GetKey();
	scope.Clear();
	scope.Add("*");
	Index<String> set;
	for(int nest_pass = 0; nest_pass < 3; nest_pass++)
		for(String s : nests) {
			if(s.GetCount() &&
			   set.Find(s) < 0 &&
			   (nest_pass == 2 ? ToUpper(s).Find(usearch_nest) >= 0 :
			    nest_pass == 1 ? s.Find(search_nest) >= 0 :
			                     s == search_nest)) {
				scope.Add(s);
				set.Add(s);
			}
		}
	if(!scope.FindSetCursor(k))
		scope.GoBegin();
	SetList();
}

void Navigator::SetList()
{
	litem.Clear();
	nest_item.Clear();
	String nest;
	String snest = scope.GetKey();
	if(snest == "*")
		snest.Clear();
	for(const NavItem& n : nitem) {
		if(IsNull(snest) || n.nest == snest) {
			if(!sorting && n.nest != nest) {
				NavItem& m = nest_item.Add();
				m.kind = KIND_NEST;
				nest = m.pretty = n.nest;
				litem.Add(&m);
			}
			litem.Add(&n);
		}
	}
	
	if(sorting)
		StableSort(litem, [=](const NavItem *a, const NavItem *b) {
			return a->uname < b->uname;
		});
	
	int lsc = list.GetScroll();
	list.Clear();
	list.SetVirtualCount(litem.GetCount());
	list.ScrollTo(lsc);
}

void Navigator::NaviSort()
{
	sorting = !sorting;
	SetList();
}
