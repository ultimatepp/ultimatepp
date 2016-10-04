#include "WRichEdit.h"

WRichEdit::WRichEdit()
{
	Add(editor.SizePos());
}

GUI_APP_MAIN
{
	Ctrl::SetXPStyle();

	WRichEdit q;
//	q.editor.SetQTF(LoadFile("e:/wedit.qtf"));
	q.Run();
//	SaveFile("e:/wedit.qtf", q.editor.GetQTF());
}
