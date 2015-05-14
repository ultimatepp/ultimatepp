#include "Browser.h"

bool MatchCib(const String& s, const String& match)
{
	if(IsNull(match)) return true;
	int q = ToUpper(s).Find(match);
	return q > 0 && !iscid(s[q - 1]) || q == 0;	
}

bool MatchPm(const String& fn, const String& pm)
{
	if(IsNull(pm))
		return true;
	return fn.StartsWith(pm);
}

bool MatchPm(int file, const String& pm)
{
	if(IsNull(pm))
		return true;
	return GetSourceFilePath(file).StartsWith(pm);
}

bool MatchPm(const Array<CppItem>& n, const String& pm)
{
	if(IsNull(pm))
		return true;
	for(int i = 0; i < n.GetCount(); i++)
		if(MatchPm(n[i].file, pm))
			return true;
	return false;
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

struct ScopeLess {
	bool operator()(const String& a, const String& b) const {
		if((*a == '<') != (*b == '<'))
			return *a > *b;
		return a < b; 
	}
};

String CodeBrowser::GetPm()
{
	String pm;
	if(TheIde() && range) {
		if(range == 1)
			pm = TheIde()->IdeGetNestFolder();
		else {
			pm = TheIde()->IdeGetFileName();
			if(range == 2)
				pm = GetFileFolder(pm);
		}
	}
	return pm;
}

String GetFileText(const String& s)
{
	return '<' + GetFileName(GetFileFolder(s)) + '/' + GetFileName(s) + '>';
}

void CodeBrowser::Load()
{
	String find = ToUpper((String)~search);
	String match = ToUpper((String)~search_scope);
	String pm = GetPm();
	Vector<String> txt;
	Vector<Value>  ndx;
	Index<int>     fi;
	Index<String>  fs;
	for(int i = 0; i < CodeBase().GetCount(); i++) {
		String s = CodeBase().GetKey(i);
		const Array<CppItem>& n = CodeBase()[i];
		if(s.GetCount())
			if(MatchCib(s, match) && (MatchCib(s, find) || HasItem(n, find)) && MatchPm(n, pm)) {
				txt.Add(s);
				ndx.Add(s);
			}
		for(int i = 0; i < n.GetCount(); i++) {
			int f = n[i].file;
			if(fi.Find(f) < 0) {
				String s = GetFileText(GetSourceFilePath(f));
				if(s.StartsWith(pm) && MatchCib(s, match) &&
				   (IsNull(find) || MatchCib(s, find) || n[i].uname.StartsWith(find))) {
					txt.Add(s);
					ndx.Add(f);
					fs.Add(s);
					fi.Add(f);
				}
			}
		}
	}
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		String pn = wspc[i];
		const Package& p = wspc.GetPackage(i);
		String pp = PackageDirectory(pn);
		for(int j = 0; j < p.GetCount(); j++)
			if(!p[j].separator) {
				String fn = AppendFileName(pp, p[j]);
				String s = GetFileText(AppendFileName(pn, p[j]));
				if(fs.Find(s) < 0 && (IsNull(find) || MatchCib(s, find)) && MatchCib(s, match) && MatchPm(fn, pm)) {
					int f = GetSourceFileIndex(SourcePath(pn, p[j]));
					txt.Add(s);
					ndx.Add(f);
					fs.Add(s);
				}
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
//	clear.Enable(IsSearch());
}

int ItemCompare(const Value& v1, const Value& v2)
{
	const CppItemInfo& a = ValueTo<CppItemInfo>(v1);
	const CppItemInfo& b = ValueTo<CppItemInfo>(v2);
	int q = a.inherited - b.inherited;
	if(q) return q;
	q = SgnCompare(GetSourceFilePath(a.file), GetSourceFilePath(b.file));
	return q ? q : a.line - b.line;
}

int ItemCompareLexical(const Value& v1, const Value& v2)
{
	const CppItemInfo& a = ValueTo<CppItemInfo>(v1);
	const CppItemInfo& b = ValueTo<CppItemInfo>(v2);
	int q = (int)b.IsType() - (int)a.IsType();
	if(q) return q;
	q = SgnCompare(a.name, b.name);
	if(q) return q;
	return SgnCompare(a.qitem, b.qitem);
}

void GatherMethods(const String& type, VectorMap<String, bool>& inherited, bool g, Index<String>& done)
{
	if(done.Find(type) >= 0) return;
	done.Add(type);
	int q = CodeBase().Find(type);
	if(q < 0) return;
	const Array<CppItem>& n = CodeBase()[q];
	Index<String> set;
	for(int i = 0; i < n.GetCount(); i++) {
		const CppItem& m = n[i];
		if(set.Find(m.qitem) < 0) {
			set.Add(m.qitem);
			if(m.IsType()) {
				Vector<String> base = Split(m.qptype, ';');
				for(int i = 0; i < base.GetCount(); i++)
					GatherMethods(base[i], inherited, true, done);
			}
			if(m.IsCode() && g) {
				bool& virt = inherited.GetAdd(m.qitem);
				virt = virt || m.virt;
			}
		}
	}
}

void GatherMethods(const String& type, VectorMap<String, bool>& inherited, bool g)
{
	Index<String> done;
	GatherMethods(type, inherited, g, done);
}

void CodeBrowser::LoadScope()
{
	String find = ToUpper((String)~search);
	Value key = item.GetKey();
	int sc = item.GetCursorSc();
	item.Clear();
	if(!scope.IsCursor())
		return;
	Value x = scope.Get(2);
	int file = IsNumber(x) ? (int)x : -1;
	String scope = file < 0 ? String(x) : String();
	int q = CodeBase().Find(scope);
	bool filematch = file >= 0 && MatchCib(GetFileText(GetSourceFilePath(file)), find);
	bool scopematch = !filematch && MatchCib(scope, find);
	if(q >= 0) {
		const Array<CppItem>& n = CodeBase()[q];
		VectorMap<String, bool> inherited;
		if(file < 0)
			GatherMethods(scope, inherited, false);
		Index<String> set;
		for(int i = 0; i < n.GetCount(); i++) {
			CppItemInfo m;
			(CppItem&) m = n[i];
			if((find.GetCount() && m.uname.StartsWith(find) || filematch && m.file == file || scopematch) && set.Find(m.qitem) < 0) {
				set.Add(m.qitem);
				int q = inherited.Find(m.qitem);
				if(q >= 0) {
					m.over = true;
					m.virt = m.virt || inherited[q];
				}
				item.Add(m.qitem, RawToValue(m));
			}
		}
	}
	item.Sort(1, sort ? ItemCompareLexical : ItemCompare);
	if(item.FindSetCursor(key))
		item.ScCursor(sc);
//	clear.Enable(IsSearch());
}

String CodeBrowser::GetCodeRef(int i) const
{
	if(scope.IsCursor())
		return MakeCodeRef(scope.GetKey(), item.Get(i, 0));
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

int SearchItemFilter(int c)
{
	c = ToUpper(c);
	return IsAlNum(c) || c == '/' || c == '.' || c == '<' || c == '>' || c == '_' ? c : 0;
}

void CodeBrowser::Goto(const String& coderef, const String& rfile)
{
	if(IsNull(coderef)) {
		item.KillCursor();
		scope.FindSetCursor(GetFileText(rfile), 1);
		return;
	}
	else
	if(coderef != GetCodeRef()) {
		if(!IsNull(search_scope)) {
			Load();
			LoadScope();
		}
		String sc, im;
		SplitCodeRef(coderef, sc, im);
		if(IsNull(sc)) {
			const CppItem *m = GetCodeRefItem(coderef, rfile);
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
	if(!scope.IsCursor() || !item.IsCursor())
		scope.SetCursor(0);
}

bool CodeBrowser::Key(dword key, int count)
{
//	clear.Enable(IsSearch());
	if(key == K_UP || key == K_DOWN) {
		if(search.HasFocus()) {
			int l, h;
			search.GetSelection(l, h);
			Value v = ~search;
			if(item.IsCursor())
				item.Key(key, count);
			else
			if(key == K_UP)
				item.GoEnd();
			else
				item.GoBegin();
			WhenKeyItem();
			search <<= v;
			search.SetFocus();
			search.SetSelection(l, h);
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
	return !IsNull(search) || !IsNull(search_scope);
}

void CodeBrowser::ClearSearch()
{
	if(!IsSearch())
		return;
	search_scope <<= search <<= Null;
	Load();
	WhenClear();
}

void CodeBrowser::SetRange(int r)
{
	if(range == r)
		r = 0;
	range = r;
	for(int i = 0; i < 3; i++)
		rangebutton[i] <<= range == i + 1;
	Load();
}

CodeBrowser::CodeBrowser()
{
	scope.AddKey();
	scope.AddColumn("Scope");
	scope.WhenSel = THISBACK(LoadScope);
	scope.NoHeader().NoGrid();
	search_scope <<= THISBACK(Load);
	search_scope.SetFilter(SearchItemFilter);
	search_scope.NullText("Search type or header ");
	item.AddKey();
	item.AddColumn("Item").SetDisplay(display).Margin(2);
	item.NoHeader();
	item.SetLineCy(BrowserFont().Info().GetHeight() + 3);
	search.NullText("Find ");
	search.SetFilter(SearchItemFilter);
	search <<= THISBACK(Search);
//	search.AddFrame(clear);
//	clear.SetImage(BrowserImg::Clear());
//	clear.NoWantFocus();
//	clear <<= THISBACK(ClearSearch);
	range = 0;
	static const char *tip[] = { "Nest", "Package", "File" };
	for(int i = 0; i < 3; i++)
		rangebutton[i].SetImage(BrowserImg::Get(BrowserImg::I_range_nest + i)).Tip(tip[i]).NoWantFocus()
		              <<= THISBACK1(SetRange, i + 1);
	SetRange(0);
	sort.Tip("Order by names");
	sort.SetImage(BrowserImg::Sort()).NoWantFocus();
	sort <<= THISBACK(LoadScope);
}
