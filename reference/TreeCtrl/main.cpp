#include "CtrlLib/CtrlLib.h"

using namespace Upp;

struct App : TopWindow {
	Splitter   horz;
	TreeCtrl   tree1;
	TreeCtrl   tree2;
	OptionTree optree;
	Option     x[10];
	StatusBar  info;
	Array<EditString> edit;

	typedef App CLASSNAME;

	void OpenDir(int id) {
		String path = tree1[id];
		for(FindFile ff(AppendFileName(path, "*.*")); ff; ff.Next()) {
			String n = ff.GetName();
			if(n != "." && n != "..")
				tree1.Add(id, ff.IsFolder() ? CtrlImg::error() : CtrlImg::File(),
				          AppendFileName(path, n), n, ff.IsFolder());
		}
	}

	void CloseDir(int id) {
		;
	}

	void LoadTree(int parent, const String& path, Progress& pi)
	{
		pi.SetText(DeFormat(path));
		for(FindFile ff(AppendFileName(path, "*.*")); ff; ff.Next()) {
			if(pi.StepCanceled())
				return;
			String n = ff.GetName();
			if(n != "." && n != "..") {
				edit.Add();
				edit.Top() <<= n;
				int q;
				static int x;
				if(++x & 1)
					q = tree2.Add(parent, ff.IsFolder() ? CtrlImg::Dir() : CtrlImg::File(),
				                  edit.Top(), 150);
				else
					q = tree2.Add(parent, ff.IsFolder() ? CtrlImg::Dir() : CtrlImg::File(), n);
				if(ff.IsFolder())
					LoadTree(q, AppendFileName(path, n), pi);
			}
		}
	}

	App() {
		horz.Add(tree1);
		horz.Add(tree2);
		horz.Add(optree);
		Add(horz.Horz().SizePos());
		optree.SetRoot("everything");
		for(int i = 0; i < 10; i++)
			optree.Add(i ? rand() % i : 0, x[i], AsString(i));
		tree1.MultiSelect();
		tree1.WhenOpen = THISFN(OpenDir);
		tree1.WhenClose = [=] (int id) { tree1.RemoveChildren(id); };
	#ifdef PLATFORM_WIN32
		String dir = String(GetExeFilePath()[0], 1) + ":\\";
	#else
		String dir = "/usr";
	#endif
		tree1.SetRoot(CtrlImg::Dir(), dir);
		tree2.SetRoot(CtrlImg::Dir(), dir);
		Progress pi;
		pi.AlignText(ALIGN_LEFT);
		LoadTree(0, dir, pi);
		tree2.NoCursor().NoRoot();
		tree2.SortDeep(0);
		Sizeable();

		tree1.WhenCursor = [=] { info = ~tree1; };
		tree1.AddFrame(info);
	}
};

GUI_APP_MAIN
{
	App().Run();
}
