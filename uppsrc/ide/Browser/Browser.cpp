#include "Browser.h"

#define IMAGECLASS BrowserImg
#define IMAGEFILE <ide/Browser/Browser.iml>
#include <Draw/iml_source.h>

bool MatchItemFile(const CppItem& m, const BrowserQuery& q)
{
	if(IsNull(q.package) && IsNull(q.file))
		return true;
	for(int i = 0; i < m.pos.GetCount(); i++) {
		int w = FileSet().Find(m.pos[i].GetFile());
		if(w >= 0) {
			const BrowserFileInfo& f = FileSet()[w];
			if((IsNull(q.package) || q.package == f.package) && (IsNull(q.file) || q.file == f.file))
				return true;
		}
	}
	return false;
}

String JoinNestKey(const String& nest, const String& key)
{
	String s = nest;
	if(s != "::")
		s << "::";
	s << key;
	return s;
}

bool MatchDocumented(const String& nest, const String& key, const String& name, const BrowserQuery& q)
{
	int n = GetRefLinks(JoinNestKey(nest, key)).GetCount();
	return q.notdocumented && q.documented || q.notdocumented && n == 0 || q.documented && n;
}

bool MatchItem(const String& nest, const CppItem& m, const String& key, const String& name, const BrowserQuery& q, bool all)
{
	if(q.a_private && m.access == PRIVATE ||
	   q.a_protected && m.access == PROTECTED ||
	   q.a_public && m.access == PUBLIC)
		if(q.type && m.IsType() ||
		   q.code && m.IsCode() ||
		   q.data && m.IsData() ||
		   q.macro && m.IsMacro())
			if(all || IsNull(q.name) || name == q.name || nest + "::" + name == q.name)
				return MatchItemFile(m, q) && MatchDocumented(nest, key, name, q);
	return false;
}

bool MatchNestName(const char *nest, const String& name)
{
	for(;;) {
		if(strncmp(nest, name, name.GetLength()) == 0) {
			int c = nest[name.GetLength()];
			if(c == '\0' || c == ':')
				return true;
		}
		while(*nest != ':') {
			if(*nest == '\0')
				return false;
			nest++;
		}
		while(*nest == ':')
			nest++;
	}
}

bool MatchNest(const char *nest, const CppNest& m, const BrowserQuery& q)
{
	if(MatchNestName(nest, q.name))
		for(int i = 0; i < m.GetCount(); i++)
			if(MatchItem(nest, m[i], m.key[i], m.name[i], q, true))
				return true;
	for(int i = 0; i < m.GetCount(); i++)
		if(MatchItem(nest, m[i], m.key[i], m.name[i], q, false))
			return true;
	return false;
}

