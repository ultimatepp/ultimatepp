#include "SmartText.h"

void SmartText::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, White);
	DrawTLText(w, 10, 10, sz.cx - 20, ToUnicode("Hello wor\tld", CHARSET_DEFAULT), Arial(20), SColorText(), 'L');
}

SmartText::SmartText()
{
	button.SetRect(200, 200, 100, 50);
	Add(button);
	button.SetLabel("\1[A5 \aWorld");
}

GUI_APP_MAIN
{
	SmartText().Run();
}
