#include "RubberBand.h"

namespace Upp {
void *base_ptr;
};

void RubberBandClass::MouseMove(Point p, dword keyflags) {
	Size old_size = resultpaint.GetSize();
	Size cur_size = Size(max(old_size.cx,p.x+1), max(old_size.cy,p.y+1));
	
	DrawingDraw iw(cur_size);
	
	if (points.GetCount()){
		if(cur_size.cx * cur_size.cy)
			iw.DrawDrawing(0, 0, old_size.cx, old_size.cy, resultpaint);
		iw.DrawLine(points[points.GetCount()-1], p);
	}
	resultpaint.Clear();
	resultpaint = iw;
	points.Add(p);
	GetMaster().Refresh();
}

void RubberBand::LeftDown(Point p, dword keyflags) {
	band.Clear();
	band.Run();
	Refresh();
}

void RubberBand::Paint ( Draw& w ) {
	const Vector<Point> &p = band.GetPoints();
	Drawing im_res = band.GetImage ();
	Size cur_size = im_res.GetSize();
	
	w.DrawRect(GetSize(), SColorFace);
	if(cur_size.cx * cur_size.cy)
		w.DrawDrawing(0, 0, cur_size.cx, cur_size.cy, im_res);
	im_res.Clear();
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
	int x;
	base_ptr = &x;
	RubberBand().Sizeable().Run();
}
