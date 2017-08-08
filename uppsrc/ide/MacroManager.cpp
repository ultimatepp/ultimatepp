#include "ide.h"

#include <ide/MacroManager/MacroManager.h>

void Ide::DoMacroManager()
{
	MacroManagerWindow manager(IdeWorkspace(), editor.StoreHlStyles());
	manager.WhenEdit = [=](String fileName, int line) {
		EditFile(fileName);
		editor.SetCursor(editor.GetPos(line));
		editor.CenterCursor();
	};
	manager.Execute();
	
	if (manager.IsGlobalMacrosChanged()) {
		Cout() << "Refreshing global macros.\n";
		SyncUsc();
		SetMenuBar();
	}
}
