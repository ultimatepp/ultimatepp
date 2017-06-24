#include "ide.h"

#include <ide/MacroManager/MacroManager.h>

void Ide::DoMacroManager()
{
	MacroManagerWindow window(IdeWorkspace(), editor.StoreHlStyles());
	window.WhenEdit = [=](String fileName, int line) {
		EditFile(fileName);
		editor.SetCursor(editor.GetPos(line));
		editor.CenterCursor();
	};
	
	window.Execute();
}
