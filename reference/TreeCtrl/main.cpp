#include "CtrlLib/CtrlLib.h"

using namespace Upp;

struct App : TopWindow {
	TreeCtrl   tree;
	Array<EditString> edit;

	typedef App CLASSNAME;
	
	virtual bool Key(dword key, int count) {
		if(key == K_DELETE)
			tree.Clear();
		return false;
	}

	App() {
		Add(tree.SizePos());
		tree.Add(0, CtrlImg::cut(), edit.Add());
	}
};

GUI_APP_MAIN
{
	App().Run();
}
