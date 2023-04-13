#include "ide.h"

struct VirtualMethod : AnnotationItem {
	String defined;
	String last_override;
};

void GatherVirtuals(ArrayMap<String, VirtualMethod>& virtuals, const String& cls,
                    Index<String>& visited, bool first)
{
	if(IsNull(cls) || visited.Find(cls) >= 0)
		return;
	visited.Add(cls);
	for(const auto& f : ~CodeIndex()) // do base classes first
		for(const AnnotationItem& m : f.value.items) {
			if(m.id == cls && m.bases.GetCount()) {
				// we cheat with With..<TopWindow> by splitting it to With... and TopWindow
				for(String s : Split(m.bases, [](int c) { return iscid(c) || c == ':' ? 0 : 1; }))
					GatherVirtuals(virtuals, s, visited, false);
			}
		}
	if(!first)
		for(const auto& f : ~CodeIndex()) // now gather virtual methods of this class
			for(const AnnotationItem& m : f.value.items) {
				if(m.nest == cls && IsFunction(m.kind)) {
					String signature = m.id.Mid(m.nest.GetCount());
					int q = virtuals.Find(signature);
					if(q >= 0)
						virtuals[q].last_override = cls;
					else
					if(m.isvirtual) {
						VirtualMethod& vm = virtuals.Add(signature);
						(AnnotationItem&)vm = m;
						vm.defined = vm.last_override = cls;
					}
				}
			}
}

struct VirtualsDisplay : Display {
	void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const {
		w.DrawRect(r, paper);
		VirtualMethod m = q.To<VirtualMethod>();
		bool focuscursor = (style & (FOCUS|CURSOR)) == (FOCUS|CURSOR) || (style & SELECT);
		PaintCpp(w, r, m.kind, m.name, m.pretty, ink, focuscursor, true);
	}
};

struct VirtualsDlg : public WithVirtualsLayout<TopWindow> {
	ArrayMap<String, VirtualMethod> virtuals;

	virtual bool Key(dword key, int count)
	{
		if(key == K_CTRL_K) {
			find.SetFocus();
			return true;
		}
		return HotKey(key) || find.Key(key, count);
	}

	void Sync() {
		String name = ~find;
		String k;
		if(list.IsCursor())
			k = list.Get("id");
		list.Clear();
		for(const auto& m : ~virtuals)
			if(ToUpper(m.value.name).Find(name) >= 0)
				list.Add(RawToValue(m.value), m.value.defined, m.value.last_override, m.value.id);
		if(!list.FindSetCursor(k, "id"))
			list.GoBegin();
	}

	void DblClk() {
		if(list.IsCursor()) {
			list.Set(5, 1);
			Break(IDOK);
		}
	}

	void Serialize(Stream& s)
	{
		SerializePlacement(s);
		list.SerializeHeader(s);
		s % add_virtual % add_override;
	}

	typedef VirtualsDlg CLASSNAME;

	VirtualsDlg(const String& nest, const String& local_bases) {
		Index<String> done;
		if(local_bases.GetCount())
			for(String b : Split(local_bases, ';'))
				GatherVirtuals(virtuals, b, done, false);
		else
			GatherVirtuals(virtuals, nest, done, true);
		CtrlLayoutOKCancel(*this, "Virtual methods");
		list.AddColumn("Virtual function").SetDisplay(Single<VirtualsDisplay>());
		list.AddColumn("Defined in");
		list.AddColumn("Last override");
		list.AddIndex("id");
		list.SetLineCy(Arial(Zy(11)).Info().GetHeight() + DPI(3));
		list.ColumnWidths("500 100 100");
		list.WhenLeftDouble = THISBACK(DblClk);
		list.MultiSelect();
		list.EvenRowColor();
		Sizeable().Zoomable();
		find.NullText("Search (Ctrl+K)");
		find.SetFilter([](int c) { return iscid(c) ? ToUpper(c) : 0; });
		find <<= THISBACK(Sync);
		add_override <<= true;
		add_virtual <<= false;
		Sync();
	}
};

INITBLOCK
{
	RegisterGlobalConfig("VirtualsDlg");
}

String AssistEditor::FindCurrentNest(String *local_bases)
{
	if(!WaitCurrentFile())
		return Null;
	AnnotationItem cm = FindCurrentAnnotation();
	int li = GetCursorLine();
	if(IsFunction(cm.kind) && local_bases) { // do local classes
		for(const AnnotationItem& lm : locals) {
			if(lm.pos.y >= cm.pos.y && lm.pos.y <= li && lm.bases.GetCount())
				cm = lm;
			if(lm.pos.y > li)
				break;
		}
		*local_bases = cm.bases;
		return cm.nest;
	}

	if(IsNull(cm.nest)) {
		Exclamation("No class can be associated with current position.");
		return Null;
	}
	return cm.nest;
}

void AssistEditor::Virtuals()
{
	SortByKey(CodeIndex());
	String local_bases;
	String nest = FindCurrentNest(&local_bases);
	if(IsNull(nest))
		return;
	VirtualsDlg dlg(nest, local_bases);
	LoadFromGlobal(dlg, "VirtualsDlg");
	int c = dlg.Run();
	StoreToGlobal(dlg, "VirtualsDlg");
	if(c != IDOK)
		return;
	String text;
	String ctext;
	if(!dlg.list.IsSelection() && dlg.list.IsCursor())
		dlg.list.Select(dlg.list.GetCursor());
	for(int i = 0; i < dlg.list.GetCount(); i++)
		if(dlg.list.IsSel(i)) {
			VirtualMethod m = dlg.list.Get(i, 0).To<VirtualMethod>();
			m.pretty.TrimEnd(" = 0");
			text << "\t";
			if(dlg.add_virtual)
				text << "virtual ";
			text << m.pretty;
			if(dlg.add_override)
				text << " override";
			text << ";\r\n";
			ctext << MakeDefinition(m, nest + "::");
		}
	Paste(text.ToWString());
	WriteClipboardText(ctext);
}
