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
	Index<String> unneeded_qualifications;

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

	VirtualsDlg(const String& nest, const String& local_bases, const Index<String>& namespaces) {
		Index<String> classes;
		if(local_bases.GetCount())
			for(String b : Split(local_bases, ';'))
				GatherVirtuals(virtuals, b, classes, false);
		else
			GatherVirtuals(virtuals, nest, classes, true);

		for(String cls : classes) {
			Vector<String> split = Split(cls, ':');
			if(split.GetCount()) {
				split.Drop(); // remove type name (e.g. [Upp, TopWindow] -> [Upp])
				String qual;
				for(String s : split) {
					MergeWith(qual, "::", s);
					if(namespaces.Find(qual) < 0) // e.g. if base is Upp::TopWindow, we will not ignore Upp::
						unneeded_qualifications.FindAdd(qual);
				}
			}
		}
		
		for(VirtualMethod& m : virtuals) { // remove unneeded qualifications
			String r;
			const char *s = m.pretty;
			const char *b = s;
			while(*s) {
				if(iscib(*s)) {
					r.Cat(b, s);
					b = s;
					try {
						CParser p(s);
						String qual;
						while(!p.IsEof()) {
							qual = p.ReadId();
							if(!p.Char2(':', ':') || !p.IsId())
								break;
							if(unneeded_qualifications.Find(qual) >= 0)
								b = p.GetPtr(); // skip uneeded qualification
							qual << "::";
						}
						s = p.GetPtr();
					}
					catch(CParser::Error) {
						if(*s) s++; // should not happen, but if it does, move on
					}
				}
				else
					s++;
			}
			r.Cat(b, s);
			m.pretty = r;
		}
	
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

AnnotationItem AssistEditor::FindCurrentNest(String *local_bases)
{
	AnnotationItem cm;
	if(!WaitCurrentFile())
		return cm;
	cm = FindCurrentAnnotation();
	int li = GetCursorLine();
	if(IsFunction(cm.kind) && local_bases) { // do local classes
		for(const AnnotationItem& lm : locals) {
			if(lm.pos.y >= cm.pos.y && lm.pos.y <= li && lm.bases.GetCount())
				cm = lm;
			if(lm.pos.y > li)
				break;
		}
		*local_bases = cm.bases;
		return cm;
	}

	if(IsNull(cm.nest))
		Exclamation("No class can be associated with current position.");
	return cm;
}

void AssistEditor::Virtuals()
{
	SortByKey(CodeIndex());
	String local_bases;
	AnnotationItem cm = FindCurrentNest(&local_bases);
	if(IsNull(cm.nest))
		return;

	Point pos = GetCurrentPos();
	AnnotationItem q; // class declaration to gather explicit namespaces in bases
	for(const AnnotationItem& m : annotations) {
		if(m.pos.y > pos.y)
			break;
		if(IsStruct(m.kind))
			q = m;
	}
	

	Index<String> namespaces; // These namespaces cannot be ignored

	try {
		int line = q.pos.y;
		while(line < GetLineCount()) {
			String s = GetUtf8Line(line++);
			CParser p(s);
			String prospective_namespace;
			bool done = false;
			while(!p.IsEof()) {
				if(p.IsId())
					prospective_namespace << p.ReadId();
				else
				if(p.Char2(':', ':')) {
					if(prospective_namespace.GetCount() > 2)
						namespaces.Add(prospective_namespace);
					prospective_namespace << "::";
				}
				else {
					prospective_namespace.Clear();
					if(p.Char('{') || p.Char(';')) {
						done = true;
						break;
					}
					p.Skip();
				}
			}
			if(done)
				break;
		}
	}
	catch(CParser::Error()) {}
	VirtualsDlg dlg(cm.nest, local_bases, namespaces);
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
			ctext << MakeDefinition(m, cm.nest + "::");
		}
	Paste(text.ToWString());
	WriteClipboardText(ctext);
}

void AssistEditor::ConvertToOverrides()
{
	Make([](String& out) {
		String r;
		const char *s = out;
		bool virt = false;
		int lvl = 0;
		while(*s) {
			const char *b = s;
			while(iscid(*s))
				s++;
			int len = int(s - b);
			if(len == 7 && memcmp(b, "virtual", 7) == 0) {
				virt = true;
				lvl = 0;
				while(*s == ' ' || *s == '\t')
					s++;
			}
			else {
				r.Cat(b, s);
				if(*s == '(')
					lvl++;
				r.Cat(*s);
				if(*s == ')') {
					lvl = max(lvl - 1, 0);
					if(lvl == 0 && virt) {
						r << " override";
						virt = false;
					}
				}
				s++;
			}
		}
		
		out = r;
	});
}

