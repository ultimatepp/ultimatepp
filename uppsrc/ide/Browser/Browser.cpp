#include "Browser.h"

#define IMAGECLASS BrowserImg
#define IMAGEFILE <ide/Browser/Browser.iml>
#include <Draw/iml_source.h>

bool MatchItemFile(const CppItem& m, const BrowserQuery& q)
{
	if(IsNull(q.package) && IsNull(q.file))
		return true;
	int w = FileSet().Find(GetCppFile(m.file));
	if(w >= 0) {
		const BrowserFileInfo& f = FileSet()[w];
		if((IsNull(q.package) || q.package == f.package) && (IsNull(q.file) || q.file == f.file))
			return true;
	}
	return false;
}

String JoinNestKey(const String& scope, const String& key)
{
	String s = scope;
	if(s != "::")
		s << "::";
	s << key;
	return s;
}

bool MatchDocumented(const String& scope, const String& key, const String& name, const BrowserQuery& q)
{
	int n = GetRefLinks(JoinNestKey(scope, key)).GetCount();
	return q.notdocumented && q.documented || q.notdocumented && n == 0 || q.documented && n;
}

bool MatchItem(const String& scope, const CppItem& m, const String& key, const String& name, const BrowserQuery& q, bool all)
{
	if(q.a_private && m.access == PRIVATE ||
	   q.a_protected && m.access == PROTECTED ||
	   q.a_public && m.access == PUBLIC)
		if(q.type && m.IsType() ||
		   q.code && m.IsCode() ||
		   q.data && m.IsData() ||
		   q.macro && m.IsMacro())
			if(all || IsNull(q.name) || name == q.name || scope + "::" + name == q.name)
				return MatchItemFile(m, q) && MatchDocumented(scope, key, name, q);
	return false;
}

bool MatchNestName(const char *scope, const String& name)
{
	for(;;) {
		if(strncmp(scope, name, name.GetLength()) == 0) {
			int c = scope[name.GetLength()];
			if(c == '\0' || c == ':')
				return true;
		}
		while(*scope != ':') {
			if(*scope == '\0')
				return false;
			scope++;
		}
		while(*scope == ':')
			scope++;
	}
}

bool MatchNest(const char *scope, const Array<CppItem>& m, const BrowserQuery& q)
{
	if(MatchNestName(scope, q.name))
		for(int i = 0; i < m.GetCount(); i++)
			if(MatchItem(scope, m[i], m[i].qitem, m[i].name, q, true))
				return true;
	for(int i = 0; i < m.GetCount(); i++)
		if(MatchItem(scope, m[i], m[i].qitem, m[i].name, q, false))
			return true;
	return false;
}

void Browser::WithSearch(EditString& search_scope, EditString& search_item)
{
	item.WantFocus();
	search_scope.NullText("Search scope", StdFont().Italic(), SColorDisabled());
	search_scope.SetFilter(CharFilterAlphaToUpper);
	s_scope = &search_scope;
	search_scope <<= THISBACK(Reload);
	search_item.NullText("Search item", StdFont().Italic(), SColorDisabled());
	search_item.SetFilter(CharFilterAlphaToUpper);
	s_item = &search_item;
	search_item <<= THISBACK(EnterNesting);
}

bool Browser::FindSet(const String& kscopeing, const String& kitem, int scopeingsc, int itemsc)
{
	if(!IsNull(kscopeing) && scopeing.FindSetCursor(kscopeing)) {
		scopeing.ScCursor(scopeingsc);
		if(!IsNull(kitem))
			for(int i = 0; i < item.GetCount(); i++)
				if(ValueTo<CppItemInfo>(item.Get(i)).qitem == kitem) {
					item.SetCursor(i);
					item.SetSbPos(itemsc);
					return true;
				}
	}
	return false;
}

