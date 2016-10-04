#include "Tree.h"

struct App : TopWindow {
	Splitter   horz;
	TreeCtrl   tree1;
	TreeCtrl   tree2;
	OptionTree optree;
	Option     x[10];
	Array<EditString> edit;

	typedef App CLASSNAME;

	void OpenDir(int id) {
		String path;
		for(int q = id; q >= 0; q = tree1.GetParent(q))
			path = AppendFileName(tree1[q], path);
		for(FindFile ff(AppendFileName(path, "*.*")); ff; ff.Next()) {
			String n = ff.GetName();
			if(n != "." && n != "..")
				tree1.Add(id, ff.IsFolder() ? CtrlImg::error() : CtrlImg::File(), n, ff.IsFolder());
		}
	}

	void CloseDir(int id) {
		tree1.RemoveChildren(id);
	}

	void LoadTree(int parent, const String& path)
	{
		for(FindFile ff(AppendFileName(path, "*.*")); ff; ff.Next()) {
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
					LoadTree(q, AppendFileName(path, n));
			}
		}
	}


	App() {
		horz.Add(tree1);
		horz.Add(tree2);
		horz.Add(optree);
		Add(horz.Horz().HSizePos().VSizePos(20, 0));
		optree.SetRoot("everything");
		for(int i = 0; i < 10; i++)
			optree.Add(i ? rand() % i : 0, x[i], AsString(i));
		tree1.WhenOpen = THISBACK(OpenDir);
		tree1.WhenClose = THISBACK(CloseDir);
		tree1.SetRoot(CtrlImg::Dir(), "E:\\");
		tree2.SetRoot(CtrlImg::Dir(), "E:\\");
		LoadTree(0, "F:\\");
		tree2.NoCursor();
		tree2.SortDeep(0);
		Sizeable();
	}
};

void Dump(TreeCtrl& tree, int id)
{
	LOG(tree.Get(id));
	int c = tree.GetChildCount(id);
	if(c) {
		LOGBEGIN();
		for(int i = 0; i < c; i++)
			Dump(tree, tree.GetChild(id, i));
		LOGEND();
	}
}

GUI_APP_MAIN
{
	App().Run();
}
