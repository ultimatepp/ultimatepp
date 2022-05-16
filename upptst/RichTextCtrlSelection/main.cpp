#include "RichTextCtrlSelection.h"

RichTextCtrlSelection::RichTextCtrlSelection()
{
	CtrlLayout(*this, "Window title");
	a <<= "This is some text";
	b <<= "This is another text";
	
	a.WhenLeftClick = [=] { b.ClearSelection(); };
	b.WhenLeftClick = [=] { a.ClearSelection(); };
}

GUI_APP_MAIN
{
	RichTextCtrlSelection().Run();
}
