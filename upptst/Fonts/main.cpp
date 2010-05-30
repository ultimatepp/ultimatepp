#include "Fonts.h"

struct DisplayWithFont : public Display {
	void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		ValueArray va = q;
		String txt = va[0];
		int face = va[1];
		Font fnt(face, r.Height() - 4);
		w.DrawRect(r, paper);
		w.DrawText(r.left, r.top + (r.Height() - fnt.Info().GetHeight()) / 2, txt, fnt, ink);
	}
};

void Fonts::Reload()
{
	list.Clear();
	for(int i = 0; i < Font::GetFaceCount(); i++)
		list.Add(Font::GetFaceName(i), ~text, i);
}

void Fonts::Copy()
{
	if(list.IsCursor())
		WriteClipboardText(list.GetKey());
}

Fonts::Fonts()
{
	CtrlLayout(*this, "Font test");
	text <<= THISBACK(Reload);
	list.AddColumn("Font");
	list.AddColumn("Text").AddIndex().SetDisplay(Single<DisplayWithFont>());
	list.WhenCursor = THISBACK(Copy);
}

GUI_APP_MAIN
{
	Fonts().Run();
}
