//#include <CtrlLib/CtrlLib.h>
#include "agg_upp_bind_test.h"



struct ClockTest : TopWindow {
	Option optR;
	
	void PaintPtr(Draw& w, double pos, double m, int d, Color color, Size sz2)
	{
		w.DrawLine(sz2.cx, sz2.cy,
		           sz2.cx + int(m * sin(pos * 2 * M_PI) * sz2.cx),
		           sz2.cy - int(m * cos(pos * 2 * M_PI) * sz2.cy),
		           d, color);
	}

	void PaintCenteredText(Draw& w, int x, int y, const char *text, Font fnt, Color c)
	{
		Size tsz = GetTextSize(text, fnt);
		w.DrawText(x - tsz.cx / 2, y - tsz.cy / 2, text, fnt, c);
	}

	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();		
		w.DrawRect(sz, SLtGray);
		sz -= 6;
		w.Offset(3, 3);
		Size sz2 = sz / 2;
//		w.DrawEllipse(0, 0, sz.cx, sz.cy, SWhite, 3, SBlack);
		
	//	Font fnt = Arial(min(sz.cx, sz.cy) / 10);
	//	for(int i = 1; i <= 12; i++) {
	//		int d = i % 3 == 0 ? 3 : 2;
	//		PaintCenteredText(w, sz2.cx + int(0.8 * sin(i * M_PI / 6) * sz2.cx),
	//		                  sz2.cy - int(0.8 * cos(i * M_PI / 6) * sz2.cy),
	//		                  AsString(i), i % 3 ? fnt : fnt().Bold(), SBlack);
	//	}
		Date date = GetSysDate();
		Time time = GetSysTime();
	//	PaintCenteredText(w, sz.cx / 2, 3 * sz.cy / 5, GetLanguageInfo().FormatDate(date),
	//	                  fnt().Bold(), SLtBlue);
		double tm = GetSysTime() - ToTime(date);
	//	PaintPtr(w, tm / 60, 0.75, 1, SRed, sz2);
	//	PaintPtr(w, tm / 3600, 0.6, 2, SCyan, sz2);
	//	PaintPtr(w, tm / 3600 / 12, 0.5, 4, SBlack, sz2);
		AggDrawer agd(RectC(0,0,sz.cx, sz.cy));
		agd.SetBackground(agg::rgba(0, 1.0, 1.0, ~optR));
//		agd.SetBackground(agg::rgba(1.0, 0.5, 0.5));
		
		agd.DrawLine(0,0,100,100,10);
		agd.SetBrushColor(agg::rgba(0, 0, 0));
//		agd.DrawLine(100,0,200,100,10);
		agd.DrawEllipse(sz2.cx, sz2.cy, sz2.cx-10, sz2.cy-10, 3);
				
		agd.End(w, ~optR);
		w.End();
	}

	void Timer()
	{
		Refresh();
	}

	ClockTest()
	{
		optR.SetRect(0,0,50,50);
		Add(optR);
		SetRect(0, 0, 600, 300);
		Sizeable().Zoomable();
		BackPaint();
		SetTimeCallback(-1000, callback(this, &ClockTest::Timer));
		Title("ClockTest");
	}
};

GUI_APP_MAIN
{
	ClockTest().Run();
}
