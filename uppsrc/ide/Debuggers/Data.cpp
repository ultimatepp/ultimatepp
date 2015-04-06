#include "Debuggers.h"

#ifdef COMPILER_MSC

#define LLOG(x) // LOG(x)

VectorMap<String, Value> Pdb::DataMap(const ArrayCtrl& a)
{
	VectorMap<String, Value> r;
	for(int i = 0; i < a.GetCount(); i++)
		r.Add(a.Get(i, 0), a.Get(i, 1));
	return r;
}

Value Pdb::Vis(const String& key, const VectorMap<String, Value>& prev, Visual rval_ vis,
               bool& ch)
{
	int q = prev.Find(key);
	ch = false;
	for(int i = 0; i < vis.part.GetCount(); i++) {
		VisualPart& p = vis.part[i];
		p.mark = false;
		if(q >= 0 && IsType<Visual>(prev[q])) {
			const Visual& v = ValueTo<Visual>(prev[q]);
			if(v.part.GetCount() == vis.part.GetCount() && v.part[i].text != p.text)
				ch = p.mark = true;
		}
	}
	return RawPickToValue(vis);
}

void Pdb::Vis(ArrayCtrl& a, const String& key, const VectorMap<String, Value>& prev,
              Visual rval_ vis)
{
	bool ch;
	a.Add(key, Vis(key, prev, vis, ch));
}

void Pdb::Locals()
{
	VectorMap<String, Value> prev = DataMap(locals);
	locals.Clear();
	int q = ~framelist;
	if(q >= 0 && q < frame.GetCount()) {
		Frame& f = frame[q];
		for(int i = 0; i < f.param.GetCount(); i++)
			Vis(locals, f.param.GetKey(i), prev, Visualise(f.param[i]));
		for(int i = 0; i < f.local.GetCount(); i++)
			Vis(locals, f.local.GetKey(i), prev, Visualise(f.local[i]));
	}
}

void Pdb::AddThis(const VectorMap<String, Val>& m, adr_t address, const VectorMap<String, Value>& prev)
{
	for(int i = 0; i < m.GetCount() && self.GetCount() < 2000; i++) {
		Val mv = m[i];
		mv.address += address;
		Visual vis;
		try {
			vis = Visualise(mv);
		}
		catch(CParser::Error e) {
			vis.Cat(e, SColorDisabled);
		}
		Vis(self, m.GetKey(i), prev, vis);
	}
}

void Pdb::AddThis(int type, adr_t address, const VectorMap<String, Value>& prev)
{
	const Type& t = GetType(type);
	AddThis(t.member, address, prev);
	AddThis(t.static_member, 0, prev);
	for(int i = 0; i < t.base.GetCount() && self.GetCount() < 2000; i++)
		AddThis(t.base[i].type, t.base[i].address + address, prev);
}

void Pdb::This()
{
	VectorMap<String, Value> prev = DataMap(locals);
	self.Clear();
	int q = ~framelist;
	if(q >= 0 && q < frame.GetCount()) {
		Frame& f = frame[q];
		for(int i = 0; i < f.local.GetCount(); i++) {
			if(f.local.GetKey(i) == "this") {
				Val val = f.local[i];
				if(val.ref > 0 || val.type < 0)
					try {
						val = GetRVal(val);
					}
					catch(CParser::Error) {}
				AddThis(val.type, val.address, prev);
				break;
			}
		}
	}
}

void Pdb::TryAuto(const String& exp, const VectorMap<String, Value>& prev)
{
	if(autos.Find(exp) < 0) {
		Visual r;
		try {
			CParser p(exp);
			Val v = Exp(p);
			Visualise(r, v, 2);
		}
		catch(CParser::Error) {
			r.Clear();
		}
		if(r.part.GetCount())
			Vis(autos, exp, prev, r);
	}
}

void Pdb::Autos()
{
	VectorMap<String, Value> prev = DataMap(autos);
	autos.Clear();
	autotext.Replace("//", "");
	CParser p(autotext);
	TryAuto("this", prev);
	while(!p.IsEof())
		if(p.IsId()) {
			String exp = p.ReadId();
			TryAuto(exp, prev);
			for(;;) {
				if(p.Char('.') && p.IsId())
					exp << '.';
				else
				if(p.Char2('-', '>') && p.IsId())
					exp << "->";
				else
				if(p.Char2(':', ':') && p.IsId())
					exp << "::";
				else
					break;
				exp << p.ReadId();
				TryAuto(exp, prev);
			}
		}
		else
			p.SkipTerm();
	autos.Sort();
}

void Pdb::Watches()
{
	VectorMap<String, Value> prev = DataMap(watches);
	for(int i = 0; i < watches.GetCount(); i++) {
		bool ch;
		watches.Set(i, 1, Vis((String)watches.Get(i, 0),
		            prev, Visualise((String)watches.Get(i, 0)), ch));
		if(ch)
			watches.SetDisplay(i, 0, Single<RedDisplay>());
		else
			watches.SetDisplay(i, 0, StdDisplay());
	}
}

