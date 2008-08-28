#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define IMAGECLASS Img
#define IMAGEFILE <MultiListExample/MultiListExample.iml>
#include <Draw/iml.h>

#include <MultiList/MultiList.h>

struct TestWindow : public TopWindow
{
	struct TestDisplay : public Display
	{
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style)	const;
	};

	MultiList 		list[3];
	Label			label[3];
	TestDisplay 	display;
	
	TestWindow();
	virtual void Layout();
};

GUI_APP_MAIN
{
	TestWindow wnd;
	wnd.SetRect(Size(800, 320));
	wnd.Run();
}

TestWindow::TestWindow()
{
	Title("MultiList Example").Sizeable();
	
	list[0].ColumnMode().Columns(3).ItemHeight(max(16, list[0].GetItemHeight()));
	list[1].RowMode().ItemWidth(80).ItemHeight(max(34, list[1].GetItemHeight()));
	list[2].ListMode().Columns(3).ItemHeight(max(16, list[2].GetItemHeight()));
	
	for (int i = 0; i < 3; i++)
		list[i].SetDisplay(display);
	
	for (int i = 0; i < 500; i++) {
		String s = Format("Item %d", i+1);
		for (int j = 0; j < 3; j++)
			list[j].Add(i, s, true);
	}

	label[0].SetLabel("Column Mode (Default):");
	label[1].SetLabel("Row Mode:");
	label[2].SetLabel("List Mode:");
	
	*this << list[0] << list[1] << list[2];
	*this << label[0] << label[1] << label[2];
}

void TestWindow::Layout()
{
	Size sz  = GetSize();
	Size lsz = label[0].GetStdSize();
	int w = (sz.cx - 8) / 3;
	Rect r(2, 0, 2+w, lsz.cy);
	for (int i = 0; i < 3; i++) {
		label[i].SetRect(r);
		r.OffsetHorz(w+2);
	}
	r = Rect(2, lsz.cy, 2+w, sz.cy-2);
	for (int i = 0; i < 3; i++) {
		list[i].SetRect(r);
		r.OffsetHorz(w+2);
	}	
}

void TestWindow::TestDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	PaintBackground(w, r, q, ink, paper, style);
	String 	s = q;
	int 	tcy = GetTextSize(s, StdFont()).cy;
	Size 	isz = r.Deflated(1).GetSize();
	isz = min(isz, Img::package().GetSize());
	isz.cx = isz.cy;
	ChPaint(w, r.left+1, r.top+1, isz.cx, isz.cy, Img::package());
	w.DrawText(r.left + isz.cx + 3, r.top+(r.GetHeight() - tcy)/2, s, StdFont(), ink);
}


