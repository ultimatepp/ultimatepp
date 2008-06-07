#include "ide.h"

struct Custom : public WithCustomLayout<TopWindow> {
	LineEdit   output;
	LineEdit   command;

	DropList   package;
	EditString when;
	EditString extension;

	void InsertCmd(String s);
	void InsertOut(String s);
	void DoMenu(Button& b, Callback1<String> cb);
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

void Custom::DoMenu(Button& b, Callback1<String> cb) {
	if(!list.IsCursor()) return;
	MenuBar menu;
//	CallbackArg<String> set;
//	set <<= cb;
	String pk = list.Get(0);
	String ext = (String)list.Get(2);
	if(ext[0] != '.')
		ext = '.' + ext;
	String sample = SourcePath(pk, ForceExt("sample", ext));
	menu.Add("Input file path (like '" + UnixPath(sample) + "')", callback1(cb, "$(PATH)"));
	menu.Add("Input file directory (like '" + UnixPath(GetFileFolder(sample)) + "')", callback1(cb, "$(DIR)"));
	menu.Add("Input file name (like '" + GetFileName(sample) + "')", callback1(cb, "$(FILE)"));
	menu.Add("Input file title (like '" + GetFileTitle(sample) + "')", callback1(cb, "$(TITLE)"));
	Vector<String> ss = SplitFlags0((String)list.Get(1));
	String opath = "f:/out/app/WIN32-ST/somefile.ext";
	menu.Add("Package output directory (like '" + UnixPath(GetFileFolder(opath)) + "')", callback1(cb, "$(OUTDIR)"));
	menu.Add("Package output path (like '" + UnixPath(opath) + "')", callback1(cb, "$(OUTPATH)"));
	menu.Add("Package output path (like '" + GetFileName(opath) + "')", callback1(cb, "$(OUTFILE)"));
	menu.Add("Package output path (like '" + GetFileTitle(opath) + "')", callback1(cb, "$(OUTTITLE)"));
	menu.Add("Semicolon separated include dirs (like '/bin/include;/cpp/inc')", callback1(cb, "$(INCLUDE)"));
	menu.Add("Prefixed include dirs (like '-inc/include -inc/cpp/inc' for '$(!-inc)')",
		     callback1(cb, "$(!)"));
	menu.Add("$ character", callback1(cb, "$$"));
	menu.Execute(b.GetScreenRect().BottomLeft());
}

void Custom::OutMenu() {
	DoMenu(outputmenu, THISBACK(InsertOut));
}

void Custom::CmdMenu() {
	DoMenu(commandmenu, THISBACK(InsertCmd));
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
		String pp = PackagePath(wspc[i]);
		RealizePath(pp);
		wspc.GetPackage(i).Save(pp);
	}
}

void Ide::CustomSteps() {
	Custom dlg;
	dlg.Perform(main);
	ScanWorkspace();
}
