#include "CtrlLib/CtrlLib.h"

using namespace Upp;

struct App : TopWindow {
    OptionTree   tree;

    typedef App CLASSNAME;

    void OpenDir(int id) {
        String path = tree.GetLabel(id);

        for(FindFile ff(AppendFileName(path, "*.*")); ff; ff.Next()) {
            String n = ff.GetName();

            if(n != "." && n != ".." && ff.IsFolder()) {
                int childId = tree.Add(id, AppendFileName(path, n));
                tree.SetNode(childId, tree.GetNode(childId).CanOpen(true));
            }
        }
    }

    void CloseDir(int id) {
        tree.RemoveChildren(id);
    }

    App() {
        Add(tree.SizePos());

        tree.WhenOpen = THISBACK(OpenDir);
        tree.WhenClose = THISBACK(CloseDir);

    #ifdef PLATFORM_WIN32
        String dir = String(GetExeFilePath()[0], 1) + ":\\";
    #else
        String dir = "/usr";
    #endif

        tree.SetRoot(dir);
        tree.Set(0, dir);

        Sizeable();
    }
};

GUI_APP_MAIN
{
    App().Run();
}