bool Browser::FindSet(const String& knesting, const String& kitem, int nestingsc, int itemsc)
{
	if(!IsNull(knesting) && nesting.FindSetCursor(knesting)) {
		nesting.ScCursor(nestingsc);
		if(!IsNull(kitem))
			for(int i = 0; i < item.GetCount(); i++)
				if(ValueTo<CppItemInfo>(item.Get(i)).key == kitem) {
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
	String knesting, kitem;
	int nestingsc, itemsc;
	if(nesting.IsCursor()) {
		knesting = nesting.GetKey();
		nestingsc = nesting.GetCursorSc();
		if(item.IsCursor()) {
			kitem = ValueTo<CppItemInfo>(item.Get(item.GetCursor())).key;
			itemsc = item.GetSbPos();
		}
	}
	nesting.Clear();
	item.Clear();
	for(int i = 0; i < base.GetCount(); i++)
		if(MatchNest(base.GetKey(i), base[i], query)) {
			CppNestingInfo f;
			f.namespacel = base[i].namespacel;
			f.nesting = base.GetKey(i);
			nesting.Add(f.nesting, i, RawToValue<CppNestingInfo>(f));
		}
	nesting.Sort();
	if(!IsNull(knesting) && nesting.FindSetCursor(knesting)) {
		nesting.ScCursor(nestingsc);
		for(int i = 0; i < item.GetCount(); i++)
			if(ValueTo<CppItemInfo>(item.Get(i)).key == kitem) {
				item.SetSbPos(itemsc);
				break;
			}
	}
}

int ItemCompare(const Value& v1, const Value& v2)
{
	const CppItemInfo& a = ValueTo<CppItemInfo>(v1);
	const CppItemInfo& b = ValueTo<CppItemInfo>(v2);
/*	int ak = a.kind >= FUNCTION && a.kind <= CLASSFUNCTIONTEMPLATE ? FUNCTION : a.kind;
	int bk = b.kind >= FUNCTION && b.kind <= CLASSFUNCTIONTEMPLATE ? FUNCTION : b.kind;
	if(ak != bk)
	return ak - bk;*/
	int q = a.inherited - b.inherited;
	if(q) return q;
	q = a.fn.Compare(b.fn);
	return q ? q : a.line - b.line;
}

void Browser::LoadNest(const String& nest, ArrayMap<String, CppItemInfo>& item, int inherited)
{
	int q = BrowserBase().Find(nest);
	if(q < 0)
		return;
	CppNest& m = BrowserBase()[q];
	bool all = !IsNull(query.name) && MatchNestName(nest, query.name) || inherited;
	String base;
	for(int i = 0; i < m.GetCount(); i++) {
		if(MatchItem(nest, m[i], m.key[i], m.name[i], query, all)) {
			const CppItem& im = m[i];
			String k = m.key[i];
			int q = item.Find(k);
			if(q >= 0 && im.IsCode())
				item[q].over = true;
			CppItemInfo& f = item.Add(k);
			if(im.IsType())
				base = im.qptype;
			f.inherited = inherited;
			(CppSimpleItem&)f = im;
			f.name = m.name[i];
			f.key = k;
			f.nesting = nest;
			int cline = 0;
			String cfn;
			int hline = 0;
			String hfn;
			for(int i = 0; i < im.pos.GetCount(); i++) {
				const CppPos& p = im.pos[i];
				String fn = p.GetFile();
				if(ToUpper(GetFileExt(p.GetFile())) == ".H") {
					if(p.line > hline) {
						hline = p.line;
						hfn = fn;
					}
				}
				else
					if(p.line > cline) {
						cline = p.line;
						cfn = fn;
					}
			}
			if(!IsNull(hfn)) {
				f.fn = hfn;
				f.line = hline;
			}
			else
			if(IsNull(f.fn)) {
				f.fn = cfn;
				f.line = cline;
			}
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
			LoadNest(b[i], item, ++inherited);
	}
}

void Browser::EnterNesting()
{
	item.Clear();
	if(nesting.IsCursor()) {
		ArrayMap<String, CppItemInfo> m;
		LoadNest(nesting.GetKey(), m, false);
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
	int q = BrowserBase().Find(m.nesting);
	if(q < 0)
		return false;
	CppNest& n = BrowserBase()[q];
	q = n.key.Find(m.key);
	return q >= 0 && q < n.item.GetCount();
}

CppItem& Browser::CurrentItem()
{
	ASSERT(nesting.IsCursor() && item.IsCursor());
	const CppItemInfo& m = ValueTo<CppItemInfo>(item.Get(item.GetCursor()));
	CppNest& n = BrowserBase().GetAdd(m.nesting);
	return n.item[n.key.Find(m.key)];
}

void Browser::GotoPos(int pos)
{
	if(!IsCurrentItem())
		return;
	CppItem& m = CurrentItem();
	CppPos& p = m.pos[pos % m.pos.GetCount()];
	WhenPos(p.GetFile(), p.line);
}

String Browser::GetItem(int i)
{
	if(i < 0 || i >= item.GetCount())
		return Null;
	String s = (String)nesting.GetKey();
	if(s != "::")
		s << "::";
	s << ValueTo<CppItemInfo>(item.Get(i)).key;
	return s;
}

CppItemInfo Browser::GetItemInfo(int i)
{
	if(i < 0 || i >= item.GetCount())
		return CppItemInfo();
	return ValueTo<CppItemInfo>(item.Get(i));
}

bool SplitNestKey(const String& s, String& nest, String& key)
{
	CParser p(s);
	if(!p.Char2(':', ':'))
		return false;
	const char *q = s;
	for(;;) {
		q = p.GetPtr();
		if(p.Char('~') || p.Id("operator"))
			break;
		if(p.IsId()) {
			String h = p.ReadId();
			if(!p.Char2(':', ':'))
				break;
			nest << "::" << h;
		}
		else
			break;
	}
	key = q;
	nest = Nvl(nest, "::");
	return true;
}

bool Browser::FindSet(const String& s)
{
	String n;
	String m;
	if(!SplitNestKey(s, n, m))
		return false;
	return FindSet(n, m);
}

void Browser::ItemMenu(Bar& bar)
{
	if(IsCurrentItem()) {
		if(WhenPos) {
			CppItem& m = CurrentItem();
			for(int i = 0; i < m.pos.GetCount(); i++) {
		    	CppPos& p = m.pos[i];
		    	bar.Add(String().Cat() << GetFileName(p.GetFile()) << " (" << p.line << ')',
		    	        IdeFileImage(p.GetFile()),
		    	        THISBACK1(GotoPos, i));
			}
			if(!m.pos.IsEmpty())
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
	bar.Add(false, AsString(nesting.GetCount()) + " nests, " + AsString(item.GetCount()) + " items", Callback());
}

void Browser::QueryNest()
{
	query.name = nesting.GetKey();
	querydlg.name <<= query.name;
	Reload();
	nesting.GoBegin();
	item.SetCursor(0);
}

void Browser::QueryWord(const String& w)
{
	StartBrowserBase();
	query.package.Clear();
	query.file.Clear();
	query.name = w;
	querydlg.Clear();
	querydlg.name <<= w;
	Reload();
	nesting.GoBegin();
}

void Browser::SetId(const String& id, const Vector<String>& nest)
{
	StartBrowserBase();
	query.package.Clear();
	query.file.Clear();
	query.name = id;
	querydlg.Clear();
	querydlg.name <<= id;
	CppBase& base = BrowserBase();
	nesting.Clear();
	for(int i = 0; i < nest.GetCount(); i++) {
		int q = base.Find(nest[i]);
		if(q >= 0) {
			CppNestingInfo f;
			f.namespacel = base[q].namespacel;
			f.nesting = nest[i];
			nesting.Add(f.nesting, i, RawToValue<CppNestingInfo>(f));
		}
	}
	nesting.GoBegin();
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

void CppNestingInfoDisplay::Paint(Draw& w, const Rect& r, const Value& q,
                                  Color _ink, Color paper, dword style) const {
	w.DrawRect(r, paper);
	const CppNestingInfo& f = ValueTo<CppNestingInfo>(q);
	bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
	int t = r.top + (r.Height() - Draw::GetStdFontCy()) / 2;
	w.DrawText(r.left, t, f.nesting, StdFont(), focuscursor ? _ink : Magenta(), f.namespacel);
	w.DrawText(r.left + GetTextSize(f.nesting, StdFont(), f.namespacel).cx, t, ~f.nesting + f.namespacel,
	           StdFont(), _ink);
}

Browser::Browser()
{
	nesting.NoHeader().NoGrid();
	nesting.AddKey();
	nesting.AddIndex();
	nesting.AddColumn("").SetDisplay(Single<CppNestingInfoDisplay>());
	nesting.WhenEnterRow = THISBACK(EnterNesting);
	nesting.WhenLeftDouble = THISBACK(QueryNest);
	nesting.NoWantFocus();
	item.WhenEnterItem = THISBACK(EnterItem);
	item.WhenLeftClick = THISBACK(ItemClick);
	item.WhenLeftDouble = THISBACK(ItemDblClk);
	item.WhenBar = THISBACK(ItemMenu);
	item.active_topics = false;
	item.Columns(2);
	split.SetPos(2000);
	Add(split.Horz(nesting, item).SizePos());
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
