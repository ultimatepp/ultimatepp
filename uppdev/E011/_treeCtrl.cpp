#include "e011.h"

void App::InitTree() {

	tree.Clear();
	tree.NoRoot();
	tree.Add(0, config_1(), 1, t_("Config"), false);
	tree.Add(0, mappa_1(), 2, t_("Map"), false);
	return;
}

void App::TreeCursor() {
	if(!IsNull(tree)) {
		if(tree.GetCursorLine() == 1) {
			mainFrame.Zoom(0);
			LoadConfForm();
			//tree.KillCursor();
		}
		else if(tree.GetCursorLine() == 2) {
			mainFrame.Zoom(1);
			//tree.KillCursor();
		}
		else {
			mainFrame.Zoom(100);
			tree.KillCursor();
		}
	}
}
