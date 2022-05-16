#include "RichTextCtrlSelection.h"

RichTextCtrlSelection::RichTextCtrlSelection()
{
	CtrlLayout(*this, "Window title");
	a <<= "This is some text";
	b <<= "This is another text";
}

GUI_APP_MAIN
{
	RichTextCtrlSelection().Run();
}