void Pdb::Explore(const Val& val, const VectorMap<String, Value>& prev)
{
	const Type& t = GetType(val.type);
	for(int i = 0; i < t.base.GetCount(); i++) {
		Val b = t.base[i];
		b.address += val.address;
		Vis(explorer, ':' + GetType(b.type).name, prev, Visualise(b));
	}
	for(int i = 0; i < t.member.GetCount(); i++) {
		Val r = t.member[i];
		r.address += val.address;
		Vis(explorer, '.' + t.member.GetKey(i), prev, Visualise(r));
	}
	for(int i = 0; i < t.static_member.GetCount(); i++)
		Vis(explorer, "::" + t.static_member.GetKey(i), prev, Visualise(t.static_member[i]));
}

void Pdb::Explorer()
{
	VectorMap<String, Value> prev = DataMap(explorer);
	explorer.Clear();
	try {
		String x = ~expexp;
		if(!IsNull(x)) {
			CParser p(x);
			Val v = Exp(p);
			Vis(explorer, "=", prev, Visualise(v));
			if(v.type >= 0 && v.ref == 0 && !v.rvalue)
				Explore(v, prev);
			if(v.ref > 0 && GetRVal(v).address)
				for(int i = 0; i < 20; i++)
					Vis(explorer, Format("[%d]", i), prev, Visualise(DeRef(Compute(v, RValue(i), '+'))));
		}
	}
	catch(CParser::Error e) {
		Visual v;
		v.Cat(e, LtRed);
		explorer.Add("", RawPickToValue(v));
	}
	exback.Enable(exprev.GetCount());
	exfw.Enable(exnext.GetCount());
}

void Pdb::Explore(const String& exp)
{
	exprev.Add(~expexp);
	expexp = exp;
	Explorer();
	DoExplorer();
}

String Pdb::GetExpExp()
{
	String w;
	if(explorer.IsCursor()) {
		w = expexp;
		for(const char *s = w; *s; s++)
			if(strchr("*/+-", *s)) {
				w = '(' + w + ')';
				break;
			}
		String k = (String)explorer.GetKey();
		if(k != "=")
			w = w + k;
	}
	return w;
}

void Pdb::ExpExp()
{
	if(explorer.GetCursor() > 0)
		Explore(GetExpExp());
}

void Pdb::ExplorerTree()
{
	SetTree(GetExpExp());
}

void Pdb::DoExplorer()
{
	tab.Set(TAB_EXPLORER);
	expexp.SetFocus();
	expexp.SetSelection();
	Explorer();
}

void Pdb::ExBack()
{
	if(exprev.GetCount()) {
		exnext.Add(~expexp);
		expexp <<= exprev.Pop();
		Explorer();
	}
}

void Pdb::ExFw()
{
	if(exnext.GetCount()) {
		exprev.Add(~expexp);
		expexp <<= exnext.Pop();
		Explorer();
	}
}

void Pdb::ExploreKey(ArrayCtrl *a)
{
	if(a && a->IsCursor()) {
		tab.Set(TAB_EXPLORER);
		Explore(a->GetKey());
	}
}

bool Pdb::Tip(const String& exp, CodeEditor::MouseTip& mt)
{
/*	mt.display = &StdDisplay();
	mt.value = exp;
	mt.sz = Size(100, 20);
	return true;*/
	DR_LOG("Pdb::Tip");
	Visual r;
	try {
		CParser p(exp);
		Val v = Exp(p);
		Visualise(r, v, 2);
		if(r.part.GetCount()) {
			mt.sz = r.GetSize() + Size(4, 4);
			mt.value = RawPickToValue(r);
			mt.display = &Single<VisualDisplay>();
			DR_LOG("Pdb::Tip true");
			return true;
		}
	}
	catch(CParser::Error) {}
	DR_LOG("Pdb::Tip false");
	return false;
}

void Pdb::Data()
{
	switch(tab.Get()) {
	case TAB_AUTOS: Autos(); break;
	case TAB_LOCALS: Locals(); break;
	case TAB_THIS: This(); break;
	case TAB_WATCHES: Watches(); break;
	case TAB_EXPLORER: Explorer(); break;
	case TAB_MEMORY: memory.Refresh(); break;
	}
}

void Pdb::TreeNode(int parent, const String& name, Pdb::Val val)
{
	NamedVal nv;
	nv.name = name;
	nv.val = val;
	tree.Add(parent, Null, RawToValue(nv), name + "=" + Visualise(val).GetString(), val.type >= 0 || val.ref > 0);
}

