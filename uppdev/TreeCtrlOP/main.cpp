#include "CtrlLib/CtrlLib.h"

using namespace Upp;


struct App : TopWindow {
	TreeCtrl   tree;
	SplitterFrame m_sf;
	Splitter test;
	ToolBar tools;
	StatusBar status;
	MenuBar menu;

	Array<Option> option;

	DocEdit edit;

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
//		Ctrl::ShowRepaint(50);

		AddFrame(TopSeparatorFrame());
		AddFrame(menu);
		AddFrame(TopSeparatorFrame());
		AddFrame(tools);
		AddFrame(TopSeparatorFrame());
		AddFrame(status);


//		AddFrame(m_sf.Left(tree.SizePos(),250));
//		AddFrame(InsetFrame());

		test.Horz(tree, edit);
		Add(test);

//		Add(tree.SizePos());

		Vector<int> parent, parent2;
		parent.Add(0);
		tree.SetRoot(Image(), "The Tree");
		for(int i = 1; i < 10000; i++)
		{
			Option& o = option.Add();
			o.SetLabel(FormatIntRoman(i, true)); // mine
	//		o.NoWantFocus();

			TreeCtrl::Node node(CtrlImg::File(), o, 300);
			if(Random(2))
				node.Set("Label");
			parent.Add(tree.Add(parent[rand() % parent.GetCount()], node)); // mine

//			parent.Add(tree.Add(parent[rand() % parent.GetCount()], CtrlImg::File(),
//					            FormatIntRoman(i, true)));
			if((rand() & 3) == 0)
				tree.Open(parent.Top());
		}
		tree.Open(0);
		tree.WhenDropInsert = THISBACK(DropInsert);
		tree.WhenDrag = THISBACK(Drag);
		tree.MultiSelect();
		tree.HighlightCtrl();
		Sizeable();

	}
};



GUI_APP_MAIN
{
	App().Run();
}