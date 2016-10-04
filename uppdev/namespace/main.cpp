#include <CtrlLib/CtrlLib.h>

struct Test : Upp::TopWindow {
	Upp::EditString text;
};

GUI_APP_MAIN
{
	Upp::PromptOK("OK!");
}
