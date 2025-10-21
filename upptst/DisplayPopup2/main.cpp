#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.AddColumn("Test");
	list.Add("Simple");
	for(int i = 0; i < 10; i++)
		list.Add("Long " + String('X', i * 100));
	DropList dl;
	dl.Add("Simple");
	for(int i = 0; i < 10; i++)
		dl.Add("Long " + String('X', i * 100));
	
	TreeCtrl tree;
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

	ColumnList clist;
	clist.Columns(3);
	clist.MultiSelect();

	for(int i = 0; i < 500; i++)
		clist.Add(FormatIntRoman(i));
	
	TopWindow win;
	win.Sizeable().Zoomable();
	win.Add(dl.TopPosZ(0).LeftPosZ(0, 100));
	win.Add(list.VSizePosZ(Zx(20), 0).LeftPosZ(0, 100));
	win.Add(tree.VSizePosZ(Zx(20), 0).LeftPosZ(100, 100));
	win.Add(clist.VSizePosZ(Zx(20), 0).LeftPosZ(200, 100));
	win.Run();
}
