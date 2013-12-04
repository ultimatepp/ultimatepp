#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	PromptYesNoCancel("&&&&This is an example of prompt!");
	SetStdFont(Serif(10));
	PromptYesNoCancel("&&&&This is an example of prompt after changing font!");
}
