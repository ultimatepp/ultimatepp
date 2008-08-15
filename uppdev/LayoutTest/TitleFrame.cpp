#include "TitleFrame.h"

// TitleFrame
void TitleFrame::Paint(Draw& w)
{
	const Style *st = St();
	Rect r = GetSize();
	r.right = GetLastChild() ? GetLastChild()->GetRect().left-1 : r.right-1;
	ChPaint(w, r, st->background[focuslook]);
	if (img) {
		w.DrawImage(r.left + 2, (r.top + 1 + r.Height() - img.GetHeight())/2, img);
		r.left += img.GetWidth() + 2;
	}
	StdDisplay().Paint(w, r, title, st->text[focuslook], Null, 0);
}

void TitleFrame::SetChildPos()
{
	const Style *st = St();
	Ctrl::LogPos lp;
	lp.x = Ctrl::Logc(ALIGN_RIGHT, 1, TitleSz()-2);
	lp.y = Ctrl::Logc(ALIGN_TOP, 1, TitleSz()-2);
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext())
	{
		c->SetPos(lp);
		lp.x.SetA(lp.x.GetA() + TitleSz());
	}
}

TitleFrame::TitleFrame()
{
	style = NULL;
	focuslook = false;
}

CH_STYLE(TitleFrame, Style, StyleDefault)
{
	background[0] = Blend(SLtGray, SWhite, 30);//Colorize(DockImg::titlegrad2, SColorShadow(), 200); // No focus
	background[1] = Blend(SLtBlue, SWhite, 30);//Colorize(DockImg::titlegrad2, Blend(SColorHighlight(), SWhite(), 40), 200); // Focus
	text[0]	= SWhite(); // No focus
	text[1]	= SWhite(); // Focus
	font	  = StdFont(12);
}
