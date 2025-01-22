#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x) // LOG(x)

VectorMap<String, Value> Pdb::DataMap(const ArrayCtrl& a)
{
	VectorMap<String, Value> r;
	for(int i = 0; i < a.GetCount(); i++)
		r.Add(a.Get(i, 0), a.Get(i, 1));
	return r;
}

Value Pdb::Vis(const String& key, const VectorMap<String, Value>& prev, Visual&& vis, bool& ch)
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
	return RawPickToValue(pick(vis));
}

void Pdb::Vis(ArrayCtrl& a, const String& key, const VectorMap<String, Value>& prev, Visual&& vis)
{
	bool ch;
	a.Add(key, Vis(key, prev, pick(vis), ch));
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
		Vis(self, m.GetKey(i), prev, pick(vis));
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
			Visualise(r, v, 0);
		}
		catch(LengthLimit) {}
		catch(CParser::Error) {
			r.Clear();
		}
		if(r.part.GetCount())
			Vis(autos, exp, prev, pick(r));
	}
}

void Pdb::Autos()
{
	VectorMap<String, Value> prev = DataMap(autos);
	autos.Clear();
	autotext.Replace("//", "");
	try {
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
	catch(LengthLimit) {}
	catch(CParser::Error) {}
}

void Pdb::Watches()
{
	VectorMap<String, Value> prev = DataMap(watches);
	for(int i = 0; i < watches.GetCount(); i++) {
		bool ch;
		watches.Set(i, 1, Vis((String)watches.Get(i, 0), prev, Visualise((String)watches.Get(i, 0)), ch));
		if(ch)
			watches.SetDisplay(i, 0, Single<RedDisplay>());
		else
			watches.SetDisplay(i, 0, StdDisplay());
	}
}

bool Pdb::Tip(const String& exp, CodeEditor::MouseTip& mt)
{
	DR_LOG("Pdb::Tip");
	Visual r;
	try {
		CParser p(exp);
		Val v = Exp(p);
		Visualise(r, v, 0);
	}
	catch(LengthLimit) {}
	catch(CParser::Error) {
		DR_LOG("Pdb::Tip false");
		return false;
	}
	if(r.part.GetCount()) {
		mt.sz = r.GetSize() + Size(4, 4);
		mt.value = RawPickToValue(pick(r));
		mt.display = &visual_display;
		DR_LOG("Pdb::Tip true");
		return true;
	}
	return false;
}

void Pdb::Data()
{
	switch(tab.Get()) {
	case TAB_AUTOS: Autos(); break;
	case TAB_LOCALS: Locals(); break;
	case TAB_THIS: This(); break;
	case TAB_WATCHES: Watches(); break;
	case TAB_MEMORY: memory.Refresh(); break;
	case TAB_BTS: BTs(); break;
	}
	SetTree(tree_exp);
}

void Pdb::MemoryGoto(const String& exp)
{
	try {
		CParser p(exp);
		Val v = Exp(p);
		adr_t adr = 0;
		if(v.ref > 0)
			adr = GetRVal(v).address;
		else
		if(v.rvalue)
			adr = (adr_t)GetInt64(v);
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

void Pdb::CopyMenu(ArrayCtrl& array, Bar& bar)
{
	auto CopyLine = [&](int i, ArrayCtrl& array) {
		return String() << array.Get(i, 0) << '=' << array.Get(i, 1).To<Visual>().GetString();
	};
	ArrayCtrl *ap = &array;
	bar.Add(array.IsCursor(), "Copy", [=] {
		ClearClipboard();
		AppendClipboardText(CopyLine(ap->GetCursor(), *ap));
	});
	bar.Add(array.GetCount(), "Copy all", [=] {
		ClearClipboard();
		String s;
		for(int i = 0; i < ap->GetCount(); i++)
			MergeWith(s, "\n", CopyLine(i, *ap));
		AppendClipboardText(s);
	});
	bar.Separator();
	bar.Add("Show type", [=] { show_type = !show_type; Data(); }).Check(show_type);
	bar.Add("No pretty-printing", [=] { raw = !raw; Data(); }).Check(raw);
	bar.Add("Copy type script (for .dbg file)", [&] { CopyTypeScript(array); });
}

void Pdb::CopyTypeScriptType(String& text, int type, const String& tabs, const String& id)
{
	if(type <= 0)
		return;
	const Type& t = GetType(type);
	for(int i = 0; i < t.member.GetCount(); i++) {
		String nid = t.member.GetKey(i);
		text << tabs << nid << " = ";
		int ref = clamp(t.member[i].ref, 0, 8);
		for(int j = 0; j < ref; j++)
			text << "DeRef(";
		text << "Field(" << id << ", " << AsCString(nid) << ")";
		for(int j = 0; j < ref; j++)
			text << ")";
		text << ";\n";
		CopyTypeScriptType(text, t.member[i].type, tabs + '\t', nid);
	}
	for(int i = 0; i < t.base.GetCount(); i++)
		CopyTypeScriptType(text, t.base[i].type, tabs, id);
}

void Pdb::CopyTypeScript(ArrayCtrl& array)
{
	if(array.IsCursor()) {
		Value q = array.Get(1);
		if(IsType<Visual>(q))
			try {
				int type = ValueTo<Visual>(q).type;
				const Type& t = GetType(type);
				String tname = t.name;
				tname.Replace("::__1", ""); // CLANG has some weird stuff in names...
				int q = tname.Find('<');
				if(q >= 0)
					tname.Trim(q);
				String text;
				text << "typename " << tname << " {\n";
				CopyTypeScriptType(text, type, "\t", "value");
				text << "}\n";
				WriteClipboardText(text);
			}
			catch(CParser::Error) {}
	}
}

void Pdb::MemMenu(ArrayCtrl& array, Bar& bar, const String& exp)
{
	if(bar.IsScanKeys())
		return;
	if(IsNull(exp))
		return;
	try {
		CParser p(exp);
		Val v = Exp(p);
		bool sep = true;
		if(v.ref > 0 && !v.reference) {
			sep = false;
			bar.Add("Memory at " + exp, THISBACK1(MemoryGoto, exp));
		}
		else
		if(v.rvalue) {
			sep = false;
			bar.Add("Memory at 0x" + FormatIntHex((dword)GetInt64(v)), THISBACK1(MemoryGoto, "&" + exp));
		}
		if(!v.rvalue) {
			sep = false;
			bar.Add("Memory at &&" + exp, THISBACK1(MemoryGoto, "&" + exp));
		}
	}
	catch(CParser::Error e) {
	}
}

void Pdb::WatchMenu(Bar& bar, const String& exp)
{
	bar.Add(exp.GetCount(), "Watch", [=] { AddWatch(exp); }).Key(IK_DBL_CLICK);
}

void Pdb::DataMenu(ArrayCtrl& array, Bar& bar)
{
	String exp = array.GetKey();
	WatchMenu(bar, exp);
	MemMenu(array, bar, exp);
	bar.Separator();
	CopyMenu(array, bar);
}

void Pdb::AddWatch(const String& s)
{
	if(s.GetCount()) {
		SetTab(3);
		if(!watches.FindSetCursor(s))
			watches.Add(s);
		Data();
	}
}

void Pdb::AddWatch()
{
	String s;
	if(EditPDBExpression("Add watch", s, this))
		AddWatch(s);
}

void Pdb::EditWatch()
{
	String s = watches.GetKey();
	if(EditPDBExpression("Edit watch", s, this)) {
		SetTab(2);
		watches.Set(0, s);
		Data();
	}
}

void Pdb::WatchesMenu(Bar& bar)
{
	String exp = watches.GetKey();
	if(!IsNull(exp))
		exp = "(" + exp + ")";
	MemMenu(watches, bar, exp);
	bar.Separator();
	CopyMenu(watches, bar);
	bar.Separator();
	bar.Add(AK_ADDWATCH, [=] { AddWatch(); });
	bool b = watches.IsCursor();
	bar.Add(b, "Edit watch..", THISBACK(EditWatch));
	bar.Add(b, "Remove watch..", [=] {
		watches.DoRemove();
		Data();
	});
	bar.Add(b, "Remove all watches..", [=] {
		if(PromptYesNo("Remove all watches?")) { watches.Clear(); Data(); }
	});
	bar.Separator();
	watches.StdBar(bar);
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
