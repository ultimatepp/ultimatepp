#include "ide.h"

struct NewPackageFileWindow : public WithNewPackageFileLayout<TopWindow> {
	NewPackageFileWindow();

	void   Type(const char *ext, const char *desc);
	String GetError();
	void   Sync();
	
	String folder;
};

NewPackageFileWindow::NewPackageFileWindow()
{
	CtrlLayoutOKCancel(*this, "New package file");

	type.SetLineCy(max(Zy(16), Draw::GetStdFontCy()));
	type.SetDropLines(20);
	Type("cpp", "C++ source file");
	Type("h", "C++ header file");
	type.AddSeparator();
	Type("lay", "Layout file (dialog templates)");
	Type("iml", "Image file (icons)");
	Type("icpp", "Initialization C++ source file");
	Type("usc", "Escape script file (scripting TheIDE)");
	Type("witz", "Skylark template file (web framework files)");
	Type("qtf", "U++ rich text file");
	type.AddSeparator();
	Type("json", "JSON file");
	Type("xml", "XML file");
	Type("html", "HTML file");
	Type("css", "CSS file");
	type.AddSeparator();
	Type("sch", "SQL schema file");
	Type("ddl", "SQL DDL script file");
	Type("sql", "SQL script file");
	type.AddSeparator();
	Type("java", "Java");
	Type("js", "JavaScript");
	Type("py", "Python");
	type.AddSeparator();
	Type("", "Other");
	
	name << [=] {
		String ext = GetFileExt(~~name);
		if(ext.GetCount()) {
			ext = ext.Mid(1);
			type <<= type.HasKey(ext) ? ext : Null;
		}
		Sync();
	};
	name <<= ".cpp";
	
	type <<= "cpp";
	
	type << [=] {
		String ext = ~type;
		if(ext.GetCount()) {
			String h = ~name;
			name <<= ForceExt(h, "." + ext);
			int q = GetFileTitle(h).GetCount();
			name.SetSelection(q, q);
		}
		Sync();
	};
	
	Sync();
}

String NewPackageFileWindow::GetError()
{
	String n = ~name;
	String p = AppendFileName(folder, n);
	if(FileExists(p))
		return String().Cat() << "File&[* \1" << p << "\1]&already exists!";
	if(*n == '.')
		return "Invalid filename!";
	return Null;
}


void NewPackageFileWindow::Sync()
{
	name.Error(GetError().GetCount());
}

void NewPackageFileWindow::Type(const char *ext, const char *desc)
{
	type.Add(ext, AttrText(desc).SetImage(IdeFileImage(String() << "x." << ext, false, false)));
}

void WorkspaceWork::NewPackageFile()
{
	NewPackageFileWindow dlg;
	dlg.folder = GetFileFolder(GetActivePackagePath());
	dlg.Open();
	dlg.name.SetFocus();
	dlg.name.SetSelection(0, 0);
	for(;;) {
		if(dlg.Run() != IDOK)
			return;
		String e = dlg.GetError();
		if(e.GetCount() == 0)
			break;
		Exclamation(e);
	}
	AddItem(~dlg.name, false, false);
}
