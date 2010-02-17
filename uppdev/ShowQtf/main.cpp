#include "ShowQtf.h"

void ShowQtf::Text()
{
	qtf <<= ~text;
}

ShowQtf::ShowQtf()
{
	text <<= THISBACK(Text);
	split.Vert(text, qtf);
	Add(split.SizePos());
}

GUI_APP_MAIN
{
	ShowQtf().Run();
}

