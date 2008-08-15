#include "RubberBand.h"

void RubberBand::LeftDown(Point p, dword keyflags)
{
	band.Clear();
	band.Run();
	Refresh();
}

void RubberBand::Paint ( Draw& w )
{
	const Vector<Point> &p = band.GetPoints();
	
	w.DrawRect(GetSize(), SColorFace);
	for (int i = 0; i < p.GetCount()-1; i++)
		w.DrawLine(p[i], p[i+1]);
	if (!band.InLoop() && p.GetCount() > 1)
		w.DrawLine(p[p.GetCount()-1], p[0]);
	w.DrawText(4, 4, AsString(p.GetCount()));
}

RubberBand::RubberBand()
{
	CtrlLayout(*this, "Rubber band test");
	band.SetMaster(*this);
	BackPaint();
}

GUI_APP_MAIN
{
	RubberBand().Sizeable().Run();
}

