#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	MenuBar   menu;

	ArrayCtrl list;
	DropList dl;
	TreeCtrl tree;
	ColumnList clist;

	MyApp() {
		list.AddColumn("Test");
		list.Add("Simple");
		for(int i = 0; i < 10; i++)
			list.Add("Long " + String('X', i * 100));
		dl.Add("Simple");
		for(int i = 0; i < 10; i++)
			dl.Add("Long " + String('X', i * 100));
		
		Vector<int> parent, parent2;
		parent.Add(0);
		tree.SetRoot(Image(), "The Tree");
		Array<Option> option;
		for(int i = 1; i < 10000; i++) {
			parent.Add(tree.Add(parent[rand() % parent.GetCount()],
			            i & 1 ? CtrlImg::open() : Image(),
			            FormatIntRoman(i, true)));
			if((rand() & 3) == 0)
				tree.Open(parent.Top());
		}
		tree.Open(0);
		tree.MultiSelect().ScrollIntoX();
	
		clist.Columns(3);
		clist.MultiSelect();
	
		for(int i = 0; i < 500; i++)
			clist.Add(FormatIntRoman(i));
		
		Sizeable().Zoomable();
		Add(dl.TopPosZ(0).LeftPosZ(0, 100));
		Add(list.VSizePosZ(Zx(20), 0).LeftPosZ(0, 100));
		Add(tree.VSizePosZ(Zx(20), 0).LeftPosZ(100, 100));
		Add(clist.VSizePosZ(Zx(20), 0).LeftPosZ(200, 100));
		
		AddFrame(menu);
		
		for(int i = 0; i < 6; i++)
			menu.Sub(AsString(i), [=](Bar& menu) {
				for(int i = 0; i < 15; i++)
					menu.Add(AsString(i), [] { PromptOK("!"); });
			});
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
