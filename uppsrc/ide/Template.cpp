#include "ide.h"

int ReadTemplateType(CParser& p)
{
	static const char *nm[] = { "id", "filename", "option", "select", "text" };
	for(int i = 0; i < __countof(nm); i++)
		if(p.Id(nm[i]))
			return i;
	p.ThrowError("Unknown type");
	return 0;
}

PackageTemplate ReadTemplate(const char *filename)
{
	PackageTemplate pt;
	FileIn in(filename);
	String header;
	String l;
	for(;;) {
		if(in.IsEof())
			break;
		l = in.GetLine();
		if(l[0] == '@' && l[1] == '@')
			break;
		header << l << '\n';
	}
	CParser p(header, filename);
	p.PassId("template");
	pt.name = p.ReadString();
	pt.main = pt.sub = false;
	for(;;) {
		if(p.Id("main"))
			pt.main = true;
		else
		if(p.Id("sub"))
			pt.sub = true;
		else
			break;
	}
	p.PassChar(';');
	while(!p.IsEof()) {
		int type = ReadTemplateType(p);
		TemplateItem& m = pt.item.Add();
		m.type = type;
		if(type == TEMPLATEITEM_SELECT) {
			p.PassChar('(');
			do
				m.value.Add(p.ReadString());
			while(p.Char(','));
			p.PassChar(')');
		}
		m.label = p.ReadString();
		m.id = p.ReadId();
		if(p.Char('=')) {
			const char *b = p.GetPtr();
			while(!p.IsChar(';')) {
				if(p.IsEof())
					p.ThrowError("unexpected end of file");
				p.SkipTerm();
			}
			m.init = String(b, p.GetPtr());
		}
		p.PassChar(';');
	}
	for(;;) {
		pt.file.Add();
		pt.file.Top().filename = l.Mid(2);
		for(;;) {
			if(in.IsEof())
				return pt;
			l = in.GetLine();
			if(l[0] == '@' && l[1] == '@')
				break;
			if(l[0] == '?' && l[1] == '?')
				pt.file.Top().condition = l.Mid(2);
			else
				pt.file.Top().text << l << "\r\n";
		}
	}
}

int NoCr(int c)
{
	return c == '\r' ? 0 : c;
}

AppPreview::AppPreview()
{
	SetFrame(ViewFrame());
	AddFrame(sb);
	sb <<= THISBACK(Scroll);
	NoWantFocus();
}

AppPreview::~AppPreview() {}

void AppPreview::MouseWheel(Point p, int zdelta, dword keyflags) {
	sb.Wheel(zdelta);
}

void AppPreview::Layout()
{
	sb.SetTotal(line.GetCount());
	sb.SetPage(GetSize().cy / CourierZ(12).GetCy());
}

void AppPreview::Paint(Draw& w)
{
	Size sz = GetSize();
	int y = 0;
	int i = sb;
	while(y < sz.cy) {
		auto is_header = i < line.GetCount() && line[i].header;
		auto font = is_header ? ArialZ(12).Bold().Italic() : CourierZ(12);
		auto rect_color = is_header ? (IsDarkTheme() ? DarkTheme(LtCyan) : LtCyan) : SColorPaper;
		
		w.DrawRect(0, y, sz.cx, font.GetCy(), rect_color);
		if(i < line.GetCount())
			w.DrawText(0, y, line[i].text, font, SColorText);
		y += font.GetCy();
		i++;
	}
}

void AppPreview::Add(const String& data, bool header)
{
	Vector<String> l = Split(Filter(data, NoCr), '\n', false);
	for(int i = 0; i < l.GetCount(); i++) {
		Line& q = line.Add();
		const char *s = l[i];
		while(*s == '\t') {
			q.text << "    ";
			s++;
		}
		q.text << s;
		q.header = header;
	}
	Refresh();
	Layout();
}

int FilterPackageName(int c)
{
	return c == '_' || IsAlNum(c) || c == '/' ? c : c == '\\' ? '/' : 0;
}

TemplateDlg::TemplateDlg()
{
	CtrlLayoutOKCancel(*this, "Create new package");
	Sizeable().Zoomable();
	description <<= THISBACK(Preview);
	delay <<= THISBACK(Preview);
	delay.SetDelay(300);
	templist.AddColumn("Template");
	templist.WhenEnterRow = THISBACK(EnterTemplate);
	ok.Disable();
	package <<= THISBACK(EnableCreate);
	package.SetFilter(FilterPackageName);
	Icon(IdeImg::CreatePackage());
}

TemplateDlg::~TemplateDlg() {}

const PackageTemplate& TemplateDlg::ActualTemplate()
{
	if(templist.GetCursor() > 0)
		return pt[templist.GetCursor() - 1];
	static PackageTemplate t;
	return t;
}

int IdFilter(int c)
{
	return c == '_' || IsAlNum(c) ? c : 0;
}

int FilenameFilter(int c)
{
	return c == '_' || IsAlNum(c) || c == '.' ? c : 0;
}

String ToUpper_Caps(const String& name)
{
	String output("");
	int str_len = name.GetLength();
	for (int i = 0; i < str_len; ++i)
	{
		int ch = name[i];
		if (IsAlNum(ch))
		{
			output += ToUpper(ch);
			if (i < str_len - 1)
				if (IsLower(ch) && IsUpper(name[i + 1]))
					output += '_';
		}
		else
			output += '_';
	}
	return output;
}

ArrayMap<String, EscValue> TemplateDlg::MakeVars0()
{
	ArrayMap<String, EscValue> var;
	String n = ~package;
	int q = n.ReverseFind('/');
	var.Add("PACKAGE", q >= 0 ? n.Mid(q + 1) : n);
	var.Add("PACKAGE_TOUPPER", ToUpper(n));
	var.Add("PACKAGE_TOUPPER_CAPS", ToUpper_Caps(n));
	var.Add("DESCRIPTION", String(~description));
	return var;
}

