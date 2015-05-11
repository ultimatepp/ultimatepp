#include "ide.h"

bool IsCb(String t) {
	int q = t.Find('<');
	if(q >= 0)
		t.Trim(q);
	return  t == "Callback" || t == "Callback1" || t == "Callback2" || t == "Callback3" ||
	        t == "Gate" || t == "Gate1" || t == "Gate2";
}

struct ThisbacksDlg : WithThisbacksLayout<TopWindow> {
	struct CbInfo {
		String scope;
		String type;
		String name;
	};

	Index<String> nname;
	Array<CbInfo> cb;

	void GatherCallbacks(const String& pfx, Index<String>& done, const String& scope, int access);
	void CbEdit(One<Ctrl>& ctrl);
	void Generate(String& ins, String& clip, const String& scope);

	typedef ThisbacksDlg CLASSNAME;

	ThisbacksDlg(const String& scope);
};

int FilterId(int c)
{
	return iscid(c) ? c : 0;
}

void ThisbacksDlg::CbEdit(One<Ctrl>& ctrl)
{
	ctrl.Create<EditString>().SetFilter(FilterId);
}

ThisbacksDlg::ThisbacksDlg(const String& scope)
{
	CtrlLayoutOKCancel(*this, "THISBACKs");
	list.AddColumn("Defined in");
	list.AddColumn("Type");
	list.AddColumn("Callback");
	list.AddColumn("Insert").Ctrls<Option>();
	list.AddColumn("Method name").Ctrls(THISBACK(CbEdit));
	list.SetLineCy(EditField::GetStdHeight());
	list.ColumnWidths("110 245 157 41 129");
	list.EvenRowColor();
	list.NoCursor();
	Sizeable().Zoomable();
	int q = CodeBase().Find(scope);
	if(q < 0)
		return;
	const Array<CppItem>& n = CodeBase()[q];
	for(int i = 0; i < n.GetCount(); i++)
		nname.FindAdd(n[i].name);
	Index<String> done;
	GatherCallbacks("", done, scope, PRIVATE);
}

void ThisbacksDlg::GatherCallbacks(const String& pfx, Index<String>& done,
                                   const String& scope, int access)
{
	if(IsNull(scope))
		return;
	String h = pfx + scope;
	if(done.Find(h) >= 0)
		return;
	done.Add(h);
	Vector<String> tparam;
	int q = CodeBase().Find(ParseTemplatedType(scope, tparam));
	if(q < 0)
		return;
	const Array<CppItem>& n = CodeBase()[q];
	for(int i = 0; i < n.GetCount(); i++) {
		const CppItem& m = n[i];
		if(m.IsData() && IsCb(m.qtype) && m.access <= access) {
			String n = m.name;
			String name = pfx + '.' + n;
			if(*name == '.')
				name = name.Mid(1);
			if(name == "WhenAction")
				n = "Action";
			else {
				Vector<String> ss = Split(name, '.');
				for(int i = 0; i < ss.GetCount(); i++) {
					String& s = ss[i];
					if(s == "WhenAction")
						s.Clear();
					else {
						if(memcmp(s, "When", 4) == 0)
							s = s.Mid(4);
						if(s.GetCount())
							s.Set(0, ToUpper(s[0]));
					}
				}
				n = Join(ss, "");
				while(nname.Find(n) >= 0)
					n = "On" + n;
			}
			list.Add(scope, m.type, name, 0, n, m.name);
		}
	}
	for(int i = 0; i < n.GetCount(); i++) {
		const CppItem& m = n[i];
		if(m.IsType() && m.access <= access) {
			Vector<String> b = Split(m.qptype, ';');
			ResolveTParam(b, tparam);
			for(int i = 0; i < b.GetCount(); i++)
				GatherCallbacks(pfx, done, b[i], min(access, (int)PROTECTED));
		}
	}
	for(int i = 0; i < n.GetCount(); i++) {
		const CppItem& m = n[i];
		if(m.IsData() && !IsCb(m.qtype)
		   && m.natural.Find('&') < 0 && m.natural.Find('*') < 0
		   && m.access <= access
		   && !m.name.StartsWith("dv___"))
			GatherCallbacks(pfx + "." + m.name, done, m.qtype, min(access, (int)PUBLIC));
	}
}

void ThisbacksDlg::Generate(String& ins, String& clip, const String& scope)
{
	String ac;
	for(int i = 0; i < list.GetCount(); i++) {
		if((bool)list.Get(i, 3)) {
			String type = list.Get(i, 1);
			String name = list.Get(i, 2);
			String method = list.Get(i, 4);
			int q = name.Find('.');
			if((String)list.Get(i, 5) == "WhenAction" && q >= 0)
				ins << name.Mid(0, q) << " <<= THISBACK(" << method;
			else
				ins << name << " = THISBACK(" << method;
			ins << ");\n";
			if(nname.Find(method) < 0) {
				String param;
				q = type.Find('<');
				if(q >= 0) {
					int qq = type.ReverseFind('>');
					if(q < qq) {
						param = String(~type + q + 1, ~type + qq);
						type.Trim(q);
					}
				}
				String n = scope;
				if(n[0] == ':' && n[1] == ':')
					n = n.Mid(2);
				String t = (memcmp(type, "Gate", 4) == 0 ? "bool " : "void ");
				clip << t << n << "::" << method << "(" << param << ")\n{\n}\n\n";
				ac << '\t' << t << method << "(" << param << ");\n";
			}
		}
	}
	clip << ac;
}

INITBLOCK
{
	RegisterGlobalConfig("ThisbacksDlg");
}

void AssistEditor::Thisbacks()
{
	Parser ctx;
	Context(ctx, GetCursor());
	if(IsNull(ctx.current_scope) || !ctx.IsInBody())
		return;
	ThisbacksDlg dlg(ctx.current_scope);
	LoadFromGlobal(dlg, "ThisbacksDlg");
	int c = dlg.Run();
	StoreToGlobal(dlg, "ThisbacksDlg");
	if(c != IDOK)
		return;
	String a, b;
	dlg.Generate(a, b, ctx.current_scope);
	Paste(a.ToWString());
	WriteClipboardText(b);
}
