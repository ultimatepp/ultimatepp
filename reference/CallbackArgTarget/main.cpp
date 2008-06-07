#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	CallbackArgTarget<int> result;
	MenuBar menu;
	for(int i = 0; i < 10; i++)
		menu.Add(AsString(i), result[i]);
	menu.Execute();
	if(IsNull(result))
		PromptOK("Menu was cancelled");
	else
		PromptOK("You have selected " + AsString((int)result));
}
