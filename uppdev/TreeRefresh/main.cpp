#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
//	Ctrl::ShowRepaint(100);
	TopWindow win;
	TreeCtrl tree;
	tree.SetRoot(Null, "Tree");
	Array<EditString> s;
	tree.BackPaint();
	for(int i = 0; i < 30; i++) {
		int id = tree.Add(0, Null, "Node " + AsString(i));
		for(int j = 0; j < 20; j++) {
			tree.Add(id, s.Add());
			s.Top() <<= AsString(j);
		}
	}
	win.Add(tree.SizePos());
	win.Run();
}
