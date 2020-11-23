#include "CtrlLib/CtrlLib.h"

using namespace Upp;

struct App : TopWindow {
	Splitter split;
	TreeCtrl tree1, tree2;
	EditInt  int_editor;

	typedef App CLASSNAME;
	
	App() {
		tree1.WhenEdited << [=](int id, const Value& v) { // using default EditString
			if(tree1.IsCursor())
				tree1.Set(id, v);
		};

		tree2.Editor(int_editor);
		tree2.WhenEdited << [=](int id, const Value& v) {
			if(tree2.IsCursor())
				tree2.Set(id, v);
		};

		split << tree1 << tree2;
		Add(split.SizePos());

		for(int i = 0; i < 100; i++) {
			tree1.Add(i ? Random(i) : 0, Null, AsString(Random()));
			tree2.Add(i ? Random(i) : 0, Null, (int)Random());
		}
		
		tree1.OpenDeep(0);
		tree2.OpenDeep(0);
	}
};

GUI_APP_MAIN
{
	App().Run();
}
