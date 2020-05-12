#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Rect rect;
	int  hline, vline;
	Point line1, line2;
	
	typedef MyApp CLASSNAME;
	
	DropList op;
	
	void Round(Rect& r)
	{
		int cx = r.GetWidth();
		r.left = r.left / 10 * 10;
		r.right = r.left + cx;
	}

	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, SColorPaper());
		DrawFrame(w, rect, SBlack());
		w.DrawRect(0, hline, sz.cx, 1, SRed());
		w.DrawRect(vline, 0, 1, sz.cy, SBlue());
		if(line1 != line2)
			w.DrawLine(line1, line2, DPI(2), SMagenta());
	}

	virtual void LeftDown(Point p, dword keyflags) {
		RectTracker tr(*this);
		Size sz = GetSize();
		switch((int)~op) {
		case 0:
			rect = tr.Track(rect, ALIGN_RIGHT, ALIGN_BOTTOM);
			break;
		case 1:
			rect = tr.Track(rect, ALIGN_LEFT, ALIGN_TOP);
			break;
		case 2:
			tr.Dashed().Animation();
			rect = tr.Track(rect, ALIGN_CENTER, ALIGN_CENTER);
			break;
		case 3:
			tr.Solid();
			vline = tr.TrackVertLine(0, 0, sz.cy, vline);
			break;
		case 4:
			tr.Solid();
			hline = tr.TrackHorzLine(0, 0, sz.cx, hline);
			break;
		case 5:
			line1 = p;
			line2 = tr.TrackLine(p.x, p.y);
			break;
		case 6:
			tr.Dashed();
			tr.MinSize(Size(-10000, -10000)); // allow negative size
			tr.Width(DPI(4));
			tr.SetColor(Yellow());
			tr.round = THISBACK(Round);
			rect = tr.Track(rect, ALIGN_RIGHT, ALIGN_BOTTOM);
			rect.Normalize();
			break;
		};
		Refresh();
	}

	MyApp() {
		rect = RectC(100, 100, 100, 100);
		hline = 150;
		vline = 150;
		Sizeable().Zoomable();
		
		line1 = line2 = Point(10, 10);
		
		Add(op.LeftPosZ(10, 200).TopPosZ(10));
		op.Add(0, "Bottom right corner of rectangle");
		op.Add(1, "Top left corner of rectangle");
		op.Add(2, "Move rectangle");
		op.Add(3, "Vertical line");
		op.Add(4, "Horizontal line");
		op.Add(5, "Free line");
		op.Add(6, "Resize rectangle, special features");
		
		op <<= 0;
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
