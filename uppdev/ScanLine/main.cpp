#include "ScanLine.h"

struct App : TopWindow {
	virtual void Paint(Draw& w) {
		ScanLine a, b;
		static byte line1[] = { 1, 50, 100, 128, 128, 128, 128, 128, 100, 50, 1 };
		a = Pack(0, line1, __countof(line1));
		ImageBuffer ib(400, 400);
		Fill(~ib, White(), ib.GetLength());
		Apply(ib[20], 100, Black(), a);
		a.x = 10;
		Apply(ib[30], 100, Blue(), a);
		for(int i = 0; i < 20; i++) {
			b = Pack(i, line1, __countof(line1));
			Apply(ib[50 + 2 * i], 100, Black(), And(a, b));
		}
		
		Rasterizer r(400);
		r.Line(100 * 256, 100 * 256, 300 * 256, 100 * 256);
		r.Line(300 * 256, 100 * 256, 200 * 256, 300 * 256);
		r.Line(200 * 256, 300 * 256, 100 * 256, 100 * 256);
		for(int y = r.MinY(); y <= r.MaxY(); y++) {
			ScanLine sl = r.Get(y);
			DUMP(sl);
			Apply(ib[y], 400, Black(), sl);
		}
		
		w.DrawRect(GetSize(), White());
		w.DrawImage(0, 0, ib);
	}
};

GUI_APP_MAIN {
	App().Run();

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
