#include <CtrlLib/CtrlLib.h>

using namespace Upp;

int MyPrompt(Event<const String&> WhenLink,
             const char *title, const Image& iconbmp, const char *qtf, bool okcancel,
             const char *button1, const char *button2, const char *button3,
             int cx, Image im1, Image im2, Image im3)
{
	LOG(title << ": " << qtf << "\n");
	return IDCANCEL;
}

INITBLOCK {
	RedirectPrompts(MyPrompt);
}

GUI_APP_MAIN
{
	PromptYesNo("Test");
}