ArrayMap<String, EscValue> TemplateDlg::MakeVars()
{
	const PackageTemplate& tp = ActualTemplate();
	ArrayMap<String, EscValue> var = MakeVars0();
	for(int i = 0; i < tp.item.GetCount(); i++)
		var.Add(tp.item[i].id, EscFromStdValue(~ctrl[i]));
	StdLib(var);
	return var;
}

void TemplateDlg::Preview()
{
	const PackageTemplate& tp = ActualTemplate();
	ArrayMap<String, EscValue> var = MakeVars();
	preview.Clear();
	for(int i = 0; i < tp.file.GetCount(); i++) {
		const FileTemplate& ft = tp.file[i];
		if(IsNull(ft.condition) || IsTrue(Evaluate(ft.condition, var))) {
			preview.Add(Expand(ft.filename, var), true);
			preview.Add(Expand(ft.text, var));
		}
	}
}

void TemplateDlg::Create()
{
	const PackageTemplate& tp = ActualTemplate();
	ArrayMap<String, EscValue> var = MakeVars();
	for(int i = 0; i < tp.file.GetCount(); i++) {
		const FileTemplate& ft = tp.file[i];
		if(IsNull(ft.condition) || IsTrue(Evaluate(ft.condition, var))) {
			String fn = Expand(ft.filename, var);
			if(!IsNull(fn)) {
				fn = AppendFileName(AppendFileName(~nest, (String)~package), fn);
				RealizePath(fn);
				SaveFile(fn, Expand(ft.text, var));
			}
		}
	}
	Package p;
	String f = AppendFileName(AppendFileName(~nest, (String)~package), (String)~package + ".upp");
	p.Load(f);
	p.description = ~description;
	p.Save(f);
}

void TemplateDlg::Inits()
{
	const PackageTemplate& tp = ActualTemplate();
	ArrayMap<String, EscValue> var = MakeVars0();
	for(int i = 0; i < tp.item.GetCount(); i++) {
		String init = tp.item[i].init;
		if(!IsNull(init) && !ctrl[i].IsModified()) {
			ctrl[i] <<= StdValueFromEsc(Evaluate(init, var));
			ctrl[i].ClearModify();
		}
	}
}

void TemplateDlg::EnterTemplate()
{
	const PackageTemplate& tp = ActualTemplate();
	ArrayMap<String, EscValue> var;
	var.Add("PACKAGE", (String)~package);
	StdLib(var);
	label.Clear();
	ctrl.Clear();
	Point pos = templist.GetRect().BottomLeft();
	pos.y += 8;
	int cx = template_end.GetRect().right - pos.x;
	for(int i = 0; i < tp.item.GetCount(); i++) {
		const TemplateItem& m = tp.item[i];
		if(m.type == TEMPLATEITEM_OPTION)
			ctrl.Create<Option>().SetLabel(m.label);
		else {
			label.Add().SetLabel(m.label).LeftPos(pos.x, cx).TopPos(pos.y);
			Add(label.Top());
			pos.y += label.Top().GetMinSize().cy;
			switch(m.type) {
			case TEMPLATEITEM_ID: {
				EditField *e = new EditField;
				e->SetFilter(IdFilter);
				ctrl.Add(e);
				break;
			}
			case TEMPLATEITEM_FILENAME: {
				EditField *e = new EditField;
				e->SetFilter(FilenameFilter);
				ctrl.Add(e);
				break;
			}
			case TEMPLATEITEM_SELECT: {
				DropList *d = new DropList;
				for(int i = 0; i < m.value.GetCount(); i++)
					d->Add(i, m.value[i]);
				if(m.value.GetCount())
					d->SetIndex(0);
				ctrl.Add(d);
				break;
			}
			case TEMPLATEITEM_TEXT: {
			    EditField *e = new EditField;
			    ctrl.Add(e);
			    break;
			}
			}
		}
		Add(ctrl.Top());
		ctrl.Top() << ~delay;
		ctrl.Top().LeftPos(pos.x, cx).TopPos(pos.y);
		pos.y += ctrl.Top().GetMinSize().cy + 6;
		ctrl.Top().ClearModify();
	}
	Inits();
	Preview();
}

void TemplateDlg::LoadNest(const char *path, bool main, bool recurse)
{
	FindFile ff(AppendFileName(path, "*.*"));
	while(ff) {
		String p = AppendFileName(path, ff.GetName());
		if(ff.IsFolder()) {
			if(recurse)
				LoadNest(p, main);
		}
		else
		if(ToLower(GetFileExt(ff.GetName())) == ".upt") {
			try {
				PackageTemplate t = ReadTemplate(p);
				if(main && t.main || !main && t.sub)
					pt.Add() = pick(t);
			}
			catch(CParser::Error e) {
				Exclamation("Package template [* " + DeQtf(p) + "] is invalid&[* " + e);
			}
		}
		ff.Next();
	}
}

void TemplateDlg::Load(const Vector<String>& p, bool main)
{
	pt.Clear();
	for(int i = 0; i < p.GetCount(); i++) {
		LoadNest(p[i], main);
		nest.Add(p[i]);
	}
	LoadNest(GetLocalDir(), main);
	LoadNest(GetFileFolder(ConfigFile("x")), main, false);
	Sort(pt, FieldRelation(&PackageTemplate::name, StdLess<String>()));
	templist.Clear();
	templist.Add("<empty>");
	for(int i = 0; i < pt.GetCount(); i++)
		templist.Add(pt[i].name);
	if(nest.GetCount())
		nest.SetIndex(0);
	templist.GoBegin();
	ActiveFocus(package);
}
