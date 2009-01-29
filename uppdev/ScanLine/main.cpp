#include "ScanLine.h"

struct App : TopWindow {
	double x1, y1, x2, y2, x3, y3;

	String Text() {
		return Format("Move(%f, %f) Line(%f, %f)  Line(%f, %f)", x1, y1, x2, y2, x3, y3);
	}
	
	virtual void LeftDown(Point p, dword keyflags)
	{
		x1 = p.x;
		y1 = p.y;
		Refresh();
		ClearClipboard();
		AppendClipboardText(Text());
	}
	virtual void RightDown(Point p, dword)
	{
		x2 = p.x;
		y2 = p.y;
		Refresh();
		ClearClipboard();
		AppendClipboardText(Text());
	}
	virtual void MouseMove(Point p, dword keyflags)
	{
		x3 = p.x;
		y3 = p.y;
		Refresh();
		ClearClipboard();
		AppendClipboardText(Text());
	}

	virtual void Paint(Draw& w) {
		ScanLine a, b;
		static byte line1[] = { 1, 50, 100, 128, 128, 128, 128, 128, 100, 50, 1 };
		a = Pack(0, line1, __countof(line1));
		ImageBuffer ib(500, 500);
		Fill(~ib, White(), ib.GetLength());
/*		Apply(ib[20], 100, Black(), a);
		a.x = 10;
		Apply(ib[30], 100, Blue(), a);
		for(int i = 0; i < 20; i++) {
			b = Pack(i, line1, __countof(line1));
			Apply(ib[50 + 2 * i], 100, Black(), And(a, b));
		}
*/		
		Rasterizer r(Size(500, 500));
//		r.SetClip(RectfC(100, 100, 200, 200));


#if 1
		r.Move( 36.000000, 142.000000);
		r.Line(480.000000, 148.000000);
		r.Line(429.000000, 148.000000);
		r.Line( 36.000000, 142.000000);
#endif	
#if 0
		r.Move(121.000000, 121.000000);
		r.Line(0.000000, 0.000000);
		r.Line(597.000000, 44.000000);
		r.Line(121.000000, 121.000000);
#endif
#if 0
		r.Move(153.000000, 297.000000);
		r.Line(173.000000, 255.000000);
		r.Line(564.000000, 213.000000);
		r.Line(153.000000, 297.000000);
#endif
#if 0
		r.Move(x1, y1);
		r.Line(x2, y2);
		r.Line(x3, y3);
		r.Line(x1, y1);
#endif
		for(int y = r.MinY(); y <= r.MaxY(); y++) {
			ScanLine sl = r.Get(y);
			DUMP(sl);
			Apply(ib[y], 500, Blue(), sl);
		}
		
		w.DrawRect(GetSize(), White());
		w.DrawImage(0, 0, ib);
		w.DrawText(0, GetSize().cy - 40, Text());
	}

	App() {
		x1 = y1 = x2 = y2 = 0;
	}
};

GUI_APP_MAIN {
	App().Run();
	return;

	ScanLine a, b, c;
	static byte line1[] = { 1, 50, 100, 128, 128, 128, 128, 128, 100, 50, 1 };
	a = Pack(0, line1, __countof(line1));
	DUMP(a);
	for(int i = 0; i < 16; i++) {
		b = Pack(i, line1, __countof(line1));
		DUMP(b);
		DUMP(And(a, b));
		DUMP(And(b, a));
	}
	
	
#ifndef _DEBUG
	int x = 0;
	b = Pack(3, line1, __countof(line1));
	for(int i = 0; i < 10000000; i++) {
		RTIMING("And");
		x += And(a, b).x;
	}
#endif
}
