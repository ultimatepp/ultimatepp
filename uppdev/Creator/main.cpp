#include <CtrlLib/CtrlLib.h>
#include <Esc/Esc.h>

struct FileTemplate {
	String filename;
	String condition;
	String text;
};

enum { TEMPLATEITEM_ID, TEMPLATEITEM_FILENAME, TEMPLATEITEM_OPTION, TEMPLATEITEM_SELECT };

struct TemplateItem {
	String         label;
	int            type;
	String         id;
	Vector<String> value;
	String         init;
};

struct PackageTemplate {
	String              name;
	bool                main;
	Array<TemplateItem> item;
	Array<FileTemplate> file;
};

int ReadTemplateType(CParser& p)
{
	static char *nm[] = { "id", "filename", "option", "select" };
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

struct AppPreview : Ctrl {
	virtual void Paint(Draw& w);
	virtual void Layout();

	ScrollBar      sb;
	struct Line : Moveable<Line> {
		String text;
		bool   header;
	};
	Vector<Line> line;

	void Scroll() { Refresh(); }

	void Clear() { line.Clear(); Refresh(); sb.Set(0); }
	void Add(const String& data, bool header = false);

	typedef AppPreview CLASSNAME;

	AppPreview() { SetFrame(InsetFrame()); AddFrame(sb); sb <<= THISBACK(Scroll); NoWantFocus(); }
};

int NoCr(int c)
{
	return c == '\r' ? 0 : c;
}

void AppPreview::Layout()
{
	sb.SetTotal(line.GetCount());
	sb.SetPage(GetSize().cy / ScreenInfo().GetFontInfo(Courier(12)).GetHeight());
}

void AppPreview::Paint(Draw& w)
{
	Size sz = GetSize();
	FontInfo fi = w.GetFontInfo(Courier(12));
	int y = 0;
	int i = sb;
	while(y < sz.cy) {
		bool hdr = i < line.GetCount() && line[i].header;
		w.DrawRect(0, y, sz.cx, fi.GetHeight(), hdr ? SLtCyan : SWhite);
		if(i < line.GetCount())
			w.DrawText(0, y, line[i].text, hdr ? Arial(12).Bold().Italic() : Courier(12), SBlack);
		y += fi.GetHeight();
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

#define LAYOUTFILE <Creator/main.lay>
#include <CtrlCore/lay.h>

struct TemplateDlg : public WithTemplateLayout<TopWindow>
{
	AppPreview      preview;

	Array<PackageTemplate> pt;

	Array<Label>  label;
	Array<Ctrl>   ctrl;
	DelayCallback delay;

	const PackageTemplate& ActualTemplate();

	void Preview();
	void EnterTemplate();
	void LoadNest(const char *path);
	void Load(const char *paths);
	void EnableCreate() { ok.Enable(!IsNull(package)); Inits(); }
	void Inits();

	ArrayMap<String, EscValue> MakeVars();
	void Create();

	typedef TemplateDlg CLASSNAME;
	TemplateDlg();
};

TemplateDlg::TemplateDlg()
{
	CtrlLayoutOKCancel(*this, "Create new main package");
	delay <<= THISBACK(Preview);
	delay.SetDelay(300);
	templist.AddColumn("Template");
	templist.WhenEnterRow = THISBACK(EnterTemplate);
	ok.Disable();
	package <<= THISBACK(EnableCreate);
}

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

ArrayMap<String, EscValue> TemplateDlg::MakeVars()
{
	const PackageTemplate& tp = ActualTemplate();
	ArrayMap<String, EscValue> var;
	var.Add("PACKAGE", (String)~package);
	for(int i = 0; i < tp.item.GetCount(); i++)
		var.Add(tp.item[i].id, EscFromStdValue(~ctrl[i]));
	StdLib(var);
	return var;
}

void TemplateDlg::Preview()
{
	const PackageTemplate& tp = ActualTemplate();
	ArrayMap<String, EscValue> var = MakeVars();
	int sc = preview.sb;
	preview.Clear();
	for(int i = 0; i < tp.file.GetCount(); i++) {
		const FileTemplate& ft = tp.file[i];
		if(IsNull(ft.condition) || IsTrue(Evaluate(ft.condition, var))) {
			preview.Add(Expand(ft.filename, var), true);
			preview.Add(Expand(ft.text, var));
		}
	}
	preview.sb = sc;
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
}

void TemplateDlg::Inits()
{
	const PackageTemplate& tp = ActualTemplate();
	ArrayMap<String, EscValue> var;
	var.Add("PACKAGE", (String)~package);
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
	int cx = cancel.GetRect().right - pos.x;
	for(int i = 0; i < tp.item.GetCount(); i++) {
		const TemplateItem& m = tp.item[i];
		if(m.type == TEMPLATEITEM_OPTION)
			ctrl.Add(new Option).SetLabelText(m.label);
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
					d->Add(m.value[i]);
				ctrl.Add(d);
				break;
			}
			}
		}
		Add(ctrl.Top());
		ctrl.Top() << delay;
		ctrl.Top().LeftPos(pos.x, cx).TopPos(pos.y);
		pos.y += ctrl.Top().GetMinSize().cy + 10;
		ctrl.Top().ClearModify();
	}
	Preview();
	Inits();
}

void TemplateDlg::LoadNest(const char *path, bool main, bool sub)
{
	FindFile ff(AppendFileName(path, "*.*"));
	while(ff) {
		String p = AppendFileName(path, ff.GetName());
		if(ff.IsFolder())
			LoadNest(p);
		else
		if(ToLower(GetFileExt(ff.GetName())) == ".upt") {
			try {
				PackageTemplate t = ReadTemplate(p);
				if(main && t.main || sub && t.sub)
					pt.Add() = t;
			}
			catch(CParser::Error e) {
				Exclamation("Package template [* " + DeQtf(p) + "] is invalid&[* " + e);
			}
		}
		ff.Next();
	}
}

void TemplateDlg::Load(const char *paths, bool main, bool sub)
{
	pt.Clear();
	Vector<String> p = Split(paths, ';');
	for(int i = 0; i < p.GetCount(); i++) {
		LoadNest(p[i], name, sub);
		nest.Add(p[i]);
	}
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

void AppMain()
{
	PackageTemplate pt;
	try {
		pt = ReadTemplate("f:/test.upt");
	}
	catch(CParser::Error& e) {
		DUMP(e);
		return;
	}
	DUMP(pt.name);
	for(int i = 0; i < pt.item.GetCount(); i++) {
		DUMP(pt.item[i].type);
		DUMP(pt.item[i].label);
		DUMP(pt.item[i].id);
		DUMP(pt.item[i].init);
	}
	AppPreview pw;
	for(int i = 0; i < pt.file.GetCount(); i++) {
		LOG("--------------");
		FileTemplate ft = pt.file[i];
		DUMP(pt.file[i].filename);
		DUMP(pt.file[i].condition);
		DUMP(pt.file[i].text);
		pw.Add(ft.filename, true);
		pw.Add(ft.text);
	}
	TemplateDlg d;
	d.Load("F:\\uppdev");
	if(d.Run() == IDOK)
		d.Create();
	String s;
	s.
}
