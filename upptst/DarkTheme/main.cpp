#include <CtrlLib/CtrlLib.h>

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
extern double DarkTheme_c[];
extern int    DarkTheme_middle;
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
	
	w.DrawRect(GetSize(), Color(20, 20, 20));
	for(int i = 0; i < col.GetCount(); i++) {
		w.DrawRect(i * 32, 0, 32, 32, col[i]);
		w.DrawRect(i * 32, 35, 32, 32, DarkTheme(col[i]));
	}
	
	Vector<Image> img = { CtrlImg::CdRom__UHD(), CtrlImg::open(), CtrlImg::new_doc(), CtrlImg::undo(), CtrlImg::select_all(), CtrlImg::copy() };
	for(int i = 0; i < img.GetCount(); i++) {
		w.DrawImage(i * 40, 70, 32, 32, img[i]);
		w.DrawImage(i * 40, 120, 32, 32, DarkTheme(img[i]));
	}

}

void MyApp::Sync()
{
	ret.Retrieve();
	DarkTheme_c[2] = 1 - DarkTheme_c[0] - DarkTheme_c[1];
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
	
	Add(r.SetInc(0.02).MinMax(0, 1));
	Add(g.SetInc(0.02).MinMax(0, 1));
	Add(middle);
	
	ret(r, DarkTheme_c[0])
	   (g, DarkTheme_c[1])
	   (middle, DarkTheme_middle);
	
	Sizeable();
};

GUI_APP_MAIN
{
	MyApp().Run();
}
