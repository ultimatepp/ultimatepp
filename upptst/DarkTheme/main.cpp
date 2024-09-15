#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS DarkImg
#define IMAGEFILE <DarkTheme/dark.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS DarkImg
#define IMAGEFILE <DarkTheme/dark.iml>
#include <Draw/iml_source.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& draw) override;
	
	CtrlRetriever ret;
	
	EditDoubleSpin r, g, b;
	EditIntSpin    middle;
	
	void Sync();
	
	MyApp();
};

namespace Upp {
extern double C_R;
extern double C_G;
extern double C_B;
};

void MyApp::Paint(Draw& w)
{
	Vector<Color> col;
	col
		<< Black()
		<< Gray()
		<< LtGray()
		<< WhiteGray()
		<< White()
		
		<< Red()
		<< Green()
		<< Brown()
		<< Blue()
		<< Magenta()
		<< Cyan()
		<< Yellow()
		<< LtRed()
		<< LtGreen()
		<< LtYellow()
		<< LtBlue()
		<< LtMagenta()
		<< LtCyan()
	;
	
	w.DrawRect(GetSize(), Black());
	for(int i = 0; i < col.GetCount(); i++) {
		w.DrawRect(i * 32, 0, 32, 32, col[i]);
		w.DrawRect(i * 32, 35, 32, 32, DarkTheme(col[i]));
	}
	
	Vector<Image> img = { CtrlImg::CdRom__UHD(), CtrlImg::open(), CtrlImg::new_doc(), CtrlImg::undo(),
	                     CtrlImg::select_all(), CtrlImg::copy(), DarkImg::read_only(), DarkImg::build_rebuild_all(),
	                     DarkImg::info(), DarkImg::Cpp() };
	for(int i = 0; i < img.GetCount(); i++) {
		w.DrawImage(i * 40, 70, 32, 32, img[i]);
		w.DrawImage(i * 40, 120, 32, 32, DarkTheme(img[i]));
	}
	
	Size tsz = GetTextSize("Test", StdFont());
	int y = 200;
	w.DrawRect(0, 200, tsz.cx * col.GetCount(), tsz.cy, White());
	for(int i = 0; i < col.GetCount(); i++)
		w.DrawText(i * tsz.cx, y, "Text", StdFont(), col[i]);
	y += tsz.cy;

	for(int i = 0; i < col.GetCount(); i++)
		w.DrawText(i * tsz.cx, y, "Text", StdFont(), DarkTheme(col[i]));
	y += tsz.cy;
/*
	for(int i = 0; i < col.GetCount(); i++)
		w.DrawText(i * tsz.cx, y, "Text", StdFont(), DarkTheme2(col[i]));
	y += tsz.cy;
*/
	y += tsz.cy;
	
	for(Color c : col) {
		w.DrawRect(0, y, tsz.cy, tsz.cy, c);
		c = DarkTheme(c);
		w.DrawRect(tsz.cy + 10, y, tsz.cy, tsz.cy, c);
		w.DrawText(2 * (tsz.cy + 10), y, AsString(c), StdFont(), White());
		y += tsz.cy;
	}

	int x = tsz.cx * 32 + 50;
	w.DrawImage(x, 0, DarkImg::ide());
	x += DarkImg::ide().GetWidth();
	w.DrawImage(x, 0, DarkTheme(DarkImg::ide()));
}

void MyApp::Sync()
{
	ret.Retrieve();
	Refresh();
}

MyApp::MyApp()
{
	int x = 0;
	int cx = Zx(50);
	auto Add = [&](Ctrl& w) {
		*this << w.BottomPos(10).LeftPos(x, cx);
		x += cx + 2;
		w << [=] { Sync(); };
	};
	
	Add(r.SetInc(0.01).MinMax(0, 1));
	Add(g.SetInc(0.01).MinMax(0, 1));
	Add(b.SetInc(0.01).MinMax(0, 1));
	Add(middle);

/*
	ret(r, DarkTheme_c[0])
	   (g, DarkTheme_c[1])
	   (middle, DarkTheme_middle);
*/

	ret(r, C_R)
	   (g, C_G)
	   (b, C_B)
	;

	Sizeable();
};

GUI_APP_MAIN
{
	DDUMP(DarkTheme(LtYellow()));
	DDUMP(DarkTheme(Yellow()));
	DDUMP(DarkTheme(Brown()));
	DDUMP(DarkTheme(LtRed()));
	DDUMP(DarkTheme(Black()));
	DDUMP(DarkTheme(LtBlue()));
	DDUMP(DarkTheme(LtBlue()));
	DDUMP(DarkTheme(LtGreen()));

	DDUMP(DarkTheme(LtRed()));
	DDUMP(DarkTheme(LtBlue()));
	MyApp().Run();
}
