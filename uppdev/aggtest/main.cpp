#include "aggtest.h"

aggtest::aggtest()
{
	CtrlLayout(*this, "Window title");
	Sizeable();
}

void aggtest::Paint(Draw& draw)
{
	if (this->GetSize().cx == 0 || this->GetSize().cy == 0)
		return;
	
	ImageDraw d(GetSize());
	d.DrawRect(GetSize(), White);
	d.DrawLine(x,y,150,150,50,Blue);
	
	draw.DrawImage(0,0,this->GetSize().cx,this->GetSize().cy,d);
}


GUI_APP_MAIN
{
	aggtest m;
	//m.BackPaint();
	m.Maximize(true);
	m.Run();
}

