#include "CtrlLib/CtrlLib.h"

using namespace Upp;

struct App : TopWindow {
	TreeCtrl   tree;
	Array<Option> option;

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
		int c = tree.DoDragAndDrop(InternalClip(tree, "mytreedrag"),
		                       tree.GetDragSample());
		PromptOK(AsString(c));
		if(c == DND_MOVE) {
			PromptOK("Hey!");
			tree.RemoveSelection();
		}
	}

	App() {
		Add(tree.SizePos());
		Vector<int> parent, parent2;
		parent.Add(0);
		tree.SetRoot(Image(), "The Tree");
		for(int i = 1; i < 10000; i++) {
			
			TreeCtrl::Node node(CtrlImg::File(), option.Add(), 20);	// mine		
			node.Set(FormatIntRoman(i, true));
			option.Top().NoWantFocus();
			parent.Add(tree.Add(parent[rand() % parent.GetCount()], node)); // mine
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