void Pdb::TreeExpand(int node)
{
	if(tree.GetChildCount(node))
		return;
	Value v = tree.Get(node);
	if(!v.Is<NamedVal>())
		return;
	const NamedVal& nv = ValueTo<NamedVal>(tree.Get(node));
	Val val = nv.val;
	if(nv.val.ref > 0) {
		val = DeRef(val);
		if(val.type < 0 || val.ref > 0) {
			TreeNode(node, '*' + nv.name, val);
			SaveTree();
			return;
		}
	}
	if(val.type < 0) {
		SaveTree();
		return;
	}
	const Type& t = GetType(val.type);
	if(t.vtbl_typeindex == -2) {
		int count = GetSymInfo(t.modbase, type.GetKey(val.type), TI_GET_COUNT);
		Val prtti;
		prtti.ref = 1;
		prtti.type = UINT4;
		prtti.address = val.address - 4;
		Val rtti = GetRVal(prtti);
		FnInfo rtf = GetFnInfo(rtti.address);
		TreeNode(node, rtf.name, prtti);
		for(int i = 0; i < count; i++) {
			Val ventry;
			ventry.type = PFUNC;
			ventry.address = val.address + 4 * i;
			TreeNode(node, NFormat("[%d]", i), ventry);
		}
		return;
	}
	if(t.vtbl_typeindex >= 0) {
		Val vtbl;
		vtbl.ref = 1;
		vtbl.address = val.address + t.vtbl_offset;
		vtbl.type = t.vtbl_typeindex;
		TreeNode(node, "virtual", vtbl);
/*
		Val vloc = GetRVal(vtbl);
		FnInfo vtbl_type = GetFnInfo(vloc.address);
		const char *p = vtbl_type.name, *e = p;
		while(*e && !(*e == ':' && e[1] == ':'))
			e++;
		String fullsym(p, e);
		FnInfo ffs = GetFnInfo(fullsym);
		if(ffs.pdbtype) {
			Val typeval;
			typeval.address = val.address;
			TypeVal(typeval, ffs.pdbtype, t.modbase);
			TreeNode(node, String().Cat() << '[' << ffs.name << ']', typeval);
		}
*/
	}
	for(int i = 0; i < t.base.GetCount(); i++) {
		Val r = t.base[i];
		r.address += val.address;
		if(r.type >= 0) {
			const Type& bt = GetType(r.type);
			TreeNode(node, bt.name, r);
		}
	}
	for(int i = 0; i < t.member.GetCount(); i++) {
		Val r = t.member[i];
		r.address += val.address;
		TreeNode(node, t.member.GetKey(i), r);
	}
	for(int i = 0; i < t.static_member.GetCount(); i++) {
		Val r = t.static_member[i];
		TreeNode(node, t.static_member.GetKey(i), r);
	}
}

String Pdb::StoreTree(int parent)
{
	String result;
	int n = tree.GetChildCount(parent);
	for(int i = 0; i < n; i++) {
		int child = tree.GetChild(parent, i);
		if(tree.IsOpen(child)) {
			const NamedVal& nv = ValueTo<NamedVal>(tree.Get(child));
			if(!IsNull(result))
				result << ';';
			result << nv.name;
			String w = StoreTree(child);
			if(!IsNull(w))
				result << '{' << w << '}';
		}
	}
	return result;
}

void Pdb::SaveTree()
{
	Value v = tree.Get(0);
	if(!IsType<NamedVal>(v))
		return;
	const NamedVal& nv = ValueTo<NamedVal>(v);
	if(nv.val.type < 0)
		return;
	String w;
	Point p = tree.GetScroll();
	w << p.x << ' ' << p.y << ' ' << tree.GetCursor() << ' ' << StoreTree(0);
	treetype.GetAdd(GetType(nv.val.type).name) = w;
	LOG("SaveTree " << GetType(nv.val.type).name << ' ' << w);
}

void Pdb::ExpandTreeType(int parent, CParser& p)
{
	for(;;) {
		String id;
		if(p.Char('*'))
			id = "*";
		if(p.IsId())
		 	id << p.ReadId();
		else
			break;
		for(;;)
			if(p.Char('<'))
				id << '<';
			else
			if(p.Char('>'))
				id << '>';
			else
			if(p.Char(','))
				id << ',';
			else
			if(p.IsInt())
				id << AsString(p.ReadInt());
			else
			if(p.IsString())
				id << '\"' << AsCString(p.ReadString()) << '\"';
			else
			if(p.IsId())
				id << p.ReadId();
			else
				break;
		int n = tree.GetChildCount(parent);
		for(int i = 0; i < n; i++) {
			int child = tree.GetChild(parent, i);
			const NamedVal& nv = ValueTo<NamedVal>(tree.Get(child));
			if(nv.name == id) {
				tree.Open(child);
				if(p.Char('{')) {
					ExpandTreeType(child, p);
					p.PassChar('}');
				}
				break;
			}
		}
		p.Char(';');
	}
}

