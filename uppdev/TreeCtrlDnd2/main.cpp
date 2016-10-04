#include "CtrlLib/CtrlLib.h"

using namespace Upp;

struct App : TopWindow {
	TreeCtrl   tree;
	Array<EditString> edit;

	typedef App CLASSNAME;

	void DropInsert(int parent, int ii, PasteClip& d)
	{
		tree.AdjustAction(parent, d);
		if(AcceptInternal<TreeCtrl>(d, "mytreedrag")) {
			tree.InsertDrop(parent, ii, d);
			tree.SetFocus();
			return;
		}
		if(AcceptText(d)) {
			tree.SetCursor(tree.Insert(parent, ii, Image(), GetString(d)));
			tree.SetFocus();
			return;
		}
	}

	void Drag()
	{
		if(tree.DoDragAndDrop(InternalClip(tree, "mytreedrag"),
		                       tree.GetDragSample()) == DND_MOVE)
			tree.RemoveSelection();
	}

	App() {
		Add(tree.SizePos());
		Vector<int> parent, parent2;
		parent.Add(0);
		tree.SetRoot(CtrlImg::menu_window(), "Groups");
		for(int i = 1; i < 100/*000*/; i++) {
			EditString & es = edit.Add();
			es.SetText(FormatIntRoman(i, true));
			parent.Add(tree.Add(parent[rand() % parent.GetCount()], Image(),
			            es));
			if((rand() & 3) == 0)
				tree.Open(parent.Top());
		}
		tree.Open(0);
		tree.WhenDropInsert = THISBACK(DropInsert);
		tree.WhenDrag = THISBACK(Drag);
		tree.MultiSelect();
		Sizeable();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
