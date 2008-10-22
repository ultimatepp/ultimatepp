#include "Browser.h"

bool MatchCib(const String& s, const String& match)
{
	if(IsNull(match)) return true;
	int q = ToUpper(s).Find(match);
	return q > 0 && !iscid(s[q - 1]) || q == 0;	
}

bool HasItem(const Array<CppItem>& n, const String& m)
{
	if(IsNull(m))
		return true;
	for(int i = 0; i < n.GetCount(); i++)
		if(n[i].uname.StartsWith(m))
			return true;
	return false;
}

bool HasItem(int file, const String& m)
{
	if(IsNull(m))
		return true;
	int q = CodeBase().Find("");
	if(q < 0)
		return false;
	const Array<CppItem>& n = CodeBase()[q];
	for(int i = 0; i < n.GetCount(); i++)
		if(n[i].file == file && n[i].uname.StartsWith(m))
			return true;
	return false;
}

struct ScopeLess {
	bool operator()(const String& a, const String& b) const {
		if((*a == '<') != (*b == '<'))
			return *a > *b;
		return a < b; 
	}
};

void CodeBrowser::Load()
{
	String find = ToUpper((String)~search);
	String match = ToUpper((String)~search_scope);
	Vector<String> txt;
	Vector<Value>  ndx;
	Index<int>     fi;
	for(int i = 0; i < CodeBase().GetCount(); i++) {
		String s = CodeBase().GetKey(i);
		const Array<CppItem>& n = CodeBase()[i];
		if(s.GetCount()) {
			if(MatchCib(s, match) && (MatchCib(s, find) || HasItem(n, find))) {
				txt.Add(s);
				ndx.Add(s);
			}
		}
		else {
			int i = 0;
			while(i < n.GetCount()) {
				fi.FindAdd(n[i].file);
				i = FindNext(n, i);
			}
		}
	}
	if(find.GetCount()) {
		txt.Add("<globals>");
		ndx.Add(Null);
	}
	else
		for(int i = 0; i < fi.GetCount(); i++) {
			String s = GetCppFile(fi[i]);
			s = '<' + GetFileName(GetFileFolder(s)) + '/' + GetFileName(s) + '>';
			if(MatchCib(s, match)) {
				txt.Add(s);
				ndx.Add(fi[i]);
			}
		}
	IndexSort(txt, ndx, ScopeLess());
	Value key = scope.GetKey();
	int sc = scope.GetCursorSc();
	scope.Clear();
	for(int i = 0; i < txt.GetCount(); i++)
		scope.Add(IsString(ndx[i]) ? ndx[i] : Null, txt[i], ndx[i]);
	if(scope.FindSetCursor(key))
		scope.ScCursor(sc);
}

int ItemCompare(const Value& v1, const Value& v2)
{
	const CppItemInfo& a = ValueTo<CppItemInfo>(v1);
	const CppItemInfo& b = ValueTo<CppItemInfo>(v2);
	int q = a.inherited - b.inherited;
	if(q) return q;
	q = SgnCompare(GetCppFile(a.file), GetCppFile(b.file));
	return q ? q : a.line - b.line;
}

void GatherMethods(const String& type, VectorMap<String, bool>& inherited, bool g)
{
	int q = CodeBase().Find(type);
	if(q < 0) return;
	const Array<CppItem>& n = CodeBase()[q];
	for(int i = 0; i < n.GetCount(); i = FindNext(n, i)) {
		const CppItem& m = n[i];
		if(m.IsType()) {
			Vector<String> base = Split(m.qptype, ';');
			for(int i = 0; i < base.GetCount(); i++)
				GatherMethods(base[i], inherited, true);
		}
		if(m.IsCode() && g) {
			bool& virt = inherited.GetAdd(m.qitem);
			virt = virt || m.virt;
		}
	}
}