void Pdb::SetTree(const String& exp)
{
	SaveTree();
	tree.Clear();
	NamedVal nv;
	try {
		CParser p(exp);
		nv.val = Exp(p);
	}
	catch(CParser::Error) {
		return;
	}
	nv.name = exp;
	String n = exp;
	if(nv.val.type >= 0)
		n = GetType(nv.val.type).name;
	tree.SetRoot(Null, RawToValue(nv), n + '=' + Visualise(nv.val).GetString());
	if(nv.val.type >= 0) {
		String w = treetype.Get(n, Null);
		LOG("SetTree " << n << ' ' << w);
		tree.Open(0);
		CParser p(w);
		try {
			Point sc;
			sc.x = p.ReadInt();
			sc.y = p.ReadInt();
			int cursor = p.ReadInt();
			ExpandTreeType(0, p);
			tree.ScrollTo(sc);
			if(cursor >= 0)
				tree.SetCursor(cursor);
		}
		catch(CParser::Error) {}
	}
}

void Pdb::SetTreeA(ArrayCtrl *array)
{
	SetTree(array->Get(0));
}

void Pdb::MemoryGoto(const String& exp)
{
	CParser p(exp);
	try {
		Val v = Exp(p);
		adr_t adr = 0;
		if(v.ref > 0)
			adr = GetRVal(v).address;
		else
		if(v.rvalue)
			adr = (adr_t)GetInt(v);
		else
			adr = v.address;
		memory.SetCursor(adr);
		memory.SetSc(adr);
		tab.Set(TAB_MEMORY);
	}
	catch(CParser::Error e) {
		Exclamation("Invalid expression!&" + DeQtf(e));
	}
}


void Pdb::MemMenu(ArrayCtrl& array, Bar& bar, const String& exp)
{
	if(bar.IsScanKeys())
		return;
	CParser p(exp);
	try {
		Val v = Exp(p);
		bool sep = true;
		if(v.ref > 0) {
			if(sep)
				bar.Separator();
			sep = false;
			bar.Add("Memory at " + exp, THISBACK1(MemoryGoto, exp));
		}
		else
		if(v.rvalue) {
			if(sep)
				bar.Separator();
			sep = false;
			bar.Add("Memory at 0x" + FormatIntHex((dword)GetInt(v)), THISBACK1(MemoryGoto, "&" + exp));
		}
		if(!v.rvalue) {
			if(sep)
				bar.Separator();
			sep = false;
			bar.Add("Memory at &&" + exp, THISBACK1(MemoryGoto, "&" + exp));
		}
	}
	catch(CParser::Error e) {
	}
}

void Pdb::DataMenu(ArrayCtrl& array, Bar& bar, const String& exp)
{
	bar.Add("Explore", THISBACK1(ExploreKey, &array));
	MemMenu(array, bar, exp);
}

void Pdb::AutosMenu(Bar& bar)
{
	DataMenu(autos, bar, autos.GetKey());
}

void Pdb::LocalsMenu(Bar& bar)
{
	DataMenu(locals, bar, locals.GetKey());
}

void Pdb::AddWatch()
{
	String s;
	if(EditPDBExpression("Add watch", s)) {
		SetTab(3);
		watches.Add(s);
		Data();
	}
}

void Pdb::EditWatch()
{
	String s = watches.GetKey();
	if(EditPDBExpression("Edit watch", s)) {
		SetTab(2);
		watches.Set(0, s);
		Data();
	}
}

void Pdb::RemoveWatch()
{
	watches.DoRemove();
	Data();
}

void Pdb::WatchesMenu(Bar& bar)
{
	String exp = watches.GetKey();
	if(!IsNull(exp))
		exp = "(" + exp + ")";
	DataMenu(watches, bar, exp);
	bar.Separator();
	bar.Add(AK_ADDWATCH, THISBACK(AddWatch));
	bool b = watches.IsCursor();
	bar.Add(b, "Edit watch..", THISBACK(EditWatch));
	bar.Add(b, "Remove watch..", THISBACK(RemoveWatch));
	bar.Separator();
	watches.StdBar(bar);
}

void Pdb::ExplorerMenu(Bar& bar)
{
	bar.Add("Explore", THISBACK(ExpExp));
	MemMenu(watches, bar, GetExpExp());
}

void Pdb::SetTab(int i)
{
	tab.Set(i);
	Tab();
}

void Pdb::ClearWatches()
{
	watches.Clear();
}

void Pdb::DropWatch(PasteClip& clip)
{
	String s = StringStream(GetString(clip)).GetLine();
	if(s.GetCount()) {
		watches.Add(s);
		clip.SetAction(DND_COPY);
		Data();
	}
}

#endif
