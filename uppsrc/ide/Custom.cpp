#include "ide.h"

struct Custom : public WithCustomLayout<TopWindow> {
	LineEdit   output;
	LineEdit   command;

	DropList   package;
	EditString when;
	EditString extension;

	void InsertCmd(String s);
	void InsertOut(String s);
	void DoMenu(Button& b, Callback1<String> cb, bool cmd);
	void OutMenu();
	void CmdMenu();

	void Perform(const char *main);

	Custom();

	typedef Custom CLASSNAME;
};

void Custom::InsertCmd(String s) {
	command.Paste(s.ToWString());
	command.SetFocus();
}

void Custom::InsertOut(String s) {
	output.Paste(s.ToWString());
	output.SetFocus();
}

void Custom::DoMenu(Button& b, Callback1<String> cb, bool cmd) {
	if(!list.IsCursor()) return;
	MenuBar menu;
//	Event<> Arg<String> set;
//	set <<= cb;
	String pk = list.Get(0);
	String ext = (String)list.Get(2);
	if(ext[0] != '.')
		ext = '.' + ext;
	String samplefile = "foo/" + ForceExt("sample", ext);
	String sample = SourcePath(pk, samplefile);
	menu.Add("Input file path (like '" + UnixPath(sample) + "')", callback1(cb, "$(PATH)"));
	menu.Add("Input file package-relative path (like '" + samplefile + "')", callback1(cb, "$(RELPATH)"));
	menu.Add("Input file directory (like '" + samplefile + "')", callback1(cb, "$(FILEDIR)"));
	menu.Add("Package directory (like '" + UnixPath(GetFileFolder(sample)) + "')", callback1(cb, "$(DIR)"));
	menu.Add("Input file name (like '" + GetFileName(sample) + "')", callback1(cb, "$(FILE)"));
	menu.Add("Input file title (like '" + GetFileTitle(sample) + "')", callback1(cb, "$(TITLE)"));
	menu.Add("Input file package (like '" + pk + "')", callback1(cb, "$(PACKAGE)"));
	Vector<String> ss = SplitFlags0((String)list.Get(1));
	String opath = "f:/out/mypackage/WIN32-ST/somefile.ext";
	menu.Add("Package output directory (like '" + opath + "')", callback1(cb, "$(OUTDIR)"));
	opath = "f:/out/WIN32-ST/myapp.exe";
	menu.Add("Executable path (like '" + UnixPath(opath) + "')", callback1(cb, "$(EXEPATH)"));
	menu.Add("Executable directory (like '" + UnixPath(GetFileFolder(opath)) + "')", callback1(cb, "$(EXEDIR)"));
	menu.Add("Executable filename (like '" + UnixPath(GetFileName(opath)) + "')", callback1(cb, "$(EXEFILE)"));
	menu.Add("Executable title (like '" + UnixPath(GetFileTitle(opath)) + "')", callback1(cb, "$(EXETITLE)"));
	menu.Add("Semicolon separated include dirs (like '/bin/include;/cpp/inc')", callback1(cb, "$(INCLUDE)"));
	menu.Add("Prefixed include dirs (like '-inc/include -inc/cpp/inc' for '$(!-inc)')",
		     callback1(cb, "$(!)"));
	menu.Add("$ character", callback1(cb, "$$"));
	if(cmd) {
		menu.Separator();
		menu.Add("copy command", callback1(cb, "cp "));
		menu.Add("chdir command", callback1(cb, "cd "));
	}
	menu.Execute(b.GetScreenRect().BottomLeft());
}

void Custom::OutMenu() {
	DoMenu(outputmenu, THISBACK(InsertOut), false);
}

void Custom::CmdMenu() {
	DoMenu(commandmenu, THISBACK(InsertCmd), true);
}

Custom::Custom() {
	CtrlLayoutExit(*this, "Custom build steps");
	Sizeable().MaximizeBox();
	list.AutoHideSb();
	list.Appending().Removing();
	list.AddColumn("Host package", 85).Edit(package);
	list.AddColumn("When", 416).Edit(when);
	when.SetFilter(CondFilter);
	list.AddColumn(".extension", 74).Edit(extension);
	list.AddCtrl(command);
	list.AddCtrl(output);
	outputmenu.SetFrame(InsetFrame());
	outputmenu.SetImage(CtrlImg::smalldown()).NoWantFocus();
	outputmenu <<= THISBACK(OutMenu);
	commandmenu.SetFrame(InsetFrame());
	commandmenu.SetImage(CtrlImg::smalldown()).NoWantFocus();
	commandmenu <<= THISBACK(CmdMenu);
}

class CustomOrder : public ArrayCtrl::Order {
public:
	virtual bool operator()(const Vector<Value>& row1, const Vector<Value>& row2) const {
		for(int i = 0; i < 3; i++)
			if(String(row1[i]) != String(row2[i]))
				return String(row1[i]) < String(row2[i]);
		return false;
	}
};

void Custom::Perform(const char *main) {
	Workspace wspc;
	wspc.Scan(main);
	list.Clear();
	int i;
	for(i = 0; i < wspc.GetCount(); i++) {
		package.Add(wspc[i]);
		if(i == 0)
			list.IndexInfo(0).InsertValue(wspc[i]);
		Array<CustomStep>& m = wspc.GetPackage(i).custom;
		for(int j = 0; j < m.GetCount(); j++)
			list.Add(wspc[i], m[j].when, m[j].ext, m[j].command, m[j].output);
	}
	list.Sort(Single<CustomOrder>());
	list.SetCursor(0);
	Run();
	for(i = 0; i < wspc.GetCount(); i++)
		wspc.GetPackage(i).custom.Clear();
	for(i = 0; i < list.GetCount(); i++) {
		String pk = list.Get(i, 0);
		for(int k = 0; k < wspc.GetCount(); k++)
			if(wspc[k] == pk) {
				CustomStep& m = wspc.GetPackage(k).custom.Add();
				m.when = list.Get(i, 1);
				m.ext = list.Get(i, 2);
				m.command = list.Get(i, 3);
				m.output = list.Get(i, 4);
				break;
			}
	}
	for(i = 0; i < wspc.GetCount(); i++) {
		String pp = PackageFile(wspc[i]);
		Package& pkg = wspc.GetPackage(i);
		if(pkg.GetCount() || FileExists(pp))
			pkg.Save(pp);
	}
}

void Ide::CustomSteps() {
	Custom dlg;
	dlg.Perform(main);
	ScanWorkspace();
}
