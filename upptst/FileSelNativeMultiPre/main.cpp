#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSelNative sel;

	sel.AllFilesType();
	sel.Type("All", "*.*");
	sel.Multi();
	sel.NoAsking();
	sel = GetDataFile("main.cpp");

	if(sel.ExecuteSaveAs("save as"))
		PromptOK("\1" + ~sel);
}
