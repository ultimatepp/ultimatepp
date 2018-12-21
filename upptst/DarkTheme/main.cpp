#include "DarkTheme.h"

DarkTheme::DarkTheme()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	PromptOK("{{@Y Just a test}}");
}
