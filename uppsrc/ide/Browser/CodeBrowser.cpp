#include "Browser.h"

bool MatchCib(const String& s, const String& match)
{
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
	int q = BrowserBase().Find("");
	if(q < 0)
		return false;
	const Array<CppItem>& n = BrowserBase()[q];
	for(int i = 0; i < n.GetCount(); i++)
		if(n[i].file == file && n[i].uname.StartsWith(m))
			return true;
	return false;
}

void CodeBrowser::Load()
{
	String find = ToUpper((String)~search);
	String match = ToUpper((String)~search_scope);
	Vector<String> txt;
	Vector<Value>  ndx;
	Index<int>     fi;
	for(int i = 0; i < BrowserBase().GetCount(); i++) {
		String s = BrowserBase().GetKey(i);
		const Array<CppItem>& n = BrowserBase()[i];
		if(s.GetCount()) {
			if(MatchCib(s, match) && HasItem(n, find)) {
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
	for(int i = 0; i < fi.GetCount(); i++) {
		String s = GetCppFile(fi[i]);
		s = '<' + GetFileName(GetFileFolder(s)) + '/' + GetFileName(s) + '>';
		if(MatchCib(s, match) && HasItem(fi[i], find)) {
			txt.Add(s);
			ndx.Add(fi[i]);
		}
	}
	IndexSort(txt, ndx);
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
	int q = BrowserBase().Find(type);
	if(q < 0) return;
	const Array<CppItem>& n = BrowserBase()[q];
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
	int q = BrowserBase().Find(scope);
	if(q >= 0) {
		const Array<CppItem>& n = BrowserBase()[q];
		VectorMap<String, bool> inherited;
		if(file < 0)
			GatherMethods(scope, inherited, false);
		for(int i = 0; i < n.GetCount(); i = FindNext(n, i)) {
			CppItemInfo m;
			(CppItem&) m = n[i];
			if((file < 0 || m.file == file) && m.uname.StartsWith(match) && m.uname.StartsWith(find)) {
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
}

String CodeBrowser::GetCodeRef()
{
	if(scope.IsCursor() && item.IsCursor())
		return MakeCodeRef(scope.GetKey(), item.GetKey());
	return Null;
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
			search_scope <<= Null;
			search_item <<= Null;
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
	search <<= THISBACK(Load);
}
