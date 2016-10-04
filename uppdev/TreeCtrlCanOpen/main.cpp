#include "CtrlLib/CtrlLib.h"

using namespace Upp;

struct App : TopWindow {
    TreeCtrl   tree;

    typedef App CLASSNAME;

    void OpenDir(int id) {
        String path = tree[id];

        for(FindFile ff(AppendFileName(path, "*.*")); ff; ff.Next()) {
            String n = ff.GetName();

            if(n != "." && n != ".." && ff.IsFolder()) {
                tree.Add(id, CtrlImg::Dir(),
                          AppendFileName(path, n), n, true);
            }
        }
        
        // I expect to get the +/- sign removed if a folder does not contain other folders
        // using the following code:
        if (tree.GetChildCount(id) == 0)
        {
			TreeCtrl::Node node = tree.GetNode(id);
            node.CanOpen(false);
            tree.SetNode(id, node);
        }
    }

    void CloseDir(int id) {
        tree.RemoveChildren(id);
    }

    App() {
        Add(tree.SizePos());

        tree.WhenOpen = THISBACK(OpenDir);
        tree.WhenClose = THISBACK(CloseDir);

		String dir = "u:/upp.src";

        tree.SetRoot(CtrlImg::Dir(), dir);

        Sizeable();
    }
};

GUI_APP_MAIN
{
    App().Run();
}