void Browser::Reload()
{
	item.active_topics = WhenShowTopic;
	CppBase& base = BrowserBase();
	String kscopeing, kitem;
	int scopeingsc, itemsc;
	if(scopeing.IsCursor()) {
		kscopeing = scopeing.GetKey();
		scopeingsc = scopeing.GetCursorSc();
		if(item.IsCursor()) {
			kitem = ValueTo<CppItemInfo>(item.Get(item.GetCursor())).qitem;
			itemsc = item.GetSbPos();
		}
	}
	scopeing.Clear();
	item.Clear();
	String srch;
	if(s_scope)
		srch = ~*s_scope;
	for(int i = 0; i < base.GetCount(); i++) {
		String k = base.GetKey(i);
		if(MatchNest(k, base[i], query) && (srch.GetCount() == 0 || ToUpper(k).Find(srch) >= 0)) {
			scopeing.Add(k, i, k);
		}
	}
	scopeing.Sort();
	if(!IsNull(kscopeing) && scopeing.FindSetCursor(kscopeing)) {
		scopeing.ScCursor(scopeingsc);
		for(int i = 0; i < item.GetCount(); i++)
			if(ValueTo<CppItemInfo>(item.Get(i)).qitem == kitem) {
				item.SetSbPos(itemsc);
				break;
			}
	}
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

void Browser::LoadNest(const String& scope, ArrayMap<String, CppItemInfo>& item, int inherited, Index<String>& rl)
{
	if(rl.Find(scope) < 0)
		return;
	rl.Add(scope);
	int q = BrowserBase().Find(scope);
	if(q < 0)
		return;
	Array<CppItem>& m = BrowserBase()[q];
	bool all = !IsNull(query.name) && MatchNestName(scope, query.name) || inherited;
	String base;
	String srch;
	if(s_item)
		srch = ~*s_item;
	for(int i = 0; i < m.GetCount(); i++) {
		if(MatchItem(scope, m[i], m[i].qitem, m[i].name, query, all) &&
		   (srch.GetCount() == 0 || ToUpper(m[i].name).Find(srch) >= 0)) {
			const CppItem& im = m[i];
			String k = m[i].qitem;
			int q = item.Find(k);
			if(q >= 0 && im.IsCode())
				item[q].over = true;
			CppItemInfo& f = item.Add(k);
			if(im.IsType())
				base = im.qptype;
			f.inherited = inherited;
			(CppItem&)f = im;
			f.scope = scope;
			if(q >= 0)
				for(;;) {
					q = item.FindNext(q);
					if(q < 0)
						break;
					if(im.IsCode())
						item[q].overed = true;
				}
		}
	}
	if(show_inherited) {
		Vector<String> b = Split(base, ';');
		for(int i = 0; i < b.GetCount(); i++)
			LoadNest(b[i], item, ++inherited, rl);
	}
}

void Browser::EnterNesting()
{
	item.Clear();
	if(scopeing.IsCursor()) {
		ArrayMap<String, CppItemInfo> m;
		Index<String> rl;
		LoadNest(scopeing.GetKey(), m, false, rl);
		for(int i = 0; i < m.GetCount(); i++)
			item.Add(RawToValue(m[i]));
		item.Sort(FnValueOrder(ItemCompare));
	}
}

void Browser::EnterItem()
{
	pos = 0;
	WhenItem();
}

void Browser::GotoItem()
{
	GotoPos(pos++);
}

void Browser::ItemClick()
{
	if(clickpos)
		GotoItem();
}

void Browser::ItemDblClk()
{
	GotoItem();
	WhenItemDblClk();
}

bool Browser::IsCurrentItem()
{
	if(!item.IsCursor())
		return false;
	const CppItemInfo& m = ValueTo<CppItemInfo>(item.Get(item.GetCursor()));
	int q = BrowserBase().Find(m.scope);
	if(q < 0)
		return false;
	Array<CppItem>& n = BrowserBase()[q];
	return FindItem(n, m.qitem) >= 0;
}

CppItem& Browser::CurrentItem()
{
	ASSERT(scopeing.IsCursor() && item.IsCursor());
	const CppItemInfo& m = ValueTo<CppItemInfo>(item.Get(item.GetCursor()));
	Array<CppItem>& n = BrowserBase().GetAdd(m.scope);
	return n[FindItem(n, m.qitem)];
}

void Browser::GotoPos(int pos)
{
	if(!IsCurrentItem())
		return;
	CppItem& m = CurrentItem();
	WhenPos(GetCppFile(m.file), m.line);
}

String Browser::GetItem(int i)
{
	if(i < 0 || i >= item.GetCount())
		return Null;
	String s = (String)scopeing.GetKey();
	if(s != "::")
		s << "::";
	s << ValueTo<CppItemInfo>(item.Get(i)).qitem;
	return s;
}

CppItemInfo Browser::GetItemInfo(int i)
{
	if(i < 0 || i >= item.GetCount())
		return CppItemInfo();
	return ValueTo<CppItemInfo>(item.Get(i));
}

bool Browser::FindSet(const String& s)
{
	String scope;
	String item;
	SplitCodeRef(s, scope, item);
	return FindSet(scope, item);
}

void Browser::ItemMenu(Bar& bar)
{
	if(IsCurrentItem()) {
		if(WhenPos) {
			CppItem& m = CurrentItem();
			String fn = GetCppFile(m.file);
	    	bar.Add(String().Cat() << GetFileName(fn) << " (" << m.line << ')',
	    	        IdeFileImage(fn),
	    	        THISBACK1(GotoPos, 0));
			bar.Separator();
		}
		if(WhenShowTopic) {
			Vector<String> link = GetRefLinks(GetItem());
			for(int i = 0; i < link.GetCount(); i++)
				bar.Add(GetTopicTitle(link[i]), BrowserImg::Ref(), THISBACK1(ShowTopic, link[i]));
			if(link.GetCount())
				bar.Separator();
		}
	}
	bar.Add("Query..", BrowserImg::Query(), THISBACK(DoDoQuery));
	bar.Separator();
	bar.Add(false, AsString(scopeing.GetCount()) + " scopes, " + AsString(item.GetCount()) + " items", Callback());
}

void Browser::QueryNest()
{
	query.name = scopeing.GetKey();
	querydlg.name <<= query.name;
	Reload();
	scopeing.GoBegin();
	item.SetCursor(0);
}

void Browser::QueryWord(const String& w)
{
	query.package.Clear();
	query.file.Clear();
	query.name = w;
	querydlg.Clear();
	querydlg.name <<= w;
	Reload();
	scopeing.GoBegin();
}

void Browser::SetId(const String& id, const Vector<String>& scope)
{
	query.package.Clear();
	query.file.Clear();
	query.name = id;
	querydlg.Clear();
	querydlg.name <<= id;
	CppBase& base = BrowserBase();
	scopeing.Clear();
	for(int i = 0; i < scope.GetCount(); i++) {
		int q = base.Find(scope[i]);
		if(q >= 0)
			scopeing.Add(scope[i], i, scope[i]);
	}
	scopeing.GoBegin();
}

void Browser::ShowTopic(String w)
{
	WhenShowTopic(w);
}

void Browser::ShowHelp()
{
	Vector<String> link = GetRefLinks(GetItem());
	if(link.GetCount())
		ShowTopic(link[0]);
}

void Browser::SerializeWspc(Stream& s)
{
	s.Magic();
	s % query;
	s % querydlg;
	String is = GetItem();
	s % is;
	if(s.IsLoading())
		FindSet(is);
}

void Browser::Serialize(Stream& s)
{
	int version = 1;
	s % version;
	s % item;
	if(version >= 1)
		s % split;
}

Browser::Browser()
{
	s_scope = s_item = NULL;
	scopeing.NoHeader().NoGrid();
	scopeing.AddKey();
	scopeing.AddIndex();
	scopeing.AddColumn("");
	scopeing.WhenEnterRow = THISBACK(EnterNesting);
	scopeing.WhenLeftDouble = THISBACK(QueryNest);
	scopeing.NoWantFocus();
	item.WhenEnterItem = THISBACK(EnterItem);
	item.WhenLeftClick = THISBACK(ItemClick);
	item.WhenLeftDouble = THISBACK(ItemDblClk);
	item.WhenBar = THISBACK(ItemMenu);
	item.active_topics = false;
	item.Columns(2);
	split.SetPos(2000);
	Add(split.Horz(scopeing, item).SizePos());
	Register(this);
	Reload();
	BackPaint();
	clickpos = true;
	show_inherited = true;
}

Browser::~Browser()
{
	UnRegister(this);
}
