#include <CodeEditor/CodeEditor.h>

using namespace Upp;

GUI_APP_MAIN {
	TopWindow win;
	CodeEditor editor;
	
	editor <<= LoadFile("C:/u/highlight/uppsrc/CodeEditor/CodeEditor.cpp");
	editor.Highlight(CodeEditor::HIGHLIGHT_CPP);
	
	win.Add(editor.SizePos());
	win.Run();
}