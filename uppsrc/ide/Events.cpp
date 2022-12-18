#include "ide.h"

struct EventsDlg : WithEventsLayout<TopWindow> {
	struct CbInfo {
		String scope;
		String type;
		String name;
	};
	
	String cls;
	Index<String> existing_methods; // to avoid duplicite names
	Array<CbInfo> cb;

	void GatherEvents(const String& var, Index<String>& done, const String& cls);
	void CbEdit(One<Ctrl>& ctrl);
	void Generate(String& ins, String& clip, const String& scope, bool in_line);

	typedef EventsDlg CLASSNAME;

	EventsDlg(const String& cls);
};

int FilterId(int c)
{
	return iscid(c) ? c : 0;
}

void EventsDlg::CbEdit(One<Ctrl>& ctrl)
{
	ctrl.Create<EditString>().SetFilter(FilterId);
}

EventsDlg::EventsDlg(const String& cls)
:	cls(cls) {
	CtrlLayoutOKCancel(*this, "Events");
	list.AddColumn("Defined");
	list.AddColumn("Type");
	list.AddColumn("Event");
	list.AddColumn("New Method Name").Ctrls(THISBACK(CbEdit));
	list.ColumnWidths("121 178 254 124");
	list.SetLineCy(EditField::GetStdHeight());
	list.EvenRowColor();
	list.SetEditLineCy();
	list.MultiSelect();
	Sizeable().Zoomable();

	for(const auto& f : ~CodeIndex())
		for(const AnnotationItem& m : f.value.items)
			if(m.nest == cls)
				existing_methods.FindAdd(m.name);

	Index<String> visited;
	GatherEvents("", visited, cls);

	Breaker(yes, IDYES);
}

void EventsDlg::GatherEvents(const String& var, Index<String>& visited, const String& cls)
{
	String h = cls + " " + var;
	if(IsNull(cls) || visited.Find(h) >= 0 || h.GetCount() > 200 || list.GetCount() > 500 || visited.GetCount() > 2000)
		return;
	visited.Add(h);
	for(const auto& f : ~CodeIndex())
		for(const AnnotationItem& m : f.value.items)
			if(m.nest == cls && IsVariable(m.kind) && (m.pretty.StartsWith("Event<") || m.pretty.StartsWith("Gate<"))) {
				String n = m.name;
				String event = Merge(".", var, n);
				if(n == "WhenAction")
					n = "Action";
				else {
					Vector<String> ss = Split(event, '.');
					for(int i = 0; i < ss.GetCount(); i++) {
						String& s = ss[i];
						if(s == "WhenAction")
							s.Clear();
						else {
							s.TrimStart("When");
							if(s.GetCount())
								s.Set(0, ToUpper(s[0]));
						}
					}
					n = Join(ss, "");
					while(existing_methods.Find(n) >= 0)
						n = "On" + n;
				}
				list.Add(cls, m.type, event, n);
			}

	for(const auto& f : ~CodeIndex())
		for(const AnnotationItem& m : f.value.items)
			if(m.id == cls && IsStruct(m.kind)) {
				// we cheat with With..<TopWindow> by splitting it to With... and TopWindow
				for(String s : Split(m.bases, [](int c) { return iscid(c) || c == ':' ? 0 : 1; }))
					GatherEvents(var, visited, s);
			}

	for(const auto& f : ~CodeIndex())
		for(const AnnotationItem& m : f.value.items) {
			if(m.nest == cls
			   && IsVariable(m.kind)
			   && !IsSourceFile(f.key)
			   && m.type.Find('&') < 0 && m.type.Find('*') < 0
			   && !m.name.StartsWith("dv___")
			   && m.pretty.Find("static") < 0) {
				GatherEvents(Merge(".", var, m.name), visited, m.type);
			}
		}
}

void EventsDlg::Generate(String& ins, String& clip, const String& scope, bool in_line)
{
	String n;
	Vector<String> nn = Split(cls, ':');
	if(nn.GetCount())
		n = nn.Top();
	String ac;
	for(int i = 0; i < list.GetCount(); i++) {
		if(list.IsSel(i)) {
			String type = list.Get(i, 1);
			String var = list.Get(i, 2);
			String method = list.Get(i, 3);

			String param;
			int q = type.Find('<');
			if(q >= 0) {
				int qq = type.ReverseFind('>');
				if(q < qq) {
					param = String(~type + q + 1, ~type + qq);
				}
			}

			q = var.ReverseFind('.');
			ins << '\t';
			if(q >= 0 && var.Mid(q + 1) == "WhenAction")
				ins << var.Mid(0, q) << " <<=";
			else
				ins << var << " =";
			if(in_line)
				ins << " [=](" << param << ") {\n\t}\n\t\n";
			else {
				ins << " THISFN(" << method << ");\n";
				String t = type.StartsWith("Gate") ? "bool " : "void ";
				clip << t << n << "::" << method << "(" << param << ")\n{\n}\n\n";
				ac << '\t' << t << method << "(" << param << ");\n";
			}
		}
	}
	clip << ac;
}

INITBLOCK
{
	RegisterGlobalConfig("EventsDlg");
}

void AssistEditor::Events()
{
	SortByKey(CodeIndex());
	String nest = FindCurrentNest();
	if(IsNull(nest))
		return;
	EventsDlg dlg(nest);
	LoadFromGlobal(dlg, "EventsDlg");
	int c = dlg.Run();
	StoreToGlobal(dlg, "EventsDlg");
	if(c != IDOK)
		return;
	String a, b;
	dlg.Generate(a, b, nest, c == IDYES);
	Paste(a.ToWString());
	if(c != IDYES)
		WriteClipboardText(b);
}
