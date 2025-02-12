#include "CtrlLib/CtrlLib.h"

using namespace Upp;

struct App : TopWindow {
	TreeCtrl   tree;

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
		tree.SetRoot(Image(), "The Tree");
		for(int i = 1; i < 10000; i++) {
			parent.Add(tree.Add(parent[rand() % parent.GetCount()], Image(),
			            FormatIntRoman(i, true)));
			if((rand() & 3) == 0)
				tree.Open(parent.Top());
		}
		tree.Open(0);
		tree.WhenDropInsert = THISFN(DropInsert);
		tree.WhenDrag = THISFN(Drag);
		tree.MultiSelect();
		Sizeable();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
