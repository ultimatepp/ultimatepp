#include "ide.h"

#define LLOG(x)  // DLOG(x)

#if 0 // TODO: Remove later...

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
	String n = ToLower((String)~target);
	int typei = Null;
	String scope = Null;
	Vector<String> h = Split(n, ':');
	if(n[0] == ':') {
		if(h.GetCount() > 1 && *n.Last() != ':') {
			n = h.Top();
			h.Drop();
		}
		else
			n.Clear();
		scope = Join(h, "::");
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
	for(int i = 0; i < item.GetCount(); i++) {
		const CppItemInfo& f = item[i];		
		if(ToLower(f.name).Find(n) >= 0 && (IsNull(typei) || typei == f.typei) && (IsNull(scope) || scope == f.scope)) {
			list.Add(f.scope, RawToValue(f), f.item, f.line, GetSourceFilePath(f.file), f.scope);
			nc.FindAdd(f.scope);
		}
	}
/*
	for(int ci = 0; ci < (n.GetCount() ? 2 : 1); ci++)
		for(int i = 0; i < item.GetCount(); i++) {
			const CppItemInfo& f = item[i];
			int q = memcmp(n, f.name, n.GetLength());
			if((n.GetLength() == 0 ||
			    (ci ? q && memcmp_i(n, f.name, n.GetLength()) == 0 : q == 0)) &&
			   (IsNull(typei) || typei == f.typei) &&
			   (IsNull(scope) || scope == f.scope)) {
				list.Add(f.scope, RawToValue(f), f.item, f.line, GetSourceFilePath(f.file), f.scope);
				nc.FindAdd(f.scope);
			}
		}
*/
	list.HeaderTab(0).SetText(Format("Scope (%d)", nc.GetCount()));
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
	return IsDigit(c) || IsAlpha(c) || c == '_' || c == ':' ? ToUpper(c) : c == '.' ? ':' : 0;
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
		return CombineCompare(a.scope, b.scope)
		                     (GetSourceFilePath(a.file), GetSourceFilePath(b.file))
		                     (a.line, b.line) < 0;
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
	CppBase& base = IsNull(s) ? CodeBase() : lbase;
	gbase = &base;
	for(int i = 0; i < base.GetCount(); i++) {
		Array<CppItem>& n = base[i];
		for(int j = 0; j < n.GetCount(); j++) {
			const CppItem& m = n[j];
			CppItemInfo mf;
			(CppItem&)mf = n[j];
			mf.scope = base.GetKey(i);
			mf.virt = false;
			mf.access = m.impl ? (int)WITHBODY : (int)PUBLIC;
			mf.item = global ? String().Cat() << GetFileName(GetSourceFilePath(m.file)) << " (" << m.line << ')'
			                 : AsString(m.line);
			mf.typei = 0;
			item.Add(mf);
		}
	}
	if(global)
		Sort(item, CppItemInfoSortGlobal());
	else
		Sort(item, CppItemInfoSortLine());
	target.SetFilter(GotoFilter);
	target <<= THISBACK(SyncList);
	list.AddColumn("Nesting");
	list.AddColumn().SetDisplay(Single<CppItemInfoDisplay>());
	list.AddColumn(global ? "Position" : "Line");
	list.SetLineCy(BrowserFont().Info().GetHeight() + 3);
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

#endif

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

bool Ide::SwapSIf(const char *cref)
{
	if(designer || !editor.assist_active)
		return false;
	Parser p;
	editor.SwapSContext(p);
	int q = CodeBase().Find(p.current_scope);
	LLOG("SwapS scope: " << p.current_scope);
	if(q < 0)
		return false;
	const Array<CppItem>& nn = CodeBase()[q];
	String qitem = QualifyKey(CodeBase(), p.current_scope, p.current_key, p.context.namespace_using);
	if(cref && MakeCodeRef(p.current_scope, p.current_key) != cref)
		return false;
	Vector<const CppItem *> n;
	for(int i = 0; i < nn.GetCount(); i++)
		if(nn[i].qitem == qitem)
			n.Add(&nn[i]);
	if(!cref && n.GetCount() < 2) {
		int typei = -1;
		for(int i = 0; i < n.GetCount(); i++) {
			if(nn[i].IsType())
				typei = i;
			if(nn[i].qitem != qitem && nn[i].name == p.current_name) {
				GotoCpp(*n[i]);
				return true;
			}
		}
		if(typei >= 0 && nn[typei].name.GetCount())
			GotoCpp(nn[typei]);
		return false;
	}
	if(n.GetCount() == 0 || IsNull(editfile))
		return false;
	int file = GetSourceFileIndex(editfile);
	int line = p.current.line;
	LLOG("SwapS line: " << line);
	int i;
	for(i = 0; i < n.GetCount(); i++) {
		LLOG("file: " << GetSourceFilePath(n[q + i].file) << ", line: " << n[q + i].line);
		if(n[i]->file == file && n[i]->line == line) {
			i++;
			break;
		}
	}
	GotoCpp(*n[i % n.GetCount()]);
	return true;
}

void Ide::SwapS()
{
	SwapSIf(NULL);
}
