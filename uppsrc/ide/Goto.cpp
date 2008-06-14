#include "ide.h"

struct GotoDlg : public WithGotoLayout<TopWindow> {
	bool               global;
	Array<CppItemInfo> item;
	Index<String>      type;
	CppBase            lbase;
	CppBase           *gbase;

	void SyncList();
	int  GetLine();
	void SyncOk();

	void Serialize(Stream& s);

	typedef GotoDlg CLASSNAME;

	GotoDlg(const String& s);
};

void GotoDlg::SyncList()
{
	list.Clear();
	String n = ~target;
	int typei = Null;
	int nesti = Null;
	Vector<String> h = Split(n, ':');
	if(n[0] == ':') {
		if(h.GetCount() > 1 && *n.Last() != ':') {
			n = h.Top();
			h.Drop();
		}
		else
			n.Clear();
		nesti = gbase->Find("::" + Join(h, "::"));
		if(nesti < 0)
			nesti = -1;
	}
	else
	if(h.GetCount() > 1 || h.GetCount() == 1 && *n.Last() == ':') {
		if(h.GetCount() > 1 && *n.Last() != ':') {
			n = h.Top();
			h.Drop();
		}
		else
			n.Clear();
		typei = type.Find(Join(h, "::"));
		if(typei < 0)
			typei = -1;
	}
	if(IsDigit(*n))
		n.Clear();
	Index<String> nc;
	for(int ci = 0; ci < (n.GetCount() ? 2 : 1); ci++)
		for(int i = 0; i < item.GetCount(); i++) {
			const CppItemInfo& f = item[i];
			int q = memcmp(n, f.name, n.GetLength());
			if((n.GetLength() == 0 ||
			    (ci ? q && memcmp_i(n, f.name, n.GetLength()) == 0 : q == 0)) &&
			   (IsNull(typei) || typei == f.typei) &&
			   (IsNull(nesti) || nesti == f.nesti)) {
				list.Add(RawToValue(CppNestingInfo(f)), RawToValue(f), f.key, f.line, f.fn, f.nesting);
				nc.FindAdd(f.nesting);
			}
		}
	list.HeaderTab(0).SetText(Format("Nesting (%d)", nc.GetCount()));
	list.HeaderTab(1).SetText(Format("Symbol (%d)", list.GetCount()));
	SyncOk();
}

int GotoDlg::GetLine()
{
	if(list.IsCursor() && list.HasFocus())
		return list.Get(3);
	String s = ~target;
	if(IsDigit(s[0]))
		return atoi(s);
	if(list.IsCursor())
		return list.Get(3);
	if(list.GetCount())
		return list.Get(0, 3);
	return -1;
}

void GotoDlg::SyncOk()
{
	ok.Enable(GetLine() >= 0);
}

int GotoFilter(int c)
{
	return IsDigit(c) || IsAlpha(c) || c == '_' || c == ':' ? c : c == '.' ? ':' : 0;
}

void GotoDlg::Serialize(Stream& s)
{
	SerializePlacement(s);
	list.SerializeHeader(s);
}

struct CppItemInfoSortLine {
	bool operator()(const CppItemInfo& a, const CppItemInfo& b) const {
		return a.line < b.line;
	}
};

struct CppItemInfoSortGlobal {
	bool operator()(const CppItemInfo& a, const CppItemInfo& b) const {
		return CombineCompare(a.nesting, b.nesting)(a.fn, b.fn)(a.line, b.line) < 0;
	}
};

