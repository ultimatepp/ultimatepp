#include "RichView.h"

void RichView::Add()
{
	text <<= String(~text) + "ajkds als fal fhdal flahjds fla fhlahjds flas fla fhdljkh asdf&";
}

void RichView::Clear()
{
	text.Clear();
}

RichView::RichView()
{
	CtrlLayout(*this, "Window title");
	add <<= THISBACK(Add);
	clear <<= THISBACK(Clear);
	text.SetZoom(Zoom(140, 1000));
	text.AutoHideSb();
	Sizeable();
	MinimizeBox();
}

GUI_APP_MAIN
{
	RichView().Run();
}
