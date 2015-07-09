#include "LayDes.h"

struct VisGenDlg : public WithVisGenLayout<TopWindow> {
	LayoutData&  layout;
	Vector<int>  sel;

	void Refresh();
	bool HasItem(const char *id);
	void Type();
	String GetName();

	typedef VisGenDlg CLASSNAME;

	VisGenDlg(LayoutData& layout, const Vector<int>& cursor);
};

bool VisGenDlg::HasItem(const char *id)
{
	for(int i = 0; i < layout.item.GetCount(); i++)
		if(layout.item[i].variable == id)
			return true;
	return false;
}

String VisGenDlg::GetName()
{
	String n = layout.name;
	int l = n.GetCount() - 6;
	if(l > 0 && n.Mid(l) == "Layout")
		n = n.Mid(0, l);
	return n + "Dlg";
}

void VisGenDlg::Refresh()
{
	String s;
	int q = ~type;
	pars.Enable(q >= 2);
	brackets.Enable(q >= 2);
	label1.Enable(q >= 2);
	toupper1.Enable(q >= 2);
	name1.Enable(q >= 2);
	quotes1.Enable(q >= 2 && !label1);
	label2.Enable(q >= 3);
	toupper2.Enable(q >= 3);
	name2.Enable(q >= 3);
	quotes2.Enable(q >= 3 && !label2);
	String oce = "\tCtrlLayout";
	bool ok = false;
	if(HasItem("ok")) {
		ok = true;
		oce << "OK";
	}
	if(HasItem("cancel"))
		oce << "Cancel";
	if(HasItem("exit"))
		oce << "Exit";
	String n = ~name;
	if(IsNull(n))
		n = GetName();
	
	String b1, b2, b3;
	if(buttons) {
		for(int i = 0; i < layout.item.GetCount(); i++) {
			String bn = layout.item[i].variable;
			if(layout.item[i].type == "Button" && findarg(bn, "cancel", "ok", "exit") < 0) {
				String mn = InitCaps(bn);
				mn.Replace("_", "");
				if(b1.GetCount() == 0)
					b1 = b2 = "\n";
				b1 << '\t' << "void " << mn << "();\n";
				b2 << '\t' << bn << " <<= THISBACK(" << mn << ");\n";
				b3 << '\n' << "void " << n << "::" << mn << "()\n{\n}\n";	
			}
		}
	}

	if(q == 0) {
		s << "class " << n << " : public With" << layout.name << "<TopWindow> {\n"
		  << "\ttypedef " << n << " CLASSNAME;\n"
		  << b1
		  << "\n"
		  << "public:\n"
		  << "\t" << n << "();\n"
		  << "};\n"
		  << "\n"
		  << n << "::" << n << "()\n"
		  << "{\n"
		  << oce << "(*this, \"\");\n"
		  << b2
		  << "}\n"
		  << b3;
	}
	else
	if(q == 1) {
		s << "struct " << n << " : With" << layout.name << "<TopWindow> {\n"
		  << "\ttypedef " << n << " CLASSNAME;\n"
		  << b1
		  << "\t" << n << "();\n"
		  << "};\n"
		  << "\n"
		  << n << "::" << n << "()\n"
		  << "{\n"
		  << oce << "(*this, \"\");\n"
		  << b2
		  << "}\n"
		  << b3;
	}
	else
	if(q == 2) {
		String n = ~name;
		if(IsNull(n))
			n = "dlg";
		s << "\tWith" << layout.name << "<TopWindow> " << n << ";\n"
		  << oce << '(' << n << ", \"\");\n";
		if(ok)
			s << "\tif(" << n << ".Execute() != IDOK)\n\t\treturn;\n";
		else
			s << '\t' << n << ".Execute();\n";
	}
	else
		for(int i = 0; i < sel.GetCount(); i++) {
			String id1 = layout.item[sel[i]].variable;
			String id2 = id1;
			int w = layout.item[sel[i]].FindProperty("SetLabel");
			String lbl;
			if(w >= 0 && IsString(~layout.item[sel[i]].property[w]))
				lbl = AsCString(ToUtf8((WString)~layout.item[sel[i]].property[w]));
			if(label1)
				id1 = lbl;
			if(label2)
				id2 = lbl;
			if(!IsNull(id1) || (q == 4 && !IsNull(id2))) {
				if((pars || brackets) && !(name1 || name2))
					s << ~name;
				if(pars)
					s << '(';
				String ss;
				if(name1 && !IsNull(~name))
					s << ~name << '.';
				if(toupper1)
					ss << ToUpper(id1);
				else
					ss << id1;
				if(quotes1 && !label1)
					ss = AsCString(ss);
				if(brackets)
					s << '[';
				s << ss;
				if(brackets)
					s << ']';
				if(q == 4) {
					s << ", ";
					String ss;
					if(name2 && !IsNull(~name))
						ss << ~name << '.';
					if(toupper2)
						ss << ToUpper(id2);
					else
						ss << id2;
					if(quotes2 && !label2)
						ss = AsCString(ss);
					if(brackets)
						s << '[';
					s << ss;
					if(brackets)
						s << ']';
				}
				if(pars)
					s << ')';
				s << '\n';
			}
		}
	view <<= s;
}

void VisGenDlg::Type()
{
	String n = GetName();
	buttons.Disable();
	switch((int)~type) {
	case 0:
	case 1:
		name <<= GetName();
		buttons.Enable();
		break;
	case 2:
		name <<= "dlg";
		break;
	default:
		name <<= "";
	}
	Refresh();
}

VisGenDlg::VisGenDlg(LayoutData& layout, const Vector<int>& cursor)
:	layout(layout)
{
	type <<= 0;
	CtrlLayoutOKCancel(*this, "Code generator");
	type <<= THISBACK(Type);
	name <<=
	pars <<=
	brackets <<=
	label1 <<=
	toupper1 <<=
	quotes1 <<=
	name1 <<=
	label2 <<=
	toupper2 <<=
	quotes2 <<=
	name2 <<= 
	buttons <<= THISBACK(Refresh);
	Refresh();
	view.Highlight("cpp");
	view.HideBar();
	view.SetFont(Courier(12));
	if(cursor.GetCount())
		sel <<= cursor;
	else
		for(int i = 0; i < layout.item.GetCount(); i++)
			sel.Add(i);
}

void LayDes::VisGen()
{
	if(IsNull(currentlayout))
		return;
	VisGenDlg dlg(CurrentLayout(), cursor);
	if(dlg.Run() == IDOK)
		WriteClipboardText(~dlg.view);
}
