#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <FileSelFileCtrl/format.lay>
#include <CtrlCore/lay.h>

GUI_APP_MAIN
{
	FileSel sel;
	
	sel.Types("Example\t*.ext");
	
	WithFormatCtrlLayout<ParentCtrl> format;
	CtrlLayout(format);
	format.list.Add("First");
	format.list.Add("Second");
	format.list.GoBegin();
	
	sel.FileCtrl(format);
	
	if(sel.ExecuteSaveAs())
		PromptOK(String() << "Path: [* \1" << ~sel << "\1]&"
		         << "Format: [* \1" << ~format.list);
}
