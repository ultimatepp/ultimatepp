#include "Browser.h"

struct StyleDlg : public WithStylesheetLayout<TopWindow> {
	virtual bool Accept();

	EditString editname;

	void Edit();
	void Load();
	void Save();
	String CommonFile(const char *s)      { return AppendFileName(GetCommonDir(), s); }

	typedef StyleDlg CLASSNAME;

	StyleDlg();
};

void StyleDlg::Edit()
{
	if(!style.IsCursor())
		return;
	RichEdit e;
	e.SetQTF((String)style.Get(1));
	e.Styles();
	style.Set(1, StylesAsQTF(e.Get()));
}

void StyleDlg::Load()
{
	style.Clear();
	FindFile ff(CommonFile("*.style"));
	while(ff) {
		style.Add(GetFileTitle(ff.GetName()), LoadFile(CommonFile(ff.GetName())));
		ff.Next();
	}
	style.Sort();
	style.GoBegin();
}

bool StyleDlg::Accept()
{
	if(!TopWindow::Accept())
		return false;
	Index<String> nm;
	for(int i = 0; i < style.GetCount(); i++) {
		String n = style.Get(i, 0);
		if(nm.Find(n) >= 0) {
			Exclamation("Duplicate style [* " + DeQtf(n) + "] !");
			return false;
		}
	}
	return true;
}

void StyleDlg::Save()
{
	Index<String> saved;
	for(int i = 0; i < style.GetCount(); i++) {
		String fn = CommonFile((String)style.Get(i, 0) + ".style");
		saved.Add(fn);
		String st = style.Get(i, 1);
		if(LoadFile(fn) != st)
			SaveFile(fn, st);
	}
	FindFile ff(CommonFile("*.style"));
	while(ff) {
		String fn = CommonFile(ff.GetName());
		if(saved.Find(fn) < 0)
			DeleteFile(fn);
		ff.Next();
	}
}

StyleDlg::StyleDlg()
{
	CtrlLayoutOKCancel(*this, "Stylesheets");
	edit <<= THISBACK(Edit);
	style.NoHeader();
	style.AddColumn().Edit(editname);
	style.AddIndex().InsertValue("[%EN-US3 $$0,0#00000000000000000000000000000000:Default]");
	style.Inserting().Removing().Duplicating().NoAppendLine();
	editname.NotNull();
	editname.MaxLen(30);
	editname.SetFilter(CharFilterID);
}

int TopicEditor::Execute(StyleDlg& d)
{
	d.Load();
	d.style.FindSetCursor(laststylesheet);
	if(!d.style.IsCursor())
		d.style.GoBegin();
	return d.Run();
}

void TopicEditor::EditStylesheets()
{
	StyleDlg d;
	if(Execute(d) == IDOK) {
		d.Save();
		if(d.style.IsCursor())
			laststylesheet = d.style.GetKey();
	}
}

void TopicEditor::StoreStylesheet()
{
	StyleDlg d;
	d.Title("Store as stylesheet");
	d.edit.Hide();
	if(Execute(d) == IDOK) {
		if(d.style.IsCursor()) {
			d.style.Set(1, AsQTF(editor.Get(), CHARSET_UTF8, QTF_ALL_STYLES));
			laststylesheet = d.style.GetKey();
		}
		d.Save();
	}
}

void TopicEditor::ApplyStylesheet()
{
	if(!topic.IsCursor())
		return;
	StyleDlg d;
	d.Title("Apply stylesheet to current topic");
	d.edit.Hide();
	if(Execute(d) != IDOK)
		return;
	d.Save();
	editor.ApplyStylesheet(ParseQTF((String)d.style.Get(1)));
}

void TopicEditor::ApplyStylesheetGroup()
{
	StyleDlg d;
	d.Title("Apply stylesheet to current group");
	d.edit.Hide();
	if(Execute(d) != IDOK)
		return;
	d.Save();
	RichText ss = ParseQTF((String)d.style.Get(1));
	int c = topic.GetCursor();
	Progress pi("Applying stylesheet");
	for(int i = 0; i < topic.GetCount(); i++) {
		topic.SetCursor(i);
		editor.ApplyStylesheet(ss);
	}
	topic.SetCursor(c);
}