GotoDlg::GotoDlg(const String& s)
{
	global = IsNull(s);
	if(!global) {
		StringStream ss(s);
		Parser parser;
		parser.Do(ss, IgnoreList(), lbase, Null, CNULL);
	}
	CtrlLayoutOKCancel(*this, IsNull(s) ? "Go to global" : "Go to line or symbol");
	CppBase& base = IsNull(s) ? BrowserBase() : lbase;
	gbase = &base;
	for(int i = 0; i < base.GetCount(); i++) {
		CppNest& n = base[i];
		for(int j = 0; j < n.GetCount(); j++) {
			const CppItem& m = n[j];
			for(int l = 0; l < m.pos.GetCount(); l++) {
				CppItemInfo mf;
				(CppSimpleItem&)mf = n[j];
				mf.nesting = base.GetKey(i);
				mf.namespacel = n.namespacel;
				const CppPos& p = m.pos[l];
				mf.name = n.name[j];
				mf.line = p.line;
				mf.fn = p.GetFile();
				mf.virt = false;
				mf.access = m.pos[l].impl ? (int)WITHBODY : (int)PUBLIC;
				mf.key = global ? String().Cat() << GetFileName(mf.fn) << " (" << p.line << ')' : AsString(p.line);
				int nl = mf.namespacel + 2;
				String tp;
				if(nl < mf.nesting.GetLength())
					tp = mf.nesting.Mid(nl);
				mf.typei = type.FindAdd(tp);
				mf.nesti = i;
				item.Add(mf);
			}
		}
	}
	if(global)
		Sort(item, CppItemInfoSortGlobal());
	else
		Sort(item, CppItemInfoSortLine());
	target.SetFilter(GotoFilter);
	target <<= THISBACK(SyncList);
	list.AddColumn("Nesting").SetDisplay(Single<CppNestingInfoDisplay>());
	list.AddColumn().SetDisplay(Single<CppItemInfoDisplay>());
	list.AddColumn(global ? "Position" : "Line");
	if(global)
		list.ColumnWidths("181 466 112");
	else
		list.ColumnWidths("174 516 37");
	list.WhenCursor = THISBACK(SyncOk);
	list.WhenLeftDouble = Breaker(IDOK);
	list.EvenRowColor();
	SyncList();
	ActiveFocus(target);
	Sizeable().Zoomable();
	Icon(IdeImg::Navigator());
}

INITBLOCK
{
	RegisterGlobalConfig("IdeGoto");
	RegisterGlobalConfig("IdeGotoGlobal");
}

void Ide::Goto()
{
	if(designer || editor.GetLength() == 0)
		return;
	GotoDlg dlg(~editor);
	LoadFromGlobal(dlg, "IdeGoto");
	int c = dlg.Run();
	StoreToGlobal(dlg, "IdeGoto");
	if(c != IDOK)
		return;
	int l = dlg.GetLine();
	if(l > 0) {
		editor.SetCursor(editor.GetPos(l - 1));
		editor.TopCursor();
	}
}

void Ide::GotoGlobal()
{
	SaveFile();
	StartBrowserBase();
	GotoDlg dlg(Null);
	LoadFromGlobal(dlg, "IdeGotoGlobal");
	int c = dlg.Run();
	StoreToGlobal(dlg, "IdeGotoGlobal");
	if(c != IDOK)
		return;
	int l = dlg.GetLine();
	if(l > 0 && dlg.list.GetCount()) {
		String file = dlg.list.IsCursor() ? dlg.list.Get(4) : dlg.list.Get(0, 4);
		GotoPos(file, l);
	}
}

void AssistEditor::SwapSContext(Parser& p)
{
	int i = GetCursor();
	if(Ch(i - 1) == ';')
		i--;
	else
		for(;;) {
			int c = Ch(i);
			if(c == '{') {
				i++;
				break;
			}
			if(c == 0 || c == ';' || c == '}')
				break;
			i++;
		}
	Context(p, i);
}

void Ide::SwapS()
{
	if(designer || !editor.assist_active)
		return;
	Parser p;
	editor.SwapSContext(p);
	int q = BrowserBase().Find(p.current_nest);
	if(q < 0)
		return;
	const CppNest& n = BrowserBase()[q];
	q = n.key.Find(QualifyKey(BrowserBase(), p.current_nest, p.current_key));
	if(q < 0)
		return;
	int    line = -1;
	if(p.current.pos.GetCount())
		line = p.current.pos.Top().line;
	const CppItem& m = n[q];
	int i;
	for(i = 0; i < m.pos.GetCount(); i++) {
		const CppPos& fp = m.pos[i];
		if(fp.GetFile() == editfile && fp.line == line) {
			i++;
			break;
		}
	}
	GotoCpp(m.pos[i % m.pos.GetCount()]);
}