void CodeBrowser::LoadScope()
{
	String find = ToUpper((String)~search);
	String match = ToUpper((String)~search_item);
	Value key = item.GetKey();
	int sc = item.GetCursorSc();
	item.Clear();
	if(!scope.IsCursor())
		return;
	Value x = scope.Get(2);
	int file = IsNumber(x) ? (int)x : -1;
	String scope = file < 0 ? String(x) : String();
	int q = CodeBase().Find(scope);
	bool all = scope.GetCount() && MatchCib(scope, find);
	if(q >= 0) {
		const Array<CppItem>& n = CodeBase()[q];
		VectorMap<String, bool> inherited;
		if(file < 0)
			GatherMethods(scope, inherited, false);
		for(int i = 0; i < n.GetCount(); i = FindNext(n, i)) {
			CppItemInfo m;
			(CppItem&) m = n[i];
			if((file < 0 || m.file == file) && m.uname.StartsWith(match) && (all || m.uname.StartsWith(find))) {
				int q = inherited.Find(m.qitem);
				if(q >= 0) {
					m.over = true;
					m.virt = m.virt || inherited[q];
				}
				item.Add(m.qitem, RawToValue(m));
			}
		}
	}
	item.Sort(1, ItemCompare);
	if(item.FindSetCursor(key))
		item.ScCursor(sc);
	clear.Enable(IsSearch());
}

String CodeBrowser::GetCodeRef(int i) const
{
	if(scope.IsCursor())
		return MakeCodeRef(scope.GetKey(), item.GetKey());
	return Null;
}

String CodeBrowser::GetCodeRef() const
{
	return item.IsCursor() ? GetCodeRef(item.GetCursor()) : String();
}

const CppItemInfo& CodeBrowser::GetItemInfo(int i) const
{
	return ValueTo<CppItemInfo>(item.Get(i, 1));
}

const CppItemInfo& CodeBrowser::GetItemInfo() const
{
	return GetItemInfo(item.GetCursor());
}

int SearchScopeFilter(int c)
{
	c = ToUpper(c);
	return IsAlNum(c) || c == '/' || c == '.' || c == '<' || c == '>' ? c : 0;
}

int SearchItemFilter(int c)
{
	c = ToUpper(c);
	return IsAlNum(c) ? c : 0;
}

void CodeBrowser::Goto(const String& coderef)
{
	if(IsNull(coderef))
		item.KillCursor();
	else
	if(coderef != GetCodeRef()) {
		if(!IsNull(search_item) || !IsNull(search_scope)) {
			Load();
			LoadScope();
		}
		String sc, im;
		SplitCodeRef(coderef, sc, im);
		if(IsNull(sc)) {
			const CppItem *m = GetCodeRefItem(coderef);
			if(m)
				scope.FindSetCursor(m->file, 2);
		}
		else
			scope.FindSetCursor(sc);
		item.FindSetCursor(im);
	}
}

void CodeBrowser::Search()
{
	Load();
	if(!scope.IsCursor())
		scope.SetCursor(0);
}

bool CodeBrowser::Key(dword key, int count)
{
	if(key == K_UP || key == K_DOWN) {
		if(search.HasFocus() || search_item.HasFocus()) {
			EditString& es = search.HasFocus() ? search : search_item;
			int l, h;
			es.GetSelection(l, h);
			Value v = ~es;
			if(item.IsCursor())
				item.Key(key, count);
			else
			if(key == K_UP)
				item.GoEnd();
			else
				item.GoBegin();
			WhenKeyItem();
			es <<= v;
			es.SetFocus();
			es.SetSelection(l, h);
			return true;
		}
		if(search_scope.HasFocus()) {
			scope.Key(key, count);
			return true;
		}
	}
	return false;
}

bool CodeBrowser::IsSearch() const
{
	return !IsNull(search) || !IsNull(search_item) || !IsNull(search);
}

void CodeBrowser::ClearSearch()
{
	if(!IsSearch())
		return;
	search_scope <<= search_item <<= search <<= Null;
	Load();
	WhenClear();
}

CodeBrowser::CodeBrowser()
{
	scope.AddKey();
	scope.AddColumn("Scope");
	scope.WhenSel = THISBACK(LoadScope);
	scope.NoHeader().NoGrid();
	search_scope <<= THISBACK(Load);
	search_scope.SetFilter(SearchScopeFilter);
	search_scope.NullText("Search type or header ");
	item.AddKey();
	item.AddColumn("Item").SetDisplay(display).Margin(2);
	item.NoHeader();
	item.SetLineCy(BrowserFont().Info().GetHeight() + 3);
	search_item.SetFilter(SearchItemFilter);
	search_item.NullText("Search in scope ");
	search_item <<= THISBACK(LoadScope);
	search.NullText("Find ");
	search.SetFilter(SearchItemFilter);
	search <<= THISBACK(Search);
	search.AddFrame(clear);
	clear.SetImage(BrowserImg::Clear());
	clear.NoWantFocus();
	clear <<= THISBACK(ClearSearch);
}
