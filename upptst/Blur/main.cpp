#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Image    img;
	Image    imgb;
	DropList radius;
	Option   co;
	
	
	void Sync() {
		double tm = (double)usecs();
		imgb = GaussianBlur(img, ~radius, co);
		tm = (double)usecs() - tm;
		Title(String() << tm << " us");
		Refresh();
	}

	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, White());
		w.DrawImage(0, 0, img);
		w.DrawImage(sz.cx / 2, 0, imgb);
	}
	
	MyApp() {
		for(int i = 1; i < 20; i++)
			radius.Add(i);
		radius <<= 5;
		co ^= radius ^= [=] { Sync(); };
		
		co.SetLabel("Parallel");
		
		Add(radius.BottomPosZ(5).LeftPosZ(10, 50));
		Add(co.BottomPosZ(5).LeftPosZ(70, 150));
	}
};

GUI_APP_MAIN
{
	MyApp app;
	app.img = StreamRaster::LoadFileAny(GetDataFile("sample.jpg"));
	app.Sync();
	app.Run();
}
