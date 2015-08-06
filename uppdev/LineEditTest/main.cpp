#include <CtrlLib/CtrlLib.h>

using namespace Upp;

String fn = "c:/xxx/test.txt";

GUI_APP_MAIN
{
	LineEdit  editor;
//	FileIn in("C:/xxx/logs/local1.info");
//	FileIn in("C:/Users/CXL/Downloads/local1.info");
	editor <<= LoadFile(fn);
	
	editor.ShowSpaces().ShowLineEndings().ShowTabs();

//	editor.Load(in);

/*	RDUMP(editor.GetRectSelection());
	Rect r = Rect(0, 0, 5, editor.GetLineCount() - 3);
	RDUMP(r);
	editor.SetRectSelection(r);
	RDUMP(editor.GetRectSelection());
	{
		RTIMING("Remove selection");
		editor.Cut();
	}*/
	TopWindow win;   
	win.Add(editor.SizePos());
	win.Run();
	SaveFile(fn, ~editor);
}
