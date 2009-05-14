#include <CodeEditor/CodeEditor.h>

using namespace Upp;

GUI_APP_MAIN {
	for(int i = 0; i < 10000; i++) {
		TIMING("Test");
		::IsClipboardFormatAvailable(CF_TEXT);
	}
	TopWindow win;
	CodeEditor cedit; //2nd test
	{
		LineEdit edit;
		win.Add(edit.SizePos());
		win.Run();
		cedit <<= ~edit;
	}
	cedit.Highlight(CodeEditor::HIGHLIGHT_CPP);
	win.Add(cedit.SizePos());
	win.Run();
}
